#include <wlib-crc_16_ccitt.hpp>
#include <tst_input.hpp>
#include <ut_catch.hpp>

using T                            = wlib::crc::CRC_16_ccitt_false;
constexpr T::used_type ref_value   = 0x29B1;
constexpr T::used_type ref_value_2 = 0xA86D;

TEST_CASE("CRC 16_ccitt_false byte pointer")
{
  T check_value;
  REQUIRE(check_value.get() == check_value.get_inital_value());

  std::byte const* begin_ptr = reinterpret_cast<std::byte const*>(input_data_set.data());
  std::byte const* end_ptr   = reinterpret_cast<std::byte const*>(input_data_set.data() + input_data_set.size());
  REQUIRE(check_value(begin_ptr, end_ptr) == ref_value);
  REQUIRE(check_value.get() == ref_value);

  check_value.reset();
  REQUIRE(check_value.get() == check_value.get_inital_value());

  REQUIRE(check_value(begin_ptr, end_ptr) == ref_value);
  REQUIRE(check_value(begin_ptr, end_ptr) == ref_value_2);
  REQUIRE(check_value.get() == ref_value_2);
}

TEST_CASE("CRC 16_ccitt_false byte pointer length")
{
  T check_value;
  REQUIRE(check_value.get() == check_value.get_inital_value());

  std::byte const* begin_ptr = reinterpret_cast<std::byte const*>(input_data_set.data());
  REQUIRE(check_value(begin_ptr, input_data_set.size()) == ref_value);
  REQUIRE(check_value.get() == ref_value);

  check_value.reset();
  REQUIRE(check_value.get() == check_value.get_inital_value());
}

TEST_CASE("CRC 16_ccitt_false span<byte const>")
{
  T check_value;
  REQUIRE(check_value.get() == check_value.get_inital_value());

  REQUIRE(check_value(std::as_bytes(std::span(input_data_set))) == ref_value);
  REQUIRE(check_value.get() == ref_value);

  check_value.reset();
  REQUIRE(check_value.get() == check_value.get_inital_value());
}
