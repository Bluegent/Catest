#include <Catest.h>
#include <iostream>

//write a function that reverses a given vector of int8_t
void reverse(std::vector<int8_t>& vec)
{
    for (size_t i = 0; i < vec.size() / 2; ++i)
    {
        int8_t aux = vec[i];
        vec[i] = vec[vec.size() - i - 1];
        vec[vec.size() - i - 1] = aux;
    }
}


// write a function that splits <number> into digits and stores the digits into a vector of int8_t, then returns the vector
std::vector<int8_t> getDigits(uint64_t number)
{
    if (number == 0)
    {
        return std::vector<int8_t>({ 0 });
    }
    std::vector<int8_t> digits;
    /*
    113 % 10 = 3
    113 / 10 = 11
    11 % 10 = 1
    11 / 10 = 1
    1 $ 10 = 1
    1 / 10 = 0
    */

    //while the number is not 0
    while (number != 0)
    {
        digits.push_back(number % 10);  // we push its last digit into our vector
        number /= 10;                   // and we store the value of the number diveded by 10 into the number variable 
        // => this truncates the last digit of the number
    }
    // 113-> {3,1,1}
    reverse(digits);
    return digits;
}

// write an equals operator for comparing two vectors of int8_t
// lhs == rhs
bool operator==(const std::vector<int8_t>& lhs, const std::vector<int8_t>& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    for (size_t i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

// operand_lhs operator operand_rhs
//      a         +        b 
//  std::cout     <<     vector

// write an operator that outputs a vector of int8_t
template <typename Placeholder>
std::ostream& operator<< (std::ostream& os, const std::vector<Placeholder>& vec)
{
    if (vec.size() == 0)
    {
        os << "[]";
        return os;
    }
    os << '[';

    for (size_t i = 0; i < vec.size(); ++i)
    {
        os << vec[i];
        if (i < vec.size() - 1)
        {
            os << ", ";
        }
    }

    os << ']';

    return os;
}

std::ostream& operator<< (std::ostream& os, const std::vector<int8_t>& vec)
{
    if (vec.size() == 0)
    {
        os << "[]";
        return os;
    }

    os << '[';

    for (size_t i = 0; i < vec.size(); ++i)
    {
        os << static_cast<int32_t>(vec[i]);
        if (i < vec.size() - 1)
        {
            os << ", ";
        }
    }

    os << ']';

    return os;
}


CATEST_F(exercises3, test_vector_reverse)
{
    std::vector<int8_t> forwards = { 1,2,3,4 };
    std::vector<int8_t> reversed = forwards;
    reverse(reversed);
    EXPECT_EQ(std::vector<int8_t>({ 4,3,2,1 }), reversed);
}


CATEST_F(exercises3, test_vector_equals)
{

    std::vector<int8_t> vec = { 10,10,100 };
    std::vector<int8_t> vec2;
    EXPECT_TRUE(vec == vec);
    EXPECT_FALSE(vec == vec2);
    EXPECT_NEQ(vec, vec2);

    std::cout << "vec: " << vec << '\n';
    std::cout << "vec2: " << vec2 << '\n';
}

CATEST_F(exercises3, test_get_digits)
{
    EXPECT_EQ(getDigits(12345), std::vector<int8_t>({ 1,2,3,4,5 }));
    EXPECT_EQ(getDigits(0), std::vector<int8_t>({ 0 }));
}


/*
N - numere naturale [0 -> inf] e.g. 20, 55
I - numere intregi [-inf -> + inf] e.g -20, -13, 45
Q - numere rationale [-inf -> inf] de forma n/m (cu numar finit de zecimale) e.g 3.14, 2, 20.00001
R - numere reale - [-inf -> inf] de forma n/m (cu numar infinit de zecimale) e.g. 3.(3), Pi,
N < I < Q < R
C - numere complexe - de forma a + b * i unde a,b apartin R
N < I < Q < R < C
*/


// a + b * i, sqrt(i) = -1, a,b are real numbers
class Complex
{
private:
    float real;         // a
    float imaginary;    // b
public:
    Complex(float real, float imaginary) : real{ real }, imaginary{ imaginary }{}
    Complex(const Complex& other) : real{ other.real }, imaginary{ other.imaginary }{}
    Complex() : real{}, imaginary{}{}
    float getReal() { return real; }
    float getImaginary() { return imaginary; }


    friend std::ostream& operator<< (std::ostream& os, const Complex& number);
    friend Complex operator+ (const Complex& lhs, const Complex& rhs);
    friend bool operator== (const Complex& lhs, const Complex& rhs);
    friend void operator+= (Complex& lhs, const Complex& rhs);
};

bool operator== (const Complex& lhs, const Complex& rhs)
{

    // c1 = 3 + 5 * i
    // c2 = 3 + 6 * i
    // c1 == c2 => false
    /*if (lhs.real != rhs.real || lhs.imaginary != rhs.imaginary)
    {
        return false;
    }
    return true;*/

    return lhs.real == rhs.real && lhs.imaginary == rhs.imaginary;
}

std::ostream& operator<< (std::ostream& os, const Complex& number)
{
    os << number.real << " + " << number.imaginary << " * i";
    return os;
}

//  a1     +    b1      * i
//  a2     +    b2      * i
//+---------------------
// (a1+a2) + (b1+b2)    * i
Complex operator+ (const Complex& lhs, const Complex& rhs)
{
    /*
    Complex result;

    result.real = lhs.real + rhs.real;
    result.imaginary = lhs.imaginary + rhs.imaginary;

    return result;*/

    return Complex{ lhs.real + rhs.real , lhs.imaginary + rhs.imaginary };
}

void operator+= (Complex& lhs, const Complex& rhs)
{
    lhs.real += rhs.real;
    lhs.imaginary += rhs.imaginary;
}

DISABLE_SUITE(exercises3);

CATEST_F(exercises3, ostream_op_test)
{
    Complex c1;
    Complex c2{ 3,5 };
    Complex c3 = c2;
    c3 += Complex{ 5,3 };

    std::cout << c1 + c2 << '\n';
    EXPECT_NEQ(c1, c2);
    EXPECT_EQ(c1 + c2, c2);
    EXPECT_EQ(c3, Complex(8, 8));


    std::vector<Complex> complexVector{ c1,c2,c3 };
    std::cout << complexVector << '\n';

    std::vector<std::string> stringVector{"ala","bala","portocala"};
    std::cout << stringVector << '\n';

    std::vector<std::vector<int32_t>> matrix{ {1,2,3},{4,5,6},{7,8,9} };
    std::cout << matrix << '\n';

}