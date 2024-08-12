#include <array>
#include <atomic>
#include <iostream>
#include <ut_catch.hpp>
#include <wlib.hpp>

namespace
{
  template <typename T> class dummy_pub: public wlib::publisher::Publisher_Interface<T>
  {
    using Notifyable_Interface = typename wlib::publisher::Publisher_Interface<T>::Notifyable_Interface;

  public:
    void puplish(T const& val)
    {
      for (auto& el : this->m_subs)
      {
        if (el == nullptr)
          continue;
        el->notify(val);
      }
    }

  private:
    std::array<Notifyable_Interface*, 2> m_subs{};

    bool try_add_subscriber(Notifyable_Interface& sub) override
    {
      for (auto& el : this->m_subs)
      {
        if (el == nullptr)
        {
          el = &sub;
          return true;
        }
      }
      return false;
    }

    void remove_subscriber(Notifyable_Interface& sub) override
    {
      for (auto& el : this->m_subs)
      {
        if (el == &sub)
        {
          el = nullptr;
          return;
        }
      }
    }
  };

  template <typename T> class dummy_sub: public wlib::publisher::Publisher_Interface<T>::Subscription_Interface
  {
  public:
    dummy_sub(T& val)
        : m_val(val)
    {
    }

  private:
    void notify(int const& val) override { this->m_val = val; }

    int& m_val;
  };

  template <> class dummy_pub<void>: public wlib::publisher::Publisher_Interface<void>
  {
    using Notifyable_Interface = typename wlib::publisher::Publisher_Interface<void>::Notifyable_Interface;

  public:
    void puplish()
    {
      for (auto& el : this->m_subs)
      {
        if (el == nullptr)
          continue;
        el->notify();
      }
    }

  private:
    std::array<Notifyable_Interface*, 2> m_subs{};

    bool try_add_subscriber(Notifyable_Interface& sub) override
    {
      for (auto& el : this->m_subs)
      {
        if (el == nullptr)
        {
          el = &sub;
          return true;
        }
      }
      return false;
    }

    void remove_subscriber(Notifyable_Interface& sub) override
    {
      for (auto& el : this->m_subs)
      {
        if (el == &sub)
        {
          el = nullptr;
          return;
        }
      }
    }
  };

  template <> class dummy_sub<void>: public wlib::publisher::Publisher_Interface<void>::Subscription_Interface
  {
  public:
    dummy_sub(int& val)
        : m_val(val)
    {
    }

  private:
    void notify() override { this->m_val++; }

    int& m_val;
  };

}    // namespace

TEST_CASE()
{
  using dummy_pub_int = dummy_pub<int>;
  dummy_pub_int pub;

  int vals[3]{};
  for (auto& ent : vals)
  {
    ent = -1;
  }

  REQUIRE(vals[0] == -1);
  REQUIRE(vals[1] == -1);
  REQUIRE(vals[2] == -1);

  dummy_sub sub_1{ vals[0] };
  REQUIRE_FALSE(sub_1.is_subscribed());
  sub_1.subscribe(pub);
  REQUIRE(sub_1.is_subscribed());

  REQUIRE_THROWS(sub_1.subscribe(pub));

  pub.puplish(1);
  REQUIRE(vals[0] == 1);
  REQUIRE(vals[1] == -1);
  REQUIRE(vals[2] == -1);

  {
    dummy_sub sub_2(vals[1]);
    sub_2.subscribe(pub);

    pub.puplish(2);
    REQUIRE(vals[0] == 2);
    REQUIRE(vals[1] == 2);
    REQUIRE(vals[2] == -1);
  }

  dummy_sub sub_3{ vals[2] };
  sub_3.subscribe(pub);

  pub.puplish(3);
  REQUIRE(vals[0] == 3);
  REQUIRE(vals[1] == 2);
  REQUIRE(vals[2] == 3);

  REQUIRE_THROWS(sub_3.subscribe(pub));

  dummy_sub sub_4{ vals[0] };
  REQUIRE_THROWS(sub_4.subscribe(pub));
}

namespace
{
  struct A;
  struct B
  {
    int b;
  };

  class dummy_sub_A_B: public wlib::publisher::Publisher_Interface<A>::Subscription_Interface
  {
  public:
    dummy_sub_A_B(B (&transform)(A const&), B& val)
        : m_val(val)
        , m_transform(transform)
    {
    }

  private:
    void notify(A const& val) override { return this->forward(this->m_transform(val)); }
    void forward(B const& val) { this->m_val = val; }

    B& m_val;
    B (&m_transform)(A const&);
  };

  struct A
  {
    int a;
  };
}    // namespace

B transform_1(A const& val) { return { val.a + 1 }; }
B transform_2(A const& val) { return { val.a * 2 }; }
B transform_3(A const& val) { return { val.a - 1 }; }

TEST_CASE()
{
  using dummy_pub_A = dummy_pub<A>;
  dummy_pub_A pub;

  B vals[3]{};
  for (auto& ent : vals)
  {
    ent.b = -1;
  }

  REQUIRE(vals[0].b == -1);
  REQUIRE(vals[1].b == -1);
  REQUIRE(vals[2].b == -1);

  dummy_sub_A_B sub_1{ transform_1, vals[0] };
  sub_1.subscribe(pub);

  pub.puplish({ 1 });
  REQUIRE(vals[0].b == 2);
  REQUIRE(vals[1].b == -1);
  REQUIRE(vals[2].b == -1);

  {
    dummy_sub_A_B sub_2{ transform_2, vals[1] };
    sub_2.subscribe(pub);

    pub.puplish({ 2 });
    REQUIRE(vals[0].b == 3);
    REQUIRE(vals[1].b == 4);
    REQUIRE(vals[2].b == -1);
  }

  dummy_sub_A_B sub_3{ transform_3, vals[2] };
  sub_3.subscribe(pub);

  pub.puplish({ 3 });
  REQUIRE(vals[0].b == 4);
  REQUIRE(vals[1].b == 4);
  REQUIRE(vals[2].b == 2);
}

namespace
{
  class tst_obj_cb_sub
  {
  public:
    tst_obj_cb_sub(wlib::publisher::Publisher_Interface<A>& pub)
        : m_sub(m_value_update_handle)
    {
      this->m_sub.subscribe(pub);
    }

    int get() const { return this->m_val; }

  private:
    void update_value(A const& val) { this->m_val = val.a + 5; }

    int                                                           m_val = -1;
    wlib::Memberfunction_Callback<tst_obj_cb_sub, void(A const&)> m_value_update_handle{ *this, &tst_obj_cb_sub::update_value };
    wlib::publisher::CallbackSubscriber<A>                        m_sub;
  };

  class tst_obj_trans_sub
  {
  public:
    tst_obj_trans_sub(wlib::publisher::Publisher_Interface<A>& pub, wlib::Callback<void(wlib::Callback<void(B const&)>&, A const&)>& sur)
        : m_sub(m_value_update_handle, sur)
    {
      this->m_sub.subscribe(pub);
    }

    int get() const { return this->m_val; }

  private:
    void update_value(B const& val) { this->m_val = val.b + 5; }

    int                                                              m_val = -1;
    wlib::Memberfunction_Callback<tst_obj_trans_sub, void(B const&)> m_value_update_handle{ *this, &tst_obj_trans_sub::update_value };
    wlib::publisher::TransformationSubscriber<A, B>                  m_sub;
  };

}    // namespace

TEST_CASE()
{
  using dummy_pub_A = dummy_pub<A>;
  dummy_pub_A pub;

  tst_obj_cb_sub obj_1{ pub };
  REQUIRE(obj_1.get() == -1);

  pub.puplish({ 3 });
  REQUIRE(obj_1.get() == 8);
  {
    tst_obj_cb_sub obj_2{ pub };
    REQUIRE(obj_1.get() == 8);
    REQUIRE(obj_2.get() == -1);

    pub.puplish({ 4 });
    REQUIRE(obj_1.get() == 9);
    REQUIRE(obj_2.get() == 9);
  }

  tst_obj_cb_sub obj_3{ pub };
  REQUIRE(obj_1.get() == 9);
  REQUIRE(obj_3.get() == -1);

  pub.puplish({ 5 });

  REQUIRE(obj_1.get() == 10);
  REQUIRE(obj_3.get() == 10);
}

void transform_sur_1(wlib::Callback<void(B const&)>& cb, A const& val) { return cb({ val.a + 1 }); }
void transform_sur_2(wlib::Callback<void(B const&)>& cb, A const& val) { return cb({ val.a * 2 }); }
void transform_sur_3(wlib::Callback<void(B const&)>& cb, A const& val) { return cb({ val.a - 1 }); }

TEST_CASE()
{
  using dummy_pub_A = dummy_pub<A>;
  dummy_pub_A pub;

  wlib::Function_Callback<void(wlib::Callback<void(B const&)>&, A const&)> sur_tmp_1{ transform_sur_1 };
  wlib::Function_Callback<void(wlib::Callback<void(B const&)>&, A const&)> sur_tmp_2{ transform_sur_2 };
  wlib::Function_Callback<void(wlib::Callback<void(B const&)>&, A const&)> sur_tmp_3{ transform_sur_3 };

  tst_obj_trans_sub obj_1{ pub, sur_tmp_1 };
  REQUIRE(obj_1.get() == -1);

  pub.puplish({ 3 });
  REQUIRE(obj_1.get() == 9);
  {
    tst_obj_trans_sub obj_2{ pub, sur_tmp_2 };
    REQUIRE(obj_1.get() == 9);
    REQUIRE(obj_2.get() == -1);

    pub.puplish({ 4 });
    REQUIRE(obj_1.get() == 10);
    REQUIRE(obj_2.get() == 13);
  }

  tst_obj_trans_sub obj_3{ pub, sur_tmp_3 };
  REQUIRE(obj_1.get() == 10);
  REQUIRE(obj_3.get() == -1);

  pub.puplish({ 5 });

  REQUIRE(obj_1.get() == 11);
  REQUIRE(obj_3.get() == 9);
}

TEST_CASE()
{
  using dummy_pub_event = dummy_pub<void>;
  dummy_pub_event pub;

  int count_1 = 0;
  int count_2 = 0;
  int count_3 = 0;

  pub.puplish();
  REQUIRE(count_1 == 0);
  REQUIRE(count_2 == 0);
  REQUIRE(count_3 == 0);

  dummy_sub<void> sub_1{ count_1 };
  sub_1.subscribe(pub);
  pub.puplish();

  REQUIRE(count_1 == 1);
  REQUIRE(count_2 == 0);
  REQUIRE(count_3 == 0);
  {
    dummy_sub<void> sub_2{ count_2 };
    sub_2.subscribe(pub);
    pub.puplish();

    REQUIRE(count_1 == 2);
    REQUIRE(count_2 == 1);
    REQUIRE(count_3 == 0);
  }
  dummy_sub<void> sub_3{ count_3 };
  sub_3.subscribe(pub);
  pub.puplish();

  REQUIRE(count_1 == 3);
  REQUIRE(count_2 == 1);
  REQUIRE(count_3 == 1);
}
