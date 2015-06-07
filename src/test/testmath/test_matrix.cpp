
#include "test_tool.h"

#include <iostream>
#include <math/matrix.hpp>
#include <math/quat.hpp>

#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib")

void test_matrix_1(int stage);
void test_matrix_2(int stage);
void test_matrix_3(int stage);
void test_quaternion(int stage);

void test_matrix()
{
    std::cout << "test matrix start." << std::endl;
    for(int i = 0; i < 10; ++i)
    {
        test_matrix_1(i);
    }

    test_matrix_2(0);
    //test_matrix_3(0);

    test_quaternion(0);
}


bool almostEqual( const Matrix & m1, const Matrix & m2, const float epsilon = TEST_EPSILON )
{
    for(int i = 0; i < 16; ++i)
	{
        if(!almostEqual(m1.mm[i], m2.mm[i], epsilon))
            return false;
    }
    return true;
}


bool almostEqual( const Quaternion & q1, const Quaternion & q2, const float epsilon = TEST_EPSILON )
{
    return almostEqual(q1.x, q2.x, TEST_EPSILON) &&
        almostEqual(q1.y, q2.y, TEST_EPSILON) &&
        almostEqual(q1.z, q2.z, TEST_EPSILON) &&
        almostEqual(q1.w, q2.w, TEST_EPSILON);
}

Vector3 random_vector3(float range)
{
    return Vector3(
        random_rf(-range, range),
        random_rf(-range, range),
        random_rf(-range, range));
}

Matrix random_matrix(float range)
{
    Matrix m;
    for(int i = 0; i < 16; ++i)
        m.mm[i] = random_rf(-range, range);
    return m;
}

Quaternion random_quaternion(float range)
{
    Vector3 axis = random_vector3(range);
    axis.normalise();

    Quaternion q;
    q.fromAngleAxis(random_rf(-range, range), axis);
    return q;
}

void test_matrix_1(int stage)
{
    std::cout << "test_matrix_1 " << stage << std::endl;

    const float MAX_V = 10000;
    const Vector3 vec = random_vector3(MAX_V);
    const float f1 = random_rf(-MAX_V, MAX_V);
    std::cout << "input: \n"
        << "float1: " << f1 << std::endl
        << "float3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;

    Matrix mat1, mat2;
    D3DXMATRIX* pMat2 = (D3DXMATRIX*)&mat2;

    mat1.setScale(vec);
    D3DXMatrixScaling(pMat2, vec.x, vec.y, vec.z);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix scale");

    mat1.setTranslate(vec);
    D3DXMatrixTranslation(pMat2, vec.x, vec.y, vec.z);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix translate");

    // rotation

    mat1.setRotate(vec.x, vec.y, vec.z);
    D3DXMatrixRotationYawPitchRoll(pMat2, vec.x, vec.y, vec.z);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix rotation yaw pitch roll");

    mat1.setRotate(vec, f1);
    D3DXMatrixRotationAxis(pMat2, (D3DXVECTOR3*)&vec, f1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix rotation axis");

    mat1.setRotateX(f1);
    D3DXMatrixRotationX(pMat2, f1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix rotation x");

    mat1.setRotateY(f1);
    D3DXMatrixRotationY(pMat2, f1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix rotation y");

    mat1.setRotateZ(f1);
    D3DXMatrixRotationZ(pMat2, f1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix rotation z");

    // quaternion rotation
    Quaternion qua1, qua2;
    qua1.fromAngleAxis(f1, vec);
    D3DXQuaternionRotationAxis((D3DXQUATERNION*)&qua2, (D3DXVECTOR3*)&vec, f1);
    DO_TEST_CASE(almostEqual(qua1, qua2), "test_matrix Quaternion");

    mat1.setRotate(qua1);
    D3DXMatrixRotationQuaternion(pMat2, (D3DXQUATERNION*)&qua2);
    DO_TEST_CASE(almostEqual(qua1, qua2), "test_matrix rotation Quaternion");

}

void print(const Matrix & v)
{
    for(int i = 0; i < 16; ++i)
        printf("%ff, ", v.mm[i]);
}

void print(const Quaternion & v)
{
    for(int i = 0; i < 4; ++i)
        printf("%ff, ", v[i]);
}

void test_matrix_2(int stage)
{
    std::cout << "test_matrix_1 " << stage << std::endl;

    const float MAX_V = 100;

    const Matrix input1 = random_matrix(MAX_V);
    const Matrix input2 = random_matrix(MAX_V);
#if 1
    std::cout << "input1: ";
    print(input1);
    std::cout << std::endl;

    std::cout << "input2: ";
    print(input2);
    std::cout << std::endl;
#endif

    Matrix mat1, mat2, tmp;
    D3DXMATRIX* pMat2 = (D3DXMATRIX*)&mat2;

    mat1 = input1;
    mat2 = input1;
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix asign");

    mat1.multiply(input1, input2);
    D3DXMatrixMultiply(pMat2, (D3DXMATRIX*)&input1, (D3DXMATRIX*)&input2);
    DO_TEST_CASE(almostEqual(mat1, mat2, 0.0004f), "test_matrix mutiply");
    //std::cout << "mat1: "; print(mat1); std::cout << std::endl;
    //std::cout << "mat2: "; print(mat2); std::cout << std::endl;

    float determinant1 = input1.getDeterminant();
    float determinant2 = D3DXMatrixDeterminant((D3DXMATRIX*)&input1);
    DO_TEST_CASE(almostEqual(determinant1, determinant2), "test_matrix determinant");

    mat1.transpose(input1);
    D3DXMatrixTranspose(pMat2, (D3DXMATRIX*)&input1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix transpose");

    mat1.invert(input1);
    D3DXMatrixInverse(pMat2, NULL, (D3DXMATRIX*)&input1);
    DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix invert");

#if 0
    {
        Quaternion rot1, rot2;
        Vector3 scale1, scale2;
        Vector3 trans1, trans2;
        input1.decompose(rot1, scale1, trans1);

        D3DXMatrixDecompose((D3DXVECTOR3*)&scale2, (D3DXQUATERNION*)&rot2, (D3DXVECTOR3*)&trans2, (D3DXMATRIX*)&input1);
        DO_TEST_CASE(almostEqual(rot1, rot2), "test_matrix decompose - rotation");
        DO_TEST_CASE(almostEqual(scale1, scale2), "test_matrix decompose - scale");
        DO_TEST_CASE(almostEqual(trans1, trans2), "test_matrix decompose - translation");

        mat1.setScale(scale1);
        tmp.setRotate(rot1);
        mat1.postMultiply(tmp);
        tmp.setTranslate(trans1);
        mat1.postMultiply(tmp);
        DO_TEST_CASE(almostEqual(mat1, input1), "test_matrix bw undecompose");

        D3DXMatrixScaling(pMat2, scale2.x, scale2.y, scale2.z);
        D3DXMatrixRotationQuaternion((D3DXMATRIX*)&tmp, (D3DXQUATERNION*)&rot2);
        D3DXMatrixMultiply(pMat2, pMat2, (D3DXMATRIX*)&tmp);
        D3DXMatrixTranslation((D3DXMATRIX*)&tmp, trans2.x, trans2.y, trans2.z);
        D3DXMatrixMultiply(pMat2, pMat2, (D3DXMATRIX*)&tmp);
        DO_TEST_CASE(almostEqual(mat2, input1), "test_matrix dx undecompose");

        DO_TEST_CASE(almostEqual(mat1, mat2), "test_matrix bw vs dx undecompose");
    }
#endif
}

void test_matrix_3(int stage)
{
    
    float pT1[] = {-49.626148f, -55.241554f, -3.097626f, 19.241920f, 61.937317f, -33.249916f, -32.432022f, 77.385788f, -63.158054f, -85.271767f, 43.394272f, -23.020111f, -2.114937f, 29.050568f, 77.233200f, -62.340157f,};
    float pT2[] = {-48.496964f, -5.612358f, -96.966461f, 93.621643f, -12.955109f, -32.981354f, -20.963165f, -10.989716f, 15.451523f, 83.727524f, -14.004944f, 8.200325f, -96.319466f, -16.397598f, -13.174835f, -78.057190f,};

    Matrix input1(pT1);
    Matrix input2(pT2);

    Matrix mat1, mat2;
    D3DXMATRIX* pMat2 = (D3DXMATRIX*)&mat2;

    mat1.multiply(input1, input2);
    D3DXMatrixMultiply(pMat2, (D3DXMATRIX*)&input1, (D3DXMATRIX*)&input2);
    DO_TEST_CASE(almostEqual(mat1, mat2, 0.0004f), "test_matrix mutiply");

    std::cout << "mat1: "; print(mat1); std::cout << std::endl;
    std::cout << "mat2: "; print(mat2); std::cout << std::endl;
}

void test_quaternion(int stage)
{
    std::cout << "test_quaternion " << stage << std::endl;

    const Quaternion input1 = random_quaternion(1000);
    const Quaternion input2 = random_quaternion(1000);
    const Vector3 f3 = random_vector3(1000);

    Quaternion quat1, quat2;
    quat1.multiply(input1, input2);
    D3DXQuaternionMultiply((D3DXQUATERNION*)&quat2, (D3DXQUATERNION*)&input1, (D3DXQUATERNION*)&input2);
    DO_TEST_CASE(almostEqual(quat1, quat2), "test_quaternion mutiply");

    quat1.fromYawPitchRoll(f3.x, f3.y, f3.z);
    D3DXQuaternionRotationYawPitchRoll((D3DXQUATERNION*)&quat2, f3.x, f3.y, f3.z);
    DO_TEST_CASE(almostEqual(quat1, quat2), "test_quaternion yaw pitch roll");

    {
        Vector3 axis = f3;
        axis.normalise();
        Matrix mat;
        mat.setRotate(axis, random_rf(-10, 10));

        quat1.fromMatrix(mat);
        D3DXQuaternionRotationMatrix((D3DXQUATERNION*)&quat2, (D3DXMATRIX*)&mat);
        DO_TEST_CASE(almostEqual(quat1, quat2), "test_quaternion matrix");
    }

    {
        const float t = random_f();
        quat1.slerp(input1, input2, t);
        D3DXQuaternionSlerp((D3DXQUATERNION*)&quat2, (D3DXQUATERNION*)&input1, (D3DXQUATERNION*)&input2, t);
        DO_TEST_CASE(almostEqual(quat1, quat2), "test_quaternion slerp");
        std::cout << "quat1: "; print(quat1); std::cout << std::endl;
        std::cout << "quat2: "; print(quat2); std::cout << std::endl;
    }

}