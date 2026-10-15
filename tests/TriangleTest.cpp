/**
 * @file TriangleTest.cpp
 */

#include <gtest/gtest.h>
#include <cmath>
#include "Triangle.hpp"
#include "math/Vec4.hpp"
#include "math/Matrix4x4.hpp"
#include "Color.hpp"

TEST(TriangleTest, ConstructorSetsPoints)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.getP1().getX(), 1.0f);
    EXPECT_FLOAT_EQ(t.getP1().getY(), 2.0f);
    EXPECT_FLOAT_EQ(t.getP1().getZ(), 3.0f);
    EXPECT_FLOAT_EQ(t.getP1().getW(), 1.0f);

    EXPECT_FLOAT_EQ(t.getP2().getX(), 3.0f);
    EXPECT_FLOAT_EQ(t.getP2().getY(), 4.0f);
    EXPECT_FLOAT_EQ(t.getP2().getZ(), 3.0f);
    EXPECT_FLOAT_EQ(t.getP2().getW(), 1.0f);

    EXPECT_FLOAT_EQ(t.getP3().getX(), 5.0f);
    EXPECT_FLOAT_EQ(t.getP3().getY(), 6.0f);
    EXPECT_FLOAT_EQ(t.getP3().getZ(), 3.0f);
    EXPECT_FLOAT_EQ(t.getP3().getW(), 1.0f);
}

TEST(TriangleTest, DefaultConstructorConstructs)
{
    Triangle t;
    (void)t;
}

TEST(TriangleTest, ConstructorCalculatesNormalizedNormal)
{
    Vec4 p1(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4 p2(1.0f, 0.0f, 0.0f, 1.0f);
    Vec4 p3(0.0f, 1.0f, 0.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.getNormal().getX(), 0.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getY(), 0.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getZ(), 1.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getW(), 0.0f);

    float length = std::sqrt(t.getNormal().getX() * t.getNormal().getX() +
                              t.getNormal().getY() * t.getNormal().getY() +
                              t.getNormal().getZ() * t.getNormal().getZ());
    EXPECT_FLOAT_EQ(length, 1.0f);
}

TEST(TriangleTest, NormalIsNormalizedRegardlessOfEdgeLength)
{
    Vec4 p1(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4 p2(5.0f, 0.0f, 0.0f, 1.0f);
    Vec4 p3(0.0f, 8.0f, 0.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.getNormal().getX(), 0.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getY(), 0.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getZ(), 1.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getW(), 0.0f);

    float length = std::sqrt(t.getNormal().getX() * t.getNormal().getX() +
                              t.getNormal().getY() * t.getNormal().getY() +
                              t.getNormal().getZ() * t.getNormal().getZ());
    EXPECT_FLOAT_EQ(length, 1.0f);
}

TEST(TriangleTest, GetBottomPointReturnsPointWithLargestY)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 9.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    Vec4 bottom = t.getBottomPoint();
    EXPECT_FLOAT_EQ(bottom.getX(), 3.0f);
    EXPECT_FLOAT_EQ(bottom.getY(), 9.0f);
    EXPECT_FLOAT_EQ(bottom.getZ(), 3.0f);
    EXPECT_FLOAT_EQ(bottom.getW(), 1.0f);
}

TEST(TriangleTest, GetTopPointReturnsPointWithSmallestY)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 9.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, -4.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    Vec4 top = t.getTopPoint();
    EXPECT_FLOAT_EQ(top.getX(), 5.0f);
    EXPECT_FLOAT_EQ(top.getY(), -4.0f);
    EXPECT_FLOAT_EQ(top.getZ(), 3.0f);
    EXPECT_FLOAT_EQ(top.getW(), 1.0f);
}

TEST(TriangleTest, GetBottomAndTopPointBreakTiesInFavorOfEarlierPoint)
{
    Vec4 p1(1.0f, 5.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 5.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 5.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.getBottomPoint().getX(), 1.0f);
    EXPECT_FLOAT_EQ(t.getTopPoint().getX(), 1.0f);
}

TEST(TriangleTest, GetLeftmostPointReturnsPointWithSmallestX)
{
    Vec4 p1(4.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(-3.0f, 9.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    Vec4 leftmost = t.getLeftmostPoint();
    EXPECT_FLOAT_EQ(leftmost.getX(), -3.0f);
    EXPECT_FLOAT_EQ(leftmost.getY(), 9.0f);
    EXPECT_FLOAT_EQ(leftmost.getZ(), 3.0f);
    EXPECT_FLOAT_EQ(leftmost.getW(), 1.0f);
}

TEST(TriangleTest, GetRightmostPointReturnsPointWithLargestX)
{
    Vec4 p1(4.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(-3.0f, 9.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    Vec4 rightmost = t.getRightmostPoint();
    EXPECT_FLOAT_EQ(rightmost.getX(), 5.0f);
    EXPECT_FLOAT_EQ(rightmost.getY(), 6.0f);
    EXPECT_FLOAT_EQ(rightmost.getZ(), 3.0f);
    EXPECT_FLOAT_EQ(rightmost.getW(), 1.0f);
}

TEST(TriangleTest, GetLeftmostAndRightmostPointBreakTiesInFavorOfEarlierPoint)
{
    Vec4 p1(5.0f, 1.0f, 3.0f, 1.0f);
    Vec4 p2(5.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 3.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.getLeftmostPoint().getY(), 1.0f);
    EXPECT_FLOAT_EQ(t.getRightmostPoint().getY(), 1.0f);
}

TEST(TriangleTest, ZthMidPointReturnsAverageOfZValues)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 9.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 12.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.zthMidPoint(), 8.0f);
}

TEST(TriangleTest, ZthMidPointHandlesNegativeAndZeroZValues)
{
    Vec4 p1(0.0f, 0.0f, -3.0f, 1.0f);
    Vec4 p2(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4 p3(0.0f, 0.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_FLOAT_EQ(t.zthMidPoint(), 0.0f);
}

TEST(TriangleTest, TransformedByAppliesMatrixToEachPoint)
{
    Vec4 r1(1.0f, 0.0f, 0.0f, 0.0f);
    Vec4 r2(0.0f, 1.0f, 0.0f, 0.0f);
    Vec4 r3(0.0f, 0.0f, 1.0f, 0.0f);
    Vec4 r4(0.0f, 0.0f, 0.0f, 1.0f);
    Matrix4x4 identity(r1, r2, r3, r4);

    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(4.0f, 5.0f, 6.0f, 1.0f);
    Vec4 p3(7.0f, 8.0f, 9.0f, 1.0f);
    Triangle triangle(p1, p2, p3);

    Triangle result;
    triangle.transformedBy(identity, result);

    EXPECT_FLOAT_EQ(result.getP1().getX(), 1.0f);
    EXPECT_FLOAT_EQ(result.getP1().getY(), 2.0f);
    EXPECT_FLOAT_EQ(result.getP1().getZ(), 3.0f);
    EXPECT_FLOAT_EQ(result.getP1().getW(), 1.0f);

    EXPECT_FLOAT_EQ(result.getP2().getX(), 4.0f);
    EXPECT_FLOAT_EQ(result.getP2().getY(), 5.0f);
    EXPECT_FLOAT_EQ(result.getP2().getZ(), 6.0f);
    EXPECT_FLOAT_EQ(result.getP2().getW(), 1.0f);

    EXPECT_FLOAT_EQ(result.getP3().getX(), 7.0f);
    EXPECT_FLOAT_EQ(result.getP3().getY(), 8.0f);
    EXPECT_FLOAT_EQ(result.getP3().getZ(), 9.0f);
    EXPECT_FLOAT_EQ(result.getP3().getW(), 1.0f);
}

TEST(TriangleTest, DefaultColorIsWhite)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    EXPECT_EQ(t.getColor(), Color::White);
}

TEST(TriangleTest, SetColorChangesColor)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    t.setColor(Color::Red);

    EXPECT_EQ(t.getColor(), Color::Red);
}

TEST(TriangleTest, SetColorCanBeCalledMultipleTimes)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    t.setColor(Color::Red);
    t.setColor(Color::Blue);

    EXPECT_EQ(t.getColor(), Color::Blue);
}

TEST(TriangleTest, SetColorDoesNotAffectOtherTriangleProperties)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);
    Vec4 normalBefore = t.getNormal();

    t.setColor(Color::Green);

    EXPECT_FLOAT_EQ(t.getP1().getX(), 1.0f);
    EXPECT_FLOAT_EQ(t.getNormal().getX(), normalBefore.getX());
    EXPECT_FLOAT_EQ(t.getNormal().getY(), normalBefore.getY());
    EXPECT_FLOAT_EQ(t.getNormal().getZ(), normalBefore.getZ());
}

TEST(TriangleTest, PrintOutputsPoints)
{
    Vec4 p1(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 p2(3.0f, 4.0f, 3.0f, 1.0f);
    Vec4 p3(5.0f, 6.0f, 3.0f, 1.0f);
    Triangle t(p1, p2, p3);

    testing::internal::CaptureStdout();
    t.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output,
              "Triangle(\n"
              "  P1: Vec4(1, 2, 3, 1)\n"
              "  P2: Vec4(3, 4, 3, 1)\n"
              "  P3: Vec4(5, 6, 3, 1)\n"
              ")\n");
}
