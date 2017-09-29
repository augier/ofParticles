#pragma once

#include "ofMain.h"

class Particle : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofVec2f pos;
    ofVec2f velocity;
    ofColor colour;
    float radius;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        vector<Particle> particles;
};

