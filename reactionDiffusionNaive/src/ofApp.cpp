#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    readFilesDirectory();
    restartButton.addListener(this, &ofApp::restartButtonPressed);
    addGui();
    
    imgToDraw.allocate(gridWidth, gridHeight, OF_IMAGE_COLOR);
    createEmptyGrids();
    pourABitofB(glm::vec2(gridWidth/2, gridHeight/2), bAmount);
}


//--------------------------------------------------------------
void ofApp::update(){
    ofPixelsRef pixels = imgToDraw.getPixels();
    for(uint y = 1; y< gridHeight -1; y++){
            for(uint x = 1; x < gridWidth -1; x++){
            auto a = grid[x][y].a;
            auto b = grid[x][y].b;
            next[x][y].a = a +(
                (dA * laplaceA(x, y)) -
                (a * b * b) +
                (feed * (1.0 - a)));
            next[x][y].b = b +(
                (dB * laplaceB(x, y)) +
                (a * b * b) -
                ((k + feed) * b));
            next[x][y].a= ofClamp(next[x][y].a, 0.0, 1.0);
            next[x][y].b= ofClamp(next[x][y].b, 0.0, 1.0);

        }
    }

    updatePixelsColor();
    swapGrids();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!hideGui){
        gui.draw();
        ofPushStyle();
        //fps
        string msg = "\n\nfps: " + ofToString(ofGetFrameRate(), 2);
        ofDrawBitmapString(msg, gridWidth, gui.getHeight());
        // file selection
        for (unsigned i = 0; i < fileNames.size(); ++i){
            if (i == selected) ofSetColor(0, 255, 0);
            else ofSetColor(255, 0, 0);
            ofDrawBitmapString(fileNames[i], gridWidth, (gui.getHeight()+20)
                               +(20 * (i + 1)));
        }
        ofPopStyle();
    }

    imgToDraw.draw(0,0);
}

void ofApp::pourABitofB(glm::vec2 pos, int amount){
    int halfAmount = floor(amount/2);
    int startingX = floor(pos.x) - halfAmount;
    int startingY = floor(pos.y) - halfAmount;
    for (int x = startingX; x < (startingX + amount); x++){
        if(x> gridWidth) { return; };
        for (int y = startingY; y < (startingY + amount); y++) {
            if (y> gridHeight) { return; };
            grid[x][y].b = 1.0;
        }
    }
};

void ofApp::createEmptyGrids(){
    for(uint x = 0; x < gridWidth; x++){
        vector<Cell> rowGrid;
        for(uint y = 0; y< gridHeight; y++){
            rowGrid.push_back(Cell());
        };
        grid.push_back(rowGrid);
        next.push_back(rowGrid);
    }
}

void ofApp::swapGrids(){
    vector<vector<Cell>> tmp;
    tmp.swap(grid);
    grid.swap(next);
    next.swap(tmp);
}

void ofApp::updatePixelsColor(){
    ofPixelsRef pixels = imgToDraw.getPixels();
    for(uint x = 0; x < gridWidth; x++){
         for(uint y = 0; y< gridHeight; y++){
            auto a = next[x][y].a;
            auto b = next[x][y].b;
            auto c = floor((a - b) * 255);
            c = ofClamp(c, 0, 255);
            ofColor color = ofColor(c, 0, c);
            pixels.setColor(x,y, color);
        };
    }
    imgToDraw.setFromPixels(pixels);
}


// in both laplace function, it is used a 3x3 Convolution Matrix
// like this
// weights = [[0.05, 0.2, 0.05], [0.2, -1.0, 0.2], [0.05, 0.2, 0.05]]
float ofApp::laplaceA(uint x, uint y){
    float sumA = 0.0;
    sumA += grid[x][y].a * -1;

    sumA += grid[x-1][y].a * 0.2;
    sumA += grid[x+1][y].a * 0.2;
    sumA += grid[x][y+1].a * 0.2;
    sumA += grid[x][y-1].a * 0.2;

    sumA += grid[x-1][y-1].a * 0.05;
    sumA += grid[x+1][y-1].a * 0.05;
    sumA += grid[x+1][y+1].a * 0.05;
    sumA += grid[x-1][y+1].a * 0.05;
    return sumA;
};

float ofApp::laplaceB(uint x, uint y){
    float sumB = 0.0;
    sumB += grid[x][y].b * -1;

    sumB += grid[x-1][y].b * 0.2;
    sumB += grid[x+1][y].b * 0.2;
    sumB += grid[x][y+1].b * 0.2;
    sumB += grid[x][y-1].b * 0.2;

    sumB += grid[x-1][y-1].b * 0.05;
    sumB += grid[x+1][y-1].b * 0.05;
    sumB += grid[x+1][y+1].b * 0.05;
    sumB += grid[x-1][y+1].b * 0.05;
    return sumB;
};

// GUI
void ofApp::addGui(){
    gui.setup();
    gui.setPosition(ofPoint(gridWidth, 0));
    gui.add(dA.setup("dA", 1.0, 0.050, 1.0));
    gui.add(dB.setup("dB", 0.5, 0.050, 1.0));
    gui.add(feed.setup("feed", 0.055, 0.018, 0.060));
    gui.add(k.setup("k", 0.062, 0.050, 0.070));


    gui.add(bAmount.setup("Chemicak B amount", 20, 10, 300));
    gui.add(restartButton.setup("restart"));
}

void ofApp::restartButtonPressed(){
    grid.clear();
    next.clear();

    createEmptyGrids();
    pourABitofB(glm::vec2(gridWidth/2, gridHeight/2),bAmount);
}

void ofApp::readFilesDirectory(){
    selected = 0;
    dir.listDir("presets");
    dir.sort();
    for(auto file:dir){
        fileNames.push_back(file.path());
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'g':
            hideGui = !hideGui;
            break;
        case 's':
            gui.saveToFile("settings.json");
            break;
        case OF_KEY_RETURN:
            gui.loadFromFile(fileNames[selected]);
            break;

        case OF_KEY_UP:
            selected = (selected - 1);
            if (selected < 0) selected = fileNames.size() - 1;
            break;

        case OF_KEY_DOWN:
            selected = (selected + 1) % fileNames.size();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
