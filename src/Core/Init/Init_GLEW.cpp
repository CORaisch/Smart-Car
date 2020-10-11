#include "Core/Init/Init_GLEW.h"

using namespace Core;
using namespace Core::Init;

void Init_GLEW::Init() {

	glewExperimental = true;
	if (glewInit() == GLEW_OK)
		std::cout << "GLEW: Initialize" << std::endl;

	if (glewIsSupported("GL_VERSION_4_0"))
		std::cout << "GLEW Version is 4.0" << std::endl;

	else
		std::cout << " GLEW Version 4.0 not supported" << std::endl;
}
