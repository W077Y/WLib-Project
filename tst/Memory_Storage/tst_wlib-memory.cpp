#include <exception>
#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

namespace
{
  struct dummy_memory: public wlib::memory::Non_Volatile_Memory_Interface
  {
    dummy_memory(std::span<std::byte> mem)
        : mem(mem)
    {
    }

    // Inherited via Non_Volatile_Memory_Interface
    virtual std::size_t capacity() const { return this->mem.size(); };
    virtual std::size_t alignment() const { return 1; };

    void write(std::size_t add, std::span<std::byte const> data) override
    {
      std::size_t const end = add + data.size();
      if (this->mem.size() < end)
        throw std::out_of_range("address out of range");

      for (std::size_t i = 0; i < data.size(); i++)
      {
        this->mem[add + i] = data[i];
      }
    }

    void flush() override {}

    void read(std::size_t add, std::span<std::byte> data) override
    {
      std::size_t const end = add + data.size();
      if (this->mem.size() < end)
        throw std::out_of_range("address out of range");

      for (std::size_t i = 0; i < data.size(); i++)
      {
        data[i] = this->mem[add + i];
      }
    }

    std::span<std::byte> mem;
  };

  struct dummy_shared_memory_resource: public wlib::Shared_Memory_Provider_Interface
  {
    // Inherited via Shared_Memory_Provider_Interface
    void lock() override {}
    void unlock() override {}

    dummy_shared_memory_resource(std::span<std::byte> memory)
        : Shared_Memory_Provider_Interface(memory)
    {
    }
  };
}    // namespace

TEST_CASE()
{
  std::array<std::byte, 10> buffer = {};
  for (auto& ent : buffer)
    ent = std::byte(0xFF);

  dummy_memory mem = { buffer };

  std::array<std::byte, 5> tmp = {};

  for (auto ent : tmp)
    REQUIRE(ent == std::byte(0x00));

  mem.read(5, tmp);

  for (auto ent : tmp)
    REQUIRE(ent == std::byte(0xFF));

  tmp = {};

  mem.read(0, tmp);
  for (auto ent : tmp)
    REQUIRE(ent == std::byte(0xFF));

  for (std::size_t i = 0; i < tmp.size(); i++)
  {
    tmp[i] = std::byte(i + 3);
  }

  mem.write(1, tmp);
  mem.flush();

  constexpr std::array<std::byte, 10> ref_data_1 = {
    std::byte(0xFF), std::byte(0x03), std::byte(0x04), std::byte(0x05), std::byte(0x06),
    std::byte(0x07), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF),
  };
  constexpr std::array<std::byte, 10> ref_data_2 = {
    std::byte(0xFF), std::byte(0x03), std::byte(0x03), std::byte(0x04), std::byte(0x05),
    std::byte(0x06), std::byte(0x07), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF),
  };

  REQUIRE(buffer == ref_data_1);

  mem.write(2, tmp);
  mem.flush();

  REQUIRE(buffer == ref_data_2);

  constexpr std::array<std::byte, 5> ref_data_3 = {
    std::byte(0xFF), std::byte(0x03), std::byte(0x03), std::byte(0x04), std::byte(0x05),
  };

  constexpr std::array<std::byte, 5> ref_data_4 = {
    std::byte(0x06), std::byte(0x07), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF),
  };

  mem.read(0, tmp);
  REQUIRE(tmp == ref_data_3);
  mem.read(5, tmp);
  REQUIRE(tmp == ref_data_4);

  REQUIRE_THROWS(mem.read(6, tmp));
  REQUIRE_THROWS(mem.write(6, tmp));
}

TEST_CASE()
{
  using T = wlib::storage::strategy::mirrow_storage_t<uint32_t>;

  std::array<std::byte, 15>    sh_buffer  = {};
  dummy_shared_memory_resource sh_buf_mem = { sh_buffer };

  std::array<std::byte, 30> buffer = {};
  dummy_memory              mem    = { buffer };

  {    // load empty storage (zeroed)
    buffer = {};
    T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
    REQUIRE(storage.load() == 0);
  }
  {    // load empty storage (0xFF)
    for (auto& el : buffer)
      el = std::byte(0xFF);

    T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
    REQUIRE(storage.load() == 0);
  }
  {    // load empty storage (0xAA)
    for (auto& el : buffer)
      el = std::byte(0xAA);

    T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
    REQUIRE(storage.load() == 0);
  }

  constexpr uint32_t value = 0xAABB'CCDD;
  for (std::size_t i = 0; i < 15; i++)
  {
    buffer = {};
    {    // load default and save value
      T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
      REQUIRE(storage.load() == 0);
      storage.save(value);
      REQUIRE(storage.load() == value);
    }
    {    // load the value
      T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
      REQUIRE(storage.load() == value);
    }
    buffer[0 + i] = ~buffer[0 + i];
    {    // load first brocken
      T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
      REQUIRE(storage.load() == value);
    }
    buffer[15 + i] = ~buffer[15 + i];
    {    // load second brocken
      T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
      REQUIRE(storage.load() == value);
    }
    buffer[0 + i]  = ~buffer[0 + i];
    buffer[15 + i] = ~buffer[15 + i];
    {    // load both brocken
      T storage{ mem, 15, { 0, 15 }, sh_buf_mem };
      REQUIRE(storage.load() == 0);
    }
  }
}
