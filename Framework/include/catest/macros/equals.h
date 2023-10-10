#ifndef CATEST_MACROS_EQUALS_H
#define CATEST_MACROS_EQUALS_H

#include <catest/macros/macro_base.h>

#define ADD_FAIL_EQ(left,right,func) ADD_FAIL(catest::generate_eq_fail_string(left,#left,right,#right),func)
#define ASSERT_EQ(left,right) if(!(left==right)) { ADD_FAIL_EQ(left,right,"ASSERT_EQ"); return; }
#define EXPECT_EQ(left,right) if(!(left==right)) { ADD_FAIL_EQ(left,right,"EXPECT_EQ");         }

#define ADD_FAIL_NEQ(left,right,func) ADD_FAIL(catest::generate_neq_fail_string(left,#left,right,#right),func)
#define ASSERT_NEQ(left,right) if(left==right) { ADD_FAIL_NEQ(left,right,"ASSERT_NEQ"); return; }
#define EXPECT_NEQ(left,right) if(left==right) { ADD_FAIL_NEQ(left,right,"EXPECT_NEQ");         }


#endif // !CATEST_MACROS_EQUALS_H
