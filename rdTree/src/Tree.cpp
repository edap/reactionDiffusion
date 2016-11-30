#include "Tree.h"

Tree::Tree(){

}

void Tree::setup(int n_planes, int resolution, int width, int height, float deg){
    //shared_ptr<ofNode> root(new ofNode);
    //root->setPosition(glm::vec3(0,0,0));
    //planeContainer.push_back(root);

    int distance = 100;
    float rot = 0;
    for (uint i = 0; i < n_planes; i++) {


        if(mergeMeshes){
            auto p = ofPlanePrimitive(width, height, resolution, resolution, OF_PRIMITIVE_TRIANGLES);
            p.setPosition(0, 0,float(distance*i));
            //p.rollDeg(rot);
            rot+= deg;

            for (int i=0; i<p.getMesh().getNumVertices(); i++) {
                ofVec3f actualVert =p.getMesh().getVerticesPointer()[i];
                ofVec3f newVert = actualVert * p.getGlobalTransformMatrix();
                p.getMesh().getVerticesPointer()[i] = newVert;
            }

            foliage.getMesh().append(p.getMesh());
        }else{
            shared_ptr<ofPlanePrimitive> plane (new ofPlanePrimitive(width, height, resolution, resolution, OF_PRIMITIVE_TRIANGLES));
            plane->setPosition(0, 0,float(distance*i));
            plane->rollDeg(rot);
            planesContainer.push_back(plane);
        }
    }
}

void Tree::draw(ofTexture texture){
    if(mergeMeshes){
        //foliage.mapTexCoordsFromTexture(texture);
        foliage.draw();
    }else{
        for(auto plane : planesContainer) {
            plane->mapTexCoordsFromTexture(texture);
            plane->draw();
        }
    }
}

void Tree::perturbatePlane(){

}

void Tree::mapTexCoordsFromTexture(ofTexture texture){
    if(mergeMeshes){
       foliage.mapTexCoordsFromTexture(texture);
    // otherwise this call is in the draw method
    };
}
