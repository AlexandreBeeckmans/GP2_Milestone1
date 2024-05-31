#include "GP2SphereMesh.h"

GP2SphereMesh::GP2SphereMesh(const glm::vec3& center, const float radius)
{
	m_Position = center;


    //Based on the following implementation :
    //https://github.com/Erkaman/cute-deferred-shading/blob/master/src/main.cpp#L573

    int stacks = 20;
    int slices = 20;
    const float PI = 3.14f;


    // loop through stacks.
    for (int i = 0; i <= stacks; ++i) {

        float V = (float)i / (float)stacks;
        float phi = V * PI;

        // loop through the slices.
        for (int j = 0; j <= slices; ++j) {

            float U = (float)j / (float)slices;
            float theta = U * (PI * 2);

            // use spherical coordinates to calculate the positions.
            float x = center.x + radius * cos(theta) * sin(phi);
            float y = center.y + radius * cos(phi);
            float z = center.z + radius * sin(theta) * sin(phi);

            float u = static_cast<float>(j) / static_cast<float>(slices);
            float v = static_cast<float>(i) / static_cast<float>(stacks);

            glm::vec3 normal = glm::normalize(glm::vec3{ x,y,z } - center);
            glm::vec3 tangent = glm::vec3{ };

            tangent.x = sin(theta) * cos(phi);
            tangent.y = sin(theta) * sin(phi);
            tangent.z = cos(theta);

            AddVertex({ x, y, z }, {1,0,0}, normal, glm::vec2{ u, v }, tangent);
        }
    }

    // Calc The Index Positions
    for (int i = 0; i < slices * stacks + slices; ++i) 
    {
        AddIndex(i + slices + 1);
        AddIndex(i);
        AddIndex(i + slices);

        AddIndex(i);
        AddIndex(i + slices + 1);
        AddIndex(i + 1);
    }
}
