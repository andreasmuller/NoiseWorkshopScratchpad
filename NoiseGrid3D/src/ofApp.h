#pragma once

#include "ofMain.h"
#include "math/MathUtils.h"

class ofApp : public ofBaseApp
{
	public:
	
		// --------------------------------
		void setup()
		{
			fontSmall.loadFont("Fonts/DIN.otf", 8 );
			
			camera.setAutoDistance( false );
			camera.setPosition( ofVec3f(0,1,4) );
			camera.setNearClip( 0.001 );
		}
	
		// --------------------------------
		void draw()
		{
			ofBackgroundGradient( ofColor(55), ofColor(0), OF_GRADIENT_CIRCULAR );
		
			float t = ofGetElapsedTimef();
			
			float mx = ofNormalize( ofGetMouseX(), 0, ofGetWidth() );
			float my = ofNormalize( ofGetMouseY(), 0, ofGetHeight() );
			
			float frequency = ofMap( mx, 0, 1, 0.1, 1.0 );
			float timeFrequency = ofMap( my, 0, 1, 0.1, 2.0 );
			
			float tn = t * timeFrequency;
			
			int res = 20;
			ofVec3f size(3, 3, 3);
			
			ofMesh mesh;
			mesh.setMode( OF_PRIMITIVE_LINES );
			
			for( int y = 0; y < res; y++ )
			{
				for( int x = 0; x < res; x++ )
				{
					for( int z = 0; z < res; z++ )
					{
						ofVec3f p;
						p.x = ofMap( x, 0, res, size.x * -0.5, size.x * 0.5 );
						p.y = ofMap( y, 0, res, size.y *  0.0, size.y * 1.0 );
						p.z = ofMap( z, 0, res, size.z * -0.5, size.z * 0.5 );
						
						ofVec3f noisePos = p * frequency;
						
						ofVec3f noise;
						noise.x = ofSignedNoise( noisePos.x, noisePos.y, noisePos.z,	tn );
						noise.y = ofSignedNoise( noisePos.y, noisePos.x, tn,			noisePos.z );
						noise.z = ofSignedNoise( noisePos.z, tn,		 noisePos.x,	noisePos.y );
						noise.normalize();
						noise *= 0.2;
					
						mesh.addVertex( p );
						mesh.addVertex( p + noise );
						
						// Color by direction
						ofVec3f velAsCol = (noise.getNormalized() + ofVec3f(1)) * 0.5;
						ofFloatColor tmpCol( velAsCol.x, velAsCol.y, velAsCol.z );
						mesh.addColor( tmpCol );
						mesh.addColor( tmpCol );
					}
				}
			}
			

			
			ofEnableDepthTest();
			
			camera.begin();
			
				// Grid
				ofSetColor( ofColor(60) );
				ofPushMatrix();
					ofRotate(90, 0, 0, -1);
					ofDrawGridPlane( size.x, (int)size.x, false );
				ofPopMatrix();
			
				ofSetColor( ofColor::white );
				mesh.draw();
		
			camera.end();
			
			ofDisableDepthTest();
		}
	
		// --------------------------------
		void keyPressed( int _key )
		{
			if( _key == ' ' )
			{
			}
		}
	
		ofEasyCam camera;
	
		ofTrueTypeFont fontSmall;
};
