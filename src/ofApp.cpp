#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the background to black for max contrast
    ofBackground(0);

    const int NUM_PARTICLES = 5;

    for(int i=0; i<NUM_PARTICLES; i++){
        Particle particle;
        particle.setup();
        particles.push_back(particle);
    }

    for(int i=0; i<particles.size(); i++){
        particles[i].setDebug(&debug);
        particles[i].setParticles(&particles);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // first set all of the particles updated to be false
    for(int i=0; i<particles.size(); i++){
        particles[i].updated = false;
    }
    for(int i=0; i<particles.size(); i++){
        particles[i].update();
    }

    energy = 0;
    for(int i=0; i<particles.size(); i++){
        energy += particles[i].energy;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int i=0; i<particles.size(); i++){
        particles[i].draw();
    }

    ofSetColor(255);
    ofDrawBitmapString("Total energy: " + to_string(energy), 10, 10);
}

void ofApp::keyPressed(int key){
    switch (key){
    case 100: // d
        debug = !debug;
    }
}

// set up a particle
void Particle::setup(){
    // assign a randomised radius
    // we assume this is 'infinitesimally small' and therefore
    // the ignore the actual radius except for visualising
    radius = ofRandom(5, 40);

    // we'll calculate it's mass as it's surface area
    mass = 2 * pi * pow(radius, 2);

    // assign it's starting position
    pos.x = ofRandom(radius, ofGetWidth() - radius);
    pos.y = ofRandom(radius, ofGetHeight() - radius);

    // assign it's velocity
    velocity.x = ofRandom(-3, 3);
    velocity.y = ofRandom(-3, 3);

    // give it a randomised colour
    colour.r = ofRandom(0, 255);
    colour.g = ofRandom(0, 255);
    colour.b = ofRandom(0, 255);
}

// particles bounce off the edges of the container with no forces
void Particle::update(){

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

    // check to see if this particle has already been updated
    // by another. Perhaps find a better way of achieving this
    if (updated){
        return;
    }

    // TODO think about what if they interact at a wall?

    for(int i=0; i<particles->size(); i++){
        // check that the particle isn't this particle
        // (is there a better way of doing this?)
        if (pos.x == particles->at(i).pos.x &&
            pos.y == particles->at(i).pos.y){
            continue;
        }

        // check to see if this particle is interacting with
        // the other particle
        if (isInteracting(&particles->at(i))){
            // get all the required information
            Particle particle2 = particles->at(i);
            float m1 = mass;
            float m2 = particle2.mass;
            ofVec2f v1 = velocity;
            ofVec2f v2 = particle2.velocity;
            ofVec2f p1 = pos;
            ofVec2f p2 = particle2.pos;

            // This mdoel is taken from http://www.euclideanspace.com/physics/dynamics/collision/twod/
            // and assumes the two particles hit each other head on. This will mean that there is some
            // strange behaviour.
            velocity = (v1 * (m1 - m2) / (m1 + m2)) + (v2 * (2 * m2) / (m1 + m2));
            ofVec2f newVelocity = (v1 * (2 * m1) / (m1 + m2)) - (v2 * (m1 - m2) / (m1 + m2));

            particles->at(i).velocity = newVelocity;
            particles->at(i).pos += particles->at(i).velocity;
            updated = true;
            particles->at(i).updated = true;
        }
    }

    // update the position using the particles velocity
    pos += velocity;
    energy = 0.5 * mass * pow(velocity.length(), 2);
}

// draw the particle (the easy bit!)
void Particle::draw(){
    ofSetColor(colour);
    ofDrawCircle(pos.x, pos.y, radius);

    // write debug information
    if (*debug){
        ofDrawBitmapString(energy, pos.x + radius, pos.y + radius);
    }}

// debug is a variable at ofApp level. Perhaps look at extending
// particle class to be of ofBaseApp also?that'
void Particle::setDebug(bool * d){
    debug = d;
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
    float dx = pos.x - particle->pos.x;
    float dy = pos.y - particle->pos.y;

    float d = sqrt(pow(dx, 2) + pow(dy,2));

    if (d < radius + particle->radius){
        return true;
    }
    return false;
}
