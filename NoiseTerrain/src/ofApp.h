#pragma once

#include "ofMain.h"
#include "Math/MathUtils.h"

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
			
			int res = 100;
			ofVec3f size(5, 1, 5);
			
			ofMesh mesh;
			mesh.setMode( OF_PRIMITIVE_TRIANGLES );
			
			for( int y = 0; y < res; y++ )
			{
				for( int x = 0; x < res; x++ )
				{
					ofVec3f p = getTerrain( ofMap( x, 0, res, size.x * -0.5f, size.x * 0.5f ),
										    ofMap( y, 0, res, size.z * -0.5f, size.z * 0.5f ),
										    frequency, size.y );
					
					ofVec3f pRight = getTerrain( ofMap( x + 1, 0, res, size.x * -0.5f, size.x * 0.5f ),
												 ofMap( y, 0, res, size.z * -0.5f, size.z * 0.5f ),
												 frequency, size.y );
					
					ofVec3f pBot = getTerrain(  ofMap( x, 0, res, size.x * -0.5f, size.x * 0.5f ),
												ofMap( y + 1, 0, res, size.z * -0.5f, size.z * 0.5f ),
												frequency, size.y );
					
					//ofVec3f normal = (pRight-p).getCrossed( (pBot-p) ).getNormalized();
					ofVec3f normal = (pBot-p).getCrossed( (pRight-p) ).getNormalized();
					
					mesh.addVertex( p );
					mesh.addNormal( normal );

					//mesh.addVertex( p );
					//mesh.addVertex( p + (normal * 1));
					
				}
			}
			
			
			for( int y = 0; y < res-1; y++ )
			{
				for( int x = 0; x < res-1; x++ )
				{
					int tmp = (y * res) + x;
					
					//mesh.addIndex( tmp ); mesh.addIndex( tmp + 1 );
					//mesh.addIndex( tmp ); mesh.addIndex( tmp + res );
					
					mesh.addTriangle( tmp, tmp + 1, tmp + 1 + res );
					mesh.addTriangle( tmp, tmp + 1 + res, tmp + res );
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
			
				ofLight light;
				light.setPosition( ofVec3f(1,4,0) );
				light.enable();
			
				ofEnableLighting();
					ofSetColor( ofColor::white );
					mesh.draw();
				ofDisableLighting();
			
				ofSphere( light.getPosition(), 0.1 );
			
			camera.end();
			
			ofDisableDepthTest();
		}

		// --------------------------------
		ofVec3f getTerrain( float _x, float _y, float _frequency, float _magnitude )
		{
			ofVec3f p( _x, 0, _y );
			//p.y = ofNoise( p.x * _frequency, p.z * _frequency ) * _magnitude;
			p.y = MathUtils::fbm( ofVec2f(p.x * _frequency, p.z * _frequency), 4, 2.0, 0.5 ) * _magnitude;
			return p;
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
