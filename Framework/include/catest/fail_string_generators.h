#ifndef CATEST_FAIL_STRING_GENERATORS_H
#define CATEST_FAIL_STRING_GENERATORS_H
#include <catest/to_string.h>

namespace catest
{


    template <typename T>
    std::string generate_value_fail_string(const T& cond, const char* condName, const T& expected)
    {
        std::stringstream fail;
        fail <<"Expected " << std::boolalpha << expected << " but got " << get_value_and_name(cond, condName);

        return fail.str();
    }

    template <typename T, typename T2>
    std::string generate_eq_fail_string(const T& left, const char* leftName, const T2& right, const  char* rightName)
    {
        std::stringstream fail;
        fail << "Expected " << get_value_and_name(right, rightName) << " and  " << get_value_and_name(left, leftName) << " to be equal";
        return fail.str();
    }


    template <typename T>
    std::string generate_neq_fail_string(const T& left, const char* leftName, const T& right, const  char* rightName)
    {
        std::stringstream fail;
        fail << "Expected " << get_value_and_name(right, rightName) << " and " << get_value_and_name(left, leftName) << " to be different";
        return fail.str();
    }


    template <typename T>
    std::string generate_neq_fail_string(const nullptr_t & left, const char* leftName, const T& right, const  char* rightName)
    {
        std::stringstream fail;
        fail << "Expected " << get_value_and_name(right, rightName) << " and " << get_value_and_name(left, leftName) << " to be different";
        return fail.str();
    }


    template <typename T>
    std::string generate_neq_fail_string(const T& left, const char* leftName, const nullptr_t & right, const  char* rightName)
    {
        std::stringstream fail;
        fail << "Expected " << get_value_and_name(right, rightName) << " and " << get_value_and_name(left, leftName) << " to be different";
        return fail.str();
    }
}


#endif // !CATEST_FAIL_STRING_GENERATORS_H
