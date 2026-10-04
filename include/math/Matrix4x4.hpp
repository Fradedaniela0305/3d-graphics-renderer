#pragma once
#include "Vec4.hpp"

/**
 * Represents a 4x4 matrix
 */
class Matrix4x4
{

public:
    /**
     * Constructor to create a 4x4 matrix
     * @param r1 - row 1 of the matrix
     * @param r2 - row 2 of the matrix
     * @param r3 - row 3 of the matrix
     * @param r4 - row 4 of the matrix
     */
    Matrix4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4);

    /**
     * Constructor to create a 4x4 matrix
     */
    Matrix4x4();

    /**
     * Transforms a vector by a 4x4 matrix
     * @param vector - vector to be transformed
     */
    Vec4 transformVector(Vec4 vector);

    /**
     * Getter for row one
     */
    Vec4 getR1();

    /**
     * Getter for row two
     */
    Vec4 getR2();

    /**
     * Getter for row three
     */
    Vec4 getR3();
    /**
     * Getter for row four
     */
    Vec4 getR4();

private:
    Vec4 rows[4];
};
