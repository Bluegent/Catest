#include <Catest.h>
#include <iostream>
#include <stdexcept>
#include "MyVector.h"

// we have an array that contains 5 byte values
// address:       0    1    2    3    4
// memory:     [ 10 , 20 , 30 , 40 , 50 ]
// 
// declaration of a pointer: <type> * <name> (opt) = <address>; -> produces a pointer named <name> to the memory address <address> that has the type <type>
// e.g. int32_t * myPointer = 0; -> a pointer named myPointer that holds the address 0 and its type is int32_t
// What is the relevance of the type of the pointer? This just means that the variable itself also has size information
// The pointer knows (or thinks) that the memory address it points to is of a particular size (4 bytes in the case of int32_t)
// This size is relevant to pointer arithmetics and dereferencing.
// 
// memory:     [ 10 , 20 , 30 , 40 , 50 ]
// byte* ptr=0;   ^  // a pointer just stores a memory address
// in this case we store the address of the first element of the array, which is 0 and holds the value 10 
// *<pointer> -  dereference operator(*) - retrieves the value stored at the address which the pointer holds
// *ptr; // will return 10. Why? Because 10 is stored in the memory at address 0.
// byte* ptr2 = 3;
// *ptr2; // will return 40
// 
// in this case, dereferncing ptr will only retrieve one byte of data from the address that ptr holds because ptr is of type byte
// which means that its size is 1 byte
// 
// if ptr was of type int32_t, then dereferncing would attempt to retrieve 4 bytes of data
// 
// let's look a bit at how the memory is actually stored
// address:       0    1    2    3    4
// memory:     [ 10 , 20 , 30 , 40 , 50 ]
// is actually
// 
// memory is a continous arrangement of bits organized into bytes
// each byte has its own address which is represented by a number that corresponds to that particular bytes
// memory is 0-indexed, which means the first byte in memory has address 0
// each memory address is stored as a 32 bit value on 32-bit systems and as a 64 bit value on 64 bit systems
// (you can check this by checking sizeof(void*) - will return 4 on 32-bit systems and 8 on 64 bit systems)
// at the end of the day, a pointer is really just a number that points to a memory cell
// 
// address:              0           1           2           3           4
// memory:     [ 0000 1010 , 0001 0100 , 0001 1110 , 0010 1000 , 0011 0010 ]
// decimal:    [        10 ,        20 ,        30 ,        40 ,        50 ]
// 
// let's say we have two pointers:
// int8_t * bytePtr = 0;
// int32_t * intPtr = 0;
// They both point to the same memory address
// dereferencing bytePtr will give us 1 byte of data because the size of int8_t is 1 byte;
// *bytePtr; // will return  binary: 0000 1010 / decimal: 10
// 
// however, if we derefenrce intPtr, it will retrieve 4 bytes of data because the size of int32_t is 4 bytes
// *intrPtr; // will return binary: 0000 1010 0001 0100 0001 1110 0010 1000 / decimal: 169090600




void modify(int64_t param)
{
    ++param;
}

void modifyPtr(int64_t* param)
{
    ++(*param);
}

void modifyRef(int64_t& param)
{
    ++param;
}

void recursive()
{
    recursive();
}


int32_t fibonacci(int32_t index)
{
    if (index == 0)
    {
        return 0;
    }
    if (index == 1)
    {
        return 1;
    }
    return fibonacci(index - 1) + fibonacci(index - 2);
}


CATEST_F(exercises6, something_something)
{
    int64_t bob = 10; // this allocates an 8 byte memory zone and puts the value 10 at that address
    &bob; // returns the address of this perviously allocated memory zone - we don't know what this is as the address space of the program
    // is dynamic
    // what we do know is that this address is a 64 bit number that starts somewhere after 0 and before 2^64
    // and we don't need to know because the & operator will retireve it for us

    int64_t* alicePtr = &bob;
    // this allocates an 8 byte memory zone and puts the value of the address of the bob variable in it

    int64_t steve = *alicePtr; // dereferncing the alicePtr pointer gives us the value that's stored at the address it holds, or in other words 
    // 10, the value that the bob variable holds
    // so now we steve also holds the value of 10

    *alicePtr += 10; //we can use a dereference pointer to manipulate the value just like we would use the bob variable
    // bob+=10; has the same effect


    int64_t* sarah = &bob;
    int64_t* linda = sarah;

    // we can have multiple pointers pointing to the same memory location
    // in this case sarah and linda both point to bob's memory address because they hold the same value
    bob = 0;
    bob += 10;

    //because sarah, linda and alice all three point to bob's memory address
    // dereferencing all three of them will give us the same value
    // we reset bob to 0 and added 10
    // so all three will give us the value 10

    if (*sarah == *linda == *alicePtr)
    {
        // will happen because all three point to the same address and thus hold the same value
    }


    int64_t** max = &sarah;
    // in this case max is a pointer... to a pointer of type int64_t that holds sarah's address
    // because pointers are themselves also just numeric variables, we can even retrieve their address with the & operator... infinitely
    int64_t*** shadow = &max;

    // what's the utility of pointers?
    // well, think of passing a variable by value to a function
    // because it's passed by value, internally in the function, the parameter is really just a copy of the value given from where the function was called
    // but if we pass a pointer to a variable instead of its value
    // whatever we do will be written to the same memory address, so the change can be reflected outside of the function


    int64_t john = 200;

    modify(john);
    // effect: we copy the value of john(200) and pass it to the modify function under the name param
    // the function then increments this param variable that it was passed
    // however the increment is not reflected upon the john in the local scope, because we really just passed john's value to the function
    bool isJohn200 = (john == 200); // will return true because the modify() function only incremented its copy of john's value

    int64_t* wynona = &john;
    modifyPtr(wynona);
    // effect: instead of copying john's value, this time we pass john's memory address to the modifyPtr function
    // the modifyPtr() function dereferences the addres and increments the value at it
    // because we passed john's local memory address and the operation was done at that memory address, modifyPtr() does not have a copy of john's value
    // thus the operation it executes is reflected outside and now john's value is 201
    bool isJohnIncremented = (john == 201); //will be true

    // C++ introduced references, which are really just constant pointers
    // so in C++ we can simply declare a function parameter as a references to implicitly pass its memory address when calling the function
    // under the hood, however, the modifyRef() function behaves just like the modifyPtr() function with the added benefit that we don't need to manually
    // retrieve john's address or deference that within the body of the function
    modifyRef(john);
    bool isJohnIncrementedAgain = (john == 202); //will be true


    int64_t alex = 42;

    // from the point when we declare a variable, until it goes out of scope, that identifier will always point to a single memory location
    // let's assume the alex variable was allocated to address 0x0000 FFF0(hex)
    // that means that there is nothing we can do to make alex correspond to any other memory address

    int64_t* samantha = &alex;
    // but pointers are not bound by this rule, as they can be freely assigned, reassigned and shifted to the left or right
    samantha++;
    // when samanatha is intialized, it points to 0x0000 FFF0(hex), alex's address
    // when we increment samantha, it will point to 0x0000 FFF8(hex)
    // why? because samantha's type is int64_t a type that has a size of 8 bytes
    // whenever we increment, decrement or otherwise do arithmetic operations on pointers, the address shall changed by a multiple of the size of the type
    // so if we increment samantha by 2, we actually move to the right by 16 bytes
    // if we decrement the samantha by 3, we move to the left by 24 bytes
    // when I say move to the left/right, I mean we add or subtract that value from the value of the pointer

    // because of pointer arithmetics, it is possible to work with arrays
    // as non-pointer variables are forced to always have the same memory address, it would be impossible to retrieve the second or subsequent elements of an array using them
    // however, if a pointer holds the address of the first element in the array
    // then incrementing once and dereferencing the pointer will give us the value of the second element

    //let's say we have a static length array of size 3 of int32_t
    int32_t ben[3] = { 10,20,30 };
    // the [] syntax hides the fact that ben is actually a pointer to type int32_t!
    // when initialized this way, ben will point to the first element in the array

    // we have two ways of getting the second item:
    int32_t bimmy = ben[1];
    // OR
    int32_t jimmy = *(ben + 1);
    //these two have the same effect so...
    bool isSame = (bimmy == jimmy); //will be true
    //it's the same for the third element
    bool isSameThirdElement = (ben[2] == *(ben + 2));

    int32_t glen = 10;
    // there is no way for me to tell the compiler to shift glen in the memory to the right or left 
    // so I can't use glen, a simple variable, to store multiple values in the same way a pointer/array can

    // well... that's not entirely true
    int32_t allen = 20;
    int32_t schmallen = 30;

    //if we get lucky, allen and schmallen might be stored in the memory right after glen

    int32_t* glena = &glen;
    int32_t* glenasSister = glena + 1;
    int32_t* allena = &allen;

    bool haveSameAddr = (glena == glenasSister);
    bool lucky = *(glena + 1) == allen; // might be true, but we have no guarantee because we don't know how the compiler/OS/program will store these variables
    // so this is definitely not a good way to work with "arrays"

    // => pointers are important and the only sane way to treat raw arrays (int arr[])
    // as the compiler guarantees that an array is a contiunous zone of memory, all the elements of a raw array will always come one after the other
    // so we can easily get each value by adding the index to the pointer to the first element to obtain an address and then dereferencing that address
    // this is actually what the [] operator does

    // some syntax disambiguation:
    //      <type> * <name> -> a pointer of type <type> named <name>
    // e.g. int32_t * ralph; -> a pointer to an int32_t named ralph;
    //      *<variable_name> -> dereference the <variable_name> variable, or in other words get the value that's stored at the address held by <variable_name>
    // e.g. *ralph -> dereference ralph, or get the value that's stored at the address that ralph holds

    // similarly for &:
    // <type> & <name>, e.g. int32_t & markus; - a reference to a in32_t variable named markus
    // &<variable_name>  - returns the memory address of <variable_name>
    // e.g.
    // int32_t serena = 20; // let's assume that serena is allocated to 0000 FFA9
    // &serna; // returns 0000 FFA9
    // int32_t * willy = &serena; // because willy is a pointer of the same type as serena, we can store serena's memory address, 0000 FFA9, in willy
    // we can use a memory address in tandem with the & operator, they are basically opposites
    // int32_t selena = *willy; dereference willy to get the value stored at that address
    // but since willy stores serena's address, then the value at that address is serena's value, 20
    // similarly, the operators can be directly chained but they will basically cancel each-other out
    // int32_t sarin_gas = *(&serena)); -> sarin_gas will have a value of 20

    int32_t* gabeOnHeap = new int32_t;
    int32_t  gabeOnStack;
    // new will attempt to allocate 4 bytes (the size of int32_t)
    // the program will tell the OS that it needs 4 bytes of memory
    // if there is enough memory left, the OS will hand the program an address
    // this address is then returned by the new operator and it can be assigned to a variable
    // in our case, we assign that address to gabeOnHeap
    // if there is not enough memory, new shall return nullptr -> hopefully we never have to think about that
    // the line is actually equivalent to the next line
    // with the difference that *gabeOnHeap is an int32_t variable on the heap
    // while gabeOnStack is an int32_t variable on the stack
    delete gabeOnHeap;
    // after we allocate memory with new, we should be curteous and clea up after ourselves
    // because if the gabeOnHeap pointer goes out of scope, the memory will not be automatically cleared
    // however when our program ends, the OS shall automatically mark all memory used by it(stack and heap) as free

    // we can also ask for more than just one block of memory of the size of the type of the variable
    int32_t* hannah = new int32_t[10];
    // in this case, new shall allocate 4 (size of int32_t) * 10 (parameter) = 40 bytes
    // in this case, what we have actually allocated is an array of size 10
    // but the variable hannah itself only points to the first element
    // new shall always return the address of at the very start of the memory block that it allocated
    // because hannah is of type int32_t, when we dereference hannah, the program will grab 4 bytes
    // from that address and convert them to an integer value/variable
    // and I say variable/value because we can actually use dereferncing to put a value at that address
    *hannah = 10; // our array becomes [10,?,?,?,?,?,?,?,?,?,?]
    // we can increment hannah to make it point to the second element
    ++hannah; // if hannah was previously pointing to 0000 0002 (hypothetical value)
    // it shall now point to 0000 0006
    // because int32_t's size is 4 and every byte has a corresponding value
    // so actually when hannah points at 0000 0002
    // the value stored at that address, being int32_t
    // is actually stored on addresses 0000 0002, 0000 0003, 0000 0004 and 0000 0005
    *hannah = 20; // our array becomes [10,20,?,?,?,?,?,?,?,?,?]
    hannah[1] = 30; // our array becomes [10,20,30,?,?,?,?,?,?,?,?]
    // the ptr[n] operator is actually equivalent to *(ptr+n) 
    // since hannah was pointing at the second element already, calling hannah[1]
    // returns the third element of the initially allocated array
    // our vector started out at 0000 0002
    // we increased hannah to    0000 0006
    // calling hannah[1] translates to
    // * (hannah +1)             0000 000A (A is 10 in hexadecimal)

    // because we shifted hannah to the right by one position, we can do this weird thing:
    hannah[-1] = 100; // our array becomes [100,20,30,?,?,?,?,?,?,?,?]
    // which is probably why you shouldn't increment pointers unless you really know what you're doing

    // let's restore hannah to her original position by decrementing
    // and then clean up after ourselves
    // when we allocate a single element using new <type>, we use delete
    // when we allocate multiple elements using new <type>[], we use delete[]
    --hannah;
    delete[] hannah;

    // it's generally good practice to also explicitly set a pointer to nullptr
    // after deletion
    // to make sure no security concerns exists from someone accessing previously allocated memory
    // that has been freed
    hannah = nullptr;
    // why?
    // because if someone gets hannah after we have deleted the memory it used to point to
    // depending on what the OS does
    // that someone might have access to the values of the array hannah was initially pointing to
    // because sometimes OSes don't instantly clear the memory, but simply mark it as free
    // this is a security concern if say hannah is a string that held a user's password
    // we think we've deleted the password from memory
    // but the OS actually just marked that zone as free without clearing the values
    // now that we set hannaht to nullptr, if someone tries to dereference it to get the password
    // they just get access violation from trying to dereference 0
    // the password will still be inside the RAM memory until the OS clears that zone or allocates it to something else
    // but there's no way to know where in the memory it is as nothing points to it any more

    // so what did we learn?
    // 1) don't do pointer arithmetics unless you know what you're doing (or just don't in general!!!)
    // 2) you can't work with arrays without pointers (because non-pointer variables can only point to one memory location)
    // 3) a program's memory is divided into two categories:
    // 3.1) stack
    //          - small (usually 1MB)
    //          - stores plain variables defined like int32_t aVar = 10; until they go out of scope
    //          - stores function calls
    //          - stores function parameters
    // 3.2) heap
    //          - large (up to 4GB on 32 bit systems, up to 128 TB on 64 bit systems - OS dependent)
    //          - stores only things that the program dynamically allocated
    // 3 cont) Both of the stack and heap are addressable byte by byte and the physical source of them is RAM
    // 4) static length stack-stored arrays and dynamic length heap-stored arrays are still using pointers!
    int32_t staticLengthArray[] = { 1,2,3 };
    //      ^^^ still a pointer, although it points to a memory zone on the stack
    int32_t* dynamicLengthArray = new int32_t[3];
    //       ^^^ just like this one, which points to a memory zone on the heap
    delete[] dynamicLengthArray;
    // 5) the <ptr>[n] operator performs pointer arithmetic for you and is probably all you will need
    // 6) if you dynmically allocate memory and store it into a pointer
    //      you should also free the memory and set the pointer to nullptr
    // 
}

void myfunc_copy(MyVector vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << '\n';
}

void myfunc(MyVector& vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << '\n';
}


DISABLE_SUITE(exercises6);
CATEST_F(exercises6, my_vector)
{
    /*
    const MyVector vec;
    size_t vecSize = vec.size(); // we can call the size method on a const MyVec object because the size() function guarantees it won't modify it
    ++vecSize;
    ++vecSize;
    */

    MyVector vec2(10);
    for (size_t i = 0; i < vec2.size(); ++i)
    {
        vec2[i] = i;
    }
    myfunc(vec2);
    /*
    Myector vec2(10); //- original
    vec2
    {
        int34_t* internalArray = new int32_t[10]; -> points to address 0x0000 00cc
        //       ^ is stored at address 0x0001 0000 on the stack
    }
    //when we call myfunc_copy(vec2), a copy of vec2 is created and transformed into myfunc_copy's parameter vec
    void myfunc_copy(MyVector vec); //-> vec is a copy of vec2

    vec
    {
        int34_t* internalArray = vec2.internalArray; // this is how the default compiler-generated copy-constructor assigns this member variable
        //       ^ points to address 0x0000 00cc just like vec2's internalArray
        //       ^ is stored at address 0x0001 0001 on the stack, unlike vec2's internalArray which is at 0x0001 0001
    }

    // at the end of myfunc_copy(), vec(the copy) goes out of scope so the destructor is called
    // the destructor checks if vec.internalArray is nullptr -> it's not, it points to 0x0000 00cc
    // the destructor calls delete[] on vec.internalArray -> the OS marks that memory zone(0x0000 00cc)  as free -> we already have a problem
    // because vec2.internalArray also points to 0x0000 00cc, which is now a deleted memory zone
    // the destructor sets vec.internalArray to nullptr, in other words now the memory address 0x0001 0001 holds the value 0(nullptr)

    // when we return here and the test function finishes, vec2 also goes out of scope -> destructor call
    // the destructor checks if vec2.internalArray is nullptr -> it's not, it (still) points to 0x0000 00cc
    // the destructor attempts to delete vec2.internalArray (0x0000 00cc) but it was already deleted by the copy
    // so we get an exeception that 0x0000 00cc is not a valid heap address
    // this issue happens because the default-generated copy-constructor that the compiler provides
    // does a "shallow" copy of an object
    // this means that we get a copy of the pointer internalArray but not a copy of the memory zone that it points to
    // so every time we make a copy, we get two different internalArray variables that point to the same memory zone
    // and when a copy goes out of scope, the destructor frees that memory zone, leaving the original pointing to invalid, deleted memory

    // in order to avoid this, we must write our own copy-constructor that does a "deep" copy of the object
    // by "deep" copy, we mean that the copy of the object will own and manage its own dynamically allocated memory zone
    // which prevents the issue of the one of them deleting the memory and leaving the other with a variable that still points to the invalid memory


*/
    for (size_t i = 10; i < 20; ++i)
    {
        vec2.push_back(i);
    }
    myfunc(vec2);


}

