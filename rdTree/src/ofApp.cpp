#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //ofDisableAlphaBlending();
    ofEnableDepthTest();

    objectLocation = glm::vec3(0.0,0.0,0.0);
    width = ofGetWidth();
    height = ofGetHeight();

    // Plane
    plane.set(1024, 1024);
    plane.setPosition(objectLocation);
    plane.setResolution(1024, 1024);

    //Sphere
    sphere.set(100, 100);
    //box
    box.set( 100 );

    readFilesDirectory();
    ofEnableSmoothing();
    restartButton.addListener(this, &ofApp::restartButtonPressed);
    if (!ofIsGLProgrammableRenderer()) {
        ofLogError("this app supports only open the programmable render pipeline");
        return 1;
    } else {
        shader.load(shadersFolder+"/passthru.vert", shadersFolder+"/grayscott.frag");
        shaderNormalMap.load(shadersFolder+"/normals.vert", shadersFolder+"/normals.frag");
        updateRender.load(shadersFolder+"/displacement.vert", shadersFolder+"/render.frag");
//        glm::mat4 mvmatrix = cam.getModelViewMatrix();
//        auto normalMatrix = glm::transpose(glm::inverse(mvmatrix));
//        updateRender.setUniformMatrix4f("normalMatrix", normalMatrix);

    };

    addGui();
    clearBuffersAndAllocate();
    // tree needs to be called after the the buffer allocation
    // because we need the texture
    tree.setup(6, 1024, 1024, 1024, 10, pingPong.src->getTexture());
    // light comes last because lightPos is set in the GUI
    light.setup();
    light.enable();
    light.setPosition(lightPos);
    light.lookAt(objectLocation);
}

//--------------------------------------------------------------
void ofApp::update(){
    light.setPosition(lightPos);
    light.lookAt(objectLocation); // ? needed?
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackgroundGradient(ofFloatColor::seaGreen, ofFloatColor::saddleBrown);
    // start ping pong
    ofDisableDepthTest();
    for( int i = 0; i < nPasses ; i++ ){
        pingPong.dst->begin();
        shader.begin();
        shader.setUniformTexture("prevTexture", pingPong.src->getTexture(), 0 );
        shader.setUniformTexture("tex0", output.getTexture(), 1 );
        shader.setUniform1f( "ru", ru);
        shader.setUniform1f( "rv", rv);
        shader.setUniform1f( "f", f);
        shader.setUniform1f( "k", k);
        pingPong.src->draw(0, 0); // draw the source texture here!!!
        shader.end();
        pingPong.dst->end();
        pingPong.swap();
    }
    pingPong.swap();
    //end ping pong
    ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();

    auto texture = pingPong.src->getTexture();
    float useNormalMapToFloat = useNormalMap? 1.0 : 0.0;
    ofEnableDepthTest();
    cam.begin();
    texture.bind();
    //plane.mapTexCoordsFromTexture(texture);
    //sphere.mapTexCoordsFromTexture(texture);
    //tree.mapTexCoordsFromTexture(texture);
    //box.mapTexCoordsFromTexture(texture);
    updateRender.begin();
    updateRender.setUniform1f("discardRed", discardRed);
    updateRender.setUniform1f("useNormalMap", useNormalMapToFloat);
    updateRender.setUniform3f("lightPos", lightPos);
    updateRender.setUniformMatrix4f("modelMatrix", modelMatrix);
    updateRender.setUniform1f("displaceAmount", displaceAmount);
    //plane.draw();
    tree.draw();
    // to debug the shaderNormalMap
    //shaderNormalMap.begin();
    //sphere.draw();
    //box.draw();
    //shaderNormalMap.end();
    updateRender.end();
    texture.unbind();
    light.draw();


    cam.end();
    ofDisableDepthTest();
    maybeDrawGui();
}

void ofApp::clearBuffersAndAllocate(){
    ofDisableDepthTest();
    // to use an image as source instead the mouse click, uncomment this:
    if(useImage){
        image.load("img2.jpg");
        width = image.getWidth();
        height = image.getHeight();
    }

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
    // uncomment this to use an image as source
    if (useImage) {
        image.draw(0,0, width, height);
    }
    pingPong.src->end();

    output.begin();
    ofClear(0, 0, 0, 255);
    output.end();
    ofEnableDepthTest();
}

// GUI
void ofApp::addGui(){
    gui.setup();
    gui.setPosition(ofPoint(0, 30));
    gui.add(ru.setup("ru", 0.25, 0.09, 0.26));
    gui.add(rv.setup("rv", 0.04, 0.04, 0.16));
    gui.add(f.setup("feed", 0.0195, 0.018, 0.060));
    gui.add(k.setup("k", 0.066, 0.050, 0.070));
	gui.add(lightPos.setup("lightPosition",
                           ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5, 100),
                           ofVec3f(-400, -400, -400),
                           ofVec3f(ofGetWidth(), ofGetHeight(),600)));
    gui.add(materialColor.setup("material",
                                ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));

    gui.add(discardRed.setup("discardRed", 0.25, 0.01, 1.0));
    gui.add(displaceAmount.setup("displaceAmount", 4.0, 0.1, 20.0));
    gui.add(useImage.setup("useImage", true));
    gui.add(useNormalMap.setup("useNormalMap", true));
    gui.add(nPasses.setup("passes", 4, 1, 30));
    gui.add(radius.setup("radius", 10, 3, 50));
    gui.add(restartButton.setup("restart"));
}

void ofApp::maybeDrawGui(){
    if(!hideGui){
        gui.draw();
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
//    pingPong.src->begin();
//    ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
//    //ofSetColor(0,255,255);
//    ofDrawCircle(x, y, radius);
//    pingPong.src->end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (!useImage) {
        pingPong.src->begin();
        ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
        ofDrawCircle(x, y, radius);
        pingPong.src->end();
    }

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
