#include "../Externals/Include/Include.h"
#include "../Externals/Include/myHeader/shader.h"
#include "../Externals/Include/myHeader/PopUpWindow.h"
#include "../Externals/Include/myHeader/texture.h"

#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3

GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;
vec2 mousePos = vec2(0, 0);
vec2 currentWH = vec2(0, 0);

using namespace glm;
using namespace std;

Shader shaderPopUpWindow;
PopUpWindow m_PopUpWindow;
Texture2D texturePopUpWindow;
Texture2D textureNormalBtn_YES;
Texture2D textureNormalBtn_NO;
Texture2D textureClickedBtn_YES;
Texture2D textureClickedBtn_NO;


void My_Init()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	
	shaderPopUpWindow.init("GLSL/PopUpwindow_vertex.vs.glsl", "GLSL/PopUpwindow_fragment.fs.glsl");
	m_PopUpWindow.init(shaderPopUpWindow.program);
	texturePopUpWindow.init("Image/woodPanel3.png");
	textureNormalBtn_YES.init("Image/ButtonNormal_CYES.png");
	textureNormalBtn_NO.init("Image/ButtonNormal_CNO.png");
	textureClickedBtn_YES.init("Image/ButtonClicked_CYES.png");
	textureClickedBtn_NO.init("Image/ButtonClicked_CNO.png");
}

void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_PopUpWindow.draw(shaderPopUpWindow.program, texturePopUpWindow.texture, textureNormalBtn_YES.texture, textureNormalBtn_NO.texture,
		textureClickedBtn_YES.texture, textureClickedBtn_NO.texture, mousePos, currentWH);

    glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
	currentWH = vec2(width, height);
	glViewport(0, 0, width, height);
	glutReshapeWindow(1600, 900);
}

void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(timer_speed, My_Timer, val);
}

void My_Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		// printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		switch (m_PopUpWindow.check(mousePos, currentWH)) {
		case 0:
			break;
		case 1:
			cout << "Yes\n";
			break;
		case 2:
			cout << "No\n";
			break;
		}
	}
	else if(state == GLUT_UP)
	{
		// printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}

void My_PassiveMouse(int x, int y)
{
	//printf("Mouse is at (%d, %d)\n", x, y);
	mousePos = vec2(x, y);
}

void My_Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'o':
	case 'O':
		if (m_PopUpWindow.wantOpen) {
			m_PopUpWindow.wantOpen = false;
			m_PopUpWindow.windowScale = 1.0f;
		}
		else {
			m_PopUpWindow.wantOpen = true;
			m_PopUpWindow.windowScale = 0.0f;
		}
		break;
	}
	// printf("Key %c is pressed at (%d, %d)\n", key, x, y);
}

void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		// printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		// printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		// printf("Left arrow is pressed at (%d, %d)\n", x, y);
		break;
	default:
		// printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
}

void My_Menu(int id)
{
	switch(id)
	{
	case MENU_TIMER_START:
		if(!timer_enabled)
		{
			timer_enabled = true;
			glutTimerFunc(timer_speed, My_Timer, 0);
		}
		break;
	case MENU_TIMER_STOP:
		timer_enabled = false;
		break;
	case MENU_EXIT:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    // Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1600, 900);
	glutCreateWindow("FinalProject"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif
    glPrintContextInfo();
	My_Init();

	// Create a menu and bind it to mouse right button.
	int menu_main = glutCreateMenu(My_Menu);
	int menu_timer = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddSubMenu("Timer", menu_timer);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_timer);
	glutAddMenuEntry("Start", MENU_TIMER_START);
	glutAddMenuEntry("Stop", MENU_TIMER_STOP);

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Register GLUT callback functions.
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutPassiveMotionFunc(My_PassiveMouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0); 

	// Enter main event loop.
	glutMainLoop();

	return 0;
}
