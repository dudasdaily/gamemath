#pragma once

#include <string>

namespace MacPlayer
{
	static int gWindowID = 0;
	static std::function<void(ScreenPoint& InNewScreenSize)> gOnResizeFunc;
	static std::function<void()> gOnTickFunc;
	
	static const char* gTitle = "SoftRenderer (Mac)";
	static char gPlayTitle[128];

    // Forward decls for GLUT callbacks
    void DisplayCallback();
    void IdleCallback();
    void ReshapeCallback(int width, int height);
    void KeyboardCallback(unsigned char key, int x, int y);
    void KeyboardUpCallback(unsigned char key, int x, int y);
    void SpecialKeyCallback(int key, int x, int y);
    void SpecialKeyUpCallback(int key, int x, int y);

	bool Create(int argc, char** argv, const ScreenPoint& InDefaultScreenSize)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(InDefaultScreenSize.X, InDefaultScreenSize.Y);
		
		int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
		int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
		glutInitWindowPosition((screenWidth - InDefaultScreenSize.X) / 2, (screenHeight - InDefaultScreenSize.Y) / 2);
		
		gWindowID = glutCreateWindow(gTitle);
		if (gWindowID < 1)
		{
			// Window creation failed
			return false;
		}

        glutDisplayFunc(DisplayCallback);
        glutIdleFunc(IdleCallback);
        glutReshapeFunc(ReshapeCallback);
        glutKeyboardFunc(KeyboardCallback);
        glutKeyboardUpFunc(KeyboardUpCallback);
        glutSpecialFunc(SpecialKeyCallback);
        glutSpecialUpFunc(SpecialKeyUpCallback);

        // Required for smooth input tracking
        glutIgnoreKeyRepeat(1);

		return true;
	}

	FORCEINLINE void RunDefaultLoop()
	{
		glutMainLoop();
	}

	FORCEINLINE void SetMacStatTitle(float frameFPS)
	{
		std::snprintf(gPlayTitle, sizeof(gPlayTitle), "%s  [%.2f fps]", gTitle, frameFPS);
		glutSetWindowTitle(gPlayTitle);
	}

    void DisplayCallback()
    {
        // Handled entirely by MacRSI pulling from buffer, but we should clear just in case
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
    }

    void IdleCallback()
    {
        if (gOnTickFunc)
        {
            gOnTickFunc();
        }
    }

    void ReshapeCallback(int width, int height)
    {
        if (gOnResizeFunc)
        {
            ScreenPoint sp(width, height);
            gOnResizeFunc(sp);
        }
    }

    void KeyboardCallback(unsigned char key, int x, int y)
    {
        MacUtil::SetKeyDown(key);
    }
    
    void KeyboardUpCallback(unsigned char key, int x, int y)
    {
        MacUtil::SetKeyUp(key);
    }

    void SpecialKeyCallback(int key, int x, int y)
    {
        MacUtil::SetSpecialKeyDown(key);
    }

    void SpecialKeyUpCallback(int key, int x, int y)
    {
        MacUtil::SetSpecialKeyUp(key);
    }
}
