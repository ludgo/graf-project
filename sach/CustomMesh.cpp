#include <glm/glm.hpp>
#include <sstream>
#include <tiny_obj_loader.h>

#include "CustomMesh.h"

using namespace std;
using namespace glm;
using namespace ppgso;

CustomMesh::CustomMesh() {

    vector<vec3> v {
            { 0.8, -0, 0.4 },
            { 0.8, -0, 0.6 },
            { 0.2, -0, 0.6 },
            { 0.2, -0, 0.4 },
            { 0.8, 1, 0.4 },
            { 0.2, 1, 0.4 },
            { 0.2, 1, 0.6 },
            { 0.8, 1, 0.6 },
            { 0.8, -0, 0.4 },
            { 0.8, 1, 0.4 },
            { 0.8, 1, 0.6 },
            { 0.8, -0, 0.6 },
            { 0.8, -0, 0.6 },
            { 0.8, 1, 0.6 },
            { 0.2, 1, 0.6 },
            { 0.2, -0, 0.6 },
            { 0.2, -0, 0.6 },
            { 0.2, 1, 0.6 },
            { 0.2, 1, 0.4 },
            { 0.2, -0, 0.4 },
            { 0.8, 1, 0.4 },
            { 0.8, -0, 0.4 },
            { 0.2, -0, 0.4 },
            { 0.2, 1, 0.4 },
            { 0.4, 1, 0.4 },
            { 0.4, 1, 0.6 },
            { 0.2, 1, 0.6 },
            { 0.2, 1.3, 0.6 },
            { 0.4, 1.3, 0.4 },
            { 0.2, 1.3, 0.4 },
            { 0.2, 1.3, 0.6 },
            { 0.4, 1.3, 0.6 },
            { 0.4, 1, 0.4 },
            { 0.4, 1.3, 0.4 },
            { 0.4, 1.3, 0.6 },
            { 0.4, 1, 0.6 },
            { 0.4, 1, 0.6 },
            { 0.4, 1.3, 0.6 },
            { 0.2, 1.3, 0.6 },
            { 0.2, 1, 0.6 },
            { 0.2, 1, 0.6 },
            { 0.2, 1.3, 0.6 },
            { 0.2, 1.3, 0.4 },
            { 0.2, 1, 0.4 },
            { 0.4, 1.3, 0.4 },
            { 0.4, 1, 0.4 },
            { 0.2, 1.3, 0.4 },
            { 0.8, -0, 0.4 },
            { 0.8, -0, 0.6 },
            { 0.6, 1, 0.6 },
            { 0.6, 1, 0.4 },
            { 0.8, 1.3, 0.4 },
            { 0.6, 1.3, 0.4 },
            { 0.6, 1.3, 0.6 },
            { 0.8, 1.3, 0.6 },
            { 0.8, 1.3, 0.4 },
            { 0.8, 1.3, 0.6 },
            { 0.8, 1.3, 0.6 },
            { 0.6, 1.3, 0.6 },
            { 0.6, 1, 0.6 },
            { 0.6, 1, 0.6 },
            { 0.6, 1.3, 0.6 },
            { 0.6, 1.3, 0.4 },
            { 0.6, 1, 0.4 },
            { 0.8, 1.3, 0.4 },
            { 0.6, 1, 0.4 },
            { 0.6, 1.3, 0.4 }
    };

    vector<vec2> vt {
            { 0.9, 0.7 },
            { 0.9, 0.8 },
            { 0.6, 0.8 },
            { 0.6, 0.7 },
            { 0.9, 0.7 },
            { 0.6, 0.7 },
            { 0.6, 0.8 },
            { 0.9, 0.8 },
            { 0.5, 0.7 },
            { 1, 0.7 },
            { 1, 0.8 },
            { 0.5, 0.8 },
            { 0.9, 0.5 },
            { 0.9, 1 },
            { 0.6, 1 },
            { 0.6, 0.5 },
            { 0.5, 0.8 },
            { 1, 0.8 },
            { 1, 0.7 },
            { 0.5, 0.7 },
            { 0.9, 0 },
            { 0.9, -0.5 },
            { 0.6, -0.5 },
            { 0.6, 0 },
            { 0, 0.7 },
            { 0, 0.8 },
            { 0, 0.8 },
            { 0.15, 0.8 },
            { 0.7, 0.7 },
            { 0.6, 0.7 },
            { 0.6, 0.8 },
            { 0.7, 0.8 },
            { 0, 0.7 },
            { 0.15, 0.7 },
            { 0.15, 0.8 },
            { 0, 0.8 },
            { 0.7, 0 },
            { 0.7, 0.15 },
            { 0.6, 0.15 },
            { 0.6, 0 },
            { 0, 0.8 },
            { 0.15, 0.8 },
            { 0.15, 0.7 },
            { 0, 0.7 },
            { 0.7, 0.15 },
            { 0.7, 0 },
            { 0.6, 0.15 },
            { 0.5, 0.7 },
            { 0.5, 0.8 },
            { 1, 0.8 },
            { 1, 0.7 },
            { 0.9, 0.7 },
            { 0.8, 0.7 },
            { 0.8, 0.8 },
            { 0.9, 0.8 },
            { 1.15, 0.7 },
            { 1.15, 0.8 },
            { 0.9, 1.15 },
            { 0.8, 1.15 },
            { 0.8, 1 },
            { 0, 0.8 },
            { 0.15, 0.8 },
            { 0.15, 0.7 },
            { 0, 0.7 },
            { 0.9, 0.15 },
            { 0.8, 0 },
            { 0.8, 0.15 }
    };

    vector<vec3> vn {
            { 0, -1, -0 },
            { 0, -1, -0 },
            { 0, -1, -0 },
            { 0, -1, -0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { -0.6396, -0.4264, -0.6396 },
            { -0.6396, -0.4264, -0.6396 },
            { -0.6396, -0.4264, -0.6396 },
            { -0.6396, -0.4264, -0.6396 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { -0.9806, -0.1961, -0 },
            { -0.9806, -0.1961, -0 },
            { -0.9806, -0.1961, -0 },
            { -0.9806, -0.1961, -0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 0, 1, 0 },
            { 1, -0, 0 },
            { 1, -0, 0 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { 0, -0, 1 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { -1, 0, 0 },
            { 0, 0, -1 },
            { 0, 0, -1 },
            { 0, 0, -1 }
    };

    vector<unsigned int> f {
            0, 1, 2, 0, 2, 3, 4, 5, 6,
            4, 6, 7, 8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15, 16, 17, 18,
            16, 18, 19, 20, 21, 22, 20, 22, 23,
            24, 25, 26, 24, 26, 27, 28, 29, 30,
            28, 30, 31, 32, 33, 34, 32, 34, 35,
            36, 37, 38, 36, 38, 39, 40, 41, 42,
            40, 42, 43, 44, 45, 23, 44, 23, 46,
            47, 48, 49, 47, 49, 50, 51, 52, 53,
            51, 53, 54, 8, 55, 56, 8, 56, 11,
            12, 57, 58, 12, 58, 59, 60, 61, 62,
            60, 62, 63, 64, 21, 65, 64, 65, 66
    };

    gl_buffer buffer;

    // Generate a vertex array object
    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);

    // Generate and upload a buffer with vertex positions to GPU
    glGenBuffers(1, &buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), v.data(),
                 GL_STATIC_DRAW);

    // Bind the buffer to "Position" attribute in program
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Generate and upload a buffer with texture coordinates to GPU
    glGenBuffers(1, &buffer.tbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.tbo);
    glBufferData(GL_ARRAY_BUFFER, vt.size() * sizeof(vec2), vt.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Generate and upload a buffer with vertex normals to GPU
    glGenBuffers(1, &buffer.nbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.nbo);
    glBufferData(GL_ARRAY_BUFFER, vn.size() * sizeof(vec3), vn.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Generate and upload a buffer with indices to GPU
    glGenBuffers(1, &buffer.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, f.size() * sizeof(unsigned int), f.data(), GL_STATIC_DRAW);
    buffer.size = (GLsizei) f.size();

    // Copy it to the end of the buffers vector
    buffers.push_back(buffer);
}

CustomMesh::CustomMesh(const string &obj_file) {
    // Load OBJ file
    shapes.clear();
    materials.clear();
    string err = tinyobj::LoadObj(shapes, materials, obj_file.c_str());

    if (!err.empty()) {
        stringstream msg;
        msg << err << endl << "Failed to load OBJ file " << obj_file << "!" << endl;
        throw runtime_error(msg.str());
    }

    // Initialize OpenGL Buffers
    for(auto& shape : shapes) {
        gl_buffer buffer;

        if(!shape.mesh.positions.empty()) {
            // Generate a vertex array object
            glGenVertexArrays(1, &buffer.vao);
            glBindVertexArray(buffer.vao);

            // Generate and upload a buffer with vertex positions to GPU
            glGenBuffers(1, &buffer.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBufferData(GL_ARRAY_BUFFER, shape.mesh.positions.size() * sizeof(float), shape.mesh.positions.data(),
                         GL_STATIC_DRAW);
//            if (obj_file == "./mesh/custom/knight-custom-white.obj") {
//                for (int k=0; k<shape.mesh.positions.size(); k++) {
//                    if (k%3==0) std::cout << "{ ";
//                    std::cout << shape.mesh.positions[k];
//                    if (k%3!=2) std::cout << ", ";
//                    if (k%3==2) std::cout << " },\n";
//                }
//            }

            // Bind the buffer to "Position" attribute in program
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        }

        if(!shape.mesh.texcoords.empty()) {
            // Generate and upload a buffer with texture coordinates to GPU
            glGenBuffers(1, &buffer.tbo);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.tbo);
            glBufferData(GL_ARRAY_BUFFER, shape.mesh.texcoords.size() * sizeof(float), shape.mesh.texcoords.data(),
                         GL_STATIC_DRAW);
//            if (obj_file == "./mesh/custom/knight-custom-white.obj") {
//                for (int k=0; k<shape.mesh.texcoords.size(); k++) {
//                    if (k%2==0) std::cout << "{ ";
//                    std::cout << shape.mesh.texcoords[k];
//                    if (k%2!=1) std::cout << ", ";
//                    if (k%2==1) std::cout << " },\n";
//                }
//            }

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        }

        if(!shape.mesh.normals.empty()) {
            // Generate and upload a buffer with vertex normals to GPU
            glGenBuffers(1, &buffer.nbo);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.nbo);
            glBufferData(GL_ARRAY_BUFFER, shape.mesh.normals.size() * sizeof(float), shape.mesh.normals.data(),
                         GL_STATIC_DRAW);
//            if (obj_file == "./mesh/custom/knight-custom-white.obj") {
//                for (int k=0; k<shape.mesh.normals.size(); k++) {
//                    if (k%3==0) std::cout << "{ ";
//                    std::cout << shape.mesh.normals[k];
//                    if (k%3!=2) std::cout << ", ";
//                    if (k%3==2) std::cout << " },\n";
//                }
//            }

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        }

        // Generate and upload a buffer with indices to GPU
        glGenBuffers(1, &buffer.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.mesh.indices.size() * sizeof(unsigned int), shape.mesh.indices.data(), GL_STATIC_DRAW);
//        if (obj_file == "./mesh/custom/knight-custom-white.obj") {
//            for (int k=0; k<shape.mesh.indices.size(); k++) {
//                std::cout << shape.mesh.indices[k];
//                if (k%9!=8) std::cout << ", ";
//                if (k%9==8) std::cout << ",\n";
//            }
//        }
        buffer.size = (GLsizei) shape.mesh.indices.size();

        // Copy it to the end of the buffers vector
        buffers.push_back(buffer);
    }
}

CustomMesh::~CustomMesh() {
    for(auto& buffer : buffers) {
        glDeleteBuffers(1, &buffer.ibo);
        glDeleteBuffers(1, &buffer.nbo);
        glDeleteBuffers(1, &buffer.tbo);
        glDeleteBuffers(1, &buffer.vbo);
        glDeleteVertexArrays(1, &buffer.vao);
    }
}

void CustomMesh::render() {
    for(auto& buffer : buffers) {
        // Draw object
        glBindVertexArray(buffer.vao);
        glDrawElements(GL_TRIANGLES, buffer.size, GL_UNSIGNED_INT, nullptr);
    }
}
