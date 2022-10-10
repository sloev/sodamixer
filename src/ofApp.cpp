#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);

    ofSetBackgroundAuto(false);

    ofSetVerticalSync(true);
    bpmVal = 60;
    int now = ofGetSystemTimeMillis();
    nextBeatMSVal = now + (60 * 1000 / bpmVal);
}

uint64_t timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void ofApp::setupGui()
{

    gui.setup("sodamixer", 0, 0);

    vector<string> blendModes;

    blendModes.push_back("disabled");
    blendModes.push_back("add");
    blendModes.push_back("subtract");
    blendModes.push_back("multiply");
    blendModes.push_back("screen");

    gui.setWhichPanel(0);

    //------ channel 1
    gui.setWhichColumn(0);
    gui.addLabel("channel 1 (most back)");
    gui.add(ch1Load.set("load 1", false));
    gui.add(ch1Show.set("show 1", true));
    gui.add(ch1FadeButton.set("trigger 1", false));
    gui.add(ch1Preview.set("preview 1", false));
    ch1PreviewFbo.allocate(previewWidth, previewHeight, GL_RGB);
    gui.addDrawableRect("ch1 preview", &ch1PreviewFbo, previewWidth, previewHeight);
    gui.addTextDropDown(ch1BlendMode.set("blendmode 1", 0), blendModes);
    ch1FadeButton.addListener(this, &ofApp::triggerCh1);

    //------ channel 2
    gui.setWhichColumn(1);
    gui.addLabel("channel 2");
    gui.add(ch2Load.set("load 2", false));
    gui.add(ch2Show.set("show 2", true));
    gui.add(ch2FadeButton.set("trigger 2", false));
    gui.add(ch2Preview.set("preview 2", false));
    ch2PreviewFbo.allocate(previewWidth, previewHeight, GL_RGB);
    gui.addDrawableRect("preview 2", &ch2PreviewFbo, previewWidth, previewHeight);
    gui.addTextDropDown(ch2BlendMode.set("blendmode 2", 0), blendModes);
    ch2FadeButton.addListener(this, &ofApp::triggerCh2);

    //------ channel 3
    gui.setWhichColumn(2);
    gui.addLabel("channel 3");
    gui.add(ch3Load.set("load 3", false));
    gui.add(ch3Show.set("show 3", true));
    gui.add(ch3FadeButton.set("trigger 3", false));
    gui.add(ch3Preview.set("preview 3", false));
    ch3PreviewFbo.allocate(previewWidth, previewHeight, GL_RGB);
    gui.addDrawableRect("preview 3", &ch3PreviewFbo, previewWidth, previewHeight);
    gui.addTextDropDown(ch3BlendMode.set("blendmode 3", 0), blendModes);
    ch3FadeButton.addListener(this, &ofApp::triggerCh3);

    //------ output
    gui.setWhichColumn(3);
    gui.addLabel("output");
    gui.add(fadeSpeed.set("fade speed", 1, 0.1, 20.0));

    gui.add(outputPreview.set("preview o", true));
    outputPreviewFbo.allocate(previewWidth, previewHeight, GL_RGB);
    outputFbo.allocate(1024, 768, GL_RGBA);
    gui.addDrawableRect("output preview", &outputPreviewFbo, previewWidth, previewHeight);

    gui.addLabel(bpmLabel.set("bpm:", "60.0"));

    gui.addLabel(oscPortLabel.set("osc_recv_port:", "12345"));

    receiver.setup(12345);
}

void ofApp::triggerCh1(bool &aint)
{
    ch1FadeButton = false;
    ch1Fade = 255.0;
}

void ofApp::triggerCh2(bool &aint)
{
    ch2FadeButton = false;
    ch2Fade = 255.0;
}

void ofApp::triggerCh3(bool &aint)
{
    ch3FadeButton = false;
    ch3Fade = 255.0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    uint64_t now = timeSinceEpochMillisec();

    if (ch1Fade > 0)
    {
        ch1Fade -= fadeSpeed;
    }
    if (ch2Fade > 0)
    {
        ch2Fade -= fadeSpeed;
    }
    if (ch3Fade > 0)
    {
        ch3Fade -= fadeSpeed;
    }
    if (ch1VideoPlayer.isPlaying())
    {
        ch1VideoPlayer.update();
        if (now >= nextBeatMSVal)
        {
            float position = ch2VideoPlayer.getPosition() * ch2VideoPlayer.getDuration();

            float nextposition = floor(position + 1.0);
            float skewVal = 1.0;
            if (nextposition > position)
            {
                skewVal = 1.0 + (nextposition - position) / 5.0;
            }
            if (nextposition < position)
            {
                skewVal = 1.0 + (position - nextposition) / 5.0;
            }

            ch2VideoPlayer.setSpeed((bpmVal / 60.0) * skewVal);
        }
    }
    if (ch2VideoPlayer.isPlaying())
    {

        ch2VideoPlayer.update();
        if (now >= nextBeatMSVal)
        {
            float position = ch2VideoPlayer.getPosition() * ch2VideoPlayer.getDuration();

            float nextposition = floor(position + 1.0);
            float skewVal = 1.0;
            if (nextposition > position)
            {
                skewVal = 1.0 + (nextposition - position) / 5.0;
            }
            if (nextposition < position)
            {
                skewVal = 1.0 + (position - nextposition) / 5.0;
            }

            ch1VideoPlayer.setSpeed((bpmVal / 60.0) * skewVal);
        }
    }

    if (ch3VideoPlayer.isPlaying())
    {

        ch3VideoPlayer.update();
        if (now >= nextBeatMSVal)
        {
            float position = ch3VideoPlayer.getPosition() * ch3VideoPlayer.getDuration();

            float nextposition = floor(position + 1.0);
            float skewVal = 1.0;
            if (nextposition > position)
            {
                skewVal = 1.0 + (nextposition - position) / 5.0;
            }
            if (nextposition < position)
            {
                skewVal = 1.0 + (position - nextposition) / 5.0;
            }

            ch3VideoPlayer.setSpeed((bpmVal / 60.0) * skewVal);
        }
    }

    outputFbo.begin();
    ofEnableAlphaBlending();
    ofDisableBlendMode();

    if (ch1BlendMode == 1)
    {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    if (ch1BlendMode == 2)
    {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    }
    if (ch1BlendMode == 3)
    {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    }
    if (ch1BlendMode == 4)
    {
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    }
    if ((ch1Show || ch1Fade > 0) && ch1VideoPlayer.isPlaying())
    {
        if (!ch1Show)
        {
            ofSetColor(255, 255, 255, ch1Fade); // notice the 4th arg (alpha) - halfway between 0 and 255
        }
        ch1VideoPlayer.draw(0, 0, outputFbo.getWidth(), outputFbo.getWidth());
        ofSetColor(255, 255, 255, 255);
    }
    ofEnableAlphaBlending();
    ofDisableBlendMode();
    if (ch2BlendMode == 1)
    {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    if (ch2BlendMode == 2)
    {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    }
    if (ch2BlendMode == 3)
    {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    }
    if (ch2BlendMode == 4)
    {
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    }
    if ((ch2Show || ch2Fade > 0) && ch2VideoPlayer.isPlaying())
    {
        if (!ch2Show)
        {
            ofSetColor(255, 255, 255, ch2Fade); // notice the 4th arg (alpha) - halfway between 0 and 255
        }
        ch2VideoPlayer.draw(0, 0, outputFbo.getWidth(), outputFbo.getWidth());
        ofSetColor(255, 255, 255, 255);
    }

    ofEnableAlphaBlending();
    ofDisableBlendMode();
    if (ch3BlendMode == 1)
    {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    if (ch3BlendMode == 2)
    {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    }
    if (ch3BlendMode == 3)
    {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    }
    if (ch3BlendMode == 4)
    {
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    }
    if ((ch3Show || ch3Fade > 0) && ch3VideoPlayer.isPlaying())
    {
        if (!ch3Show)
        {
            ofSetColor(255, 255, 255, ch3Fade); // notice the 4th arg (alpha) - halfway between 0 and 255
        }
        ch3VideoPlayer.draw(0, 0, outputFbo.getWidth(), outputFbo.getWidth());
        ofSetColor(255, 255, 255, 255);
    }
    outputFbo.end();

    // channel 1
    if (ch1Preview && ch1VideoPlayer.isPlaying())
    {
        ch1PreviewFbo.begin();
        ch1VideoPlayer.draw(0, 0, previewWidth, previewHeight);
        ch1PreviewFbo.end();
    }
    // channel 2
    if (ch2Preview && ch2VideoPlayer.isPlaying())
    {
        ch2PreviewFbo.begin();
        ch2VideoPlayer.draw(0, 0, previewWidth, previewHeight);
        ch2PreviewFbo.end();
    }

    // channel 3
    if (ch3Preview && ch3VideoPlayer.isPlaying())
    {
        ch3PreviewFbo.begin();
        ch3VideoPlayer.draw(0, 0, previewWidth, previewHeight);
        ch3PreviewFbo.end();
    }

    // output
    if (outputPreview)
    {
        outputPreviewFbo.begin();
        outputFbo.draw(0, 0, previewWidth, previewHeight);
        outputPreviewFbo.end();
    }
    while (receiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for mouse moved message
        if (m.getAddress() == "/sodamixer/ch1/trigger")
        {
            bool foo = false;
            triggerCh1(foo);
        }
        if (m.getAddress() == "/sodamixer/ch2/trigger")
        {
            bool foo = false;
            triggerCh2(foo);
        }
        if (m.getAddress() == "/sodamixer/ch3/trigger")
        {
            bool foo = false;
            triggerCh3(foo);
        }

        if (m.getAddress() == "/sodamixer/ch1/toggle")
        {
            ch1Show = !ch1Show;
        }
        if (m.getAddress() == "/sodamixer/ch2/toggle")
        {
            ch2Show = !ch2Show;
        }
        if (m.getAddress() == "/sodamixer/ch3/toggle")
        {
            ch3Show = !ch3Show;
        }

        if (m.getAddress() == "/beatape/next_beat_epoch_ms")
        {
            nextBeatMSVal = (uint64_t)m.getArgAsFloat(0);
        }
        if (m.getAddress() == "/beatape/bpm")
        {
            bpmVal = m.getArgAsFloat(0);
            bpmLabel = ofToString(bpmVal);
        }
    }

    if (now >= nextBeatMSVal)
    {
        nextBeatMSVal = now + (60.0 * 1000.0 / bpmVal);
    }

    gui.update();
}

//--------------------------------------------------------------
void ofApp::drawOutput(ofEventArgs &args)
{
    outputFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::draw()
{
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
    if (dragInfo.files.size() > 0)
    {
        if (ch1Load)
        {
            ch1Load = false;
            ch1VideoPlayer.load(dragInfo.files[0]);
            ch1VideoPlayer.setLoopState(OF_LOOP_NORMAL);
            ch1VideoPlayer.play();
        }
        if (ch2Load)
        {
            ch2Load = false;
            ch2VideoPlayer.load(dragInfo.files[0]);
            ch2VideoPlayer.setLoopState(OF_LOOP_NORMAL);
            ch2VideoPlayer.play();
        }
        if (ch3Load)
        {
            ch3Load = false;
            ch3VideoPlayer.load(dragInfo.files[0]);
            ch3VideoPlayer.setLoopState(OF_LOOP_NORMAL);
            ch3VideoPlayer.play();
        }
    }
}