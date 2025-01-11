#include <Catest.h>
#include <memory>
#include <iostream>

// Single Linked List - a linked list is a data structure made up of nodes that hold a value and link/reference to the next node in the list



// (Node1 Val: A, Next: Node2) - (Node2 Val: B, Next: Node3) - (Node3 Val: C, Next: null)


template <typename ValueType>
class Node;

//template <typename ValueType>
//using NodePtr = std::shared_ptr<Node<ValueType>>;


template <typename ValueType>
using NodePtr = Node<ValueType> *;

template <typename ValueType>
class Node
{
public:
    ValueType _value;
    NodePtr<ValueType> _next;

    Node()
        : _value{}
        , _next{nullptr}
    {

    }
    Node(const ValueType & value)
        : _value{ value }
        , _next{ nullptr }
    {

    }
};

template <typename ValueType>
class LinkedList
{
protected:
    NodePtr<ValueType> _first;
    NodePtr<ValueType> _last;
public:
    ValueType& operator[](const size_t index)
    {
        throw std::out_of_range("Invaild index");
    }
};




CATEST_F(LinkedListOop, ListPlayground)
{
    Node<int> node1(13);
    Node<int> node2(42);
    node1._next = NodePtr<int>(&node2);
    //node1 -> node2

    Node<int> nodea(22);

    node1._next = NodePtr<int>(&nodea);
    nodea._next = NodePtr<int>(&node2);

   // node1 -> nodea -> node2
    

    for (NodePtr<int> node = NodePtr<int>(&node1); node != nullptr; node = node->_next)
    {
        std::cout << node->_value << " ";
    }
    std::cout << "\n";


    LinkedList<int> list;

    //populate list

    int third = list[3];
}

DISABLE_SUITE(LinkedListOop);