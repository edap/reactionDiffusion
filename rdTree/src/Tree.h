#pragma once
#include "ofMain.h"

class Tree {
public:
    Tree();
    std::vector<shared_ptr<ofPlanePrimitive> > planesContainer;
    void setup(int n_planes, int resolution, int width, int height, float deg);
    void draw(ofTexture);
    void mapTexCoordsFromTexture(ofTexture texture);

private:
    bool mergeMeshes = false;
    void perturbatePlane();
    of3dPrimitive foliage;
    ofNode node;
};
