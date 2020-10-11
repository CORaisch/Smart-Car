#include "Core/Init/Init_GLUT.h"

using namespace Core::Init;

Core::IListener* Init_GLUT::listener = NULL;
Core::WindowInfo Init_GLUT::windowInformation;

void Init_GLUT::init(const Core::WindowInfo& windowInfo, const Core::ContextInfo& contextInfo, const Core::FramebufferInfo& framebufferInfo) {

	int fakeargc = 1;
	char *fakeargv[] = { (char*)"fake", NULL };
	glutInit(&fakeargc, fakeargv);

	if (contextInfo.core) {
		glutInitContextVersion(contextInfo.major_version, contextInfo.minor_version);
		glutInitContextProfile(GLUT_CORE_PROFILE);
	} else
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	//these functions were called in the old main.cpp
	//Now we use info from the structures
	glutInitDisplayMode(framebufferInfo.flags);
	glutInitWindowPosition(windowInfo.position_x, windowInfo.position_y);
	glutInitWindowSize(windowInfo.width, windowInfo.height);

	glutCreateWindow(windowInfo.name.c_str());
	windowInformation = windowInfo;

	std::cout << "GLUT:initialized" << std::endl;
	//these callbacks are used for rendering
	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	//callbacks for Keyboard Events
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	//init GLEW
	Init::Init_GLEW::Init();

	//cleanup
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//our method to display some info. Needs contextInfo and windowinfo
	printOpenGLInfo(windowInfo, contextInfo);
}

void Init_GLUT::run() {
	glutMainLoop();
}

void Init_GLUT::close() {
	std::cout << "GLUT:\tFinished" << std::endl;
	glutLeaveMainLoop();
}

void Init_GLUT::idleCallback(void) {
	glutPostRedisplay();
}

void Init_GLUT::displayCallback() {
	if (listener) {
		listener->notifyBeginFrame();
		listener->notifyDisplayFrame();

		glutSwapBuffers();

		listener->notifyEndFrame();
	}
}

void Init_GLUT::reshapeCallback(int width, int height) {
	if (windowInformation.isReshapable == true) {
		if (listener) {
			listener->notifyReshape(width, height, windowInformation.width, windowInformation.height);
		}
		windowInformation.width = width;
		windowInformation.height = height;
	}
}

void Init_GLUT::closeCallback() {
	close();
}

void Init_GLUT::enterFullscreen() {
	glutFullScreen();
}

void Init_GLUT::exitFullscreen() {
	glutLeaveFullScreen();
}

void Init_GLUT::keyDown(unsigned char key, int x, int y) {
	if (listener)
		listener->keyDown(key, x, y);
}

void Init_GLUT::keyUp(unsigned char key, int x, int y) {
	if (listener)
		listener->keyUp(key, x, y);
}

void Init_GLUT::setListener(Core::IListener*& iListener) {
	listener = iListener;
}

void Init_GLUT::printOpenGLInfo(const Core::WindowInfo& windowInfo, const Core::ContextInfo& contextInfo) {

	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);

	std::cout << "******************************************************************************" << std::endl;
	std::cout << "GLUT:\tInitialise" << std::endl;
	std::cout << "GLUT:\tVendor : " << vendor << std::endl;
	std::cout << "GLUT:\tRenderer : " << renderer << std::endl;
	std::cout << "GLUT:\tOpenGl version: " << version << std::endl;
	std::cout << "******************************************************************************" << std::endl;
}
