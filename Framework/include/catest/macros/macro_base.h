#ifndef CATEST_MACROS_BASE_H
#define CATEST_MACROS_BASE_H
#include <catest/base.h>
#include <catest/fail_string_generators.h>

#define ADD_FAIL(reason,func) base.addFailure( { catest::get_fail_reason(__FILE__,__LINE__,reason,func) })

#endif // !CATEST_MACROS_BASE_h



