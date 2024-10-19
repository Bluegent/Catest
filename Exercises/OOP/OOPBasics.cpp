#include <Catest.h>
#include <iostream>
#include <memory>

//SOLID
//Interface, Abstract Class
//Encapsulation
//RAII




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
        : age{ 0 }
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
        : age{ other.age } //typically copies all the values from the parameter instance to this instance (this is not a rule)
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
} // everything that goes out of scope shall be destructed


// Rule of 5 - every time you implemente one of the five following functions, you should (not: you must) write all the others as well:
// - default constructor
// - copy constructor
// - copy assignment operator
// - destructor
// - move constructor
// - move assignment operator



//Smart Pointers
// Shared/Weak/Unique Pointer


class Widget
{
protected:
    std::string name;
    int32_t id;
public:

    Widget(const std::string& name, const int32_t id)
        : name{ name }
        , id{ id }
    {
    }

    void doThing()
    {
        std::cout << "Name:" << name << " Id: " << id << "\n";
    }
};


CATEST_F(OOP, OOPPlaygroundSmartPtr)
{

    // value variables like this are allocated to the stack memory, which is limited to 1MB
    Widget thing("TestWidget", 10);
    // stack-bound objects will automatically be deconstructed when going out of scope

    // if our class takes up a lot of memory, let's say ~500 kB, we don't want to occupy half the stack as the whole program uses it
    // which means we should use the heap
    // in order to use the heap, we need to use the 'new' operator
    // the 'new' operator allocates memory for a type and returns a pointer to it
    // which means in order to store the result, we need a pointer
    // this is also called dynamic memory allocation
    // when calling the new operator for a program-defined type, we must also provide constructor parameters (when it is the case)
    Widget* dynamicWidget = new Widget("DynamicWidget", 20);
    dynamicWidget->doThing();
    // dynamic/heap allocated objects DO NOT get automatically deconstructed, so we must explicitly call the delete operator for them
    // when we no longer need them
    // the 'delete' operator does 2 things:
    // calls the destructor of the object
    // marks the memory as "free", so that it can be used for other things

    // plain pointers are copyable, we can have any number of pointers pointing to the same instance
    Widget* plainWidgetCopy = dynamicWidget;
    plainWidgetCopy->doThing();

    // when we delete the oringal instance, though, the copies will now be dangling pointers
    // which means they point to an already deleted memory zone
    delete dynamicWidget;

    // plainWidgetCopy->doThing(); // undefined behaivor!


    // when using new we always have to manually manage the memory, which means when we no longer need it, we have to explicitly delete it
    // manually managing memory can lead to accidental memory leaks if we forget to delete what we allocate
    // to offset what is called a raw/dumb/plain pointer, we have the concept of smart pointers
    // the C++11 standard template library provides : unique_ptr, shared_ptr and weak_ptr


    // these three are available inside the <memory> header
    {
        // unique_ptr<T> - denotes that ownership of the instance belongs to one and only entity
        std::unique_ptr<Widget> uniqueWidget = std::make_unique<Widget>("UniqueWidget", 42); // <- call to constructor, uses 'new' under the hood
        // in order to make a unique pointer, we must use the function std::make_unique<T>
        // and pass the parameters we would regularly pass to a constructor call
        uniqueWidget->doThing();

        // the advantage of unique_ptr is that although it dynamically allocates memory
        // we don't have to explicitly delte it
        // once the unique_ptr goes out of scope, the destructor of the object is called
        // and the memory is freed
    } // <- because uniqueWidget goes out of scope, it will be deleted here automatically

    //unique_ptr is non-copyable, which means only one pointer to that instance can exist
    //std::unique_ptr<Widget> copyUniqueWidget = uniqueWidget; // <- not allowed
    {
        // shared_ptr<T> - unlike unique_ptr, shared_ptr is copyable which means ownership of the instance is shared between whoever has a reference to it
        std::shared_ptr<Widget> sharedWidget = std::make_shared<Widget>("SharedWidget", 13); // <- call to constructor, uses 'new' under the hood
        // in order to make a shared_ptr, we must use std::make_shared<T>       
        sharedWidget->doThing();

        // shared_ptr internally holds a reference count
        // when one is created, the reference count starts at 1
        // when the shared_ptr is copied, the reference count incremented by 1
        // when a copy of the shared_ptr goes out of scope, the reference count is decremented by 1
        // when the reference count reaches 0, the shared pointer automatically calls the destructor and frees the memory it holds

        {
            std::shared_ptr<Widget> sharedWidgetCopy = sharedWidget; //allowed, as shared_ptr is copyable
            // reference count of sharedWidget goes up to 2
        } // <- sharedWidgetCopy goes out of scope so reference count goes back to 1
    } // <- sharedWidget goes out of scope, reference count is now 0 so the Widget instance is deleted
    // the advantage(and minor disadvantage) of shared_ptr is that as long as someone holds a reference to the instance
    // the instance will not be deleted!
    // this means that if holding a shared_ptr, we have the guarantee that it points to a valid memory location (if it was created properly)
    // but this also leads to the potential circular reference problem
    // e.g. instance "a" of class A holds a shared_ptr to instance "b" of class B, instance b holds a reference to instance "a" -> neither will ever go out of scope
    // so neither will be deleted unless one of the references is explicitly dropped
    // circular references can lead to unexpected memory leaks


    // weak_ptr<T> - only holds a weak reference to a shared ptr - this means it does not have ownership
    // weak_ptr points to something that may or may not be a valid instance
    // the advantage of weak_ptr is that it does not increase the reference count of a shared ptr
    // which means that the entity holding a weak ptr does not want to extend the lifetime of the 
    // resource it points to
    std::shared_ptr<Widget> shareableWidget = std::make_shared<Widget>("UniqueWidget", 42);
    std::weak_ptr<Widget> weakWidget = shareableWidget;

    // furthermore, weak_ptr cannot be used in the same way as a plain/raw pointer or a shared/unique ptr
    // in order to use the resource, we must temporarily convert it to a shared_ptr
    // weakWidget->doThing(); // <- not allowed
    {
        std::shared_ptr<Widget> temporaryWidget = weakWidget.lock(); // <- this operation might return a nullptr if the shared_ptr the weak_ptr refers to has been deallocated
        // do our stuff with temporaryWidget
        if (temporaryWidget) // but before we do stuff, we have to check if the pointer is actually valid 
        {
            temporaryWidget->doThing();
        }
    }

}




class ComplexWidget
{
protected:
    Widget internalWidget; // if ComplexWidget holds a Widget object directly, then each instance of ComplexWidget will also own an instance of Widget
    // this might be fine, but what if we want to share Widgets between our ComplexWidgets?
    // every time we would make a copy of ComplexWidget, we would actually make a deep copy of the Widget
    // then if we modify the original Widget, the copy Widget would not reflect this change -> depending on what we want to do this might be good or not!
public:

    ComplexWidget(const std::string& widgetName)
        : internalWidget(widgetName, 0)
    {

    }
    void doComplexThing()
    {
        internalWidget.doThing();
    }
};

// but what if we want to share the instance of the widget?
// option 1 - hold a reference


class ComplexWidgetRef
{
protected:
    Widget& internalWidget; // this means that we won't hold copies of the Widget between instances of ComplexWidgetRef, only copies of the reference itself
    // this means if we modify one Widget, then this will be reflected across all copies of ComplexWidgetRef
public:
    ComplexWidgetRef(const std::string& widgetName)
        : internalWidget{ Widget(widgetName, 0) } // this won't work because the reference is pointing to an rvalue object that goes out of scope at the end of the constructor!
    {

    }

    ComplexWidgetRef(Widget& widget) //requires that the memory for widget be managed externally, we don't know if it's on the stack or heap, less control etc.
        : internalWidget{ widget }
    {

    }

    ComplexWidgetRef(const ComplexWidgetRef& other) // problem: we need an explicit copy constructor to properly copy the reference
        : internalWidget{ other.internalWidget }
    {

    }
    void doComplexThing()
    {
        internalWidget.doThing();
    }
};



//option 2 - plain pointer

class ComplexWidgetPtr
{
protected:
    Widget* internalWidget;// this means that we won't hold copies of the Widget between instances of ComplexWidgetPtr, only copies of the pointer itself
public:
    ComplexWidgetPtr(const std::string& widgetName)
        : internalWidget{ new Widget(widgetName, 0) } // we explicitly dynamically allocated memory for the widget so we must be careful to also delete it
    {

    }

    ~ComplexWidgetPtr()
    {
        delete internalWidget;
        // if we have a copy of ComplexWidgetPtr, now the internalWidget pointer of that copy is a dangling pointer that points to an invalid memory location!
        // furthermore, when the original gets destructed, it might attempt to delete the original pointer again -> access violation
    }

    void doComplexThing()
    {
        internalWidget->doThing();
    }
};


//option 3 - shared_ptr

class ComplexWidgetSharedPtr
{
protected:
    std::shared_ptr<Widget> internalWidget;// this means that we won't hold copies of the Widget between instances of ComplexWidgetPtr, only copies of the shared_ptr itself
public:
    ComplexWidgetSharedPtr(const std::string& widgetName)
        : internalWidget{ std::make_shared<Widget>(widgetName, 0) }
    {

    }

    ~ComplexWidgetSharedPtr()
    {
        // no longer necessary to explicitly delete internalWidget
        // when all instances of ComplexWidgetSharedPtr that hold a reference to the same Widget object go out of scope, it shall automatically be deleted
        // in fact, we don't even need the destructor but it was kept for the sake of the comment
    }

    void doComplexThing()
    {
        internalWidget->doThing();
    }
};


CATEST_F(OOP, OOPPlaygroundSmartPtrComplex)
{
    Widget widget("TestRef", 0);
    ComplexWidgetRef ref(widget);
    ref.doComplexThing();
    ComplexWidgetRef copyRef = ref;
    copyRef.doComplexThing();

    ComplexWidgetPtr ptr("TestPtr");
    {
       // ComplexWidgetPtr copyPtr = ptr; // this must be commented, otherwise the test will always fail
       // copyPtr.doComplexThing();
    } // copyPtr goes out of scope and deletes the internalWidget
    ptr.doComplexThing(); // if the above scope is uncommented => undefined behavior as internalWidget is a dangling pointer

    ComplexWidgetSharedPtr shPtr("TestSharedPtr");

    {
        ComplexWidgetSharedPtr shPtrCopy = shPtr;
        shPtrCopy.doComplexThing();
    } // <- shPtrCopy goes out of scope, but internalWidget is not deleted, the reference count just decreases by 1

    shPtr.doComplexThing(); // valid as internalWidget is still a valid instance
}
