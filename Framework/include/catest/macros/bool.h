#ifndef CATEST_MACROS_BOOL_H
#define CATEST_MACROS_BOOL_H
#include <catest/macros/macro_base.h>

#define ADD_FAIL_VALUE(condition,value,func) ADD_FAIL(catest::generate_value_fail_string(condition,#condition,value),func)


#define ASSERT_TRUE(condition) if(!(condition)) { ADD_FAIL_VALUE(condition,true,"ASSERT_TRUE"); return;};
#define EXPECT_TRUE(condition) if(!(condition)) { ADD_FAIL_VALUE(condition,true,"EXPECT_TRUE");        };

#define ASSERT_FALSE(condition) if(condition) { ADD_FAIL_VALUE(condition,false,"ASSERT_FALSE"); return;};
#define EXPECT_FALSE(condition) if(condition) { ADD_FAIL_VALUE(condition,false,"EXPECT_FALSE");        };


#endif // 
