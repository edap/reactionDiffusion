#include "Terrain.h"

Terrain::Terrain(){
    
}

void Terrain::setup(int _cols,
                    int _rows,
                    float _frequence,
                    int _time_x,
                    int _time_y,
                    int _time_z,
                    float _max_z,
                    float _min_z
                    ){
        // grid vars
        cols = _cols;
        rows = _rows;
        dim_cells = width/float(cols);

        // perlin noise values that affect the
        // initial position of the vertices in the grid
        frequence = _frequence;
        time_x = _time_x;
        time_y = _time_y;
        time_z = _time_z;

        // perlin noise update values, initialization
        max_z = _max_z;
        min_z = _min_z;


        //mesh population
        int current_row = 0;
        int current_col = 0;
        for (int x = 0; x <= cols; x++) {
            for (int y = 0; y <= rows; y++) {
                float noise_x = ofSignedNoise(time_x);
                float noise_y = ofSignedNoise(time_y);
                float noise_z = ofSignedNoise(time_z);
                float tmp_x = ofMap(noise_x * frequence, 0, 1, 0, dim_cells);
                float tmp_y = ofMap(noise_y * frequence, 0, 1, 0, dim_cells);
                float tmp_z = ofMap(noise_z * frequence, 0, 1, min_z, max_z);
                ofVec3f v = ofVec3f(current_row + tmp_x,
                                    current_col + tmp_y,
                                    tmp_z);
                // uncomment this line to avoid noise in the initial setup
                //ofVec3f v = ofVec3f(current_row, current_col, 0);
                mesh.addVertex(v);
                current_col += dim_cells;
                time_y += 0.1;
                time_x += 0.1;
                time_z += 0.1;
            }
            current_col = 0;
            current_row += dim_cells;
        }
        cout << mesh.getNumVertices() << endl;
        //Adding index, square per square, re-using vertices
        int current_index = 0;
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++) {
                //first triangle
                mesh.addTriangle(current_index,
                                 current_index + 1,
                                 (current_index + rows)+1
                                 );
                //second triangle
                mesh.addTriangle(
                                 (current_index + 1),
                                 ((current_index + rows)+1),
                                 (current_index + rows)+2
                                 );
                current_index += 1;
            }
            current_index += 1;
        }
    getMesh() = mesh;
    //mesh.clear();
    calcNormals(mesh);

}

void Terrain::draw(){
    mesh.draw();
}

void Terrain::calcNormals(ofMesh& mesh){
    mesh.clearNormals();
    // https://forum.openframeworks.cc/t/mesh-lighting-normal-issues/18127/11
    // via https://github.com/ofZach/ofxMeshUtils/blob/master/src/ofxMeshUtils.cpp
    // inspired by http://www.iquilezles.org/www/articles/normals/normals.htm
    for( int i=0; i < mesh.getVertices().size(); i++ ) mesh.addNormal(ofPoint(0,0,0));

    //itero un triangolo alla volta
    for( int i=0; i < mesh.getIndices().size(); i+=3 ){
        const int ia = mesh.getIndices()[i];
        const int ib = mesh.getIndices()[i+1];
        const int ic = mesh.getIndices()[i+2];

        //calcolo la posizione e la direzione di due vertici di un triangolo
        ofVec3f e1 = mesh.getVertices()[ia] - mesh.getVertices()[ib];
        ofVec3f e2 = mesh.getVertices()[ic] - mesh.getVertices()[ib];
        //uso i due vertici per trovare il normal
        ofVec3f no = e2.cross( e1 );

        // depending on your clockwise / winding order, you might want to reverse the e2 / e1 above if your normals are flipped.

        mesh.getNormals()[ia] += no;
        mesh.getNormals()[ib] += no;
        mesh.getNormals()[ic] += no;
    }
    //normalizzo tutti i vettori che definiscono i normals
    for(int i=0; i < mesh.getNormals().size(); i++ ) {
        mesh.getNormals()[i].normalize();
    }
}


