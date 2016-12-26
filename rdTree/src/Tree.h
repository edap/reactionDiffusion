#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Tree {
public:
    Tree();
    void setup(int n_planes, int resolution, int width, int height, float deg, ofTexture tex, string objPath);
    void draw();
    void drawTrunk();

private:
    void perturbatePlane();
    of3dPrimitive foliage;
    ofVec3f position;
    ofxAssimpModelLoader trunk;
    //ofVboMesh trunk;

    ofNode node;
    //vector<glm::vec3> offsets;
};
