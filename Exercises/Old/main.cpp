#include <Catest.h>
#include <iostream>



int main(int argc, const char * argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        std::cout <<"argument#" << i <<" : " << argv[i] << "\n";
    }
    

    catest::TestRunner::run_tests();
}


