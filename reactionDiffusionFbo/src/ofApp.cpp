#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    restartButton.addListener(this, &ofApp::restartButtonPressed);
    if (!ofIsGLProgrammableRenderer()) {
        ofLogError("this app supports only open the programmable render pipeline");
        return 1;
    } else {
        shader.load(shadersFolder+"/passthru.vert", shadersFolder+"/grayscott.frag");
    };
    image.load("img.jpg");
    output.allocate(image.getWidth(), image.getHeight(), GL_RGBA);
    pingPong.allocate(image.getWidth(), image.getHeight(), GL_RGBA);
    pingPong.clear();
    pingPong.src->begin();
    image.draw(0,0, width, height);
    pingPong.src->end();

    output.begin();
    ofClear(0, 0, 0, 255);
    output.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    pingPong.dst->begin();
    shader.begin();
    shader.setUniformTexture("prevTexture", pingPong.src->getTexture(), 0 );
    shader.setUniformTexture("tex0", output.getTexture(), 1 );
    shader.setUniform1f( "ru", (float)ru);
    shader.setUniform1f( "rv", (float)rv);
    shader.setUniform1f( "f", (float)f );
    shader.setUniform1f( "k", (float)k );
    pingPong.src->draw(0, 0); // draw the source texture here!!!
    shader.end();
    pingPong.dst->end();
    pingPong.swap();


    //    for( int i = 0; i < 5 ; i++ ){
    //        pingPong.dst->begin();
    //        ofClear(0,0,0,255);
    //        // bind the texture
    //        image.getTexture().bind();
    //        shader.begin();
    //        shader.setUniformTexture("prevTexture", pingPong.src->getTexture(), 0);
    //        shader.setUniformTexture("srcTexture", output, 1);
    //        shader.setUniform1f( "ru", (float)ru);
    //        shader.setUniform1f( "rv", (float)rv);
    //        shader.setUniform1f( "f", (float)f );
    //        shader.setUniform1f( "k", (float)k );
    //        shader.end();
    //        //unbind the texture
    //        image.getTexture().unbind();
    //
    //        pingPong.dst->end();
    //        pingPong.swap();
    //    }
    //
    //    pingPong.swap();
}

//--------------------------------------------------------------
void ofApp::draw(){
    pingPong.src->draw(0,0);
}

// GUI
void ofApp::addGui(){
    gui.setup();
    gui.setPosition(ofPoint(width, 0));
    gui.add(dA.setup("dA", 1.0, 0.050, 1.0));
    gui.add(dB.setup("dB", 0.5, 0.050, 1.0));
    gui.add(feed.setup("feed", 0.055, 0.018, 0.060));
    //gui.add(k.setup("k", 0.062, 0.050, 0.070));


    gui.add(bAmount.setup("Chemicak B amount", 20, 10, 300));
    gui.add(restartButton.setup("restart"));
}

void ofApp::maybeDrawGui(){
    if(!hideGui){
        gui.draw();
        ofPushStyle();
        //fps
        string msg = "\n\nfps: " + ofToString(ofGetFrameRate(), 2);
        ofDrawBitmapString(msg, width, gui.getHeight());
        // file selection
        for (unsigned i = 0; i < fileNames.size(); ++i){
            if (i == selected) ofSetColor(0, 255, 0);
            else ofSetColor(255, 0, 0);
            ofDrawBitmapString(fileNames[i], width, (gui.getHeight()+20)
                               +(20 * (i + 1)));
        }
        ofPopStyle();
    }
}

void ofApp::restartButtonPressed(){

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
    pingPong.src->begin();
    ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
    ofDrawCircle(x, y, 3);
    pingPong.src->end();
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
