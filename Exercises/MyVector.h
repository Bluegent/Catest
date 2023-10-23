#pragma once
#include <stdexcept>
#include <cstdint>

// write a class that functions similarly to std::vector<int32_t>
class MyVector
{
private:
    size_t elementCount;
    int32_t* _data;
    size_t pushPosition;
    size_t _capacity;
public:

    MyVector()  // default constructor - builds an empty array of size 0
        : _data{ nullptr }
        , elementCount{ 0 }
        , pushPosition{ 0 }
        , _capacity{ 0 }
    {

    }

    MyVector(const size_t size) // parameter constructor - builds an array of size <size> of uninitialized elements
        : _data{ new int32_t[size] }
        , elementCount{ size }
        , pushPosition{ size }
        , _capacity{ size }
    {
        for (size_t i = 0; i < elementCount; ++i)
        {
            _data[i] = 0;
        }
    }
    /*
    MyVector a = b; -> this calls the copy contstructor which is something like:
    a.MyVector(b); -> in other words we are constructing a from b, so the parameter const MyVector& other for a's copy constructor is b
    */
    MyVector(const MyVector& other) // copy constructor 
        : _data{ new int32_t[other.elementCount] } // we allocate memory for the internal vector
        , elementCount{ other.elementCount }
        , pushPosition{ other.pushPosition }
        , _capacity{ other._capacity }
    {
        // and do a "deep" copy - we copy the internal array element by element so that
        // the copy manages its own internal array
        // this ensures that when the copy goes out of scope, it will delete its own managed memory
        // and thus does not cause issues with the original's managed memory/array
        for (size_t i = 0; i < elementCount; ++i)
        {
            _data[i] = other._data[i];
        }
    }

    ~MyVector()
    {
        delete[] _data;
        _data = nullptr;
    }


    size_t size() const // const tells the compiler that this member function won't modify the object the function is called on
    {
        return elementCount;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    int32_t* data() const
    {
        return _data;
    }

    void resize(size_t newCapacity)
    {
        int32_t* newInternalArray = new int32_t[newCapacity]; // we allocate new memory
        size_t min = newCapacity > _capacity ? _capacity : newCapacity; // we figure out which one is the smallest array
        // if the old internal array is not null, then 
        if (_data != nullptr)
        {
            // we copy from 0 to the smallest size of the two into the new memory zone
            for (size_t i = 0; i < min; ++i)
            {
                newInternalArray[i] = _data[i];
            }
            // we delete the old memory since we no longer need it 
            delete[] _data;
        }
        //and finally we assign the old internal pointer to the new one
        _data = newInternalArray;
        _capacity = newCapacity; // and we update the capacity to reflect how much the array has grown/shrunk
        if (_capacity < elementCount)
        {
            elementCount = _capacity;
        }
    }


    void push_back(const int32_t value)
    {
        if (pushPosition >= _capacity)
        {
            resize(_capacity + _capacity / 2);
        }
        ++elementCount;
        _data[pushPosition++] = value;
    }

    int32_t& operator[](const size_t index)
    {
        if (_data == nullptr || index >= elementCount)
        {
            throw std::out_of_range("index out of range or data pointer is null (MyVector)");
        }
        return _data[index];
    }
};