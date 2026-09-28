#pragma once

/**
 * Represents a 4th dimensional vector
 */
class Vec4
{

public:
    /**
     * Constructor to create a 4th dimensional vector
     * @param x - represents x-coordinate
     * @param y - represents y-coordinate
     * @param z - represents z-coordinate
     * @param w - represents w-coordinate
     */
    Vec4(float x, float y, float z, float w);

    /**
     * Constructor to create a 4th dimensional vector
     */
    Vec4();


    /**
     * Getter for x component
     */
    float getX();

    /**
     * Getter for y component
     */

    float getY();

    /**
     * Getter for z component
     */
    float getZ();

    /**
     * Getter for w component
     */
    float getW();

    /**
     * Setter for x component
     * @param x - value to set the x component to
     */
    void setX(float x);

    /**
     * Setter for y component
     * @param y - value to set the y component to
     */
    void setY(float y);

    /**
     * Setter for z component
     * @param z - value to set the z component to
     */
    void setZ(float z);

    /**
     * Setter for w component
     * @param w - value to set the w component to
     */
    void setW(float w);

    /**
     * Returns a new vector representing this vector plus the given vector
     * @param other - vector to add to this vector
     */
    Vec4 operator+(Vec4 other);

    /**
     * Returns a new vector representing this vector minus the given vector
     * @param other - vector to subtract from this vector
     */
    Vec4 operator-(Vec4 other);

    /**
     * Returns a new vector with each component multiplied by the given factor
     * @param factor - value to multiply each component by
     */
    Vec4 operator*(float factor);

    /**
     * Returns a new vector with each component multiplied by the other vector's corresponding component
     * @param other - vector to multiply this vector by, component-wise
     */
    Vec4 operator*(Vec4 other);

    /**
     * Returns a new vector with the x, y, and z components divided by w (perspective divide)
     */
    Vec4 perspectiveDivide();

    /**
     * Returns the cross product of the two given vectors, calculated as a x b.
     * Only the x, y, and z components participate; the w component of the result is always 0,
     * since the cross product represents a direction rather than a position.
     * @param a - first vector
     * @param b - second vector
     */
    static Vec4 cross(Vec4 a, Vec4 b);

    /**
     * Returns the dot product of the normalized versions of the two given vectors,
     * equivalent to the cosine of the angle between them. Returns 0 if either vector has zero length.
     * @param a - first vector
     * @param b - second vector
     */
    static float dot(Vec4 a, Vec4 b);

    /**
     * Prints the vector's components to standard output
     */
    void print();

private:
    float components[4];
};
