#pragma once
#include <vector>
#include "Triangle.hpp"
#include <fstream>
#include <sstream>
#include <string>

/**
 * Represents a mesh
 */
class Mesh
{

public:
    /**
     * Constructor to create a mesh
     */
    Mesh(std::vector<Triangle> mesh);

    /**
     * Constructor to create a mesh
     */
    Mesh();

    /**
     * Add a triangle to the mesh
     */
    void addTriangle(Triangle triangle);

    /**
     * Getter for mesh
     */
    std::vector<Triangle>& getMesh();

    /**
     * Loader for .obj files
     */
    bool loadFromObjectFile(std::string path);

    /**
     * Sorts mesh based on triangle's zth-component. From smallest to largest.
     */
    void sortMesh();

    /**
     * Removes all triangles from the mesh
     */
    void clearMesh();

private:
    std::vector<Triangle> mesh;
};
