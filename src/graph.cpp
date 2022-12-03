#include "graph.hpp"

#include <stdint.h>

static float marchTable[][4] = {
    // IGNORE
    { 0, 0, 0, 0 },

    // 0001
    { 0.0, 0.5, 0.5, 0.0 },

    // 0010
    { 0.5, 0.0, 1.0, 0.5 },

    // 0011
    { 0.0, 0.5, 1.0, 0.5 },

    // 0100
    { 0.5, 1.0, 1.0, 0.5 },

    // 0101
    { 0.0, 1.0, 1.0, 0.0 },

    // 0110
    { 0.5, 0.0, 0.5, 1.0 },

    // 0111
    { 0.0, 0.5, 0.5, 1.0 },

    // 1000
    { 0.0, 0.5, 0.5, 1.0 },

    // 1001
    { 0.5, 0.0, 0.5, 1.0 },

    // 1010
    { 0.0, 0.0, 1.0, 1.0 },

    // 1011
    { 0.5, 1.0, 1.0, 0.5 },

    // 1100
    { 0.0, 0.5, 1.0, 0.5 },
    
    // 1101
    { 0.5, 0.0, 1.0, 0.5 },

    // 1110
    { 0.0, 0.5, 0.5, 0.0 },
};

std::vector<float> buildGraphMesh(std::unique_ptr<ASTNode>& node, double xmin, double xmax, double ymin, double ymax) {
    double xstep = (xmax - xmin) / RESOLUTION;
    double ystep = (ymax - ymin) / RESOLUTION;

    uint8_t funcTable[RESOLUTION][RESOLUTION];
    for (int i = 0; i < RESOLUTION; i++) {
        for (int j = 0; j < RESOLUTION; j++) {
            double x = xmin + i * xstep;
            double y = ymin + j * ystep;

            funcTable[i][j] = (uint8_t)(node->evaluate(x, y) > 0);
        }
    }

    std::vector<float> mesh;
    for (int i = 0; i < RESOLUTION - 1; i++) {
        for (int j = 0; j < RESOLUTION - 1; j++) {
            uint8_t bottomLeft = funcTable[i][j];
            uint8_t bottomRight = funcTable[i + 1][j];
            uint8_t topLeft = funcTable[i][j + 1];
            uint8_t topRight = funcTable[i + 1][j + 1];

            uint8_t marchCode = (topLeft << 3) | (topRight << 2) | (bottomRight << 1) | bottomLeft;

            if (marchCode == 0 || marchCode == 15)
                continue;

            float* offsets = marchTable[marchCode];

            mesh.push_back(i + offsets[0]);
            mesh.push_back(j + offsets[1]);
            
            mesh.push_back(i + offsets[2]);
            mesh.push_back(j + offsets[3]);
        }
    }

    return mesh;
}