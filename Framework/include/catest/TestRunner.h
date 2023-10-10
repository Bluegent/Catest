#ifndef CATEST_TEST_RUNNER_H
#define CATEST_TEST_RUNNER_H

#include <vector>
#include <string>
#include <catest/base.h>


namespace catest
{
    struct TestRunner
    {
    private:
        std::vector<std::string> disabled;
        TestRunner();
        static TestRunner* instance;
        test_collection tests;
        void run();
    public:
        static TestRunner& get_instance();
        static void run_tests();
        void disable_suite(const std::string& name);
        void pushTest(TestBase& test);
    };
}

#endif // !CATEST_TEST_RUNNER_H
