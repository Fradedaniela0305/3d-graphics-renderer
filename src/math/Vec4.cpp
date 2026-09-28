#include "math/Vec4.hpp"
#include <cmath>
#include <iostream>

/**
 * Constructor to create a 4th dimensional vector
 * @param x - represents x-coordinate
 * @param y - represents y-coordinate
 * @param z - represents z-coordinate
 * @param w - represents w-coordinate
 */
Vec4::Vec4(float x, float y, float z, float w)
{
    components[0] = x;
    components[1] = y;
    components[2] = z;
    components[3] = w;
}

/**
 * Constructor to create a 4th dimensional vector
 */
Vec4::Vec4()
{
}

/**
 * Getter for x component
 */
float Vec4::getX()
{
    return components[0];
}

/**
 * Getter for y component
 */

float Vec4::getY()
{
    return components[1];
}

/**
 * Getter for z component
 */
float Vec4::getZ()
{
    return components[2];
}

/**
 * Getter for w component
 */
float Vec4::getW()
{
    return components[3];
}

/**
 * Setter for x component
 */
void Vec4::setX(float x)
{
    components[0] = x;
}

/**
 * Setter for y component
 */
void Vec4::setY(float y)
{
    components[1] = y;
}

/**
 * Setter for z component
 */
void Vec4::setZ(float z)
{
    components[2] = z;
}

/**
 * Setter for w component
 */
void Vec4::setW(float w)
{
    components[3] = w;
}

/**
 * Returns a new vector representing this vector plus the given vector
 */
Vec4 Vec4::operator+(Vec4 other)
{
    return Vec4(components[0] + other.getX(), components[1] + other.getY(), components[2] + other.getZ(), components[3] + other.getW());
}

/**
 * Returns a new vector representing this vector minus the given vector
 */
Vec4 Vec4::operator-(Vec4 other)
{
    return Vec4(components[0] - other.getX(), components[1] - other.getY(), components[2] - other.getZ(), components[3] - other.getW());
}

/**
 * Returns a new vector with each component multiplied by the given factor
 */
Vec4 Vec4::operator*(float factor)
{
    return Vec4(components[0] * factor, components[1] * factor, components[2] * factor, components[3] * factor);
}

/**
 * Returns a new vector with each component multiplied by the other vector's corresponding component
 */
Vec4 Vec4::operator*(Vec4 other)
{
    return Vec4(components[0] * other.getX(), components[1] * other.getY(), components[2] * other.getZ(), components[3] * other.getW());
}

/**
 * Returns a new vector with the x and y components divided by w (perspective divide)
 */
Vec4 Vec4::perspectiveDivide()
{

    if (components[3] != 0)
    {
        return Vec4(components[0] / components[3], components[1] / components[3], components[2] / components[3], components[3]);
    } 
    return Vec4(components[0], components[1], components[2], components[3]);
}

/**
 * Returns the cross product of the two given vectors, calculated as a x b
 */
Vec4 Vec4::cross(Vec4 a, Vec4 b)
{
    return Vec4(
        a.getY() * b.getZ() - a.getZ() * b.getY(),
        a.getZ() * b.getX() - a.getX() * b.getZ(),
        a.getX() * b.getY() - a.getY() * b.getX(),
        0.0f);
}

/**
 * Returns the dot product of the normalized versions of the two given vectors
 */
float Vec4::dot(Vec4 a, Vec4 b)
{
    float aLength = std::sqrt(a.getX() * a.getX() + a.getY() * a.getY() + a.getZ() * a.getZ() + a.getW() * a.getW());
    float bLength = std::sqrt(b.getX() * b.getX() + b.getY() * b.getY() + b.getZ() * b.getZ() + b.getW() * b.getW());
    if (aLength == 0 || bLength == 0)
    {
        return 0.0f;
    }
    return (a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ() + a.getW() * b.getW()) / (aLength * bLength);
}

/**
 * Prints the vector's components to standard output
 */
void Vec4::print()
{
    std::cout << "Vec4(" << components[0] << ", " << components[1] << ", " << components[2] << ", " << components[3] << ")" << std::endl;
}
