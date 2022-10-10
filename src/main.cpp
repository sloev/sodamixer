#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main()
{
	ofGLFWWindowSettings settings;

	settings.setSize(300, 300);
	settings.setPosition(glm::vec2(500, 300));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	settings.setSize(1100, 500);
	settings.setPosition(glm::vec2(0, 0));
	settings.resizable = true;
	// uncomment next line to share main's OpenGL resources with gui
	settings.shareContextWith = mainWindow;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);

	shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->setupGui();

	ofAddListener(mainWindow->events().draw, mainApp.get(), &ofApp::drawOutput);

	ofRunApp(guiWindow, mainApp);
	ofRunMainLoop();
}
