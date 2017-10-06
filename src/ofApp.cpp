#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the background to black for max contrast
    ofBackground(0);

    const int NUM_PARTICLES = 10;

    for(int i=0; i<NUM_PARTICLES; i++){
        Particle particle;
        particle.setup();
        particles.push_back(particle);
    }

    for(int i=0; i<particles.size(); i++){
        particles[i].setParticles(&particles);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
}

// set up a particle
void Particle::setup(){
    // assign a randomised radius
    // we assume this is 'infinitesimally small' and therefore
    // the ignore the actual radius except for visualising
    radius = 20;

    // assign it's starting position
    pos.x = ofRandom(radius, ofGetWidth() - radius);
    pos.y = ofRandom(radius, ofGetHeight() - radius);

    // assign it's velocity
    velocity.x = ofRandom(-5, 5);
    velocity.y = ofRandom(-5, 5);

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

    ofVec2f reverseX(-1, 1);
    ofVec2f reverseY(1, -1);

    // right
    if (pos.x > maxX || pos.x < minX){
        velocity *= reverseX;
    }

    if (pos.y > maxY || pos.y < minY){
        velocity *= reverseY;
    }

    // TODO think about what if they interact at a wall?

    for(int i=0; i<particles->size(); i++){
        // check that the particle isn't this particle
        // (is there a better way of doing this?)
        if (pos.x == particles->at(i).pos.x &&
            pos.y == particles->at(i).pos.y){
            continue;
        }

        if (isInteracting(&particles->at(i))){
            colour = ofColor(255,0,0);
        }
    }
}

// draw the particle (the easy bit!)
void Particle::draw(){
    ofSetColor(colour);
    ofDrawCircle(pos.x, pos.y, radius);
}

void Particle::setParticles(vector<Particle> *p){
    particles = p;
}

// in order to tell if a particle interacts with another one
// their areas must be overlapping. The simplest way to calculate
// this is to see if the distance between the two points is
// smaller than the two particles radii combined.
bool Particle::isInteracting(Particle * particle){
    // calculate the distance between two particles
    float d = pos.distance(particle->pos);

    if (d < radius + particle->radius){
        return true;
    }
    return false;
}
