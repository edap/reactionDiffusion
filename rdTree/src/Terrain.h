#pragma once
#include "ofMain.h"

class Terrain : public of3dPrimitive {
public:
    Terrain();
    void setup(int _cols = 60,
               int _rows = 60,
               float _frequence = 0.2,
               int _time_x = 1,
               int _time_y = 10,
               int _time_z = 100,
               float _max_z = 300.0,
               float _min_z = 50.0);
    void draw();

    int width = ofGetWidth();
    int height = ofGetHeight();

    ofVboMesh mesh;
    vector<ofVec3f> net;

    //grid variables
    int cols;
    int rows;
    float dim_cells;

    //noise variables
    //1 noise positioning
    float time_x;
    float time_y;
    float time_z;
    float frequence;

    //2noise updating
    float z_off;
    int min_z;
    int max_z;
    float z_off_increment;
    float amplitude;
    float freq;
    float speed;
    int zoom;
    bool draw_faces;
    bool camera_mouse;


    float red, green, blue, alpha;
    ofColor backgroundColor;


private:
    void calcNormals(ofMesh& mesh);
    of3dPrimitive foliage;
    ofVboMesh trunk;
};
