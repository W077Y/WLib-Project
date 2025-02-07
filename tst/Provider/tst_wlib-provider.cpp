#include <exception>
#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

namespace
{
  class obj
  {
  public:
    obj(int val)
        : value(val)
    {
    }

    obj(obj const&) = delete;
    obj(obj&&)      = delete;

    int value = 0;
  };

  struct dummy_provider: public wlib::Shared_Resource_Provider_Interface<obj>
  {
    dummy_provider(resource_t& res)
        : Shared_Resource_Provider_Interface(res)
    {
    }
    // Inherited via Shared_Resource_Provider_Interface
    void lock() override
    {
      if (this->locked)
        throw std::runtime_error("locked");
      this->locked = true;
    }
    void unlock() override { this->locked = false; }
    bool locked = false;
  };

}    // namespace

TEST_CASE()
{
  obj            resource = { 0 };
  dummy_provider provider{ resource };

  REQUIRE(resource.value == 0);
  REQUIRE(provider.locked == false);

  {
    auto token        = provider.request();
    token.get().value = 5;

    REQUIRE(resource.value == 5);
    REQUIRE(provider.locked == true);
  }

  REQUIRE(resource.value == 5);
  REQUIRE(provider.locked == false);
}
