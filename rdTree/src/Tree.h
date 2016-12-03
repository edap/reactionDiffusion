#pragma once
#include "ofMain.h"

class Tree {
public:
    Tree();
    void setup(int n_planes, int resolution, int width, int height, float deg, ofTexture tex);
    void draw();

private:
    void perturbatePlane();
    of3dPrimitive foliage;
    ofNode node;
    vector<glm::vec3> offsets;
};
