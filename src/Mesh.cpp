#include "Mesh.hpp"

/**
 * Constructor to create a mesh
 */
Mesh::Mesh(std::vector<Triangle> mesh) : mesh(mesh)
{
}

/**
 * Constructor to create a mesh
 */
Mesh::Mesh()
{
}

/**
 * Add a triangle to the mesh
 */
void Mesh::addTriangle(Triangle triangle)
{
    mesh.push_back(triangle);
}

/**
 * Getter for mesh
 */
std::vector<Triangle>& Mesh::getMesh()
{
    return mesh;
}

/**
 * Extracts the vertex index from an OBJ face token.
 * Handles "v", "v/vt", "v/vt/vn" and "v//vn" forms, returning just the
 * leading vertex index (the part before the first '/', if any).
 */
static int parseFaceVertexIndex(const std::string &token)
{
    return std::stoi(token.substr(0, token.find('/')));
}

/**
 * Loader for .obj files
 */
bool Mesh::loadFromObjectFile(std::string path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    std::vector<Vec4> vertex;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            vertex.push_back(Vec4{x, y, z, 1});
        }

        else if (type == "f")
        {
            std::string t1, t2, t3;
            ss >> t1 >> t2 >> t3;

            int v1 = parseFaceVertexIndex(t1);
            int v2 = parseFaceVertexIndex(t2);
            int v3 = parseFaceVertexIndex(t3);

            mesh.push_back(Triangle{vertex.at(v1 - 1), vertex.at(v2 - 1), vertex.at(v3 - 1)});
        }
    }

    return true;
}

/**
 * Sorts mesh based on triangle's zth-component. From largest to smallest.
 */
void Mesh::sortMesh()
{
    std::sort(mesh.begin(), mesh.end(), [](Triangle &a, Triangle &b)
              { return a.zthMidPoint() > b.zthMidPoint(); });
}

/**
 * Removes all triangles from the mesh
 */
void Mesh::clearMesh()
{
    mesh.clear();
}
