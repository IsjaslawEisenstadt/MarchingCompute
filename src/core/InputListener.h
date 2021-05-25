#pragma once

class InputListener
{
public:
	virtual void OnKeyInput(int key, int scancode, int action, int mods) = 0;
	virtual void OnMouseButtonInput(int button, int action, int mods) = 0;
	virtual void OnMouseMotionInput(double xpos, double ypos) = 0;
	virtual void OnScrollInput(double xoffset, double yoffset) = 0;
};
