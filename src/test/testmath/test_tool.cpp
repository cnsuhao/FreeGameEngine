#include "test_tool.h"

#include <cstdlib>
#include <iostream>

const float TEST_EPSILON = 0.000001f;
const int RANDOM_HIGHST = 32767;

int random_max()
{
    return RANDOM_HIGHST;
}

int random_i()
{
    return rand() % RANDOM_HIGHST;
}

float random_f()
{
    float ret = (float)random_i();
    return ret / RANDOM_HIGHST;
}

float random_rf(float low, float high)
{
    return low + random_f() * (high - low);
}

int random_ri(int low, int high)
{
    return low + int(random_f() * (high - low));
}


void doTestExpr(bool cond, const char * desc, const char * expr, const char * file, const char * func, int line)
{
    if(cond)
    {
        //std::cout << "succed -( " << expr << " )- " << desc << std::endl;
    }
    else
    {
        std::cout << "\nfailed - " << desc << "\n"
            << "expr: " << expr << "\n"
            << "file: " << file << "\n"
            << "func: " << func << "\n"
            << "line: " << line << "\n"
            << std::endl;
    }
}
