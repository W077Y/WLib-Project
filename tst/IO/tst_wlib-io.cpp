#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

namespace
{
  struct dummy_input final: public wlib::io::DigitalInput_Interface
  {
    // Inherited via DigitalInput_Interface
    bool get() const override { return this->in_value; }

    bool in_value = false;
  };

  struct dummy_output final: public wlib::io::DigitalOutput_Interface
  {
    // Inherited via DigitalOutput_Interface
    bool get() const override { return this->in_value; }
    void set_high() override { this->out_value = true; }
    void set_low() override { this->out_value = false; }
    void toggle() override { this->out_value = !this->out_value; }

    bool in_value  = false;
    bool out_value = false;
  };

}    // namespace

TEST_CASE()
{
  dummy_input                       input;
  wlib::io::DigitalInput_Interface& in = input;

  REQUIRE(in.get() == false);
  input.in_value = true;
  REQUIRE(in.get() == true);
  input.in_value = false;
  REQUIRE(in.get() == false);
}

TEST_CASE()
{
  dummy_output                       output;
  wlib::io::DigitalInput_Interface&  in  = output;
  wlib::io::DigitalOutput_Interface& out = output;

  REQUIRE(output.out_value == false);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  output.in_value = true;
  REQUIRE(output.out_value == false);
  REQUIRE(in.get() == true);
  REQUIRE(out.get() == true);
  
  output.in_value = false;
  REQUIRE(output.out_value == false);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  out.set_high();
  REQUIRE(output.out_value == true);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  out.set_low();
  REQUIRE(output.out_value == false);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  out.toggle();
  REQUIRE(output.out_value == true);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  out.set(false);
  REQUIRE(output.out_value == false);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);

  out.set(true);
  REQUIRE(output.out_value == true);
  REQUIRE(in.get() == false);
  REQUIRE(out.get() == false);
}
