#include "Tree.h"

Tree::Tree(){

}

void Tree::setup(int n_planes, int resolution, int width, int height, float deg, ofTexture texture, string objPath){
    //shared_ptr<ofNode> root(new ofNode);
    //root->setPosition(glm::vec3(0,0,0));
    //planeContainer.push_back(root);

    int distance = 100;
    float rot = 0;
    for (uint i = 0; i < n_planes; i++) {
            auto p = ofPlanePrimitive(width, height, resolution, resolution, OF_PRIMITIVE_TRIANGLES);

            p.mapTexCoordsFromTexture(texture);
            p.setPosition(0, 0,float(distance*i));
            p.roll(rot);
            //p.rollDeg(rot);
            auto mesh =p.getMesh();
            for (int i=0; i<mesh.getNumVertices(); i++) {
                auto vert = mesh.getVertex(i);

                float time = ofGetElapsedTimef();
                float timeScale = 5.0;
                float displacementScale = 50.75;

                // A typical design pattern for using Perlin noise uses a couple parameters:
                // ofSignedNoise(time*timeScale+timeOffset)*displacementScale
                //     ofSignedNoise(time) gives us noise values that change smoothly over
                //         time
                //     ofSignedNoise(time*timeScale) allows us to control the smoothness of
                //         our noise (smaller timeScale, smoother values)
                //     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise
                //         function to control multiple things and have them look as if they
                //         are moving independently
                //     ofSignedNoise(time)*displacementScale allows us to change the bounds
                //         of the noise from [-1, 1] to whatever we want
                // Combine all of those parameters together, and you've got some nice
                // control over your noise

                vert.x += (ofSignedNoise(time*timeScale)) * displacementScale;
                vert.y += (ofSignedNoise(time*timeScale)) * displacementScale;
                vert.z += (ofSignedNoise(time*timeScale)) * displacementScale;
                p.getMesh().setVertex(i, vert);
            }

            rot+= deg;

            for (int i=0; i<p.getMesh().getNumVertices(); i++) {
                ofVec3f actualVert =p.getMesh().getVerticesPointer()[i];
                ofVec3f newVert = actualVert * p.getGlobalTransformMatrix();
                p.getMesh().getVerticesPointer()[i] = newVert;
            }

        foliage.getMesh().append(p.getMesh());
        foliage.move(0, 0, 50);
    }
    trunk.loadModel(objPath);
    trunk.setRotation(0, 90, 1, 0, 0);

    //you have to rotate the loaded model (google it)
    // than you have to find the right proportions
    // than you have to stop the rd before it becomes a plane
}

void Tree::draw(){
    foliage.draw();
}

void Tree::drawTrunk(){
    trunk.drawFaces();
}

void Tree::perturbatePlane(){


}
