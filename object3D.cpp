#include "lab_m1/Tema2/object3D.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

namespace object3D
{
    Mesh* CreateMountain(const std::string& name, glm::vec3 color)
    {
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, 0, -1), color),
            VertexFormat(glm::vec3(1, 0, -1), color),
            VertexFormat(glm::vec3(1, 0,  1), color),
            VertexFormat(glm::vec3(-1, 0,  1), color),
            VertexFormat(glm::vec3(0, 2,  0), color)
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3,

            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };

        Mesh* mountain = new Mesh(name);
        mountain->InitFromData(vertices, indices);
        return mountain;
    }

    Mesh* CreateCylinder(
        const std::string& name,
        glm::vec3 color,
        float radius,
        float height,
        int segments)
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < segments; i++) {
            float angle = 2.0f * glm::pi<float>() * i / segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            vertices.push_back(VertexFormat(glm::vec3(x, 0, z), glm::vec3(0, 1, 0), color));
            vertices.push_back(VertexFormat(glm::vec3(x, height, z), glm::vec3(0, 1, 0), color));
        }

        for (int i = 0; i < segments; i++) {
            int next = (i + 1) % segments;
            int i0 = i * 2;
            int i1 = next * 2;
            int o0 = i * 2 + 1;
            int o1 = next * 2 + 1;

            indices.push_back(i0); indices.push_back(o0); indices.push_back(i1);
            indices.push_back(i1); indices.push_back(o0); indices.push_back(o1);
        }

        int baseCenterIndex = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), color));
        for (int i = 0; i < segments; i++) {
            int next = (i + 1) % segments;
            indices.push_back(baseCenterIndex);
            indices.push_back(i = next * 2);
            indices.push_back(i * 2);
        }

        int topCenterIndex = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(0, height, 0), glm::vec3(0, 1, 0), color));
        for (int i = 0; i < segments; i++) {
            int next = (i + 1) % segments;
            indices.push_back(topCenterIndex);
            indices.push_back(i * 2 + 1);
            indices.push_back(next * 2 + 1);
        }

        Mesh* cylinder = new Mesh(name);
        cylinder->InitFromData(vertices, indices);
        return cylinder;
    }

    Mesh* CreateTriangle(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill)
    {

        glm::vec3 corner = leftBottomCorner;
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length / 2, length, 0), color)
        };
        Mesh* triangle = new Mesh(name);
        std::vector<unsigned int> indices = { 0, 1, 2 };
        if (!fill) {
            triangle->SetDrawMode(GL_LINE_LOOP);
        }
        triangle->InitFromData(vertices, indices);
        return triangle;

    }

    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill, bool half)
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        vertices.push_back(VertexFormat(center, color));
        if (half) {
            for (int i = 0; i <= 50; i++) {
                float angle = glm::pi<float>() * i / 50;
                float x = radius * cos(angle);
                float y = radius * sin(angle);
                vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
                indices.push_back(i + 1);
            }
            indices.push_back(1);
        }
        else {
            for (int i = 0; i <= 100; i++) {
                float angle = 2 * glm::pi<float>() * i / 100;
                float x = radius * cos(angle);
                float y = radius * sin(angle);
                vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
                indices.push_back(i + 1);
            }
            indices.push_back(1);
        }

        Mesh* circle = new Mesh(name);

        if (!fill) {
            circle->SetDrawMode(GL_LINE_LOOP);
        }
        else {
            circle->SetDrawMode(GL_TRIANGLE_FAN);
        }

        circle->InitFromData(vertices, indices);
        return circle;
    }
}
