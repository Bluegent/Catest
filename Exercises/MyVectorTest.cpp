#include <Catest.h>
#include "MyVector.h"
#include <vector>
#include <iostream>

CATEST_F(myvector_suite, constructors)
{
    // default constructor
    MyVector defaultVector;
    EXPECT_EQ(defaultVector.size(), 0);
    EXPECT_EQ(defaultVector.capacity(), 0);
    EXPECT_EQ(defaultVector.data(), nullptr);

    // parameter constructor
    size_t paramVecSize = 10;
    MyVector paramVec(paramVecSize);
    EXPECT_EQ(paramVec.size(), paramVecSize);
    EXPECT_EQ(paramVec.capacity(), paramVecSize);
    EXPECT_NEQ(paramVec.data(), nullptr);
    for (size_t i = 0; i < paramVec.size(); ++i)
    {
        EXPECT_EQ(paramVec.data()[i], 0);
    }



    // copy constructor
    MyVector copyVec = paramVec;
    EXPECT_EQ(copyVec.size(), paramVec.size());
    EXPECT_EQ(copyVec.capacity(), paramVec.capacity());
    EXPECT_NEQ(copyVec.data(), nullptr);
    EXPECT_NEQ(copyVec.data(), paramVec.data());

    // destructor
    MyVector destructVec(paramVecSize);
    destructVec.~MyVector(); // probably shouldn't do this but the language allows for explicit destructor calling so...
    EXPECT_EQ(destructVec.data(), nullptr);
}



CATEST_F(myvector_suite, operator_square_brackets)
{
    MyVector defaultVec;
    try
    {
        defaultVec[0];
        // ^ if this does not throw, then we continue to the next statement
        // so the next statement shall fail the test
        FAIL("operator [] did not throw for null data pointer");
    }
    catch (std::out_of_range exception)
    {
        // do nothing so the test proceed onwards
    }

    MyVector vector(2);
    try
    {
        vector[3] = 10;
        FAIL("operator [] did not throw for index out of bounds");
    }
    catch (std::out_of_range exception)
    {
        // do nothing
    }

    int32_t aValue = 42;
    vector[0] = aValue;
    EXPECT_EQ(vector[0], aValue);
}


CATEST_F(myvector_suite,push_back)
{
    int32_t aValue = 42;
    size_t vecSize = 5;

    MyVector vec(vecSize);     // 0 0 0 0 0
    vec.push_back(aValue);     // 0 0 0 0 0 42
    EXPECT_EQ(vec[vecSize], aValue);
    EXPECT_EQ(vec.size(), vecSize + 1);

    vec.push_back(aValue);     // 0 0 0 0 0 42 42
    EXPECT_EQ(vec[vecSize+1], aValue);
    EXPECT_EQ(vec.size(), vecSize + 2);
}