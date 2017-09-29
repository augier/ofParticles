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
    radius = ofRandom(1, 5);
    
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
    
}

// draw the particle (the easy bit!)
void Particle::draw(){
    ofDrawCircle(pos.x, pos.y, radius);
}
