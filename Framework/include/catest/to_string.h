#ifndef CATEST_TO_STRING_H
#define CATEST_TO_STRING_H

#include <type_traits>
#include <sstream>
#include <string>


namespace catest
{
    //evil voodoo code I copied from StackOverflow
    //and might try to understand later maybe not
    template<typename S, typename T>
    class is_streamable
    {
        template<typename SS, typename TT>
        static auto test(int)
            -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

        template<typename, typename>
        static auto test(...)->std::false_type;

    public:
        static const bool value = decltype(test<S, T>(0))::value;
    };
    //evil voodoo code I copied from StackOverflow


    template <typename T, bool is_streamable = false>
    struct StringConvertor
    {
        static std::string get(const T& value)
        {
            return "UNKNOWN";
        }
    };

    template <typename T>
    struct StringConvertor<T, true>
    {
        static std::string get(const T& value)
        {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
    };

    template <>
    struct StringConvertor<bool, true>
    {
        static std::string get(const bool& value)
        {
            std::stringstream ss;
            ss << std::boolalpha << value;
            return ss.str();
        }
    };

    template <typename T>
    std::string get_value_and_name(const T& param, const char* name)
    {
        std::stringstream ss;
        ss << name << "(" << StringConvertor<T, is_streamable<std::stringstream, T>::value>::get(param) << ")";
        return ss.str();;
    }
}

#endif // !CATEST_TO_STRING_H
