#pragma once
#include <Catest.h>
#include <iostream>
#include <bitset>

class MyClass
{
private:
    int value;
public:
    MyClass(const int value) : value{ value } {}
    friend bool operator==(const MyClass& left, const MyClass& right)
    {
        return left.value == right.value;
    }
};

class MyClassStreamable
{
private:
    int value;
public:
    MyClassStreamable(const int value) : value{ value } {}
    friend bool operator==(const MyClassStreamable& left, const MyClassStreamable& right)
    {
        return left.value == right.value;
    }
    friend std::ostream& operator<<(std::ostream& os, const MyClassStreamable& obj)
    {
        os << obj.value;
        return os;
    }
};


CATEST_F(catest_fw_tests_expected_fail, equals_fail_not_streamable)
{
    MyClass obj1{ 10 };
    MyClass obj2{ 11 };

    ASSERT_EQ(obj1, obj2);
}

CATEST_F(catest_fw_tests_expected_fail, equals_fail_streamable)
{
    MyClassStreamable obj1{ 10 };
    MyClassStreamable obj2{ 11 };

    ASSERT_EQ(obj1, obj2);
}


CATEST_F(catest_fw_tests_expected_fail, assert_true_fail)
{
    ASSERT_TRUE(false);
}


CATEST_F(catest_fw_tests_expected_fail, assert_false_fail)
{
    ASSERT_FALSE(true);
}

CATEST_F(catest_fw_tests_expected_fail, assert_eq_fail_numeric)
{
    ASSERT_EQ(20, 10);
}

CATEST_F(catest_fw_tests_expected_fail, assert_neq_fail_numeric)
{
    ASSERT_NEQ(10, 10);
}

CATEST_F(catest_fw_tests_expected_fail, expect_neq_fail_numeric)
{
    EXPECT_NEQ(10, 10);
}

CATEST_F(catest_fw_tests_expected_fail, expect_eq_fail_numeric_double_fail)
{
    EXPECT_EQ(10, 20);
    EXPECT_EQ(20, 10);
}

CATEST_F(catest_fw_tests_expected_fail, expect_assert_eq_fail_numeric_double_fail)
{
    EXPECT_EQ(10, 20);
    ASSERT_EQ(10, 20);
    EXPECT_EQ(20, 10);
}



CATEST_F(catest_disabled_suite,you_cant_see_me_my_time_is_now)
{

}



DISABLE_SUITE(catest_disabled_suite);