#ifndef CATEST_MACROS_TEST_BASE_H
#define CATEST_MACROS_TEST_BASE_H

#include <catest/TestBase.h>
#define CATEST_F(suite,name) void name(catest::TestBase& base); \
namespace catest{ \
        class TestBase_##name : public TestBase { \
        public: \
            TestBase_##name() : TestBase(#name,#suite,&name) {} \
        } registerer_##name;}\
void name(catest::TestBase& base) \




#endif // !CATEST_MACROS_TEST_BASE_H
