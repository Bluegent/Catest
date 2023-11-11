#include <Catest.h>
#include <vector>
#include <iostream>
#include <map>
#include <string>

// you are given a car that stars at position 0,0 facing North on an endless plane
// the car can go forwards or backwards one unit without changing its direction
// the car can turn left or turn right which will result in a change of direction as well as some forward/side motion
// e.g  if the car is at 0,0 facing North and it turns right, then it shall end up at 1,1 facing East
//      if the car is at 0,0 facing North and it turns left, then it shall end up at -1,1 facing West
// given a vector of such actions, determine the position and direction of the car after the final action

enum class Action
{
    Forwards,
    Backwards,
    TurnRight,
    TurnLeft
};

enum class Direction
{
    North,
    East,
    South,
    West
};

void runCar_ugly(const std::vector<Action>& actions, int32_t& x, int32_t& y, Direction& direction)
{
    for (size_t i = 0; i < actions.size(); ++i)
    {
        switch (actions[i])
        {
        case Action::Forwards:
            switch (direction)
            {
            case Direction::North:
                ++y;
                break;
            case Direction::East:
                ++x;
                break;
            case Direction::South:
                --y;
                break;
            case Direction::West:
                --x;
                break;
            }
            break;
        case Action::Backwards:
            switch (direction)
            {
            case Direction::North:
                --y;
                break;
            case Direction::East:
                --x;
                break;
            case Direction::South:
                ++y;
                break;
            case Direction::West:
                ++x;
                break;
            }
            break;
        case Action::TurnRight:
            switch (direction)
            {
            case Direction::North:
                ++x;
                ++y;
                direction = Direction::East;
                break;
            case Direction::East:
                ++x;
                --y;
                direction = Direction::South;
                break;
            case Direction::South:
                direction = Direction::West;
                --y;
                --x;
                break;
            case Direction::West:
                direction = Direction::North;
                --x;
                ++y;
                break;
            }
            break;
        case Action::TurnLeft:
            switch (direction)
            {
            case Direction::North:
                --x;
                ++y;
                direction = Direction::West;
                break;
            case Direction::East:
                ++x;
                ++y;
                direction = Direction::North;
                break;
            case Direction::South:
                direction = Direction::East;
                --y;
                ++x;
                break;
            case Direction::West:
                direction = Direction::South;
                --x;
                --y;
                break;
            }
            break;

        }
    }
}

void goForward(int32_t& x, int32_t& y, const Direction& dir)
{
    switch (dir)
    {
    case Direction::North:
        ++y;
        break;
    case Direction::East:
        ++x;
        break;
    case Direction::South:
        --y;
        break;
    case Direction::West:
        --x;
        break;
    }
}

void goBackward(int32_t& x, int32_t& y, const Direction& dir)
{
    switch (dir)
    {
    case Direction::North:
        --y;
        break;
    case Direction::East:
        --x;
        break;
    case Direction::South:
        ++y;
        break;
    case Direction::West:
        ++x;
        break;
    }
}


void turnRight(int32_t& x, int32_t& y, Direction& dir)
{
    switch (dir)
    {
    case Direction::North:
        ++x;
        ++y;
        dir = Direction::East;
        break;
    case Direction::East:
        ++x;
        --y;
        dir = Direction::South;
        break;
    case Direction::South:
        dir = Direction::West;
        --y;
        --x;
        break;
    case Direction::West:
        dir = Direction::North;
        --x;
        ++y;
        break;
    }
}

void turnLeft(int32_t& x, int32_t& y, Direction& dir)
{
    switch (dir)
    {
    case Direction::North:
        --x;
        ++y;
        dir = Direction::West;
        break;
    case Direction::East:
        ++x;
        ++y;
        dir = Direction::North;
        break;
    case Direction::South:
        dir = Direction::East;
        --y;
        ++x;
        break;
    case Direction::West:
        dir = Direction::South;
        --x;
        --y;
        break;
    }
}

void runCar(const std::vector<Action>& actions, int32_t& x, int32_t& y, Direction& direction)
{
    for (size_t i = 0; i < actions.size(); ++i)
    {
        switch (actions[i])
        {
        case Action::Forwards:
            goForward(x, y, direction);
            break;
        case Action::Backwards:
            goBackward(x, y, direction);
            break;
        case Action::TurnRight:
            turnRight(x, y, direction);
            break;
        case Action::TurnLeft:
            turnLeft(x, y, direction);
            break;

        }
    }
}


CATEST_F(CarTurn, test_basic_moves)
{
    std::vector<Action> actions = { Action::Forwards,Action::TurnRight };
    int32_t x = 0;
    int32_t y = 0;
    Direction dir = Direction::North;
    runCar(actions, x, y, dir);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 2);
    EXPECT_EQ(dir, Direction::East);
}


CATEST_F(CarTurn, test_basic_moves_2)
{
    std::vector<Action> actions = { Action::Backwards,Action::TurnLeft };
    int32_t x = 0;
    int32_t y = 0;
    Direction dir = Direction::North;
    runCar(actions, x, y, dir);
    EXPECT_EQ(x, -1);
    EXPECT_EQ(y, 0);
    EXPECT_EQ(dir, Direction::West);
}


// you are given a car that stars at position 0,0 facing North on an endless plane
// the car can go forwards or backwards one unit without changing its direction
// the car can turn left or turn right which will result in a change of direction as well as some forward/side motion
// e.g  if the car is at 0,0 facing North and it turns right, then it shall end up at 1,1 facing East
//      if the car is at 0,0 facing North and it turns left, then it shall end up at -1,1 facing West
// given a vector of such actions, determine the position and direction of the car after the final action


class Car
{
private:
    Direction dir;
    int32_t x;
    int32_t y;
public:
    Car()
        : dir{Direction::North}
        , x{0}
        , y{0}
    {}
    int32_t getX() const
    {
        return x;
    }
    int32_t getY() const
    {
        return y;
    }
    Direction getDirection() const
    {
        return dir;
    }
    void goForward();
    void goBackward();
    void turnRight();
    void turnLeft();
    void run(const std::vector<Action>& actions);
};



void Car::goForward()
{
    switch (dir)
    {
    case Direction::North:
        ++y;
        break;
    case Direction::East:
        ++x;
        break;
    case Direction::South:
        --y;
        break;
    case Direction::West:
        --x;
        break;
    }
}

void Car::goBackward()
{
    switch (dir)
    {
    case Direction::North:
        --y;
        break;
    case Direction::East:
        --x;
        break;
    case Direction::South:
        ++y;
        break;
    case Direction::West:
        ++x;
        break;
    }
}

void Car::turnRight()
{
    switch (dir)
    {
    case Direction::North:
        ++x;
        ++y;
        dir = Direction::East;
        break;
    case Direction::East:
        ++x;
        --y;
        dir = Direction::South;
        break;
    case Direction::South:
        dir = Direction::West;
        --y;
        --x;
        break;
    case Direction::West:
        dir = Direction::North;
        --x;
        ++y;
        break;
    }
}

void Car::turnLeft()
{
    switch (dir)
    {
    case Direction::North:
        --x;
        ++y;
        dir = Direction::West;
        break;
    case Direction::East:
        ++x;
        ++y;
        dir = Direction::North;
        break;
    case Direction::South:
        dir = Direction::East;
        --y;
        ++x;
        break;
    case Direction::West:
        dir = Direction::South;
        --x;
        --y;
        break;
    }
}

void Car::run(const std::vector<Action>& actions)
{
    for (const Action & anAction : actions)
    {
        // anAction = actions[i]
        switch (anAction)
        {
        case Action::Forwards:
            goForward();
            break;
        case Action::Backwards:
            goBackward();
            break;
        case Action::TurnRight:
            turnRight();
            break;
        case Action::TurnLeft:
            turnLeft();
            break;

        }
    }
}

DISABLE_SUITE(CarTurn);
CATEST_F(CarTurn, oop_basic_moves)
{
    Car car;
    std::vector<Action> actions = { Action::Backwards,Action::TurnLeft };
    car.run(actions);

    EXPECT_EQ(car.getX(), -1);
    EXPECT_EQ(car.getY(), 0);
    EXPECT_EQ(car.getDirection(), Direction::West);
}

CATEST_F(CarTurn, map_for_each)
{
    std::map<std::string, std::string> people;
    people.emplace("1950301233545","Gogu Cosmin");
    people.emplace("2900512666243", "Floarea Lizeta");
    people.emplace("2480312453435", "Antica Dedemult");

    for (const auto& entry : people)
    {
        std::cout << "CNP: " << entry.first << " Name: " << entry.second << '\n';
    }


    std::cout << people["2480312453435"] << '\n';

}