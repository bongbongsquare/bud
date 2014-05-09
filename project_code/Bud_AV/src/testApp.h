#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioOut(float * input, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    // add your maxim objects here
    ofSoundStream soundStream;
    int		sampleRate;
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofImage grab;
    
    
    //These are the synthesiser bits
    maxiOsc VCO1[6],VCO2[6],LFO1[6],LFO2[6];
    maxiFilter VCF[6];
    maxiEnvelope ADSR[6];
    
    //These are the control values for the envelope
    
    double adsrEnv[8]={1,100,0.125,100,0.125,200,0,1000};
    
    //This is a bunch of control signals so that we can hear something
    
    maxiOsc timer;//this is the metronome
    int currentCount,lastCount,voice=0;//these values are used to check if we have a new beat this sample
    
    //and these are some variables we can use to pass stuff around
    
    double VCO1out[6],VCO2out[6],LFO1out[6],LFO2out[6],VCFout[6],ADSRout[6],mix,pitch[6];
    
    
    ofSoundPlayer  birds;
    float 		   * fftSmoothed;
    
    
    
    int myWidth,myHeight;
    
    bool bFullscreen;
    bool m_mousePressed;
    
    vector <ofImage> images;
    ofImage keyed;
    
    ofEasyCam cam;
    ofVec3f cameraRotation;
    float zoom, zoomTarget;
    
    ofDirectory dir;
    int currentImage;
    int elements;
    int length;
    
    float timeScale;
    float displacementScale;
    
    int tracktime;
    
    
};
