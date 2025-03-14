#include <iostream>
#include <string>
#include <ut_catch.hpp>
//
#include <wlib.hpp>

TEST_CASE()
{
  wlib::StaticStringBuilder<120> builder = {};

  REQUIRE(builder.get_length() == 0);
  REQUIRE(builder.get_remaining_chars() == 120UL);
}

TEST_CASE()
{
  constexpr std::size_t                  buffer_size = 120;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

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
  constexpr std::size_t                  buffer_size = 200;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

  namespace N                                     = wlib::format;
  constexpr wlib::formator_t<float>       ftor_f  = { "", ", ", { 5, 3, N::sign_mode_t::both, N::format_t::fixed } };
  constexpr wlib::formator_t<double>      ftor_d  = { "", ", ", { 5, 3, N::sign_mode_t::space, N::format_t::fixed } };
  constexpr wlib::formator_t<long double> ftor_ld = { "", "\n", { 5, 3, N::sign_mode_t::negativ_only, N::format_t::fixed } };

  constexpr wlib::formator_t<char>               ftor_c   = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed char>        ftor_sc  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned char>      ftor_uc  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed short>       ftor_ss  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned short>     ftor_us  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed int>         ftor_si  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned int>       ftor_ui  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed long>        ftor_sl  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned long>      ftor_ul  = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed long int>    ftor_sli = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned long int>  ftor_uli = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<signed long long>   ftor_sll = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<unsigned long long> ftor_ull = { " - ", "\n", { 5, N::sign_mode_t::negativ_only } };

  using wlib::fmt;
  builder << fmt(ftor_f, 3.14f);
  builder << fmt(ftor_d, 3.14);
  builder << fmt(ftor_ld, static_cast<long double>(3.14));
  builder << fmt(ftor_c, static_cast<char>(1));
  builder << fmt(ftor_uc, static_cast<unsigned char>(2));
  builder << fmt(ftor_sc, static_cast<signed char>(3));
  builder << fmt(ftor_ss, static_cast<signed short>(4));
  builder << fmt(ftor_us, static_cast<unsigned short>(5));
  builder << fmt(ftor_si, static_cast<signed int>(6));
  builder << fmt(ftor_ui, static_cast<unsigned int>(7));
  builder << fmt(ftor_sl, static_cast<signed long>(8));
  builder << fmt(ftor_ul, static_cast<unsigned long>(9));
  builder << fmt(ftor_sli, static_cast<signed long int>(10));
  builder << fmt(ftor_uli, static_cast<unsigned long int>(11));
  builder << fmt(ftor_sll, static_cast<signed long long>(12));
  builder << fmt(ftor_ull, static_cast<unsigned long>(13));

  constexpr std::string_view ref = "+3.140,  3.140, 3.140\n"
                                   " -     1\n"
                                   " -     2\n"
                                   " -     3\n"
                                   " -     4\n"
                                   " -     5\n"
                                   " -     6\n"
                                   " -     7\n"
                                   " -     8\n"
                                   " -     9\n"
                                   " -    10\n"
                                   " -    11\n"
                                   " -    12\n"
                                   " -    13\n";

  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t                  buffer_size = 120;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

  namespace N                                = wlib::format;
  constexpr wlib::formator_t<float>  ftor_f1 = { "", "\n", { 9, 3, N::sign_mode_t::both, N::format_t::fixed } };
  constexpr wlib::formator_t<float>  ftor_f2 = { "", "\n", { 9, 3, N::sign_mode_t::negativ_only, N::format_t::fixed } };
  constexpr wlib::formator_t<double> ftor_d  = { "", "\n", { 9, 3, N::sign_mode_t::space, N::format_t::fixed } };

  using wlib::fmt;

  builder << fmt(ftor_f1, 10.5f);
  builder << fmt(ftor_f2, -10.5f);
  builder << fmt(ftor_d, 10.5);

  //                               "123456789"
  constexpr std::string_view ref = "  +10.500\n"
                                   "  -10.500\n"
                                   "   10.500\n";
  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t                  buffer_size = 120;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

  namespace N                             = wlib::format;
  constexpr wlib::formator_t<int> ftor_i1 = { "", "\n", { 9, N::sign_mode_t::both } };
  constexpr wlib::formator_t<int> ftor_i2 = { "", "\n", { 9, N::sign_mode_t::negativ_only } };
  constexpr wlib::formator_t<int> ftor_i3 = { "", "\n", { 9, N::sign_mode_t::space } };

  using wlib::fmt;

  builder << fmt(ftor_i1, 105);
  builder << fmt(ftor_i2, -105);
  builder << fmt(ftor_i3, 105);

  //                               "123456789"
  constexpr std::string_view ref = "     +105\n"
                                   "     -105\n"
                                   "      105\n";
  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t                  buffer_size = 120;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

  namespace N                                         = wlib::format;
  constexpr wlib::formator_t<std::string_view> ftor_l = { "aaa", "bbb\n", 5, N::alignment_t::left };
  constexpr wlib::formator_t<std::string_view> ftor_c = { "aaa", "bbb\n", 5, N::alignment_t::center };
  constexpr wlib::formator_t<std::string_view> ftor_r = { "aaa", "bbb\n", 5, N::alignment_t::right };

  using wlib::fmt;
  builder << fmt(ftor_l, "c");
  builder << fmt(ftor_c, std::string_view{ "c" });
  builder << fmt(ftor_r, "c");

  //                                  "12345"
  constexpr std::string_view ref = "aaac    bbb\n"
                                   "aaa  c  bbb\n"
                                   "aaa    cbbb\n";
  REQUIRE(builder.as_string_view() == ref);
}

TEST_CASE()
{
  constexpr std::size_t                  buffer_size = 120;
  wlib::StaticStringBuilder<buffer_size> builder     = {};

  using wlib::fmt;
  float  val_f = 59.0;
  double val_d = 60.3;
  int    val_i = 61;
  builder << "aaa" << val_f << "bbb\n";
  builder << "aaa" << val_d << "bbb\n";
  builder << "aaa" << val_i << "bbb\n";

  constexpr std::string_view ref = "aaa59bbb\n"
                                   "aaa60.3bbb\n"
                                   "aaa61bbb\n";
  REQUIRE(builder.as_string_view() == ref);
}
