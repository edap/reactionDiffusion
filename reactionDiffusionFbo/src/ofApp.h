#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA){
        // Allocate
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
            FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }

        // Clean
        clear();
    }

    void swap(){
        std::swap(src,dst);
    }

    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }

    ofFbo& operator[]( int n ){ return FBOs[n];}
    ofFbo   *src = &FBOs[0];       // Source       ->  Ping
    ofFbo   *dst = &FBOs[1];       // Destination  ->  Pong

private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO�s
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
    void maybeDrawGui();

    ofFbo output;
    pingPongBuffer pingPong;
    ofShader shader;
    string shadersFolder = "shaders_gl3";
    ofImage image;

    float ru = 0.25f;
    float f = 0.0195f;
    float rv = 0.04f;
    float k = 0.066f;

    //GUI, not implemented yet
    void addGui();
    void readFilesDirectory();
    void restartButtonPressed();
    bool hideGui = true;
    int gridWidth = 512;
    int gridHeight = 512;

    ofxFloatSlider dA;
    ofxFloatSlider dB;
    ofxFloatSlider feed;
    //ofxFloatSlider k;

    ofxIntSlider bAmount;
    ofxButton restartButton;
    ofxPanel gui;
    vector<string> fileNames;
    ofDirectory dir;
    int selected;

};