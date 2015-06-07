#ifndef ORA_TEST_TOOL_H
#define ORA_TEST_TOOL_H


extern const float TEST_EPSILON;

void    doTestExpr(bool cond, const char * desc, const char * expr, const char * file, const char * func, int line);

#define DO_TEST_CASE(EXP, DESC) doTestExpr(bool(EXP), DESC, #EXP, __FILE__, __FUNCTION__, __LINE__)

int random_max();

// ret = [0, random_max)
int     random_i();

// ret = [0.0f, 1.0f)
float   random_f();

// ret = [low, high)
float   random_rf(float low, float high);

// ret = [low, high)
int     random_ri(int low, int high);

#endif //ORA_TEST_TOOL_H
