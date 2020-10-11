#include "Core/EventHandler.h"

EventHandler::EventHandler() {
	this->keyboardState = new bool[256];
	for (int i = 0; i < 256; i++)
		this->keyboardState[i] = false;
}

EventHandler::~EventHandler() {
	delete this->keyboardState;
}

void EventHandler::keyDown(unsigned char key, int x, int y) {
	this->keyboardState[key] = true;
}

void EventHandler::keyUp(unsigned char key, int x, int y) {
	this->keyboardState[key] = false;
}

bool EventHandler::getKeyState(unsigned char key) {
	return this->keyboardState[key];
}
