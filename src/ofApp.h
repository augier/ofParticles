#pragma once

#include "ofMain.h"

class Particle : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void setParticles(vector<Particle> * particles);
    bool isInteracting(Particle * particle);

    ofVec2f pos;
    ofVec2f velocity;
    ofColor colour;
    float radius;

    vector<Particle> * particles;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


        vector<Particle> particles;
};

