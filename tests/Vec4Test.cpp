/**
 * @file Vec4Test.cpp
 */

#include <gtest/gtest.h>
#include "math/Vec4.hpp"

TEST(Vec4Test, ConstructorSetsComponents)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, DefaultConstructorConstructs)
{
    Vec4 v;
    (void)v;
}

TEST(Vec4Test, SetXChangesXComponentOnly)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    v.setX(10.0f);
    EXPECT_FLOAT_EQ(v.getX(), 10.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, SetYChangesYComponentOnly)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    v.setY(10.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 10.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, SetZChangesZComponentOnly)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    v.setZ(10.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 10.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, SetWChangesWComponentOnly)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    v.setW(10.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 10.0f);
}

TEST(Vec4Test, OperatorPlusVec4AddsAmountToEachComponent)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 result = v + Vec4(2.0f, 5.0f, 1.0f, 0.5f);
    EXPECT_FLOAT_EQ(result.getX(), 5.0f);
    EXPECT_FLOAT_EQ(result.getY(), 9.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 6.0f);
    EXPECT_FLOAT_EQ(result.getW(), 6.5f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, OperatorTimesFloatMultipliesEachComponent)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 result = v * 2.0f;
    EXPECT_FLOAT_EQ(result.getX(), 6.0f);
    EXPECT_FLOAT_EQ(result.getY(), 8.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 10.0f);
    EXPECT_FLOAT_EQ(result.getW(), 12.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, OperatorTimesVec4MultipliesEachComponentByOwnFactor)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 result = v * Vec4(2.0f, 5.0f, 3.0f, 0.5f);
    EXPECT_FLOAT_EQ(result.getX(), 6.0f);
    EXPECT_FLOAT_EQ(result.getY(), 20.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 15.0f);
    EXPECT_FLOAT_EQ(result.getW(), 3.0f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, OperatorPlusAddsEachComponent)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 other(1.0f, 5.0f, 2.0f, 0.5f);
    Vec4 result = v + other;
    EXPECT_FLOAT_EQ(result.getX(), 4.0f);
    EXPECT_FLOAT_EQ(result.getY(), 9.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 7.0f);
    EXPECT_FLOAT_EQ(result.getW(), 6.5f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, OperatorMinusSubtractsEachComponent)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 other(1.0f, 5.0f, 2.0f, 0.5f);
    Vec4 result = v - other;
    EXPECT_FLOAT_EQ(result.getX(), 2.0f);
    EXPECT_FLOAT_EQ(result.getY(), -1.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 3.0f);
    EXPECT_FLOAT_EQ(result.getW(), 5.5f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, PerspectiveDivideDividesXYAndZByW)
{
    Vec4 v(6.0f, 8.0f, 5.0f, 2.0f);
    Vec4 result = v.perspectiveDivide();
    EXPECT_FLOAT_EQ(result.getX(), 3.0f);
    EXPECT_FLOAT_EQ(result.getY(), 4.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 2.5f);
    EXPECT_FLOAT_EQ(result.getW(), 2.0f);
    EXPECT_FLOAT_EQ(v.getX(), 6.0f);
    EXPECT_FLOAT_EQ(v.getY(), 8.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 2.0f);
}

TEST(Vec4Test, PerspectiveDivideLeavesWUnchanged)
{
    Vec4 v(10.0f, 20.0f, -3.5f, 4.0f);
    Vec4 result = v.perspectiveDivide();
    EXPECT_FLOAT_EQ(result.getZ(), -0.875f);
    EXPECT_FLOAT_EQ(result.getW(), 4.0f);
}

TEST(Vec4Test, CrossCalculatesACrossBFromXYZComponents)
{
    Vec4 v(1.0f, 0.0f, 0.0f, 1.0f);
    Vec4 other(0.0f, 1.0f, 0.0f, 1.0f);
    Vec4 result = Vec4::cross(v, other);
    EXPECT_FLOAT_EQ(result.getX(), 0.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
    EXPECT_FLOAT_EQ(result.getZ(), 1.0f);
    EXPECT_FLOAT_EQ(v.getX(), 1.0f);
    EXPECT_FLOAT_EQ(v.getY(), 0.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 0.0f);
    EXPECT_FLOAT_EQ(v.getW(), 1.0f);
}

TEST(Vec4Test, CrossResultHasWComponentOfZero)
{
    Vec4 v(2.0f, 3.0f, 4.0f, 5.0f);
    Vec4 other(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 result = Vec4::cross(v, other);
    EXPECT_FLOAT_EQ(result.getX(), -3.0f);
    EXPECT_FLOAT_EQ(result.getY(), 6.0f);
    EXPECT_FLOAT_EQ(result.getZ(), -3.0f);
    EXPECT_FLOAT_EQ(result.getW(), 0.0f);
}

TEST(Vec4Test, DotCalculatesDotProductOfNormalizedVectors)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 other(1.0f, 5.0f, 2.0f, 0.5f);
    float result = Vec4::dot(v, other);
    EXPECT_NEAR(result, 0.7058145f, 1e-6f);
    EXPECT_FLOAT_EQ(v.getX(), 3.0f);
    EXPECT_FLOAT_EQ(v.getY(), 4.0f);
    EXPECT_FLOAT_EQ(v.getZ(), 5.0f);
    EXPECT_FLOAT_EQ(v.getW(), 6.0f);
}

TEST(Vec4Test, DotIsCommutative)
{
    Vec4 v(2.0f, 3.0f, 4.0f, 5.0f);
    Vec4 other(5.0f, 6.0f, 7.0f, 8.0f);
    float result = Vec4::dot(v, other);
    float reversed = Vec4::dot(other, v);
    EXPECT_FLOAT_EQ(result, reversed);
}

TEST(Vec4Test, DotOfPerpendicularVectorsIsZero)
{
    Vec4 v(1.0f, 0.0f, 0.0f, 0.0f);
    Vec4 other(0.0f, 1.0f, 0.0f, 0.0f);
    float result = Vec4::dot(v, other);
    EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(Vec4Test, DotOfParallelVectorsIsOne)
{
    Vec4 v(2.0f, 0.0f, 0.0f, 0.0f);
    Vec4 other(5.0f, 0.0f, 0.0f, 0.0f);
    float result = Vec4::dot(v, other);
    EXPECT_FLOAT_EQ(result, 1.0f);
}

TEST(Vec4Test, DotOfOppositeVectorsIsNegativeOne)
{
    Vec4 v(2.0f, 0.0f, 0.0f, 0.0f);
    Vec4 other(-5.0f, 0.0f, 0.0f, 0.0f);
    float result = Vec4::dot(v, other);
    EXPECT_FLOAT_EQ(result, -1.0f);
}

TEST(Vec4Test, DotWithZeroLengthVectorIsZero)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    Vec4 zero(0.0f, 0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(Vec4::dot(v, zero), 0.0f);
    EXPECT_FLOAT_EQ(Vec4::dot(zero, v), 0.0f);
}

TEST(Vec4Test, PrintOutputsComponents)
{
    Vec4 v(3.0f, 4.0f, 5.0f, 6.0f);
    testing::internal::CaptureStdout();
    v.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Vec4(3, 4, 5, 6)\n");
}
