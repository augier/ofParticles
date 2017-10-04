#pragma once

#include "ofMain.h"

class Particle : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void setDebug(bool * debug);
    void setParticles(vector<Particle> * particles);
    bool isInteracting(Particle * particle);

    ofVec2f pos;
    ofVec2f velocity;
    ofColor colour;
    float radius;
    float mass;
    bool updated;
    float energy;
    bool * debug;

    vector<Particle> * particles;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    bool debug; // used for displaying debug information
    float energy;
    vector<Particle> particles;
};

