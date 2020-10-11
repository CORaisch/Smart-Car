#pragma once

#include "Core/Init/ContextInfo.h"
#include "Core/Init/FrameBufferInfo.h"
#include "Core/Init/WindowInfo.h"
#include "Core/Init/IListener.h"
#include "Core/Init/Init_GLEW.h"
#include <iostream>

namespace Core {
	namespace Init {

		class Init_GLUT {

		public:
			static void init(const Core::WindowInfo& window, const Core::ContextInfo& context, const Core::FramebufferInfo& framebufferInfo);

		public:
			static void run();
			static void close();

			void enterFullscreen();
			void exitFullscreen();

			//used to print info about GL
			static void printOpenGLInfo(const Core::WindowInfo& windowInfo, const Core::ContextInfo& context);
			static void setListener(Core::IListener*& iListener);

		private:
			static void idleCallback(void);
			static void displayCallback(void);
			static void reshapeCallback(int width, int height);
			static void closeCallback();
			static void keyDown(unsigned char key, int x, int y);
			static void keyUp(unsigned char key, int x, int y);
			static Core::IListener* listener;
			static Core::WindowInfo windowInformation;
		};
	}
}
