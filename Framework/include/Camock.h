#ifndef CATEST_CAMOCK_H
#define CATEST_CAMOCK_H
#include <typeinfo>
#include <vector>

namespace catest
{
    class ExpectedCall
    {
    public:
        void* functionPtr;
        std::vector<void*> params;
    };
}

#endif // !CATEST_CAMOCK_H
