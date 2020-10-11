#pragma once

class EventHandler {

private:
	bool* keyboardState;

public:
	EventHandler();
	~EventHandler();

	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	bool getKeyState(unsigned char key);
};