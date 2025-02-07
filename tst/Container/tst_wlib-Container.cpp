#include <algorithm>
#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

TEST_CASE()
{
  using T = wlib::container::circular_buffer_t<int, 10>;

  auto sum_elements = [](T const& con) -> int
  {
    int cnt = 0;
    for (auto const& el : con)
      cnt += el;
    return cnt;
  };

  T buffer;

  REQUIRE(buffer.occupied_entries() == 0);
  REQUIRE(sum_elements(buffer) == 0);
  int i = 1;
  for (; i < 10; i++)
  {
    buffer.push(i + 1);
    REQUIRE(buffer.occupied_entries() == i);
  }
  REQUIRE(sum_elements(buffer) == 54);

  for (; i < 20; i++)
  {
    buffer.push(i + 1);
    REQUIRE(buffer.occupied_entries() == 10);
  }
  REQUIRE(sum_elements(buffer) == (20 + 11) * 5);

  for (int j = 0; j < buffer.occupied_entries(); j++)
  {
    REQUIRE(buffer[j] == i - j);
  }

  {
    int j = 0;
    for (auto val : buffer)
    {
      REQUIRE(val == i - j);
      j++;
    }
  }

  buffer.clear();
  REQUIRE(buffer.occupied_entries() == 0);

  REQUIRE(std::ranges::count(buffer, 0) == 0);

  for (int j = 0; j < 3; j++)
    buffer.push(1);

  REQUIRE(std::ranges::count(buffer, 0) == 0);
  REQUIRE(std::ranges::count(buffer, 1) == 3);
}

TEST_CASE()
{
  using T = wlib::container::circular_buffer_t<int, 5>;

  T buffer;

  int sum = 0;
  for (auto entry : buffer)
    sum += entry;

  REQUIRE(sum == 0);

  for (std::size_t i = 0; i < 100; i++)
    buffer.push(1);

  sum = 0;
  for (auto entry : buffer)
    sum += entry;

  REQUIRE(sum == 5);
}
