#pragma once

class TriangleMesh {
public:
    TriangleMesh(); // Constructor
    void draw(); // Draw the triangle mesh
    ~TriangleMesh(); // Destructor

private:
    unsigned int VBO, VAO, vertex_count; // Vertex Buffer Object, Vertex Array Object, number of vertices
};