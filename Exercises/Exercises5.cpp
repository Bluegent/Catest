#include <Catest.h>
#include <iostream>

class Animal
{
public:
    virtual void make_noise() = 0;
};

class Cat : public Animal
{
public:
    virtual void make_noise()
    {
        std::cout << "Meow!\n";
    }
};

class Dog : public Animal
{
public:
    virtual void make_noise()
    {
        std::cout << "Bark!\n";
    }
};


CATEST_F(exercise5, animal_test)
{
    std::vector<Animal*> animals;

    Cat Nox;
    Dog Ruff;
    Cat Mozart;
    Dog Fetita;
    animals.push_back(&Nox);
    animals.push_back(&Ruff);
    animals.push_back(&Mozart);
    animals.push_back(&Fetita);

    for (size_t i = 0; i < animals.size(); ++i)
    {
        animals[i]->make_noise();
    }

}



// model a Shape base class that has an area and perimeter member function
// from this Shape class, derive a Circle and a Square class and implement the two member functions for each

class Shape
{
public:
    virtual float area() = 0;
    virtual float perimeter() = 0;
};

const float pi = 3.1415f;
class Circle : public Shape
{
private:
   
public:
    float radius;
    virtual float area()
    {
        return radius * radius * pi;
    }
    virtual float perimeter()
    {
        return 2.f * pi * radius;
    }
};

class Square : public Shape
{
private:
    float side;
public:
    Square()  // default constructor -> takes no parameters
        : // after the constructor name, the column (:) marks the start of the initializer list
        side{ 0.f } // in the initializer list, you initialize (ideally every) member variable(s) of the class
    {  // after the initializer list we have the constructor body where we can do anything we can do in any other function
    }

    Square(float side_t) // parameter constructor - you can have as many parameters as you want
        : side{ side_t }
    {

    }

    float getSide()
    {
        return side;
    }

    void setSide(float side)
    {
        this->side = side;

    }

    virtual float area()
    {
        return side * side;
    }
    virtual float perimeter()
    {
        return side * 4.f;
    }
};


CATEST_F(exercise5, shape_test)
{
    Circle circle;
    Square square(5.f);

    square.getSide();

    circle.radius = 10.f;

    EXPECT_EQ(square.area(), 25.f);
    EXPECT_EQ(square.perimeter(), 20.f);
    
    float area = circle.area();
    float perim = circle.perimeter();
}

DISABLE_SUITE(exercise5);