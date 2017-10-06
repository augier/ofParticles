#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the background to black for max contrast
    ofBackground(0);

    particle.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    particle.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    particle.draw();
}

// set up a particle
void Particle::setup(){
    // assign a randomised radius
    // we assume this is 'infinitesimally small' and therefore
    // the ignore the actual radius except for visualising
    radius = 3;

    // assign it's starting position
    pos.x = ofRandom(radius, ofGetWidth() - radius);
    pos.y = ofRandom(radius, ofGetHeight() - radius);

    // assign it's velocity
    velocity.x = ofRandom(-10, 10);
    velocity.y = ofRandom(-10, 10);

    // give it a randomised colour
    colour.r = ofRandom(0, 255);
    colour.g = ofRandom(0, 255);
    colour.b = ofRandom(0, 255);
}

// particles bounce off the edges of the container with no forces
void Particle::update(){
    // update the position using the particles velocity
    pos.x += velocity.x;
    pos.y += velocity.y;

    // calculate min and max for both x and y
    // to keep the maths simple, ignore the radius
    float minX = 0;
    float minY = 0;
    float maxX = ofGetWidth();
    float maxY = ofGetHeight();

    // need to detect whether the particle has reached the
    // edge of the container by comparing the particles
    // position with one edge at a time. Each time an edge
    // is reached the velocity

    ofVec2f reverseX(-0.5, 1);
    ofVec2f reverseY(1, -0.5);

    // right
    if (pos.x > maxX || pos.x < minX){
        velocity *= reverseX;
    }

    if (pos.y > maxY || pos.y < minY){
        velocity *= reverseY;
    }
}

// draw the particle (the easy bit!)
void Particle::draw(){
    ofSetColor(colour);
    ofDrawCircle(pos.x, pos.y, radius);
}
