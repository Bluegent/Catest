#ifndef CATEST_RUNNER_MACROS_H
#define CATEST_RUNNER_MACROS_H

#include <catest/TestRunner.h>

#define DISABLE_SUITE(suite) \
namespace catest { \
    struct TestSuiteDisabler##suite {\
    TestSuiteDisabler##suite(){\
            TestRunner::get_instance().disable_suite(#suite); \
        }\
    } ##suite_disabler;\
}\

#endif // !CATEST_RUNNER_MACROS_H
