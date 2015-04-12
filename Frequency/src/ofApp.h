#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp
{
	public:
	
		// --------------------------------
		void draw()
		{
			ofBackgroundGradient( ofColor(55), ofColor(0), OF_GRADIENT_CIRCULAR );
			ofEnableAlphaBlending();
			
			float mx = ofNormalize( ofGetMouseX(), 0, ofGetWidth() );
			float my = ofNormalize( ofGetMouseY(), 0, ofGetHeight() );
			
			float frequency = mx * 0.02;
			float time = 0; //ofGetElapsedTimef() * (my * 2.0);
			
			float noiseMagnitude = 300;
			int res = ofGetWidth();
			
			ofMesh mesh;
			mesh.setMode( OF_PRIMITIVE_LINE_STRIP );
			for( int i = 0; i < res; i++ )
			{
				ofVec2f pos = ofVec2f( ofMap( i, 0, res, 0, ofGetWidth() ), ofGetHeight() * 0.5f );
				
				float noisePos = (pos.x * frequency);
				float noiseValue = ofSignedNoise( noisePos, time );
				
				ofVec2f drawPos = pos + ofVec2f(0,noiseValue * noiseMagnitude);
				
				mesh.addVertex( drawPos );
			}

			ofSetColor( ofColor::white );
			mesh.draw();
		}
};
