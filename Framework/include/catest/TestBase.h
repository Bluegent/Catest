#ifndef CATEST_TEST_BASE_H
#define CATEST_TEST_BASE_H

#include <catest/base.h>

namespace catest
{
    class FailureReason
    {
    private:
        const std::string reason;
    public:
        FailureReason(const std::string& reason) : reason{ reason } {}
        const std::string& get() const { return reason; }
    };

    class TestBase
    {
    private:
        std::vector<FailureReason> reasons;
        const std::string name;
        const std::string suiteName;
        func function;
    public:
        TestBase(const std::string& name, const std::string& suiteName, func function);
        bool run();
        void addFailure(const FailureReason& reason);
        bool isFailed() const;
        const std::string& getName() const;
        const std::string& getSuite() const;
        const void printFailure();
    };
}

#endif // !CATEST_TEST_BASE_H
