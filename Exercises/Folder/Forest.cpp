#include <cstdint>
#include <Catest.h>
#include <iostream>
// Simulate a simplified forest environment
// 

// Wolves
// Every season, each wolf will attempt to eat one rabbit ( 50% success rate)
// If a wolf goes without eating for two seasons in a row, it dies
// Every 4 seasons, all the wolves that have eaten in the current season shall mate in pairs, producing 1-2 offspring
// when two wolves mate, they form a pack until the offspring grow up
// a pack try to hunt a rabbit for every member of the pack
// wolf offspring mature in 4 seasons and then are able to hunt themselves
// wolves shall live for 8 * 4 seasons
// Rabbits
// every season, if not hunted, a rabbit will eat a bush and spread its seeds, producing 1-3 other bushes
// every 2 seasons, rabbits that have eaten in the current season shall mate, producing 2-3 offspring
// rabbit cubs mature in 1 season and can start eating bushes
// rabbits shall live for 2 * 4 seasons
// Bushes
// seedling bushes take 4 seasons to mature


using Age = uint32_t;


namespace Constants
{
    const Age BushMaturityDuration = 4;
    const Age RabbitMaturityDuration = 2;
    const Age WolfMaturityDuration = 4;
};


class ForestEntity
{
protected:
    Age age{ 0 };
    bool alive{ true };
public:
    void advanceAge()
    {
        ++age;
    }
    Age getAge() const
    {
        return age;
    }
    void kill()
    {
        alive = false;
    }
    bool isAlive() const
    {
        return alive;
    }

    virtual bool isMature() const = 0;
};

class Bush : public ForestEntity
{
private:
    
public:
    virtual bool isMature() const override
    {
        return age >= Constants::BushMaturityDuration;
    }
};

class Rabbit : public ForestEntity
{
private:

public:
    virtual bool isMature() const override
    {
        return age >= Constants::RabbitMaturityDuration;
    }
};
class Wolf : public ForestEntity
{
private:

public:
    virtual bool isMature()const override
    {
        return age >= Constants::WolfMaturityDuration;
    }
};


CATEST_F(Forest, Playground)
{
}