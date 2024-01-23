#include <Catest.h>
#include <iostream>
#include <string>


template <typename T>
class Node
{
public:
    T value;
    Node<T>* next;
    Node<T>* previous;

    Node(const T& value)
        : value{ value }
        , next{}
        , previous{}
    {
    }

    void link(Node<T>* nextNode)
    {
        next = nextNode;
        nextNode->previous = this;
        // (*nextNode).previous = this;
    }
};

DISABLE_SUITE(LinkedList);
CATEST_F(LinkedList, Pointers)
{
    //ValueHolder<int32_t> holder{};
    int64_t aBigValue = std::numeric_limits<int64_t>::max();
    int32_t* intPtr = reinterpret_cast<int32_t*>(&aBigValue);
    int64_t* longPtr = &aBigValue;

    std::cout << aBigValue << '\n';
    std::cout << intPtr << ": " << *intPtr << '\n';
    std::cout << longPtr << ": " << *longPtr << '\n';

    int32_t arr[] = { 10,20,30 };
    int32_t* arrPtr = arr;
    int64_t* longArrPtr = reinterpret_cast<int64_t*>(arrPtr);

    std::cout << arr[0] << " " << arr[1] << " " << arr[2] << '\n';
    std::cout << arrPtr << ": " << *arrPtr << '\n';
    std::cout << (arrPtr + 1) << ": " << *(arrPtr + 1) << '\n';
    std::cout << longArrPtr << ": " << *longArrPtr << '\n';
    std::cout << (longArrPtr + 1) << ": " << *(longArrPtr + 1) << '\n'; //undefined behavior

    Node<int32_t> realNode{ 10 };
    Node<int32_t>* node = &realNode;
    Node<int32_t>* nullNode{};

    std::cout << node << '\n';
    std::cout << node->value << '\n';
    std::cout << (*node).value << '\n';
    std::cout << realNode.value << '\n';

    std::cout << nullNode << '\n';

    std::cout << realNode.next << '\n';

    Node<int32_t> secondNode{ 23 };

    std::cout << secondNode.next << '\n';

    realNode.next = &secondNode;

    std::cout << realNode.next << '\n';
    std::cout << &secondNode << '\n';

    std::cout << realNode.value << ' ' << realNode.next->value << '\n';

    Node<int32_t> thirdNode{ 42 };

    secondNode.next = &thirdNode;

    std::cout << realNode.value << ' ' << realNode.next->value << ' ' << realNode.next->next->value << '\n';

}


Node<std::string>* getEndNode(Node<std::string>* n)
{

    if (n == nullptr)
    {
        return nullptr;
    }

    Node<std::string>* currentNode = n;
    
    do
    {
        if (currentNode->next == nullptr)
        {
            return currentNode;
        }
        currentNode = currentNode->next;

    } while (currentNode != nullptr);
}


Node<std::string>* getStartNode(Node<std::string>* n)
{
    if (n == nullptr)
    {
        return nullptr;
    }

    Node<std::string>* currentNode = n;

    do
    {
        if (currentNode->previous == nullptr)
        {
            return currentNode;
        }
        currentNode = currentNode->previous;

    } while (currentNode != nullptr);
}

CATEST_F(LinkedList, BrowserBack)
{
    Node<std::string> startNode{ "//home" };

    Node<std::string> google{ "google.com" };
    startNode.link(&google);

    Node<std::string> search{ "google.com/search/q?=hello_world" };
    google.link(&search);

    Node<std::string> last{ "learncpp.com/hello_world" };
    search.link(&last);

    EXPECT_EQ(getEndNode(&startNode)->value, last.value);
    EXPECT_EQ(getStartNode(&last)->value, startNode.value);

    EXPECT_EQ(last.previous->value, search.value);
}