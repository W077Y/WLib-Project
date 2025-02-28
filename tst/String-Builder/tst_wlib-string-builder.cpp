#include <iostream>
#include <string>
#include <ut_catch.hpp>
//
#include <wlib.hpp>

namespace test_float_value_format_t
{
  using test_t = wlib::Internal::floating_point_number_format_t;

  namespace static_test_1
  {
    auto constexpr fmt = test_t{ "+7.3" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::allways);
    static_assert(fmt.get_width() == 7);
    static_assert(fmt.get_precision() == 3);
    static_assert(fmt.get_format() == std::chars_format::general);
  }    // namespace static_test_1
  namespace static_test_2
  {
    auto constexpr fmt = test_t{ "25.23f" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 25);
    static_assert(fmt.get_precision() == 23);
    static_assert(fmt.get_format() == std::chars_format::fixed);
  }    // namespace static_test_2
  namespace static_test_3
  {
    auto constexpr fmt = test_t{ "25.23E" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 25);
    static_assert(fmt.get_precision() == 23);
    static_assert(fmt.get_format() == std::chars_format::scientific);
  }    // namespace static_test_3
  namespace static_test_4
  {
    auto constexpr fmt = test_t{ ".23E" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 0);
    static_assert(fmt.get_precision() == 23);
    static_assert(fmt.get_format() == std::chars_format::scientific);
  }    // namespace static_test_4
  namespace static_test_5
  {
    auto constexpr fmt = test_t{ "E" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 0);
    static_assert(fmt.get_precision() == -1);
    static_assert(fmt.get_format() == std::chars_format::scientific);
  }    // namespace static_test_5
  namespace static_test_6
  {
    auto constexpr fmt = test_t{ "" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 0);
    static_assert(fmt.get_precision() == -1);
    static_assert(fmt.get_format() == std::chars_format::general);
  }    // namespace static_test_6
  namespace static_test_7
  {
    auto constexpr fmt = test_t{ " 17.9X" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 17);
    static_assert(fmt.get_precision() == 9);
    static_assert(fmt.get_format() == std::chars_format::hex);
  }    // namespace static_test_7
}    // namespace test_float_value_format_t

namespace test_int_value_format_t
{
  using test_t = wlib::Internal::integer_number_format_t;

  namespace static_test_1
  {
    auto constexpr fmt = test_t{ "+7" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::allways);
    static_assert(fmt.get_width() == 7);
    static_assert(fmt.get_base() == 10);
  }    // namespace static_test_1
  namespace static_test_2
  {
    auto constexpr fmt = test_t{ " 7" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 7);
    static_assert(fmt.get_base() == 10);
  }    // namespace static_test_2
  namespace static_test_3
  {
    auto constexpr fmt = test_t{ "-12x" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 12);
    static_assert(fmt.get_base() == 16);
  }    // namespace static_test_3
  namespace static_test_4
  {
    auto constexpr fmt = test_t{ "12b" };
    static_assert(fmt.get_sign() == test_t::sign_mode_t::negativ_only);
    static_assert(fmt.get_width() == 12);
    static_assert(fmt.get_base() == 2);
  }    // namespace static_test_4
}    // namespace test_int_value_format_t

TEST_CASE()
{
  wlib::StringBuilder<120> builder = {};

  REQUIRE(builder.get_length() == 0);
  REQUIRE(builder.get_remaining_chars() == 120UL);
}

TEST_CASE()
{
  constexpr std::size_t            buffer_size = 120;
  wlib::StringBuilder<buffer_size> builder     = {};

  REQUIRE(builder.get_remaining_chars() == buffer_size);

  constexpr char const*      test_c_str    = "Hallo this is a test c string.";
  constexpr std::string_view test_str_view = "Hallo this is a test string-view.";

  builder << test_c_str;
  REQUIRE(std::string_view(builder.as_c_str()) == std::string_view(test_c_str));
  REQUIRE(builder.as_string_view() == std::string_view(test_c_str));
  REQUIRE(builder.get_length() == std::string_view(test_c_str).length());
  REQUIRE(builder.get_remaining_chars() == buffer_size - std::string_view(test_c_str).length());

  builder.clear();
  REQUIRE(builder.get_length() == 0);
  REQUIRE(builder.get_remaining_chars() == buffer_size);

  builder << test_str_view;
  REQUIRE(std::string_view(builder.as_c_str()) == test_str_view);
  REQUIRE(builder.as_string_view() == std::string_view(test_str_view));
  REQUIRE(builder.get_length() == test_str_view.length());
  REQUIRE(builder.get_remaining_chars() == 120 - std::string_view(test_str_view).length());

  builder.clear();
  REQUIRE(builder.get_length() == 0);
  REQUIRE(builder.get_remaining_chars() == buffer_size);

  builder << test_c_str << test_str_view;
  std::string ref_str = {};
  ref_str.append(test_c_str).append(test_str_view);
  REQUIRE(std::string_view(builder.as_c_str()) == ref_str);
  REQUIRE(builder.as_string_view() == ref_str);
  REQUIRE(builder.get_length() == ref_str.length());
  REQUIRE(builder.get_remaining_chars() == buffer_size - ref_str.length());
}

TEST_CASE()
{
  constexpr std::size_t            buffer_size = 120;
  wlib::StringBuilder<buffer_size> builder     = {};

  using wlib::fmt;

  builder << fmt("+5.3", 3.14f) << ", ";
  builder << fmt("-5.3", 3.14) << ", ";
  builder << fmt("5.3", static_cast<long double>(3.14)) << "\n";
  builder << fmt("5", static_cast<char>(1)) << "\n";
  builder << fmt("5", static_cast<unsigned char>(2)) << "\n";
  builder << fmt("5", static_cast<signed char>(3)) << "\n";
  builder << fmt("5", static_cast<signed short>(4)) << "\n";
  builder << fmt("5", static_cast<unsigned short>(5)) << "\n";
  builder << fmt("5", static_cast<signed int>(6)) << "\n";
  builder << fmt("5", static_cast<unsigned int>(7)) << "\n";
  builder << fmt("5", static_cast<signed long>(8)) << "\n";
  builder << fmt("5", static_cast<unsigned long>(9)) << "\n";
  builder << fmt("5", static_cast<signed long int>(10)) << "\n";
  builder << fmt("5", static_cast<unsigned long int>(11)) << "\n";
  builder << fmt("5", static_cast<signed long long>(12)) << "\n";
  builder << fmt("5", static_cast<unsigned long>(13)) << "\n";

  constexpr std::string_view ref = "+3.14,  3.14,  3.14\n"
                                   "    1\n"
                                   "    2\n"
                                   "    3\n"
                                   "    4\n"
                                   "    5\n"
                                   "    6\n"
                                   "    7\n"
                                   "    8\n"
                                   "    9\n"
                                   "   10\n"
                                   "   11\n"
                                   "   12\n"
                                   "   13\n";

  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t            buffer_size = 120;
  wlib::StringBuilder<buffer_size> builder     = {};

  using wlib::fmt;

  builder << fmt("+9.3f", 10.5f) << "\n";
  builder << fmt("-9.3f", -10.5f) << "\n";
  builder << fmt("9.3f", 10.5) << "\n";

  //                               "123456789"
  constexpr std::string_view ref = "  +10.500\n"
                                   "  -10.500\n"
                                   "   10.500\n";
  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t            buffer_size = 120;
  wlib::StringBuilder<buffer_size> builder     = {};

  using wlib::fmt;

  builder << fmt("+9", 105) << "\n";
  builder << fmt("-9", -105) << "\n";
  builder << fmt("9", 105) << "\n";

  //                               "123456789"
  constexpr std::string_view ref = "     +105\n"
                                   "     -105\n"
                                   "      105\n";
  REQUIRE(builder.as_string_view() == ref);
}



