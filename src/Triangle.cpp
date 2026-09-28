#include "Triangle.hpp"
#include <iostream>
#include <cmath>

/**
 * Constructor to create a triangle in 4-dimensional space with 3 points. Last coordinate is set to 0
 */
Triangle::Triangle(Vec4 p1, Vec4 p2, Vec4 p3)
{
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    color = Color{255, 255, 255};
    normal = calculateNormal();
}

/**
 * Constructor to create a triangle in 4-dimensional space with 3 points. Last coordinate is set to 0
 */
Triangle::Triangle(Vec4 p1, Vec4 p2, Vec4 p3, Color color, Vec4 normal)
{
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    color = color;
    normal = normal;
}

/**
 * Constructor to create a triangle in 4-dimensional space
 */
Triangle::Triangle()
{
}

/**
 * Getter for point one
 */
Vec4 Triangle::getP1()
{
    return points[0];
}

/**
 * Getter for point two
 */
Vec4 Triangle::getP2()
{
    return points[1];
}

/**
 * Getter for point three
 */
Vec4 Triangle::getP3()
{
    return points[2];
}

/**
 * Getter for all points
 */
std::vector<Vec4> Triangle::getPoints()
{
    return points;
}

/**
 * Getter for the triangle's normal vector
 */
Vec4 Triangle::getNormal()
{
    return normal;
}

/**
 * Getter for triangle's color
 */
Color Triangle::getColor()
{
    return color;
}

/**
 * Setter for triangle's color
 */
void Triangle::setColor(Color color)
{
    this->color = color;
}

/**
 * Calculates the normalized normal vector of the triangle from its points
 */
Vec4 Triangle::calculateNormal()
{
    Vec4 edge1 = points[1] - points[0];
    Vec4 edge2 = points[2] - points[0];

    Vec4 cross = Vec4::cross(edge1, edge2);

    float length = std::sqrt(cross.getX() * cross.getX() + cross.getY() * cross.getY() + cross.getZ() * cross.getZ());
    if (length == 0)
    {
        return cross;
    }
    return cross * (1.0f / length);
}

/**
 * Returns the triangle's bottom point, i.e. the point with the largest y value.
 * If multiple points share the largest y value, the first such point (in P1, P2, P3 order) is returned.
 */
Vec4 Triangle::getBottomPoint()
{
    Vec4 bottom = points[0];
    for (int i = 1; i < 3; i++)
    {
        if (points[i].getY() > bottom.getY())
        {
            bottom = points[i];
        }
    }
    return bottom;
}

/**
 * Returns the triangle's top point, i.e. the point with the smallest y value.
 * If multiple points share the smallest y value, the first such point (in P1, P2, P3 order) is returned.
 */
Vec4 Triangle::getTopPoint()
{
    Vec4 top = points[0];
    for (int i = 1; i < 3; i++)
    {
        if (points[i].getY() < top.getY())
        {
            top = points[i];
        }
    }
    return top;
}

/**
 * Returns the triangle's leftmost point, i.e. the point with the smallest x value.
 * If multiple points share the smallest x value, the first such point (in P1, P2, P3 order) is returned.
 */
Vec4 Triangle::getLeftmostPoint()
{
    Vec4 leftmost = points[0];
    for (int i = 1; i < 3; i++)
    {
        if (points[i].getX() < leftmost.getX())
        {
            leftmost = points[i];
        }
    }
    return leftmost;
}

/**
 * Returns the triangle's rightmost point, i.e. the point with the largest x value.
 * If multiple points share the largest x value, the first such point (in P1, P2, P3 order) is returned.
 */
Vec4 Triangle::getRightmostPoint()
{
    Vec4 rightmost = points[0];
    for (int i = 1; i < 3; i++)
    {
        if (points[i].getX() > rightmost.getX())
        {
            rightmost = points[i];
        }
    }
    return rightmost;
}

/**
* Returns true if one of the triangles edges crosses between a certain point.
 */
bool Triangle::isBehindNearPlane(float nearPlane)
{
    return points[0].getZ() < nearPlane ||
           points[1].getZ() < nearPlane ||
           points[2].getZ() < nearPlane;
}

/**
 * Returns midpoint of zth value of the triangle
 */
float Triangle::zthMidPoint() {
    return (points[0].getZ() + points[1].getZ() + points[2].getZ()) / 3.0f;
}

/**
 * Prints the triangle's points to standard output
 */
void Triangle::print()
{
    std::cout << "Triangle(" << std::endl;
    std::cout << "  P1: ";
    points[0].print();
    std::cout << "  P2: ";
    points[1].print();
    std::cout << "  P3: ";
    points[2].print();
    std::cout << ")" << std::endl;
}
