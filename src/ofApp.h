#pragma once

#include "ofMain.h"

// Represents a particle in 2d space
class Particle : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void setDebug(bool* debug);
    void setParticles(vector<Particle>* particles);
    bool isInteracting(Particle * particle);
    void updateParticles(Particle * particle2);

    ofVec2f p; // position (centre of mass)
    ofVec2f v; // velocity
    ofVec2f w; // angular velocity
    float m; // mass
    ofColor colour;
    float radius;

    bool updated; // has this particle been updated
    float energy; // useful for ensuring the physics checks out
    bool * debug; // pointer to apps debug flag

    vector<Particle>* particles; // pointer to other particles
};

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    bool debug; // used for displaying debug information
    float energy; // useful to ensuring the physics checks out
    vector<Particle> particles; // map of all the particles
};

