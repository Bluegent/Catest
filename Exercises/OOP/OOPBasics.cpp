#include <Catest.h>
#include <iostream>


//SOLID
//Interface, Abstract Class
//Encapsulation
//RAII
//Smart Pointers



// const methods - when a method has const at the end e.g.:
class Object
{
private:
    float weight{ 10.f };
public:
    float getWeight() const
    {
        return weight;
    }
};
// this signifies that the method can be called on const instances of that type because
// it guarantees that the state of the object shall NOT BE MODIFIED in the scope of the method
// in our example, getWeight() does not modify weight, but simply returns a copy of it



// Inheritance - a class that is derived from a base class inherits behaviors and members
// Inheritance = IS A , if class Cat inherits class Animal, then we say that a Cat is an Animal
// What is inherited? - methods and members, anything that is public, private or protected, but access is given only to public and protected 
// Can you inherit multiple base classes? Yes. Potentially leads to the diamond problemd.
// upcasting - treating a Derived class as a base class 
// upcasting is guaranteed to succeed as any Derived IS A Base
// downcasting - treating an instance of Base as an instant of Derived
// downcasting is NOT guaranteed to succeed as you don't know for sure if the instance of Base IS Derived1 or Derived2 or ...

class Base
{
protected:
    bool aMember{ true };
    int32_t value{ 0 };
public:
    Base& increment()
    {
        ++value;
        return *this;
    }

    int32_t getValue()
    {
        return value;
    }
};


class Derived : public Base
{
private:
    bool anotherMemmber{ false };
public:
    void anotherMethod()
    {

    }
};

CATEST_F(OOP, OOPPlaygroundInheritance)
{
    Base baseObj;
    std::cout << baseObj.getValue() << "\n";
    baseObj.increment().increment().increment();
    std::cout << baseObj.getValue() << "\n";

    Derived derivedObj;
    std::cout << derivedObj.getValue() << "\n";
    derivedObj.increment().increment().increment();
    std::cout << derivedObj.getValue() << "\n";
}


class Entity
{
public:
    virtual void exist()
    {
        std::cout << "Exist as an entity\n";
    }
};

class Robot : public Entity
{
public:
    void beepbeep()
    {

    }

    virtual void exist() override
    {
        std::cout << "Exist as a robot\n";
    }

};

class Animal : public Entity
{
public:
    void makeNoise()
    {
    };
    virtual void exist() override
    {
        std::cout << "Exist as an animal\n";
    }

};

class RoboDog : public Robot, public virtual Animal
{

};

CATEST_F(OOP, OOPPlaygroundInheritance2)
{
    RoboDog doggy;
}


// Polymorphism / virtual methods
// virtual specifier - we use virtual when we want a descendant to be able to override behavior of the base class
// pure virtual method - a method that is marked as virtual and also = 0 -> signifies that the method has no implementation in the base class
// => a class that has at least one pure virtual method is an Abstract Class
// an abstract class cannot be instantied! 
// Why? Because if we could instantiate one, then called the pure virtual method, we would get an error as there is no implementation for it!
// As a consequence, any class deriving from an abstract class must
// 1. implement all the pure virtual methods in order to be instantiated OR
// 2. also be an abstract class

class Vehicle
{
public:
    virtual uint32_t getWheelCount() const = 0;
    virtual ~Vehicle() = default;
};


// When overriding an inherited virtual method
// the derived class can keep or remove the virtual specifier -> this means you can prevent further descendents from overriding the method
// the derived class must mark the method as "override"
class Car : public Vehicle
{
public:
    virtual uint32_t getWheelCount() const override
    {
        return 4u;
    }
    void startEngine()
    {

    }
};

// Polymoprhism - the ability for derived classes to be treated/behave like their base class

class Motorbike : public Vehicle
{
public:
    virtual uint32_t getWheelCount() const override
    {
        return 2u;
    }
};

// We have the following hierarchy: Vehicle <- Car
//                                          <- Motorbike
// We know that vehicles behave a certain way, in this case they allow us to know how many wheels they have
// So with polymorphism, we can treat any instance of Car or Motorbike as a vehicle
// AND be guaranteed that they will behave as their derived type .e.g Cars have 4 wheels, Motorbikes have two wheels


CATEST_F(OOP, OOPPlaygroundPolymorphism)
{
    // Vehicle car; // cannot instantiate as Vehicle is an abstract class
    Car ford;
    Motorbike yamaha;
    ford.getWheelCount(); // will return 4
    yamaha.getWheelCount(); // will return 2

    //polymorphism allows us to store both these derived type as either pointers or references to Vehicle
    Vehicle& yamahaAsVehicle = yamaha;
    Vehicle& fordAsVehicle = ford;
    // and despite the base type, we still get correct behavior
    fordAsVehicle.getWheelCount(); // will return 4
    yamahaAsVehicle.getWheelCount(); //will return 2

    Car mini;
    Car& miniRef = mini;
    miniRef.startEngine();
    Vehicle& miniAsVehicle = mini;
    miniAsVehicle.getWheelCount(); // will return 4

    // Polymorphism allows us to store collections of different objects as their base type and treat them the same
    std::vector<const Vehicle*> vehiclesIOwn;
    
    // because vehiclesIOwn is a vector of Vehicle&, we can push anything derived from Vehicle in it, including types that
    // at the time of writing this code do not exist yet!
    vehiclesIOwn.push_back(&ford);
    vehiclesIOwn.push_back(&yamaha);
    vehiclesIOwn.push_back(&mini);

    for (size_t i = 0; i < vehiclesIOwn.size(); ++i)
    {
        vehiclesIOwn[i]->getWheelCount(); // will return depending on the true type of that vehicle
    }
}

// Conclusion: Polymorphism allows us to treat a derived type as a base type and still get the correct derived type behavior
// as long as the base class method we are using is marked as virtual


class Obj
{
private:
    int32_t aValue{ 0 };
public:
    int32_t getValue() const
    {
        return aValue;
    }
};


class ObjWithVirtual
{
private:
    int32_t aValue{ 0 };
public:
    virtual int32_t getValue() const
    {
        return aValue;
    }
};

CATEST_F(OOP, OOPPlaygroundPolymorphism2)
{
    std::cout << sizeof(Obj) << "\n"; // will print 4 as the int32_t from Obj is 4 bytes
    std::cout << sizeof(ObjWithVirtual) << "\n"; // will print 12 + padding, 4 bytes from the int32_t and an additional 8 bytes from the vtable pointer
    // the vtable/virtual table is a structured used intenrally by the program to call the correct virtual method on an object depending on its type
}
// conclusion: virtual methods increase the size in memory of your classes!




//Aggregation/Composition
// Composition = HAS A relationship -if an instance of class A HAS An instance of class B, then we say that we have composition
// Composition also means class A has ownership/manages the lifetime of the instance of class B 

namespace comp
{
    class Engine
    {
    public:
        void makeNoise()
        {
            std::cout << "Vroom!\n";
        }
    };

    class Car
    {
    protected:
        Engine engine; // composition: Car HAS A Engine
        //furthermore, if the Car goes out of scope, so shall the Engine, meaning the car has ownership and control over the lifetime of the composed class
    public:
        void drive()
        {
            engine.makeNoise();
        }
    };
}


// Aggregation - Composition but the object does not have ownership of its dependency

namespace aggr
{
    class Engine
    {
    public:
        void makeNoise()
        {
            std::cout << "Vroom!\n";
        }
    };

    class Car
    {
    protected:
        Engine* engine{ nullptr }; // aggregation: Car HAS A Engine
        //furthermore, if the Car goes out of scope, the engine shall still exists as its lifetime is managed externally
    public:
        void setEngine(Engine* engine)
        {
            this->engine = engine;
        }
        void drive()
        {
            engine->makeNoise();
        }
    };
}


//Association
//Association = USES A

// When a class does not own or inherit from another class, but still makes use of an external instance within a method call

class Pen
{
public:
    void write() 
    {

    }
};

class ContractSigner
{
public:
    void sign(Pen& pen)
    {
        pen.write();
    }
};


// Relationships in order of strength of coupling/depdency between classes:
// Association < Aggregation < Composition < Inheritance



//Constructors/Destructors - special methods for classes/structs

// Constructor
// - constructs an instance of a class
// - has no return type
// - has the same name as the class itself
// - may or may not have parameters => there are different types of constructors
// - several default ones are provided if you don't write your own
// - the default constructor is automatically called when declaring a local variable of the class type

// Destructor
// - destructs the instance of the class
// - has no return type
// - has the same name as a class but preceded by a ~
// - does not have any parameters
// - is automatically called when a local instance of the class goes out of scope

class Billy
{
protected:
    uint32_t age;
public:
    Billy() // default constructor
        : age{0}
    {
        std::cout << "Billy is alive!\n";
    }
    // A parameter constructor has any amount of parameters
    Billy(const uint32_t age) // parameter constructructor
        // initializer list - a list of the member variables in the order of their declaration as well as what value the initialize them with
        : age{ age } 
    {
        std::cout << "Billy is alive and " << age << " years old!\n";
    }

    //copy constructor
    Billy(const Billy& other) // always takes a const ref of the class as a parameter
        : age{other.age} //typically copies all the values from the parameter instance to this instance (this is not a rule)
    {
        std::cout << "Billy is alive and " << age << " years old and a clone!!!\n";
    }

    ~Billy() // destructor
    {
        std::cout << "Billy is dead!\n";
    }
};


CATEST_F(OOP, OOPPlaygroundCosntrDestr)
{
    {
        Billy billy;
    } // billy goes out of scope, so we get a call to the destructor
    Billy william(35); // call to parameter constructor
    Billy willClone = william; // call to copy constructor
}


// Rule of 5 - every time you implemente one of the five following functions, you should write all the others as well:
// - default constructor
// - copy constructor
// - copy assignment operator
// - destructor
// - move constructor
// - move assignment operator