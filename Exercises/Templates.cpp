#include <Catest.h>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T>
void swap(T & x, T & y)
{
    T z = x;
    x = y;
    y = z;
}

DISABLE_SUITE(Templates);

CATEST_F(Templates, test_swap)
{
    int32_t x = 10;
    int32_t y = 20;
    float z = 3.14f;
    float q = 420.69f;

    swap(x, y);
    swap(z, q);

    ASSERT_EQ(x, 20);
    ASSERT_EQ(y, 10);
    ASSERT_FEQ(z, 420.69);
    ASSERT_FEQ(q, 3.14);
}

template <typename T>
class NumberHolder
{
public:
    T number;
    NumberHolder(T number)
        : number{ number }
    {

    }
    void print()
    {
        std::cout <<"NumberHolder<???>: " << number << '\n';
    }
};

template <>
void NumberHolder<int32_t>::print()
{
    std::cout << "NumberHolder<int32_t>: " << number << '\n';
}

template <>
void NumberHolder<int64_t>::print()
{
    std::cout << "NumberHolder<int64_t>: " << number << '\n';
}


template <>
void NumberHolder<float>::print()
{
    std::cout << "NumberHolder<float>: " << number << '\n';
}



CATEST_F(Templates, template_class)
{
    NumberHolder<int64_t> intHolder(42);
    NumberHolder floatHolder(3.14f);
    NumberHolder doubleholder(3.14);
    intHolder.print();
    floatHolder.print();
    doubleholder.print();
    
}




