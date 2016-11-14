#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableAlphaBlending();
    ofEnableDepthTest();
    width = ofGetWidth();
    height = ofGetHeight();

    //Light
    lightPos = glm::vec3(0.8,0.3,0.2);
    light.setup();
    light.enable();
    light.setPosition(lightPos);
    light.lookAt(ofVec3f(0,0,0));

    // Plane
    plane.set(640, 480);
    plane.setPosition(0, 0, 0);
    plane.setResolution(20, 20);

    readFilesDirectory();
    ofEnableSmoothing();
    restartButton.addListener(this, &ofApp::restartButtonPressed);
    if (!ofIsGLProgrammableRenderer()) {
        ofLogError("this app supports only open the programmable render pipeline");
        return 1;
    } else {
        //shader.load(shadersFolder+"/passthru.vert", shadersFolder+"/grayscott.frag");
        shader.load(shadersFolder+"/normalGrayScott.vert", shadersFolder+"/lightGrayScott.frag");
    };
    clearBuffersAndAllocate();
    addGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout << materialColor.getParameter() << endl;

    auto lightPos = light.getGlobalPosition();
    for( int i = 0; i < nPasses ; i++ ){
        pingPong.dst->begin();
        shader.begin();
        shader.setUniformTexture("prevTexture", pingPong.src->getTexture(), 0 );
        shader.setUniformTexture("tex0", output.getTexture(), 1 );
        shader.setUniform3f("uLightPosition", lightPos);
        shader.setUniform4f("uMaterialColor", ofColor(materialColor));
        shader.setUniform1f( "ru", ru);
        shader.setUniform1f( "rv", rv);
        shader.setUniform1f( "f", f );
        shader.setUniform1f( "k", k );
        pingPong.src->draw(0, 0); // draw the source texture here!!!
        shader.end();

        pingPong.dst->end();
        pingPong.swap();
    }
    pingPong.swap();
    light.setPosition(lightPos);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //pingPong.src->draw(0,0);

    cam.begin();
    pingPong.src->getDepthTexture().bind();
    shader.begin();
    plane.draw();
    shader.end();
    pingPong.src->getDepthTexture().unbind();
    cam.end();

    maybeDrawGui();

}

void ofApp::clearBuffersAndAllocate(){
    // to use an image as source instead the mouse click, uncomment this:
    // image.load("img.jpg");
    // width = image.getWidth();
    // height = image.getHeight();
    // output.allocate(width, height, GL_RGBA);
    // pingPong.allocate(width, height, GL_RGBA);
    ofFbo::Settings settings;
    settings.width = width;
    settings.height	= height;
    settings.internalformat = GL_RGBA; // or GL_RGBA?
    settings.useDepth = true;
    settings.depthStencilAsTexture = true;

    output.allocate(settings);
    pingPong.allocate(settings);
    pingPong.clear();
    pingPong.src->begin();

    // throwing a circle
    for (int i = 0; i< 300; i++) {
        ofSetColor(ofColor(0, 255, 255));
        ofDrawCircle(ofRandom(0, width),
                     ofRandom(0, height),
                     20);

        ofSetColor(ofColor(120, 200, 255));
        ofDrawCircle(ofRandom(0, width),
                     ofRandom(0, height),
                     20);
    }
    pingPong.src->end();

    output.begin();
    ofClear(0, 0, 0, 255);
    output.end();
}

// GUI
void ofApp::addGui(){
    gui.setup();
    gui.setPosition(ofPoint(0, 30));
    gui.add(ru.setup("ru", 0.19, 0.09, 0.26));
    gui.add(rv.setup("rv", 0.05, 0.04, 0.16));
    gui.add(f.setup("feed", 0.06, 0.018, 0.060));
    gui.add(k.setup("k", 0.062, 0.050, 0.070));

    gui.add(lightPos.set("lightPosition",lightPos
                         ));
    gui.add(materialColor.setup("material",
                                 ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));


    gui.add(nPasses.setup("passes", 10, 1, 30));
    gui.add(restartButton.setup("restart"));
}

void ofApp::maybeDrawGui(){
    if(!hideGui){
        ofDisableDepthTest();
        gui.draw();
        ofEnableDepthTest();
        ofPushStyle();
        string displayGui = "press 'g' to toggle the gui, up and down to change presets";
        ofDrawBitmapString(displayGui, 0, 10);
        //fps
        string msg = "\n\nfps: " + ofToString(ofGetFrameRate(), 2);
        ofDrawBitmapString(msg, 0, 0);
        // file selection
        for (unsigned i = 0; i < fileNames.size(); ++i){
            if (i == selected) ofSetColor(0, 255, 0);
            else ofSetColor(0, 0, 255);
            ofDrawBitmapString(fileNames[i], 0, (gui.getHeight()+20)
                               +(20 * (i + 1)));
        }
        ofPopStyle();
    }
}

void ofApp::restartButtonPressed(){
    clearBuffersAndAllocate();
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