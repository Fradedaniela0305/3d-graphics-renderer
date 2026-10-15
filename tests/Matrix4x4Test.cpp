/**
 * @file Matrix4x4Test.cpp
 */

#include <gtest/gtest.h>
#include "math/Matrix4x4.hpp"
#include "math/Vec4.hpp"

TEST(Matrix4x4Test, DefaultConstructorConstructs)
{
    Matrix4x4 m;
    (void)m;
}

TEST(Matrix4x4Test, ConstructorSetsRows)
{
    Vec4 r1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 r2(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 r3(9.0f, 10.0f, 11.0f, 12.0f);
    Vec4 r4(13.0f, 14.0f, 15.0f, 16.0f);
    Matrix4x4 m(r1, r2, r3, r4);

    EXPECT_FLOAT_EQ(m.getR1().getX(), 1.0f);
    EXPECT_FLOAT_EQ(m.getR1().getW(), 4.0f);

    EXPECT_FLOAT_EQ(m.getR2().getX(), 5.0f);
    EXPECT_FLOAT_EQ(m.getR2().getW(), 8.0f);

    EXPECT_FLOAT_EQ(m.getR3().getX(), 9.0f);
    EXPECT_FLOAT_EQ(m.getR3().getW(), 12.0f);

    EXPECT_FLOAT_EQ(m.getR4().getX(), 13.0f);
    EXPECT_FLOAT_EQ(m.getR4().getW(), 16.0f);
}

TEST(Matrix4x4Test, TransformerTransformsVector)
{
    Vec4 r1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 r2(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 r3(9.0f, 10.0f, 11.0f, 12.0f);
    Vec4 r4(13.0f, 14.0f, 15.0f, 16.0f);
    Matrix4x4 m(r1, r2, r3, r4);

    Vec4 vector(2.0f, 1.0f, 0, 3.0f);
    Vec4 result = m.transformVector(vector);

    EXPECT_FLOAT_EQ(result.getX(), 16.0f);
    EXPECT_FLOAT_EQ(result.getY(), 40.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 64.0f);
    EXPECT_FLOAT_EQ(result.getW(), 88.0f);

}
