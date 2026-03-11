#pragma once

#include <chrono>

namespace MacUtil
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	float GetCyclesPerMilliSeconds()
	{
		// returning 1.0f means GetCurrentTimeStamp should return milliseconds
		return 1.0f;
	}

	long long GetCurrentTimeStamp()
	{
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	}

    // A simple key state tracking map for FreeGLUT
    static bool gKeys[256];
    static bool gSpecialKeys[256];

    void SetKeyDown(unsigned char key) { gKeys[key] = true; }
    void SetKeyUp(unsigned char key) { gKeys[key] = false; }
    void SetSpecialKeyDown(int key) { gSpecialKeys[key] = true; }
    void SetSpecialKeyUp(int key) { gSpecialKeys[key] = false; }

	float GetXAxisInput()
	{
		bool isLeft = gSpecialKeys[GLUT_KEY_LEFT];
		bool isRight = gSpecialKeys[GLUT_KEY_RIGHT];
		if (isLeft ^ isRight)
		{
			return isLeft ? -1.f : 1.f;
		}
		return 0.f;
	}

	float GetYAxisInput()
	{
		bool isDown = gSpecialKeys[GLUT_KEY_DOWN];
		bool isUp = gSpecialKeys[GLUT_KEY_UP];
		if (isDown ^ isUp)
		{
			return isDown ? -1.f : 1.f;
		}
		return 0.f;
	}

	float GetZAxisInput()
	{
		bool isDown = gSpecialKeys[GLUT_KEY_PAGE_DOWN];
		bool isUp = gSpecialKeys[GLUT_KEY_PAGE_UP];
		if (isDown ^ isUp)
		{
			return isDown ? -1.f : 1.f;
		}
		return 0.f;
	}

	float GetWAxisInput()
	{
		bool isDown = gSpecialKeys[GLUT_KEY_END];
		bool isUp = gSpecialKeys[GLUT_KEY_HOME];
		if (isDown ^ isUp)
		{
			return isDown ? -1.f : 1.f;
		}
		return 0.f;
	}

    #define ISPRESSED(KeyArray, KeyCode) return KeyArray[KeyCode]

	void BindInput(InputManager& InInputManager)
	{
		InInputManager.SetInputAxis(InputAxis::XAxis, GetXAxisInput);
		InInputManager.SetInputAxis(InputAxis::YAxis, GetYAxisInput);
		InInputManager.SetInputAxis(InputAxis::ZAxis, GetZAxisInput);
		InInputManager.SetInputAxis(InputAxis::WAxis, GetWAxisInput);
		InInputManager.SetInputButton(InputButton::Space, []() { ISPRESSED(gKeys, ' '); });
		InInputManager.SetInputButton(InputButton::Z, []() { return gKeys['z'] || gKeys['Z']; });
		InInputManager.SetInputButton(InputButton::X, []() { return gKeys['x'] || gKeys['X']; });
	}

	void BindSystemInput(SystemInputManager& InSystemInputManager)
	{
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F1, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F1); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F2, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F2); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F3, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F3); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F4, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F4); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F5, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F5); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F6, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F6); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F7, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F7); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F8, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F8); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F9, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F9); });
		InSystemInputManager.SetSystemInputButton(SystemInputButton::F10, []() { ISPRESSED(gSpecialKeys, GLUT_KEY_F10); });
	}
}
