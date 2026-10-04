/**
 * @file Matrix4x4.cpp
 */

#include "math/Matrix4x4.hpp"

/**
 * Constructor to create a 4x4 matrix
 * @param r1 - row 1 of the matrix
 * @param r2 - row 2 of the matrix
 * @param r3 - row 3 of the matrix
 * @param r4 - row 4 of the matrix
 */
Matrix4x4::Matrix4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4)
{
    rows[0] = r1;
    rows[1] = r2;
    rows[2] = r3;
    rows[3] = r4;
}

/**
 * Constructor to create a 4x4 matrix
 */
Matrix4x4::Matrix4x4()
{
}

/**
 * Constructor to create a 4x4 matrix
 * @param vector - vector that is transformed
 */
Vec4 Matrix4x4::transformVector(Vec4 vector)
{

    float vecX = vector.getX();
    float vecY = vector.getY();
    float vecZ = vector.getZ();
    float vecW = vector.getW();

    float x = vecX * rows[0].getX() + vecY * rows[0].getY() + vecZ * rows[0].getZ() + vecW * rows[0].getW();
    float y = vecX * rows[1].getX() + vecY * rows[1].getY() + vecZ * rows[1].getZ() + vecW * rows[1].getW();
    float z = vecX * rows[2].getX() + vecY * rows[2].getY() + vecZ * rows[2].getZ() + vecW * rows[2].getW();
    float w = vecX * rows[3].getX() + vecY * rows[3].getY() + vecZ * rows[3].getZ() + vecW * rows[3].getW();

    return Vec4{x, y, z, w};
}

/**
 * Getter for row one
 */
Vec4 Matrix4x4::getR1()
{
    return rows[0];
}

/**
 * Getter for row two
 */
Vec4 Matrix4x4::getR2()
{
    return rows[1];
}

/**
 * Getter for row three
 */
Vec4 Matrix4x4::getR3()
{
    return rows[2];
}
/**
 * Getter for row four
 */
Vec4 Matrix4x4::getR4()
{
    return rows[3];
}
