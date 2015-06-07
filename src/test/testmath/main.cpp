#include <iostream>

#include <cstdlib>
#include <ctime>
#include <math/matrix.hpp>

void test_matrix();

int main()
{
    srand((unsigned int)time(NULL));

    std::cout << "test math start ..." << std::endl;

    test_matrix();

    std::cout << "test math end." << std::endl;
    return 0;
}