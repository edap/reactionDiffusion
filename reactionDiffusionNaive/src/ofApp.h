#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Cell {
public:
    Cell(){};
    double a = 1.0;
    double b = 0.0;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void addGui();
    void readFilesDirectory();
    void restartButtonPressed();
    float laplaceA(uint x, uint y);
    float laplaceB(uint x, uint y);
    void pourABitofB(glm::vec2 pos, int amount);
    void createEmptyGrids();
    void updatePixelsColor();
    void swapGrids();

    bool hideGui = false;
    int gridWidth = 512;
    int gridHeight = 512;

    ofxFloatSlider dA;
    ofxFloatSlider dB;
    ofxFloatSlider feed;
    ofxFloatSlider k;

    ofxIntSlider bAmount;
    ofxButton restartButton;
    ofxPanel gui;
    vector<string> fileNames;
    ofDirectory dir;
    int selected;

    vector<vector<Cell>> grid;
    vector<vector<Cell>> next;
    ofImage imgToDraw;
    
};
