#include "ofApp.h"

//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	
	// Give us a starting point for the camera
	camera.setNearClip(0.01f);
	camera.setPosition( 0, 4, 10 );
	camera.setMovementMaxSpeed( 0.1f );
	
	int textureSize = 128;
	int numParticles = textureSize * textureSize;
	
	// Allocate buffers
	ofFbo::Settings fboSettings;
	fboSettings.width  = textureSize;
	fboSettings.height = textureSize;
	
	// We can create several color buffers for one FBO if we want to store velocity for instance,
	// then write to them simultaneously from a shader using gl_FragData[0], gl_FragData[1], etc.
	fboSettings.numColorbuffers = 1;
	
	fboSettings.useDepth = false;
	fboSettings.internalformat = GL_RGBA32F;			// Gotta store the data as floats, they won't be clamped to 0..1
	fboSettings.textureTarget  = GL_TEXTURE_2D;
	fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSettings.wrapModeVertical   = GL_CLAMP_TO_EDGE;
	fboSettings.minFilter = GL_NEAREST;					// No interpolation, that would mess up data reads later!
	fboSettings.maxFilter = GL_NEAREST;
	
	ofDisableTextureEdgeHack();
		particleData.allocate( fboSettings );
	ofEnableTextureEdgeHack();
	
	particleMaxAge = 4.0;
	
	ofVec4f* startPosAndAge = new ofVec4f[numParticles];
	
	mesh.setMode( OF_PRIMITIVE_POINTS );
	
	int tmpIndex = 0;
	for( int y = 0; y < textureSize; y++ )
	{
		for( int x = 0; x < textureSize; x++ )
		{
			ofVec3f pos = MathUtils::randomPointOnSphere();// + ofVec3f(0,0.5,0);
			float startAge = ofRandom( particleMaxAge );
			
			startPosAndAge[tmpIndex] = ofVec4f( pos.x, pos.y, pos.z, startAge );
			
			ofVec2f texCoord;
			texCoord.x = ofMap( x + 0.5f,	0, textureSize,	0.0f, 1.0f );
			texCoord.y = ofMap( y + 0.5f,	0, textureSize,	0.0f, 1.0f );
			
			mesh.addVertex( ofVec3f(x,y,0) );
			mesh.addTexCoord( texCoord );
			tmpIndex++;
		}
	}
		
	// Upload it to the source texture
	particleData.source()->getTextureReference(0).loadData( (float*)&startPosAndAge[0].x,	 textureSize, textureSize, GL_RGBA );
	
	updateShader.load("Shaders/Particles/GL2/Update");
	drawShader.load("Shaders/Particles/GL2/DrawPixel");
}

//-----------------------------------------------------------------------------------------
//
void ofApp::update()
{
}

//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);	

	// Update particles ----------
	
	ofEnableBlendMode( OF_BLENDMODE_DISABLED ); // Important! We just want to write the data as is to the target fbo
	particleData.dest()->begin();
	
		updateShader.begin();

			updateShader.setUniformTexture("u_positionAndAgeTex", particleData.source()->getTextureReference(0), 0 );

			updateShader.setUniform1f("u_time",  ofGetElapsedTimef() );
			updateShader.setUniform1f("u_timeStep", 1.0 / 60.0 );
	
			updateShader.setUniform1f("u_particleMaxAge", particleMaxAge );
	
			particleData.source()->draw(0,0);
		
		updateShader.end();
	
	particleData.dest()->end();

	particleData.swap();
	// --------------------------
	
	
	ofEnableDepthTest();
	camera.begin();
	
		// draw a grid on the floor
		ofSetColor( ofColor(60) );
		ofPushMatrix();
			ofRotate(90, 0, 0, -1);
			ofDrawGridPlane( 10, 10, false );
		ofPopMatrix();

	ofSetColor( ofColor::white );
		drawShader.begin();
			drawShader.setUniformTexture("u_positionAndAgeTex", particleData.source()->getTextureReference(0), 0 );
			mesh.draw();
		drawShader.end();
	
	camera.end();

	ofSetColor( ofColor::white );
	ofDisableDepthTest();


	fontSmall.drawStringShadowed(ofToString(ofGetFrameRate(),2), ofGetWidth()-35, ofGetHeight() - 6, ofColor::whiteSmoke, ofColor::black );
}

//-----------------------------------------------------------------------------------------
//
void ofApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}
