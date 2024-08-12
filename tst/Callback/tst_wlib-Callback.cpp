#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

int tst_fnc_1(float val) { return val * 3; }
int tst_fnc_2(float val) { return val * 2; }

TEST_CASE()
{

  wlib::Function_Callback<int(float)> f_cb_1{ tst_fnc_1 };
  wlib::Function_Callback<int(float)> f_cb_2{ tst_fnc_2 };

  for (float tst_val : { 0.1f, 1.9f, 3.0f, 123.6f })
  {
    REQUIRE(f_cb_1(tst_val) == tst_fnc_1(tst_val));
    REQUIRE(f_cb_2(tst_val) == tst_fnc_2(tst_val));
  }
}

class dummy_obj: public wlib::Callback<int(int)>
{
public:
  dummy_obj(int fac)
      : m_fac(fac)
  {
  }

  int operator()(int val) override { return this->m_fac * val; }

private:
  int m_fac;
};

TEST_CASE()
{
  dummy_obj obj1{ 1 };
  dummy_obj obj2{ 2 };
  dummy_obj obj3{ 3 };

  using cb_t = wlib::Callback<int(int)>;
  cb_t& cb1  = obj1;
  cb_t& cb2  = obj2;
  cb_t& cb3  = obj3;

  for (int i = 0; i < 5; i++)
  {
    REQUIRE(cb1(i) == i);
    REQUIRE(cb2(i) == 2 * i);
    REQUIRE(cb3(i) == 3 * i);
  }
}

class dummy_obj_mfp
{
public:
  dummy_obj_mfp(int fac1, int fac2, int fac3)
      : m_fac_1(fac1)
      , m_fac_2(fac2)
      , m_fac_3(fac3)
  {
  }

  int m_func_1(int val)
  {
    this->m_tmp++;
    return this->m_fac_1 * val;
  }
  int m_func_2(int val)
  {
    this->m_tmp--;
    return this->m_fac_2 * val;
  }
  int m_func_3(int val) const { return this->m_fac_3 * val; }

private:
  int m_fac_1 = 0;
  int m_fac_2 = 0;
  int m_fac_3 = 0;
  int m_tmp   = 0;
};

TEST_CASE()
{

  using cb_t = wlib::Callback<int(int)>;
  dummy_obj_mfp       obj1{ 1, 2, 3 };
  dummy_obj_mfp       obj2{ 3, 2, 1 };
  dummy_obj_mfp const obj3{ 2, 2, 2 };

  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_1_1{ obj1, &dummy_obj_mfp::m_func_1 };
  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_1_2{ obj1, &dummy_obj_mfp::m_func_2 };
  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_1_3{ obj1, &dummy_obj_mfp::m_func_3 };

  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_2_1{ obj2, &dummy_obj_mfp::m_func_1 };
  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_2_2{ obj2, &dummy_obj_mfp::m_func_2 };
  wlib::Memberfunction_Callback<dummy_obj_mfp, int(int)> mf_cb_2_3{ obj2, &dummy_obj_mfp::m_func_3 };

  wlib::Memberfunction_Callback<dummy_obj_mfp const, int(int)> mf_cb_3_3{ obj3, &dummy_obj_mfp::m_func_3 };

  cb_t& cb11 = mf_cb_1_1;
  cb_t& cb12 = mf_cb_1_2;
  cb_t& cb13 = mf_cb_1_3;
  cb_t& cb21 = mf_cb_2_1;
  cb_t& cb22 = mf_cb_2_2;
  cb_t& cb23 = mf_cb_2_3;

  cb_t& cb33 = mf_cb_3_3;

  for (int i = 0; i < 5; i++)
  {
    REQUIRE(cb11(i) == i);
    REQUIRE(cb12(i) == 2 * i);
    REQUIRE(cb13(i) == 3 * i);

    REQUIRE(cb21(i) == 3 * i);
    REQUIRE(cb22(i) == 2 * i);
    REQUIRE(cb23(i) == 1 * i);

    REQUIRE(cb33(i) == 2 * i);
  }
}
