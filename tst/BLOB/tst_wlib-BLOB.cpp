#include <iostream>
#include <ut_catch.hpp>
#include <wlib-BLOB.hpp>

void print_buffer(wlib::blob::MemoryBlob const& blob, std::span<std::byte> const& buffer)
{
  for (std::byte entry : blob.get_span())
  {
    std::cout << static_cast<int>(entry) << ", ";
  }
  std::cout << std::endl;
  for (std::byte entry : buffer)
  {
    std::cout << static_cast<int>(entry) << ", ";
  }
  std::cout << std::endl;
}

TEST_CASE()
{
  std::byte            buffer[30]{};
  std::byte*           buffer_begin = buffer;
  std::byte*           buffer_end   = buffer + sizeof(buffer);
  std::span<std::byte> buffer_span{ buffer };
  {
    wlib::blob::MemoryBlob blob{ { buffer_begin, buffer_end } };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 30);
    REQUIRE(blob.get_number_of_used_bytes() == 0);
  }
  {
    wlib::blob::MemoryBlob blob{ { buffer_begin, sizeof(buffer) } };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 30);
    REQUIRE(blob.get_number_of_used_bytes() == 0);
  }
  {
    wlib::blob::MemoryBlob blob{ buffer };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 30);
    REQUIRE(blob.get_number_of_used_bytes() == 0);
  }
  {
    wlib::blob::MemoryBlob blob{ buffer_span };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 30);
    REQUIRE(blob.get_number_of_used_bytes() == 0);
  }
  {
    wlib::blob::MemoryBlob blob{ { buffer_begin, sizeof(buffer) }, 3 };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 27);
    REQUIRE(blob.get_number_of_used_bytes() == 3);
  }
  {
    wlib::blob::MemoryBlob blob{ buffer_span, 3 };
    REQUIRE(blob.get_total_number_of_bytes() == 30);
    REQUIRE(blob.get_number_of_free_bytes() == 27);
    REQUIRE(blob.get_number_of_used_bytes() == 3);
  }
}

TEST_CASE()
{
  std::byte                    buffer[30]{};
  wlib::blob::MemoryBlob const blob{ buffer };
  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  std::span<std::byte const> blob_span = blob.get_span();

  REQUIRE(&buffer[0] == blob_span.data());
}

TEST_CASE()
{
  std::byte              buffer[30]{};
  wlib::blob::MemoryBlob blob{ buffer };
  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  std::span<std::byte> blob_span = blob.get_span();

  REQUIRE(&buffer[0] == blob_span.data());
}

TEST_CASE()
{
  std::byte              buffer[30]{};
  wlib::blob::MemoryBlob blob{ buffer };
  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.adjust_position(5);

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 25);
  REQUIRE(blob.get_number_of_used_bytes() == 5);

  REQUIRE_THROWS(blob.adjust_position(-6));
  REQUIRE_THROWS(blob.adjust_position(30));

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 25);
  REQUIRE(blob.get_number_of_used_bytes() == 5);

  blob.set_position(30);

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 30);

  REQUIRE_THROWS(blob.set_position(31));

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 30);

  blob.clear();

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);
}

TEST_CASE()
{
  std::byte buffer[30]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.insert_back(static_cast<std::byte>(0x03));
  blob.insert_back(static_cast<std::byte>(0x04));
  blob.insert_front(static_cast<std::byte>(0x02));
  blob.insert_front(static_cast<std::byte>(0x01));

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 26);
  REQUIRE(blob.get_number_of_used_bytes() == 4);

  blob.clear();

  REQUIRE(blob.get_total_number_of_bytes() == 30);
  REQUIRE(blob.get_number_of_free_bytes() == 30);
  REQUIRE(blob.get_number_of_used_bytes() == 0);
}

TEST_CASE()
{
  std::byte buffer[14]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_total_number_of_bytes() == 14);
  REQUIRE(blob.get_number_of_free_bytes() == 14);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  uint32_t val_in[] = {
    0x0011'2233,
    0x4455'6677,
    0x8899'AABB,
    0xCCDD'EEFF,
  };

  std::size_t suc_count = 0;
  for (uint32_t val : val_in)
  {
    if (blob.try_insert_back(val))
      suc_count++;
  }
  REQUIRE(suc_count == 3);
  REQUIRE(blob.get_total_number_of_bytes() == 14);
  REQUIRE(blob.get_number_of_free_bytes() == 2);
  REQUIRE(blob.get_number_of_used_bytes() == 12);

  uint32_t val_out[4] = {};

  suc_count = 0;
  for (uint32_t& val : val_out)
  {
    if (blob.try_extract_front(val))
      suc_count++;
  }
  REQUIRE(suc_count == 3);
  for (std::size_t i = 0; i < suc_count; i++)
  {
    REQUIRE(val_in[i] == val_out[i]);
  }

  blob.clear();
  REQUIRE(blob.get_total_number_of_bytes() == 14);
  REQUIRE(blob.get_number_of_free_bytes() == 14);
  REQUIRE(blob.get_number_of_used_bytes() == 0);
}

TEST_CASE()
{
  static constexpr std::size_t buffer_size = 1 + sizeof(std::byte) + sizeof(char) + sizeof(signed char) + sizeof(unsigned char) + sizeof(char8_t) +
                                             sizeof(char16_t) + sizeof(char32_t) + sizeof(signed short) + sizeof(unsigned short) + sizeof(signed int) +
                                             sizeof(unsigned int) + sizeof(signed long) + sizeof(unsigned long) + sizeof(signed long long) +
                                             sizeof(unsigned long long) + sizeof(float) + sizeof(double) + sizeof(long double);

  std::byte buffer[buffer_size]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_total_number_of_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_free_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  bool ret = true;
  ret &= blob.try_insert_back(static_cast<std::byte>(0x01));
  ret &= blob.try_insert_back(static_cast<char>(0x02));
  ret &= blob.try_insert_back(static_cast<signed char>(0x03));
  ret &= blob.try_insert_back(static_cast<unsigned char>(0x04));
  ret &= blob.try_insert_back(static_cast<char8_t>(0x05));
  ret &= blob.try_insert_back(static_cast<char16_t>(0x06));
  ret &= blob.try_insert_back(static_cast<char32_t>(0x07));
  ret &= blob.try_insert_back(static_cast<signed short>(0x08));
  ret &= blob.try_insert_back(static_cast<unsigned short>(0x09));
  ret &= blob.try_insert_back(static_cast<signed int>(0x0A));
  ret &= blob.try_insert_back(static_cast<unsigned int>(0x0B));
  ret &= blob.try_insert_back(static_cast<signed long>(0x0C));
  ret &= blob.try_insert_back(static_cast<unsigned long>(0x0D));
  ret &= blob.try_insert_back(static_cast<signed long long>(0x0E));
  ret &= blob.try_insert_back(static_cast<unsigned long long>(0x0F));
  ret &= blob.try_insert_back(static_cast<float>(0x10));
  ret &= blob.try_insert_back(static_cast<double>(0x11));
  ret &= blob.try_insert_back(static_cast<long double>(0x12));

  REQUIRE(ret);
  REQUIRE(blob.get_total_number_of_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_free_bytes() == 1);
  REQUIRE(blob.get_number_of_used_bytes() == buffer_size - 1);
  REQUIRE_FALSE(blob.try_insert_back(static_cast<long double>(0x13)));

  long double        val_long_double;
  double             val_double;
  float              val_float;
  unsigned long long val_unsigned_long_long;
  signed long long   val_signed_long_long;
  unsigned long      val_unsigned_long;
  signed long        val_signed_long;
  unsigned int       val_unsigned_int;
  signed int         val_signed_int;
  unsigned short     val_unsigned_short;
  signed short       val_signed_short;
  char32_t           val_char32_t;
  char16_t           val_char16_t;
  char8_t            val_char8_t;
  unsigned char      val_unsigned_char;
  signed char        val_signed_char;
  char               val_char;
  std::byte          val_byte;

  ret &= blob.try_extract_back(val_long_double);
  ret &= blob.try_extract_back(val_double);
  ret &= blob.try_extract_back(val_float);
  ret &= blob.try_extract_back(val_unsigned_long_long);
  ret &= blob.try_extract_back(val_signed_long_long);
  ret &= blob.try_extract_back(val_unsigned_long);
  ret &= blob.try_extract_back(val_signed_long);
  ret &= blob.try_extract_back(val_unsigned_int);
  ret &= blob.try_extract_back(val_signed_int);
  ret &= blob.try_extract_back(val_unsigned_short);
  ret &= blob.try_extract_back(val_signed_short);
  ret &= blob.try_extract_back(val_char32_t);
  ret &= blob.try_extract_back(val_char16_t);
  ret &= blob.try_extract_back(val_char8_t);
  ret &= blob.try_extract_back(val_unsigned_char);
  ret &= blob.try_extract_back(val_signed_char);
  ret &= blob.try_extract_back(val_char);
  ret &= blob.try_extract_back(val_byte);

  REQUIRE(val_long_double == static_cast<long double>(0x12));
  REQUIRE(val_double == static_cast<double>(0x11));
  REQUIRE(val_float == static_cast<float>(0x10));
  REQUIRE(val_unsigned_long_long == static_cast<unsigned long long>(0x0F));
  REQUIRE(val_signed_long_long == static_cast<signed long long>(0x0E));
  REQUIRE(val_unsigned_long == static_cast<unsigned long>(0x0D));
  REQUIRE(val_signed_long == static_cast<signed long>(0x0C));
  REQUIRE(val_unsigned_int == static_cast<unsigned int>(0x0B));
  REQUIRE(val_signed_int == static_cast<signed int>(0x0A));
  REQUIRE(val_unsigned_short == static_cast<unsigned short>(0x09));
  REQUIRE(val_signed_short == static_cast<signed short>(0x08));
  REQUIRE(val_char32_t == static_cast<char32_t>(0x07));
  REQUIRE(val_char16_t == static_cast<char16_t>(0x06));
  REQUIRE(val_char8_t == static_cast<char8_t>(0x05));
  REQUIRE(val_unsigned_char == static_cast<unsigned char>(0x04));
  REQUIRE(val_signed_char == static_cast<signed char>(0x03));
  REQUIRE(val_char == static_cast<char>(0x02));
  REQUIRE(val_byte == static_cast<std::byte>(0x01));

  REQUIRE(ret);
  REQUIRE(blob.get_total_number_of_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_free_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  REQUIRE(blob.try_insert_back(static_cast<long long>(0x13)));

  REQUIRE(blob.get_total_number_of_bytes() == buffer_size);
  REQUIRE(blob.get_number_of_free_bytes() == buffer_size - sizeof(long long));
  REQUIRE(blob.get_number_of_used_bytes() == sizeof(long long));
}

TEST_CASE()
{
  std::byte buffer[14]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_total_number_of_bytes() == 14);
  REQUIRE(blob.get_number_of_free_bytes() == 14);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  REQUIRE_FALSE(blob.try_adjust_position(1000));

  REQUIRE(blob.get_number_of_free_bytes() == 14);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  REQUIRE_FALSE(blob.try_adjust_position(-1000));

  REQUIRE(blob.get_number_of_free_bytes() == 14);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  REQUIRE(blob.try_adjust_position(5));

  REQUIRE(blob.get_number_of_free_bytes() == 9);
  REQUIRE(blob.get_number_of_used_bytes() == 5);

  REQUIRE(blob.try_adjust_position(-3));

  REQUIRE(blob.get_number_of_free_bytes() == 12);
  REQUIRE(blob.get_number_of_used_bytes() == 2);
}

TEST_CASE()
{
  std::byte buffer[14]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_number_of_used_bytes() == 0);
  REQUIRE_THROWS(blob.insert(1, static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.insert(1, static_cast<uint16_t>(0), std::endian::little));
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.insert_back(static_cast<uint32_t>(0x00AA'BBEF), std::endian::big);
  blob.insert_front(static_cast<uint32_t>(0xDECC'DDAD), std::endian::big);
  blob.insert(4, static_cast<uint16_t>(0xBE00), std::endian::big);
  REQUIRE(blob.get_number_of_used_bytes() == 10);

  REQUIRE_FALSE(blob.try_insert(1, std::span<std::byte>(&buffer[0], std::numeric_limits<std::size_t>::max())));
  REQUIRE_FALSE(blob.try_insert_reverse(1, std::span<std::byte>(&buffer[0], std::numeric_limits<std::size_t>::max())));

  REQUIRE_FALSE(blob.try_insert(0, std::span<std::byte>(&buffer[0], std::numeric_limits<std::size_t>::max())));
  REQUIRE_FALSE(blob.try_insert_reverse(0, std::span<std::byte>(&buffer[0], std::numeric_limits<std::size_t>::max())));

  REQUIRE(blob.extract<uint16_t>(1, std::endian::little) == 0xDDCC);
  REQUIRE(blob.extract<uint32_t>(3, std::endian::big) == 0x0000'AABB);
  REQUIRE(blob.extract_front<uint32_t>(std::endian::big) == 0xDEADBEEF);
  REQUIRE(blob.get_number_of_used_bytes() == 0);
}

TEST_CASE()
{
  std::byte buffer[14]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_number_of_used_bytes() == 0);
  REQUIRE_THROWS(blob.overwrite(0, static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.overwrite(0, static_cast<uint16_t>(0), std::endian::little));
  REQUIRE_THROWS(blob.overwrite_back(static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.overwrite_back(static_cast<uint16_t>(0), std::endian::little));
  REQUIRE_THROWS(blob.overwrite_front(static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.overwrite_front(static_cast<uint16_t>(0), std::endian::little));
  REQUIRE(blob.get_number_of_used_bytes() == 0);
  REQUIRE(blob.try_set_position(10));
  REQUIRE(blob.get_number_of_used_bytes() == 10);

  blob.overwrite_back(static_cast<uint32_t>(0x00AA'BBEF), std::endian::big);
  blob.overwrite_front(static_cast<uint32_t>(0xDECC'DDAD), std::endian::big);
  blob.overwrite(4, static_cast<uint16_t>(0xBE00), std::endian::big);
  REQUIRE(blob.get_number_of_used_bytes() == 10);

  REQUIRE(blob.extract<uint16_t>(1, std::endian::little) == 0xDDCC);
  REQUIRE(blob.extract<uint32_t>(3, std::endian::big) == 0x0000'AABB);
  REQUIRE(blob.extract_front<uint32_t>(std::endian::big) == 0xDEADBEEF);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.clear();
  blob.adjust_position(10);
  REQUIRE(blob.get_number_of_used_bytes() == 10);

  std::byte dummy[6] = {
    std::byte(0xAA), std::byte(0xBB), std::byte(0xCC), std::byte(0xDD), std::byte(0xEE), std::byte(0xFF),
  };
}

TEST_CASE()
{
  std::byte buffer[6]{
    std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04), std::byte(0x05), std::byte(0x06),
  };

  wlib::blob::MemoryBlob const blob{ buffer, sizeof(buffer) };

  REQUIRE(blob.get_total_number_of_bytes() == 6);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 6);

  uint32_t const val_1 = blob.read_front<uint32_t>(std::endian::little);
  uint32_t const val_2 = blob.read_front<uint32_t>(std::endian::big);
  uint32_t const val_3 = blob.read_back<uint32_t>(std::endian::little);
  uint32_t const val_4 = blob.read_back<uint32_t>(std::endian::big);
  uint32_t const val_5 = blob.read<uint32_t>(1, std::endian::little);
  uint32_t const val_6 = blob.read<uint32_t>(1, std::endian::big);

  REQUIRE(val_1 == 0x0403'0201);
  REQUIRE(val_2 == 0x0102'0304);
  REQUIRE(val_3 == 0x0605'0403);
  REQUIRE(val_4 == 0x0304'0506);
  REQUIRE(val_5 == 0x0504'0302);
  REQUIRE(val_6 == 0x0203'0405);

  REQUIRE_THROWS(blob.read<uint32_t>(std::numeric_limits<std::size_t>::max(), std::endian::little));
  REQUIRE_THROWS(blob.read<uint32_t>(std::numeric_limits<std::size_t>::max() - 4, std::endian::little));
  REQUIRE_THROWS(blob.read<uint32_t>(std::numeric_limits<std::size_t>::max(), std::endian::big));
  REQUIRE_THROWS(blob.read<uint32_t>(std::numeric_limits<std::size_t>::max() - 4, std::endian::big));

  REQUIRE_THROWS(blob.read<uint32_t>(3));
  REQUIRE_THROWS(blob.read_front<uint64_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_front<uint64_t>(std::endian::big));
  REQUIRE_THROWS(blob.read_back<uint64_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_back<uint64_t>(std::endian::big));
}

TEST_CASE()
{
  std::byte buffer[6]{
    std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04), std::byte(0x05), std::byte(0x06),
  };

  wlib::blob::MemoryBlob blob{ buffer, sizeof(buffer) };

  REQUIRE(blob.get_total_number_of_bytes() == 6);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 6);

  blob.overwrite_front(static_cast<uint16_t>(0xAABB), std::endian::little);
  blob.overwrite_back(static_cast<uint16_t>(0xEEFF), std::endian::big);
  blob.overwrite(2, static_cast<uint16_t>(0xCCDD), std::endian::big);

  REQUIRE(buffer[0] == std::byte(0xBB));
  REQUIRE(buffer[1] == std::byte(0xAA));
  REQUIRE(buffer[2] == std::byte(0xCC));
  REQUIRE(buffer[3] == std::byte(0xDD));
  REQUIRE(buffer[4] == std::byte(0xEE));
  REQUIRE(buffer[5] == std::byte(0xFF));

  REQUIRE_THROWS(blob.overwrite(3, static_cast<uint32_t>(0)));
  REQUIRE_THROWS(blob.overwrite_front(static_cast<uint64_t>(0)));
  REQUIRE_THROWS(blob.overwrite_back(static_cast<uint64_t>(0)));
}

TEST_CASE()
{
  std::byte buffer[6]{};

  wlib::blob::MemoryBlob blob{ buffer };

  REQUIRE(blob.get_total_number_of_bytes() == 6);
  REQUIRE(blob.get_number_of_free_bytes() == 6);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.insert_back(static_cast<uint16_t>(0xEEFF), std::endian::big);
  blob.insert_front(static_cast<uint16_t>(0xAABB), std::endian::little);
  blob.insert(2, static_cast<uint16_t>(0xCCDD), std::endian::big);

  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 6);

  REQUIRE(buffer[0] == std::byte(0xBB));
  REQUIRE(buffer[1] == std::byte(0xAA));
  REQUIRE(buffer[2] == std::byte(0xCC));
  REQUIRE(buffer[3] == std::byte(0xDD));
  REQUIRE(buffer[4] == std::byte(0xEE));
  REQUIRE(buffer[5] == std::byte(0xFF));

  REQUIRE_THROWS(blob.insert(3, static_cast<uint32_t>(0)));
  REQUIRE_THROWS(blob.insert_front(static_cast<uint64_t>(0)));
  REQUIRE_THROWS(blob.insert_back(static_cast<uint64_t>(0)));
}

TEST_CASE()
{
  std::byte buffer[9]{
    std::byte(0x01), std::byte(0x02), std::byte(0xDE), std::byte(0xAD), std::byte(0x05), std::byte(0x05), std::byte(0xBE), std::byte(0xEF), std::byte(0x06),
  };

  wlib::blob::MemoryBlob blob{ buffer, sizeof(buffer) };

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 9);

  blob.remove_back(1);
  blob.remove_front(2);
  blob.remove(2, 2);

  REQUIRE(blob.get_number_of_free_bytes() == 5);
  REQUIRE(blob.get_number_of_used_bytes() == 4);

  REQUIRE(buffer[0] == std::byte(0xDE));
  REQUIRE(buffer[1] == std::byte(0xAD));
  REQUIRE(buffer[2] == std::byte(0xBE));
  REQUIRE(buffer[3] == std::byte(0xEF));

  REQUIRE_THROWS(blob.remove(0, 5));
  REQUIRE_THROWS(blob.remove(5, 1));
  REQUIRE_THROWS(blob.remove_front(5));
  REQUIRE_THROWS(blob.remove_back(5));
}

TEST_CASE()
{
  std::byte buffer[9]{
    std::byte(0x01), std::byte(0x02), std::byte(0xDE), std::byte(0xAD), std::byte(0x05), std::byte(0x05), std::byte(0xBE), std::byte(0xEF), std::byte(0x06),
  };

  wlib::blob::MemoryBlob blob{ buffer, sizeof(buffer) };

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 9);

  REQUIRE(blob.extract_back<uint8_t>() == 0x06);
  REQUIRE(blob.extract_front<uint16_t>(std::endian::big) == 0x0102);
  REQUIRE(blob.extract<uint16_t>(2, std::endian::little) == 0x0505);

  REQUIRE(blob.get_number_of_free_bytes() == 5);
  REQUIRE(blob.get_number_of_used_bytes() == 4);

  REQUIRE(blob.extract_back<uint32_t>(std::endian::big) == 0xDEADBEEF);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  REQUIRE_THROWS(blob.extract<uint32_t>(0));
  REQUIRE_THROWS(blob.extract<uint32_t>(1));
  REQUIRE_THROWS(blob.extract_front<uint32_t>());
  REQUIRE_THROWS(blob.extract_back<uint32_t>());
}

TEST_CASE()
{

  std::byte buffer[9]{
    std::byte(0x01), std::byte(0x02), std::byte(0xDE), std::byte(0xAD), std::byte(0x05), std::byte(0x05), std::byte(0xBE), std::byte(0xEF), std::byte(0x06),
  };

  wlib::blob::MemoryBlob blob{ buffer, sizeof(buffer) };
  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 9);

  REQUIRE(buffer[2] == std::byte(0xDE));
  blob.remove<uint16_t>(2);
  REQUIRE(blob.get_number_of_used_bytes() == 7);
  REQUIRE(buffer[2] == std::byte(0x05));
  REQUIRE(buffer[6] == std::byte(0x06));

  blob.remove_back<uint16_t>();
  REQUIRE(blob.get_number_of_used_bytes() == 5);
  REQUIRE(buffer[3] == std::byte(0x05));
  REQUIRE(buffer[4] == std::byte(0xBE));

  blob.remove_front<uint16_t>();
  REQUIRE(blob.get_number_of_used_bytes() == 3);
  REQUIRE(buffer[0] == std::byte(0x05));
  REQUIRE(buffer[1] == std::byte(0x05));
  REQUIRE(buffer[2] == std::byte(0xBE));

  REQUIRE_THROWS(blob.remove<uint32_t>(2));
  REQUIRE_THROWS(blob.remove_back<uint32_t>());
  REQUIRE_THROWS(blob.remove_front<uint32_t>());
}

TEST_CASE()
{
  std::byte const buffer[9]{
    std::byte(0x01), std::byte(0x02), std::byte(0xDE), std::byte(0xAD), std::byte(0x05), std::byte(0x05), std::byte(0xBE), std::byte(0xEF), std::byte(0x06),
  };

  wlib::blob::ConstMemoryBlob blob{ buffer };

  std::span<std::byte const> spn_1 = blob.get_span();
  REQUIRE(spn_1.data() == buffer);
  REQUIRE(spn_1.size() == 9);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 9);
  REQUIRE(blob.get_number_of_processed_bytes() == 0);

  uint16_t val1  = blob.extract_front<uint16_t>(std::endian::little);
  uint16_t val2a = blob.read_front<uint16_t>(std::endian::big);
  uint16_t val2  = blob.extract_front<uint16_t>(std::endian::big);

  REQUIRE(val1 == 0x0201);
  REQUIRE(val2a == val2);
  REQUIRE(val2 == 0xDEAD);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 5);
  REQUIRE(blob.get_number_of_processed_bytes() == 4);

  uint16_t val3  = blob.extract_back<uint16_t>(std::endian::little);
  uint16_t val4a = blob.read_back<uint16_t>(std::endian::big);
  uint16_t val4  = blob.extract_back<uint16_t>(std::endian::big);

  REQUIRE(val3 == 0x06EF);
  REQUIRE(val4a == val4);
  REQUIRE(val4 == 0x05BE);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 1);
  REQUIRE(blob.get_number_of_processed_bytes() == 8);

  std::span<std::byte const> spn_2 = blob.get_span();
  REQUIRE(spn_2.data() == buffer + 4);
  REQUIRE(spn_2.size() == 1);

  REQUIRE_THROWS(blob.extract_back<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.extract_back<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.extract_front<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.extract_front<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.read_back<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_back<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.read_front<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_front<uint16_t>(std::endian::big));

  blob.reset();
  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 9);
  REQUIRE(blob.get_number_of_processed_bytes() == 0);

  uint16_t val5 = blob.read<uint16_t>(2, std::endian::big);
  uint16_t val6 = blob.read<uint16_t>(5, std::endian::little);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 9);
  REQUIRE(blob.get_number_of_processed_bytes() == 0);

  REQUIRE(val5 == 0xDEAD);
  REQUIRE(val6 == 0xBE05);

  blob.remove_front(3);
  blob.remove_back(3);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 3);
  REQUIRE(blob.get_number_of_processed_bytes() == 6);

  REQUIRE_THROWS(blob.remove_front(4));
  REQUIRE_THROWS(blob.remove_back(4));

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 3);
  REQUIRE(blob.get_number_of_processed_bytes() == 6);

  REQUIRE_THROWS(blob.read<uint32_t>(2, std::endian::big));
  REQUIRE_THROWS(blob.read<uint32_t>(5, std::endian::little));

  blob.reset();
  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_remaining_bytes() == 9);
  REQUIRE(blob.get_number_of_processed_bytes() == 0);

  blob.remove_front(1);
  blob.remove_back(1);

  uint32_t val7 = 0xAABBCCDD;
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max(), val7, std::endian::big));
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max() - 1, val7, std::endian::big));
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max() - 5, val7, std::endian::big));
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max(), val7, std::endian::little));
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max() - 1, val7, std::endian::little));
  REQUIRE_FALSE(blob.try_read(std::numeric_limits<std::size_t>::max() - 5, val7, std::endian::little));
  REQUIRE(val7 == 0xAABBCCDD);
}

TEST_CASE()
{

  wlib::blob::StaticBlob<9> blob;
  blob.insert_back(static_cast<uint32_t>(0xDEADBEEF), std::endian::big);
  blob.insert(2, static_cast<uint16_t>(0x0505), std::endian::little);
  blob.insert_front(static_cast<uint16_t>(0x0201), std::endian::little);
  blob.insert_back(static_cast<uint8_t>(0x06));

  std::span<std::byte const> spn_1 = blob.get_span();
  REQUIRE(spn_1.size() == 9);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_used_bytes() == 9);
  REQUIRE(blob.get_number_of_free_bytes() == 0);

  uint16_t val1  = blob.extract_front<uint16_t>(std::endian::little);
  uint16_t val2a = blob.read_front<uint16_t>(std::endian::big);
  uint16_t val2  = blob.extract_front<uint16_t>(std::endian::big);

  REQUIRE(val1 == 0x0201);
  REQUIRE(val2a == val2);
  REQUIRE(val2 == 0xDEAD);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_used_bytes() == 5);
  REQUIRE(blob.get_number_of_free_bytes() == 4);

  uint16_t val3  = blob.extract_back<uint16_t>(std::endian::little);
  uint16_t val4a = blob.read_back<uint16_t>(std::endian::big);
  uint16_t val4  = blob.extract_back<uint16_t>(std::endian::big);

  REQUIRE(val3 == 0x06EF);
  REQUIRE(val4a == val4);
  REQUIRE(val4 == 0x05BE);

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_used_bytes() == 1);
  REQUIRE(blob.get_number_of_free_bytes() == 8);

  std::span<std::byte const> spn_2 = blob.get_span();
  REQUIRE(spn_2.size() == 1);

  REQUIRE_THROWS(blob.extract_back<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.extract_back<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.extract_front<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.extract_front<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.read_back<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_back<uint16_t>(std::endian::big));
  REQUIRE_THROWS(blob.read_front<uint16_t>(std::endian::little));
  REQUIRE_THROWS(blob.read_front<uint16_t>(std::endian::big));

  wlib::blob::StaticBlob<9> blob2(blob);
  blob.clear();

  REQUIRE(blob.get_total_number_of_bytes() == 9);
  REQUIRE(blob.get_number_of_used_bytes() == 0);
  REQUIRE(blob.get_number_of_free_bytes() == 9);

  REQUIRE(blob2.get_total_number_of_bytes() == 9);
  REQUIRE(blob2.get_number_of_used_bytes() == 1);
  REQUIRE(blob2.get_number_of_free_bytes() == 8);

  blob.insert_back(static_cast<uint64_t>(0x0102'0304'0506'0708));

  blob2 = blob;
  REQUIRE(blob2.get_total_number_of_bytes() == 9);
  REQUIRE(blob2.get_number_of_used_bytes() == 8);
  REQUIRE(blob2.get_number_of_free_bytes() == 1);
}

TEST_CASE()
{
  std::byte const buffer[9]{
    std::byte(0x01), std::byte(0x02), std::byte(0xDE), std::byte(0xAD), std::byte(0x05), std::byte(0x05), std::byte(0xBE), std::byte(0xEF), std::byte(0x06),
  };

  wlib::blob::StaticBlob<27> blob;

  REQUIRE(blob.get_total_number_of_bytes() == 27);
  REQUIRE(blob.get_number_of_free_bytes() == 27);
  REQUIRE(blob.get_number_of_used_bytes() == 0);

  blob.insert_back(buffer);
  blob.insert_front(buffer);
  blob.insert(9, buffer);

  REQUIRE(blob.get_total_number_of_bytes() == 27);
  REQUIRE(blob.get_number_of_free_bytes() == 0);
  REQUIRE(blob.get_number_of_used_bytes() == 27);

  REQUIRE_THROWS(blob.insert_back(buffer));
  REQUIRE_THROWS(blob.insert_front(buffer));
  REQUIRE_THROWS(blob.insert(9, buffer));

  REQUIRE_THROWS(blob.insert_back(static_cast<uint16_t>(0), std::endian::little));
  REQUIRE_THROWS(blob.insert_back(static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.insert_front(static_cast<uint16_t>(0), std::endian::little));
  REQUIRE_THROWS(blob.insert_front(static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.insert(9, static_cast<uint16_t>(0), std::endian::big));
  REQUIRE_THROWS(blob.insert(9, static_cast<uint16_t>(0), std::endian::little));
}

namespace my_space
{
  struct S
  {
    char   a;
    int    b;
    double c;

    auto operator<=>(S const&) const = default;

    friend wlib::blob::MemoryBlob&  operator<<(wlib::blob::MemoryBlob&, S const&);
    template <typename T> friend T& operator>>(T&, S&);
  };

  class SS
  {
  public:
    SS(char a, int b, double c, S s)
        : a(a)
        , b(b)
        , c(c)
        , d(s)
    {
    }

    auto operator<=>(SS const&) const = default;

  private:
    char   a;
    int    b;
    double c;
    S      d;

    friend wlib::blob::MemoryBlob&  operator<<(wlib::blob::MemoryBlob&, SS const&);
    template <typename T> friend T& operator>>(T&, std::optional<SS>&);
  };
}    // namespace my_space

TEST_CASE()
{
  std::byte buffer[(sizeof(char) + sizeof(int) + sizeof(double)) * 3] = {};

  wlib::blob::MemoryBlob blob{ buffer };

  my_space::S  s  = { 's', 3, 3.14 };
  my_space::SS ss = { 't', 4, 4.2, s };
  blob << s << ss;

  wlib::blob::ConstMemoryBlob c_blob{ buffer };

  my_space::S                 s_out2;
  std::optional<my_space::SS> ss_out2;
  c_blob >> s_out2 >> ss_out2;

  my_space::S                 s_out;
  std::optional<my_space::SS> ss_out;
  blob >> s_out >> ss_out;

  REQUIRE(s == s_out);
  REQUIRE(s == s_out2);

  REQUIRE(ss == ss_out.value());
  REQUIRE(ss == ss_out2.value());
}

namespace my_space
{
  wlib::blob::MemoryBlob&  operator<<(wlib::blob::MemoryBlob& blob, my_space::S const& s) { return blob << s.a << s.b << s.c; }
  template <typename T> T& operator>>(T& blob, my_space::S& s) { return blob >> s.a >> s.b >> s.c; }

  wlib::blob::MemoryBlob&  operator<<(wlib::blob::MemoryBlob& blob, my_space::SS const& ss) { return blob << ss.a << ss.b << ss.c << ss.d; }
  template <typename T> T& operator>>(T& blob, std::optional<my_space::SS>& ss)
  {
    char   a;
    int    b;
    double c;
    S      d;

    blob >> a >> b >> c >> d;
    ss = SS(a, b, c, d);

    return blob;
  }
}    // namespace my_space

TEST_CASE()
{
  std::byte buffer[(sizeof(char) + sizeof(int) + sizeof(double)) * 3] = {};

  wlib::blob::MemoryBlob blob{ buffer };

  my_space::S s[3] = { { 's', 3, 3.14 }, { 't', 4, 6.28 }, { 'u', 5, 9.42 } };

  blob << std::span<my_space::S const>(s);

  wlib::blob::ConstMemoryBlob c_blob{ buffer };

  my_space::S s_out2[3];
  c_blob >> std::span<my_space::S>(s_out2);

  my_space::S s_out[3];
  blob >> std::span<my_space::S>(s_out);

  for (std::size_t i = 0; i < 3; i++)
  {
    REQUIRE(s[i] == s_out[i]);
    REQUIRE(s[i] == s_out2[i]);
  }
}

TEST_CASE()
{

  std::array<std::byte, 10> ref_1 = {
    std::byte(0xCC), std::byte(0xCC), std::byte(0xDD), std::byte(0xDD), std::byte(0xDD),
    std::byte(0xCC), std::byte(0xAA), std::byte(0xBB), std::byte(0xBB), std::byte(0xBB),
  };
  std::array<std::byte, 10> ref_2 = {
    std::byte(0x0C), std::byte(0x0C), std::byte(0x0D), std::byte(0x0D), std::byte(0x0D),
    std::byte(0x0C), std::byte(0x0A), std::byte(0x0B), std::byte(0x0B), std::byte(0x0B),
  };

  std::array<std::byte, 10> mem;
  wlib::blob::MemoryBlob    blob{ mem };

  REQUIRE(blob.try_insert_back(std::byte(0xAA)));
  REQUIRE(blob.try_insert_back(std::byte(0xBB), 3));
  REQUIRE(blob.try_insert_front(std::byte(0xCC), 3));
  REQUIRE(blob.try_insert(2, std::byte(0xDD), 3));

  REQUIRE(blob.try_insert(2, std::byte(0xDD), 0));

  REQUIRE(!blob.try_insert_back(std::byte(0x00), 3));
  REQUIRE(!blob.try_insert_front(std::byte(0x00), 3));
  REQUIRE(!blob.try_insert(5, std::byte(0x00), 3));

  REQUIRE(mem == ref_1);

  blob.clear();

  REQUIRE_NOTHROW(blob.insert_back(std::byte(0x0A)));
  REQUIRE_NOTHROW(blob.insert_back(std::byte(0x0B), 3));
  REQUIRE_NOTHROW(blob.insert_front(std::byte(0x0C), 3));
  REQUIRE_NOTHROW(blob.insert(2, std::byte(0x0D), 3));

  REQUIRE_NOTHROW(blob.insert(2, std::byte(0x0D), 0));

  REQUIRE_THROWS(blob.insert_back(std::byte(0x00), 3));
  REQUIRE_THROWS(blob.insert_front(std::byte(0x00), 3));
  REQUIRE_THROWS(blob.insert(5, std::byte(0x00), 3));

  REQUIRE(mem == ref_2);
}
