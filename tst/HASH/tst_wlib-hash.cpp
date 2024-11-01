#include <fstream>
#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>



std::byte zero_value_test_sum[32] = {
  std::byte(0xe3), std::byte(0xb0), std::byte(0xc4), std::byte(0x42), std::byte(0x98), std::byte(0xfc), std::byte(0x1c), std::byte(0x14),
  std::byte(0x9a), std::byte(0xfb), std::byte(0xf4), std::byte(0xc8), std::byte(0x99), std::byte(0x6f), std::byte(0xb9), std::byte(0x24),
  std::byte(0x27), std::byte(0xae), std::byte(0x41), std::byte(0xe4), std::byte(0x64), std::byte(0x9b), std::byte(0x93), std::byte(0x4c),
  std::byte(0xa4), std::byte(0x95), std::byte(0x99), std::byte(0x1b), std::byte(0x78), std::byte(0x52), std::byte(0xb8), std::byte(0x55),
};

std::byte abc_value_test_sum[32] = {
  std::byte(0xBA), std::byte(0x78), std::byte(0x16), std::byte(0xBF), std::byte(0x8F), std::byte(0x01), std::byte(0xCF), std::byte(0xEA),
  std::byte(0x41), std::byte(0x41), std::byte(0x40), std::byte(0xDE), std::byte(0x5D), std::byte(0xAE), std::byte(0x22), std::byte(0x23),
  std::byte(0xB0), std::byte(0x03), std::byte(0x61), std::byte(0xA3), std::byte(0x96), std::byte(0x17), std::byte(0x7A), std::byte(0x9C),
  std::byte(0xB4), std::byte(0x10), std::byte(0xFF), std::byte(0x61), std::byte(0xF2), std::byte(0x00), std::byte(0x15), std::byte(0xAD),
};

char const b63_value[]            = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+";
std::byte  b63_value_test_sum[32] = {
  std::byte(0xE6), std::byte(0x09), std::byte(0x70), std::byte(0x06), std::byte(0x65), std::byte(0xE4), std::byte(0x02), std::byte(0x03),
  std::byte(0xC7), std::byte(0x82), std::byte(0x62), std::byte(0xF8), std::byte(0xF1), std::byte(0x1D), std::byte(0x42), std::byte(0x84),
  std::byte(0x4F), std::byte(0x6D), std::byte(0x85), std::byte(0x64), std::byte(0xE6), std::byte(0xA5), std::byte(0x77), std::byte(0xC1),
  std::byte(0xBF), std::byte(0x56), std::byte(0x48), std::byte(0xE5), std::byte(0x86), std::byte(0xDB), std::byte(0x84), std::byte(0x50),
};

char const b64_value[]            = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-";
std::byte  b64_value_test_sum[32] = {

  std::byte(0xC5), std::byte(0x21), std::byte(0x62), std::byte(0xA1), std::byte(0x05), std::byte(0xCF), std::byte(0x80), std::byte(0xB3),
  std::byte(0x0E), std::byte(0xEA), std::byte(0xB0), std::byte(0x7F), std::byte(0x55), std::byte(0x61), std::byte(0xD5), std::byte(0x27),
  std::byte(0xB6), std::byte(0x2C), std::byte(0x8B), std::byte(0x17), std::byte(0xC5), std::byte(0xCE), std::byte(0xE1), std::byte(0x0D),
  std::byte(0x80), std::byte(0x1F), std::byte(0x96), std::byte(0x3F), std::byte(0x48), std::byte(0xBD), std::byte(0xD6), std::byte(0xFC),
};

char const b65_value[]            = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-=";
std::byte  b65_value_test_sum[32] = {
  std::byte(0xA5), std::byte(0xD4), std::byte(0x60), std::byte(0x77), std::byte(0x2B), std::byte(0x91), std::byte(0xDD), std::byte(0xD0),
  std::byte(0x1C), std::byte(0x11), std::byte(0x84), std::byte(0xE3), std::byte(0x2D), std::byte(0x2F), std::byte(0xA4), std::byte(0x90),
  std::byte(0xE9), std::byte(0x5D), std::byte(0xCB), std::byte(0xFD), std::byte(0xC7), std::byte(0x08), std::byte(0x77), std::byte(0x60),
  std::byte(0x2F), std::byte(0x2A), std::byte(0x3B), std::byte(0xE0), std::byte(0xE8), std::byte(0x56), std::byte(0x9F), std::byte(0x4D),
};

std::byte bin_file_value_test_sum[32] = {
  std::byte(0xA6), std::byte(0x7D), std::byte(0x42), std::byte(0x3C), std::byte(0x8E), std::byte(0x71), std::byte(0x39), std::byte(0xAC),
  std::byte(0x83), std::byte(0x35), std::byte(0x34), std::byte(0xF4), std::byte(0xB5), std::byte(0xC4), std::byte(0x74), std::byte(0x5A),
  std::byte(0xE7), std::byte(0x37), std::byte(0x99), std::byte(0x6C), std::byte(0xEE), std::byte(0x6E), std::byte(0xD4), std::byte(0x09),
  std::byte(0x6B), std::byte(0x64), std::byte(0xB0), std::byte(0x5A), std::byte(0xC9), std::byte(0x34), std::byte(0xA8), std::byte(0x9C),
};

using sha_256 = wlib::hash::sha_256;

void print(sha_256::hash_t const& hash)
{
  char  buf[2048] = {};
  char* end       = buf + sizeof(buf);
  char* pos       = buf;

  pos += snprintf(pos, end - pos, "0x");
  for (std::byte val : hash)
  {
    pos += snprintf(pos, end - pos, " %02X", int(val));
  }
  std::cout << buf << std::endl;
}

TEST_CASE("HASH ZERO")
{
  sha_256 sha;
  auto    hash = sha.get();

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(zero_value_test_sum[i] == hash[i]);
  }
}

TEST_CASE("HASH abc")
{
  constexpr std::byte tst[] = { std::byte('a'), std::byte('b'), std::byte('c') };

  sha_256 sha;
  sha(tst);
  auto hash = sha.get();

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(abc_value_test_sum[i] == hash[i]);
  }
}

TEST_CASE("HASH b63")
{
  std::span<std::byte const> tst = { reinterpret_cast<std::byte const*>(b63_value), 63 };

  sha_256 sha;
  auto    hash = sha(tst).get();

  sha.reset();

  auto part_hash = sha({ tst.data(), tst.data() + 10 })({ tst.data() + 10, tst.data() + 20 })({ tst.data() + 20, tst.data() + 63 }).get();

  REQUIRE(part_hash == hash);

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(b63_value_test_sum[i] == hash[i]);
  }
}

TEST_CASE("HASH b64")
{
  std::span<std::byte const> tst = { reinterpret_cast<std::byte const*>(b64_value), 64 };

  sha_256 sha;
  sha(tst);
  auto hash = sha.get();

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(b64_value_test_sum[i] == hash[i]);
  }
}

TEST_CASE("HASH b65")
{
  std::span<std::byte const> tst = { reinterpret_cast<std::byte const*>(b65_value), 65 };

  sha_256 sha;
  sha(tst);
  auto hash = sha.get();

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(b65_value_test_sum[i] == hash[i]);
  }
}

TEST_CASE("HASH bin-file")
{
  sha_256 sha;

  std::ifstream file;
  file.open(TEST_DATA_FILE_PATH, std::ios::binary | std::ios::in);

  REQUIRE(file.is_open());

  char blk[37]{};

  file.read(blk, sizeof(blk));
  std::streamsize chunk_size = file.gcount();
  while (chunk_size > 0)
  {
    std::byte* begin = reinterpret_cast<std::byte*>(blk);
    sha({ begin, size_t(chunk_size) });
    file.read(blk, sizeof(blk));
    chunk_size = file.gcount();
  }
  file.close();

  auto hash = sha.get();

  print(hash);

  REQUIRE(hash.size() == 32);
  for (int i = 0; i < hash.size(); i++)
  {
    REQUIRE(bin_file_value_test_sum[i] == hash[i]);
  }
}
