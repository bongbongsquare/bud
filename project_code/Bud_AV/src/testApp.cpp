#include "testApp.h"
#define NUM_IMAGES 5

ofMesh mesh;
ofImage image;

ofEasyCam easyCam;
vector<ofVec3f> offsets;
ofImage numFolders;

float imageX[NUM_IMAGES];
float imageY[NUM_IMAGES];
float imageZ[NUM_IMAGES];
float imageResize[NUM_IMAGES];

maxiOsc mySine,myOtherSine,myPhasor;
maxiOsc sineBank[10];

//--------------------------------------------------------------
void testApp::setup(){
    
    
    
    ofSetWindowTitle("flower");
    ofSetFrameRate(60);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    //    ofEnableDepthTest();
    
    tracktime=0;
    
    //sound
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
    ofBackground(0);
    
    int bufferSize		= 512;
	sampleRate 			= 44100;
    
    soundStream.listDevices();
    
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(100, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
	soundStream.setup(this, 2, 2, sampleRate, bufferSize, 4);
    
    

    //mesh
    
    image.loadImage("22-2.png");
    image.resize(length, length/2);
    
    float intensityThreshold = 50.0;
    int w = image.getWidth();
    int h = image.getHeight();
    for (int x=0; x<w; x+=4) {
        for (int y=0; y<h; y+=4) {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if (intensity >= intensityThreshold) {
                float saturation = c.getLightness();
                float z = ofMap(saturation, 0, 255, -200, 200);;
                ofVec3f pos(4*x, 4*y, 4*z);
                mesh.addVertex(pos);
                mesh.addColor(c);
                offsets.push_back(ofVec3f(ofRandom(0,1000), ofRandom(0,1000), ofRandom(0,1000)));
            }
        }
    }
    
    
    float connectionDistance = 50;
    int numVerts = mesh.getNumVertices();
    for (int a=0; a<numVerts; a++) {
        ofVec3f verta = mesh.getVertex(a);
        for (int b=a+1; b<numVerts; b++) {
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance) {
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    cout << mesh.getNumVertices() << endl;  // It should be ~64,000

    
}

//--------------------------------------------------------------
void testApp::update(){
    
    if(ofSetFullscreen){
        ofHideCursor();
    }else{
        ofShowCursor();
    }
    
	scaledVol = ofMap(smoothedVol, 2.0, 0.17, 0.0, 0.5, true);
	volHistory.push_back( scaledVol );
	if( volHistory.size() >= 100){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    
    //mesh
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        
        float time = ofGetElapsedTimef();
        timeScale = 5.;
        displacementScale = 1;
        ofVec3f timeOffsets = offsets[i];
        
        vert.x += mix*(ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        vert.y += mix*(ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        vert.z += mix*(ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        mesh.setVertex(i, vert);
    }
    

    
    
    cout << ofGetWindowWidth();
    cout << endl;
    cout << ofGetWindowHeight();
    cout << endl;
    
}
//--------------------------------------------------------------
void testApp::draw(){
    
    ofColor centerColor = ofColor(0, 0, 50);
    ofColor edgeColor(0);
    ofBackgroundGradient(centerColor,edgeColor,  OF_GRADIENT_CIRCULAR);
    
    easyCam.begin();
    ofPushMatrix();
    
    elements=5;
    float spacing = FOUR_PI/elements;
    
    
    //1.1

    if (tracktime< 60 ) {
        
        ofTranslate(0-ofGetWindowWidth()/4, 0, -2500);
        
        float spinX = sin(ofGetElapsedTimef()*.35f*voice);
        float spinY = cos(ofGetElapsedTimef()*.75f);
        float spinZ = cos(ofGetElapsedTimef()*.35f);
        ofRotate(spinX, 1.0, 0.0, 0.0);
        ofRotate(spinY, 0.0, 0.5, 0.0);
//      ofRotate(spinZ, 0.0, 0.0, 1.0);
        
        for(int i=0; i<10; i++){
            imageX[i] = (-ofGetWindowWidth()/4, ofGetWindowWidth()/4);
            imageY[i] = (-ofGetWindowHeight()/2, ofGetWindowHeight()/2);
//          imageZ[i] = ofRandom(-ofGetWindowHeight()/4, ofGetWindowHeight()/4);
            ofTranslate(imageX[i], imageY[i]);
            
            for (int i=0;i<elements;i++) {
                ofRotate(elements*20*sin(4)*(voice*2),0,0, 0.6);
                currentCount=(int)timer.phasor(1);
                mesh.draw();
            }
        }

    }
    
    
    //1.2
    
    if (tracktime> 60 && tracktime< 80 ) {
        
        ofTranslate(0-ofGetWindowWidth()/4, 0, -3000);
        
        float spinX = sin(ofGetElapsedTimef()*.35f);
        float spinY = cos(ofGetElapsedTimef()*.75f);
        float spinZ = cos(ofGetElapsedTimef()*.35f);
//        ofRotate(spinX, 1.0, 0.0, 0.0);
//        ofRotate(spinY, 0.0, 0.5, 0.0);
//        ofRotate(spinZ, 0.0, 0.0, 1.0);
        
        for(int i=0; i<10; i++){
            imageX[i] = (-ofGetWindowWidth()/4, ofGetWindowWidth()/4);
            imageY[i] = (-ofGetWindowHeight()/5, ofGetWindowHeight()/5);
//            imageZ[i] = ofRandom(-ofGetWindowHeight()/4, ofGetWindowHeight()/4);
            ofTranslate(imageX[i], imageY[i]);
            
            for (int i=0;i<elements;i++) {
                ofRotate(elements*20*sin(4)*voice,spinX/2,spinY/4, 0.6);
                currentCount=(int)timer.phasor(1);
                mesh.draw();
            }
        }
    }
    
    
    
    //2
    
    else if (tracktime < 140 || tracktime > 260 ) {
        
        ofTranslate(0-ofGetWindowWidth()/4, 0, -1300);
        
        float spinX = sin(ofGetElapsedTimef()*.100f);
        float spinY = cos(ofGetElapsedTimef()*.075f);
        float spinZ = sin(ofGetElapsedTimef()*.35f);
        ofRotate(spinX, 1.0, 0.0, 0.0);
        ofRotate(spinY, 0.0, 1.0, 0.0);
        ofRotate(spinZ, 0.0, 0.0, 1.0);
        
        for(int i=0; i<5; i++)
        {
 //           ofTranslate(imageX[i], imageY[i], imageZ[i]);
            for (int i=0;i<elements;i++) {
            ofRotate(elements*20*sin(4),spinX,spinY, spinZ);
            mesh.draw();
            }
        }
    }
    
    
    //3
    
    else if (tracktime < 170 || tracktime > 210 ) {
        
        ofTranslate(0, 0, -1500);
        
        float spinX = sin(ofGetElapsedTimef()*.35f);
        float spinY = cos(ofGetElapsedTimef()*.075f);
        float spinZ = sin(ofGetElapsedTimef()*.010f);
        ofRotate(spinX, 1.0, 0.0, 0.0);
        ofRotate(spinY, 0.0, 1.0, 0.0);
//        ofRotate(spinZ, 0.0, 0.0, 1.0);
        
        for(int i=0; i<10; i++)
        {
//            ofTranslate(imageX[i], imageY[i], imageZ[i]);
            
            for (int i=0;i<elements;i++) {
                ofRotate(elements*20*sin(4),0,0, -spinZ*(mix/2));
                mesh.draw();
            }
        }
    }
    
    
     //4
    
    else {
        
        ofTranslate(0, 0, -1500);
        
        float spinX = sin(ofGetElapsedTimef()*.35f);
        float spinY = cos(ofGetElapsedTimef()*.015f*mix);
        float spinZ = sin(ofGetElapsedTimef()*.35f);
        ofRotate(spinX, 1.0, 0.0, 0.0);
        //ofRotate(spinY, 0.0, 1.0, 0.0);
        //ofRotate(spinZ, 0.0, 0.0, 1.0);
        
        for(int i=0; i<7; i++)
        {
            for (int i=0;i<elements;i++) {
                ofRotate(elements*20*sin(4),spinX,0, 1.0);
                mesh.draw();
            }
        }
    }
    
    
    if (tracktime > 350 ) {
        tracktime = 0;
    }
    
    ofBeginShape();
    ofEndShape(false);
    ofPopMatrix();
    easyCam.end();
}
//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    

    
    for (int i = 0; i < bufferSize; i++){
        
        
        mix=0.;
        
        
        currentCount=(int)timer.phasor(4.);//ticks 8 times a second
        
        
        
        if (lastCount!=currentCount) {
            
            if (voice==8) {
                voice=0;
            }
            
            ADSR[voice].trigger(0, adsrEnv[0]);
            pitch[voice]=voice;
            voice++;
            
            lastCount=0;
            tracktime++;
        }
        
        
        //1
        
        if ( tracktime< 80 ) {
            
            for (int i=0; i<6; i++) {
                
                double wave=0;
                double f0 = 5;
                for(int i=0; i < 10; i++) {
                    double thisSine = wave + sineBank[i].sinewave(f0 + (i * f0));
                    double multiplier = 0.1/ (i+0.7);
                    thisSine = thisSine * multiplier;
                    wave = wave + thisSine;
                }
                wave *= 0.5;
                
                ADSRout[i]=ADSR[i].line(2,adsrEnv);//8 value/time.
                LFO1out[i]=LFO1[i].sinebuf(20);//0.2 hz
                VCO1out[i]=VCO1[i].pulse(10*pitch[i],0.05);
                VCO2out[i]=VCO2[i].pulse((20*pitch[i])+LFO1out[i],0.05);
                VCFout[i]=VCF[i].lores((VCO1out[i]+VCO2out[i])*.2, 250+((pitch[i]+LFO1out[i])*10), 1.);
                mix+=VCFout[i]*ADSRout[i]*6*wave;
            }
            
        }
        
        
        //2
        
        else if (tracktime < 140 || tracktime > 260 ) {
            
            for (int i=0; i<3; i++) {
                
                double wave=0;
                double f0 = 10;
                for(int i=0; i < 10; i++) {
                    double thisSine = wave + sineBank[i].sinewave(f0 + (i * f0));
                    double multiplier = 0.5 / (i+1.0);
                    thisSine = thisSine * multiplier;
                    wave = wave + thisSine;
                }
                wave *= 0.3;
                
                ADSRout[i]=ADSR[i].line(8,adsrEnv);
                LFO1out[i]=LFO1[i].sinebuf(50);
                VCO1out[i]=VCO1[i].pulse(5*pitch[i],0.02);
                VCO2out[i]=VCO2[i].pulse((50*pitch[i])+LFO1out[i],0.2);
                
                
                VCFout[i]=VCF[i].lores((VCO1out[i]+VCO2out[i])*.5, 250+((pitch[i]+LFO1out[i])*100), 0.02);
                mix+=wave * VCFout[i]*ADSRout[i];
            }
        }
        
        
        //3
        
        else if (tracktime < 170 || tracktime > 210 ) {
            
            double wave=0;
            double f0 = 100;
            for(int i=0; i < 10; i++) {
                double thisSine = wave + sineBank[i].sinewave(f0 + (i * f0));
                double multiplier = 0.1/ (i+0.1);
                thisSine = thisSine * multiplier;
                wave = wave + thisSine;
            }
            wave *= 0.2;
            
            for (int i=0; i<3; i++) {
                
                ADSRout[i]=ADSR[i].line(2,adsrEnv);//8 value/time.
                LFO1out[i]=LFO1[i].sinebuf(20);//0.2 hz
                VCO1out[i]=VCO1[i].pulse(10*pitch[i],0.5);
                VCO2out[i]=VCO2[i].pulse((20*pitch[i])+LFO1out[i],0.5);
                VCFout[i]=VCF[i].lores((VCO1out[i]+VCO2out[i])*.2, 250+((pitch[i]+LFO1out[i])*10), 5.);
                mix+=wave * VCFout[i]*ADSRout[i];
            }
        }
        
        
        
        //4
        
        else {
            for (int i=0; i<3; i++) {
                
                ADSRout[i]=ADSR[i].line(2,adsrEnv);//8 value/time.
                LFO1out[i]=LFO1[i].sinebuf(20);//0.2 hz
                VCO1out[i]=VCO1[i].pulse(10*pitch[i],0.5);
                VCO2out[i]=VCO2[i].pulse((20*pitch[i])+LFO1out[i],0.5);
                VCFout[i]=VCF[i].lores((VCO1out[i]+VCO2out[i])*.2, 250+((pitch[i]+LFO1out[i])*10), 5.);
                mix+=VCFout[i]*ADSRout[i];
                
            }
            
        }
        
        if (tracktime > 350 ) {
            tracktime = 0;
        }
        
        output[i*nChannels    ] = mix*0.7;
        output[i*nChannels + 1] = mix*0.7;
        
    }
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){

	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key == 'f'){
		bFullscreen = !bFullscreen;
        
		if(!bFullscreen){
			ofSetWindowShape(300,300);
			ofSetFullscreen(false);
			int screenW = ofGetScreenWidth();
			int screenH = ofGetScreenHeight();
			ofSetWindowPosition(screenW/2-700, screenH/2-600);
		} else if(bFullscreen == 1){
			ofSetFullscreen(true);
		}
	}
    
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

