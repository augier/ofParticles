#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the background to black for max contrast
    ofBackground(0);

    const int NUM_PARTICLES = 200;

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
    radius = ofRandom(5, 10);

//    radius = r;
    // we'll calculate it's mass as it's surface area
    m = 2 * pi * pow(radius, 2);

    // assign it's starting position
    p.x = ofRandom(radius, ofGetWidth() - radius);
    p.y = ofRandom(radius, ofGetHeight() - radius);

    // assign it's velocity
    v.x = ofRandom(-3, 3);
    v.y = ofRandom(-3, 3);

    // assign a random angular velocity
    w.x = ofRandom(-0.5, 0.5);
    w.y = ofRandom(-0.5, 0.5);

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
    if (p.x > maxX || p.x < minX){
        v *= reverseX;
    }

    if (p.y > maxY || p.y < minY){
        v *= reverseY;
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
        if (p.x == particles->at(i).p.x &&
            p.y == particles->at(i).p.y){
            continue;
        }


        Particle * particle2 = &particles->at(i);
        // check to see if this particle is interacting with
        // the other particle
        if (isInteracting(particle2)){
            // Using the following model to update veloctiy and angular velocity:
            // http://www.euclideanspace.com/physics/dynamics/collision/twod/

            // get all the required information:
            // mass
            float m1 = m;
            float m2 = particle2->m;
            // radii
            float radius1 = radius;
            float radius2 = particle2->radius;
            // inertia
            float i1 = (pi/2)*pow(radius1, 4);
            float i2 = (pi/2)*pow(radius2, 4);
            // velocity
            ofVec2f v1 = v;
            ofVec2f v2 = particle2->v;
            // position
            ofVec2f p1 = p;
            ofVec2f p2 = particle2->p;
            // angular momentum
            ofVec2f w1 = w;
            ofVec2f w2 = particle2->w;

            // relative vector of collision point to centre of mass
            ofVec2f r1 = (p1 - p2).scale(radius1 / radius2);
            ofVec2f r2 = (p2 - p1).scale(radius2 / radius1);

            // Impulse
            ofVec2f j;

            // e is the coefficient of restitution. It's fun to vary this.
            float e = 0.9;


            float k=1/(m1*m1)+ 2/(m1*m2) +1/(m2*m2) - r1.x*r1.x/(m1*i1) - r2.x*r2.x/(m1*i2)  - r1.y*r1.y/(m1*i1)
            - r1.y*r1.y/(m2*i1) - r1.x*r1.x/(m2*i1) - r2.x*r2.x/(m2*i2) - r2.y*r2.y/(m1*i2)
            - r2.y*r2.y/(m2*i2) + r1.y*r1.y*r2.x*r2.x/(i1*i2) + r1.x*r1.x*r2.y*r2.y/(i1*i2) - 2*r1.x*r1.y*r2.x*r2.y/(i1*i2);

            // set the impulse in the two dimensions
            j.x = (e+1)/k * (v1.x - v2.x)*( 1/m1 - r1.x*r1.x/i1 + 1/m2 - r2.x*r2.x/i2)
            - (e+1)/k * (v1.y - v2.y)* (r1.x*r1.y / i1 + r2.x*r2.y / i2);
            j.y = - (e+1)/k * (v1.x - v2.x) * (r1.x*r1.y / i1 + r2.x*r2.y / i2)
            + (e+1)/k  * (v1.y - v2.y) * ( 1/m1 - r1.y*r1.y/i1 + 1/m2 - r2.y*r2.y/i2);

            // velocity and angular velocity after the collision
            ofVec2f v1f = v1 - j / m1;
            ofVec2f v2f = v2 + j / m2;
            ofVec2f w1f;
            ofVec2f w2f;
            w1f.x = w1.x - (j.x*r1.y - j.y*r1.x) /i1;
            w1f.y = w1.y - (j.x*r1.y - j.y*r1.x) /i1;
            w2f.x = w2.x - (j.x*r2.y - j.y*r2.x) /i2;
            w2f.y = w2.y - (j.x*r2.y - j.y*r2.x) /i2;

            // update this particles velocities
            v = v1f;
            w = w1f;

            particles->at(i).v = v2f;
            particles->at(i).w = w2f;

            particles->at(i).p += particles->at(i).v;
            updated = true;
            particles->at(i).updated =
        }
    }
    // update the position using the particles velocity
    p += v;
    energy = 0.5 * m * pow(v.length(), 2);
}

// draw the particle (the easy bit!)
void Particle::draw(){
    ofSetColor(colour);
    ofDrawCircle(p.x, p.y, radius);
    // write debug information
    if (*debug){
        ofDrawBitmapString(energy, p.x + radius, p.y + radius);
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
    float dx = p.x - particle->p.x;
    float dy = p.y - particle->p.y;

    float d = sqrt(pow(dx, 2) + pow(dy,2));

    if (d < radius + particle->radius){
        return true;
    }
    return false;
}
