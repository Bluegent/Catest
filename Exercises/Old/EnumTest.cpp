#include <Catest.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>





/*
switch (date.month)
{
case Month::Jan:
    stream << "Jan";
    break;
case Month::Feb:
    stream << "Feb";
    break;
case  Month::Mar:
    stream << "Mar";
    break;
case  Month::Apr:
    stream << "Apr";
    break;
case  Month::May:
    stream << "May";
    break;
case  Month::Jun:
    stream << "Jun";
    break;
case  Month::Jul:
    stream << "Jul";
    break;
case  Month::Aug:
    stream << "Aug";
    break;
case  Month::Sep:
    stream << "Sep";
    break;
case  Month::Oct:
    stream << "Oct";
    break;
case  Month::Nov:
    stream << "Nov";
    break;
case  Month::Dec:
    stream << "Dec";
    break;
}
*/

enum class Month
{
    Jan = 0,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};

struct Date
{
    int32_t day;
    Month month;
    int32_t year;
    std::string getMonthName() const
    {
        return Date::monthNames[static_cast<size_t>(month)];
    }
private:
    static const std::vector<std::string> monthNames;
};
const std::vector<std::string> Date::monthNames = { "Jan","Feb","Mar","Apr","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };


Month operator+(int lhs, Month rhs)
{
    return Month::Apr;
}

// write a function that takes a Date object and outputs it to console
// dd/Mon/year e.g. 01/Feb/2015
std::string date(const Date& date)
{
    std::stringstream stream;

    int a = 10;
    Month b = Month::Jan;
    Month c = a + b;

    stream << std::setw(2) << std::setfill('0') << date.day << '/';

    //stream << date.month;

    stream << '/' << date.year;

    return stream.str();
}
DISABLE_SUITE(enum_test);

CATEST_F(enum_test, yes)
{

    Date aDate{ 9, Month::Mar ,2010 };
    std::string dateString = date(aDate);
    //EXPECT_EQ(dateString, "09/Mar/2010");
}

std::string addUsingStream(const int32_t lhs, const std::string& rhs)
{
    //method 1 : using stringstream
    std::stringstream stream;

    stream << lhs << rhs;

    return stream.str();
}

std::string addUsingToString(const int32_t lhs, const std::string& rhs)
{
    // method 2: using std::to_string
    return std::to_string(lhs) + rhs;
}


std::string addUsingManualConversion(int32_t lhs, const std::string& rhs)
{
    // method 3: manually converting the int32_t to a string
    std::string result;
    std::vector<int8_t> digits;
    //let's get the digits of lhs
    while (lhs != 0)
    {
        digits.push_back(lhs % 10);
        lhs = lhs / 10;
    }
    std::cout << '\n';

    for (int32_t i = digits.size() - 1; i >= 0; --i)
    {
        result += digits[i] + '0';
    }
    return result + rhs;
}


std::string operator+ (int32_t lhs, const std::string& rhs)
{
    //return addUsingStream(lhs,rhs);
    //return addUsingToString(lhs, rhs);
    return addUsingManualConversion(lhs, rhs);
}


std::string operator+ (const std::string& lhs, int32_t rhs)
{
    std::stringstream stream;

    stream << lhs << rhs;

    return stream.str();
}

CATEST_F(exercises7, operator_plus_string_int32_t)
{
    using namespace std::string_literals;
    int32_t a = 10;
    int32_t b = 20;
    int32_t c = a + b; //30
    std::string stringA = "ala";
    std::string stringB = "bala";
    std::string result = stringA + stringB; //"alabala"

    std::string resultOfint32_tPlusString = 1234 + stringA;
    EXPECT_EQ(resultOfint32_tPlusString, "1234ala");

    std::string res2 = stringA + 1234;

    EXPECT_EQ(res2, "ala1234");

    std::string aReallyLongString = "Ana are "s + 30 + " de mere si Gicu are "s + 40 + " de prune."s;
    EXPECT_EQ(aReallyLongString, "Ana are 30 de mere si Gicu are 40 de prune.");

    std::string aConfusingString = "Ana are "s + (30 + 40) + " de mere."s;

}


// prefer const type & param -> this guarantees that we won't modify param and we don't copy it either
// if I want to modify it and have the change reflected outside: type & param 
// if I want a copy of it -> type param


// we have a simple robot that can move either up, down, left or right and moves on an endless plane
// the robot starts at position 0,0
// implement a function that takes a vector of moves and returns true if the robot has executed a loop
// a loop is defined as any sequence of moves where the robot eventually ends up in the 0,0 position again
// it still counts as a loop if the robot does not stop at 0,0

enum class Move
{
    Up,
    Down,
    Left,
    Right
};
bool hasLoop(const std::vector<Move>& moves)
{
    int32_t x = 0;
    int32_t y = 0;

    for (size_t i = 0; i < moves.size(); ++i)
    {
        switch (moves[i])
        {
        case Move::Up:
            ++y;
            break;
        case Move::Down:
            --y;
            break;
        case Move::Right:
            ++x;
            break;
        case Move::Left:
            --x;
            break;
        }

        if (x == 0 && y == 0)
        {
            return true;
        }
    }
    return false;

}



CATEST_F(exercises7, simple_movement_loop)
{
    std::vector<Move> moves = { Move::Up,Move::Right,Move::Down,Move::Left };
    EXPECT_TRUE(hasLoop(moves));
    std::vector<Move > moves2 = { Move::Up,Move::Down,Move::Down };
    EXPECT_TRUE(hasLoop(moves2));
}



// we have a simple robot that can move either up, down, left or right and moves on an endless plane
// the robot starts at position 0,0
// implement a function that takes a vector of moves and returns true if the robot has executed a strict loop
// a strict loop is defined as any sequence of moves where the robot's last move puts it at position 0,0

bool hasStrictLoop(const std::vector<Move>& moves)
{
    int32_t x = 0;
    int32_t y = 0;

    for (size_t i = 0; i < moves.size(); ++i)
    {
        switch (moves[i])
        {
        case Move::Up:
            ++y;
            break;
        case Move::Down:
            --y;
            break;
        case Move::Right:
            ++x;
            break;
        case Move::Left:
            --x;
            break;
        }
    }
    if (x == 0 && y == 0)
    {
        return true;
    }
    return false;
}




// we have a simple robot that can move either up, down, left or right and moves on an endless plane
// the robot starts at position 0,0
// implement a function that takes a vector of moves and returns the robot's position after it has executed all the moves

struct Position
{
    int32_t x;
    int32_t y;

    Position(int32_t x, int32_t y)
        : x{ x }
        , y{ y }
    {

    }
    Position()
        : x{ }
        , y{ }
    {

    }

    float distance(const Position& pos) const
    {
        return sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2));
    }

};

bool operator== (const Position& lhs, const Position& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


Position executeMoves(const std::vector<Move>& moves)
{
    Position pos;
    for (size_t i = 0; i < moves.size(); ++i)
    {
        switch (moves[i])
        {
        case Move::Up:
            ++pos.y;
            break;
        case Move::Down:
            --pos.y;
            break;
        case Move::Right:
            ++pos.x;
            break;
        case Move::Left:
            --pos.x;
            break;
        }
    }
    return pos;
}





CATEST_F(exercises7, simple_movement_position)
{
    std::vector<Move> moves = { Move::Up,Move::Right,Move::Down,Move::Left , Move::Up };
    Position expected(0, 1);
    Position real = executeMoves(moves);
    EXPECT_EQ(real, expected);
}


// write a function that takes two positions and calulates the distance between them

float distance(const Position& pos1, const Position& pos2)
{
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}




// write a function that takes a vector of lengths of the sides(float) of an n-gon and returns its perimeter
float perimeter(const std::vector<float> & lengths)
{
    float sum{0.f};
    for (size_t i = 0; i < lengths.size(); ++i)
    {
        sum += lengths[i];
    }
    return sum;
}


// write a function that takes a vector of 2D points that represent an n-sided polygon
// the function shall calculate the perieter of the polygon and return it
float perimeter(const std::vector<Position> & points)
{
    float sum = 0.f;
    for (size_t i = 0; i < points.size() - 1 ; ++i)
    {
       sum+= points[i].distance(points[i + 1]);
    }
    sum +=points[points.size() - 1].distance(points[0]);


    return sum;
}


struct Garbage
{
    int32_t yes;
    int64_t no;
    int64_t more;
    int64_t garbage;
    bool yesno;
    void aMethod() {};
};


struct GarbageVirt
{
    int32_t yes;
    int64_t no;
    int64_t more;
    int64_t garbage;
    bool yesno;
    virtual void aMethod() {};

};
DISABLE_SUITE(exercises7);

CATEST_F(exercises7, rvalue_lvalue)
{
    size_t sizeOfPos = sizeof(Position);
    size_t sizeOfPosPtr = sizeof(Position*);
    size_t sizeOfGarb = sizeof(Garbage);
    size_t sizeOfGarbPtr = sizeof(Garbage*);
    size_t sizeOfGarbVirt = sizeof(GarbageVirt);
    size_t sizeOfGarbVirtPtr = sizeof(GarbageVirt*);
    size_t sizeOfVecInt32 = sizeof(std::vector<int32_t>);
    size_t sizeOfVecInt64 = sizeof(std::vector<int64_t>);
    size_t sizeOfVecInt64Ptr = sizeof(std::vector<int64_t>*);

}