#pragma once
#include "Color.hpp"
#include "math/Vec4.hpp"
#include "math/Matrix4x4.hpp"
#include <array>
#include <vector>

/**
 * Represents a triangle with 3 points in 3-dimensional space, points are in clockwise order.
 * Points are fixed at construction time and cannot be changed afterwards.
 */
class Triangle
{

public:
    /**
     * Constructor to create a triangle in 2-dimensional space with 3 points
     */
    Triangle(Vec4 p1, Vec4 p2, Vec4 p3);

    /**
     * Constructor to create a triangle in 2-dimensional
     */
    Triangle();

    /**
     * Getter for point one
     */
    Vec4 getP1();

    /**
     * Getter for point two
     */
    Vec4 getP2();

    /**
     * Getter for point three
     */
    Vec4 getP3();

    /**
     * Getter for all points
     */
    std::array<Vec4, 3> getPoints();

    /**
     * Getter for triangle's color
     */
    Color getColor();

    /**
     * Setter for triangle's color
     * @param color - set the triangle to that color
     */
    void setColor(Color color);

    /**
     * Getter for the triangle's normal vector
     */
    Vec4 getNormal();

    /**
     * Returns the triangle's bottom point, i.e. the point with the largest y value.
     * If multiple points share the largest y value, the first such point (in P1, P2, P3 order) is returned.
     */
    Vec4 getBottomPoint();

    /**
     * Returns the triangle's top point, i.e. the point with the smallest y value.
     * If multiple points share the smallest y value, the first such point (in P1, P2, P3 order) is returned.
     */
    Vec4 getTopPoint();

    /**
     * Returns the triangle's leftmost point, i.e. the point with the smallest x value.
     * If multiple points share the smallest x value, the first such point (in P1, P2, P3 order) is returned.
     */
    Vec4 getLeftmostPoint();

    /**
     * Returns the triangle's rightmost point, i.e. the point with the largest x value.
     * If multiple points share the largest x value, the first such point (in P1, P2, P3 order) is returned.
     */
    Vec4 getRightmostPoint();

    /**
     * Returns true if one of the triangles edges crosses between a certain point.
     * @param nearPlane - the point to cross
     */
    bool isBehindNearPlane(float nearPlane);

    /**
     * Prints the triangle's points to standard output
     */
    void print();

    /**
     * Returns midpoint of zth value of the triangle
     */
    float zthMidPoint();

    /**
     * Writes this triangle's points, transformed by the given matrix, into out.
     * @param matrix - the matrix to transform this triangle's points by
     * @param out - triangle to overwrite with the transformed points
     */
    void transformedBy(Matrix4x4 &matrix, Triangle &out);

private:
    std::array<Vec4, 3> points;
    Vec4 normal;
    Color color;

    /**
     * Calculates the normalized normal vector of the triangle from its points.
     * The resulting vector's w component is always 0, since a normal represents
     * a direction rather than a position.
     */
    Vec4 calculateNormal();
};
