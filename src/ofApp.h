#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxControlPanel.h"

#include "ofxOsc.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void setupGui();
	void update();
	void draw();
	void drawOutput(ofEventArgs &args);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void triggerCh1(bool &aint);
	void triggerCh2(bool &aint);
	void triggerCh3(bool &aint);

	ofxControlPanel gui;

	// ch1
	ofParameter<bool> ch1Load;
	ofParameter<bool> ch1Show;
	ofParameter<bool> ch1Preview;
	ofParameter<bool> ch1FadeButton;
	float ch1Fade;
	ofVideoPlayer ch1VideoPlayer;
	ofFbo ch1PreviewFbo;
	ofParameter<int> ch1BlendMode;

	// ch2
	ofParameter<bool> ch2Load;
	ofParameter<bool> ch2Show;
	ofParameter<bool> ch2Preview;
	ofParameter<bool> ch2FadeButton;
	float ch2Fade;
	ofVideoPlayer ch2VideoPlayer;
	ofFbo ch2PreviewFbo;
	ofParameter<int> ch2BlendMode;

	// ch3
	ofParameter<bool> ch3Load;
	ofParameter<bool> ch3Show;
	ofParameter<bool> ch3Preview;
	ofParameter<bool> ch3FadeButton;
	float ch3Fade;
	ofVideoPlayer ch3VideoPlayer;
	ofFbo ch3PreviewFbo;
	ofParameter<int> ch3BlendMode;

	// output
	ofParameter<float> fadeSpeed;
	ofParameter<bool> outputPreview;
	ofFbo outputPreviewFbo;
	ofFbo outputFbo;

	// osc
	ofParameter<string> oscPortLabel;
	ofxOscReceiver receiver;
	ofParameter<string> bpmLabel;

	// bpm
	float bpmVal;

	uint64_t nextBeatMSVal;

	int previewWidth = 200;
	int previewHeight = 150;
};
