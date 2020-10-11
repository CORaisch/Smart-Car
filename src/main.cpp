#include "Core/Init/Init_GLUT.h"
#include "Managers/Scene_Manager.h"

using namespace Core;

int main(int argc, char **argv) {

  std::cout << "\033[0;0H\033[2J" << std::flush; // reset and clear terminal

	int window_x = 800;
	int window_y = 600;
	WindowInfo window(std::string("Smart Car"), 400, 200, window_x, window_y, true);

	ContextInfo context(4, 0, true);
	FramebufferInfo frameBufferInfo(true, true, true, true);
	Init::Init_GLUT::init(window, context, frameBufferInfo);

	IListener* scene = new Managers::Scene_Manager(window_x, window_y);
	Init::Init_GLUT::setListener(scene);

	Init::Init_GLUT::run();

	delete scene;
	return 0;
}
