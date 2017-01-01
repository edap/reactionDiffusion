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
    ofVec2f position;
    ofxAssimpModelLoader model;
    of3dPrimitive trunk;
    int maxXandYposition = 800;
    int startingFoliageHeight = 100;
    int trunkHeight = 550; //this value depends on the loaded mesh and has to be
    //adjusted manually. It is used to calculate the foliage's level position,
    //but it could results wrong positioned if the trunk is too small or too big
};
