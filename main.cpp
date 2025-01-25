// compile: g++ -o output.exe main.cpp -mwindows -lopengl32 -Os -s

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winuser.h> // WM_MOUSEWHEEL
#include <stdlib.h>	 // srand, rand
#include <stdio.h>	 // sprintf_s, swprintf_s
#include <math.h>	 // sin, cos, ...
#include <time.h>	 // time

#define GL_PI 3.1415926535
#include <gl\gl.h> //lib opengl32 - OpenGL

HDC hDC = NULL;	  // main window device context
HGLRC hRC = NULL; // main render device context

#define TIMER_INDEX_FRAME 1		// next frame timer
#define TIMER_INDEX_SCENE 2		// next scene timer
#define TIMER_TIME_FRAME 32		// 1000 / ms = fps
#define TIMER_TIME_SCENE 300000 // next scene time (5 min)
#define SCENE_COUNT 18			// number of scenes 0-17

const wchar_t lpClassName[] = L"OpenGL_AL2D";
const wchar_t lpWindowTitle[] = L"Artifical Life 2D";

const float viewVolume = 100.0f;
const float viewScale = 100.0f;

BOOL fullscreen = TRUE; // fullscreen or normal window
int scene = 0;			// actual scene index
int speed = 100;		// 10-ultra fast, 30-fast, 60-normal, 90-slow, 120-very slow, 180-ultra slow, 600-almost standing

GLdouble t = GL_PI * 4; // animation time (GL_PI * 4 means center of the screen for beginning of static animation)

inline double magnitude(double k, double e)
{
	return sqrt(pow(k, 2) + pow(e, 2));
}

static void glSceneUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear screen
	glLoadIdentity();			  // resets transformation matrix back to its default state
	glScalef(viewScale, viewScale, viewScale);

	glOrtho(-200.0f, 200.0f, -200.0f, 200.0f, -200.0f, 200.0f);

	GLfloat x, y, alpha;
	GLdouble k, e, o, d, q, c, px, py;
	GLdouble moving = abs((t * 16) - 402) - 201; // used for move of static objects (-201 to +201 and begin from 0 to negative side)
	switch (scene)
	{
	case 0: // cute jellyfish
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 100));
				glColor4f(.92f, .86f, 0.99f, alpha); // ebddfd
				//  draw
				k = x / 8 - 12.5;
				e = y / 8 - 12.5;
				o = (pow(k, 2) + pow(e, 2)) / 139; // magnitude(k, e) ^ 2 / 139;
				d = 9 * cos(o);
				px = ((x + sin(d) * d * k) / 2 + 150 + o * k * sin(t + d * o)) - 200 + moving;
				py = -1 * (y / 9 - d * 15 - cos(d * 2) * d + 220 + d * sin(d - t)) + 200;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 1: // octopus
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 100));
				glColor4f(.0f, .25f, 0.5f, alpha * 5);
				//  draw
				k = x / 8 - 12.5;
				e = y / 8 - 12.5;
				o = (pow(k, 2) + pow(e, 2)) / 169; // magnitude(k, e) ^ 2 / 169;
				d = .5 + 5 * cos(o);
				px = x + d * k * sin(d * 2 + o + t) + e * cos(e + t) - 100 + moving;
				py = -1 * (o * 135 - y / 4 - d * 6 * cos(d * 3 + o * 9 + t) + 125) + 200;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 2: // holey jellyfish
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 100));
				glColor4f(1.0f, 1.0f, .4f, alpha);
				//  draw
				k = x / 8 - 12.5;
				e = y / 8 - 12.5;
				o = magnitude(k, e) / 3;
				d = 5 * cos(o);
				px = ((x + (d * k + k * 3) * sin(d * 2.5 - t)) / 2) - 50 + moving;
				py = -1 * (d * 13 + (d * 3 + 12) * (3 + cos(d * 3 - t)) + d * e) + 50;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 3: // millipede
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				glColor4f(.85f, .55f, .25f, 0.075f);
				//  draw
				k = x / 8 - 12.5;
				e = y / 8 - 9;
				o = magnitude(k, e) / 4;
				q = x + y + e / k + o * k * cos(y / 3) * sin(o * 2 - t) * o / 2;
				c = o * e / 40 - t / 8;
				px = q / 3 * atan(3 * sin(c)) + moving;
				py = o * sin(x / 9) - y * cos(c) + q / 4 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 4: // eared centipede
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 100));
				glColor4f(0.92f, 0.92f, 0.92f, alpha);
				// draw
				k = x / 8 - 12;
				e = y / 8 - 9;
				d = (pow(k, 2) + pow(e, 2)) / 50; // magnitude(k, e) ^ 2 / 50;
				q = x / 3 + e + 60 + 1 / k + k / sin(e) * sin(d - t);
				c = d / 4 - t / 8;
				px = q * sin(c);
				py = (q - y * d / 9) * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 5: // hungry ancistrus
		for (y = 100; y < 300; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(1, alpha, (float)abs(moving / 200), alpha * 3);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				o = magnitude(k, e) / 3;
				d = 5 * cos(o);
				px = (x + d * k * sin(d * 2.5 - t) + k / 2 * sin(y / 3 + t)) / 2 - 100 + moving;
				py = (d * 19 + (d - 2) * 5 * abs(cos(d / 2 - t / 2)) + d * e) + 25;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 6: // fossil
		for (y = 100; y < 300; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(.5f, 1.0f, .5f, alpha * 3);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				o = magnitude(k, e) / 4;
				q = x + y / 3 + k / cos(y / 8) + 1 / k + o * k * cos(y / 8 - t) * sin(o * 4 - t);
				c = o * e / 50 - t / 8;
				px = q / 3 * atan(2 * sin(c));
				py = (y * tan(c) + q) / 3 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 7: // soul eater
		for (y = 100; y < 300; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(.2f, .2f, 1.0f, 3 * alpha + 0.02f);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				o = magnitude(k, e) / 3.5;
				d = 5 * cos(o);
				c = (d + o) / 4 - t / 8;
				px = (k * atan(3 * cos(d * 9)) + x / 2) * cos(c);
				py = (k * d * cos(o - d + t) + y / 2) * sin(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 8: // hedelix
		for (y = 99; y < 300; y += 5)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++) // 101..299 = 200 is center of line
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(0, 1, y / 300, alpha * 5);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				d = (pow(k, 2) + pow(e, 2)) / 99; // magnitude(k, e) ^ 2 / 99;
				q = x / 3 + k * 0.5 / cos(y * 5) * sin(d * d - t);
				c = d / 2 - t / 8;
				px = q * sin(c) + e * sin(d + k - t);
				py = (q + y / 8 + d * 9) * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 9: // furry blanket
		for (y = 99; y < 300; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++) // 101..299 = 200 is center of line
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(1, y / 300, (float)((int)(x + y) % 100) / 100, 3 * alpha);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				d = (pow(k, 2) + pow(e, 2)) / 99; // magnitude(k, e) ^ 2 / 99;
				q = x / 2 + k / atan(9 * cos(e)) * sin(d * 4 - t);
				c = d / 3 - t / 8;
				px = (y / 3 + d + q) * cos(c);
				py = q * sin(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 10: // tripple team
		for (y = 99; y < 300; y += 2)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++) // 101..299 = 200 is center of line
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(1, y / 300, (float)((int)(x + y) % 100) / 100, 4 * alpha - 0.1f);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				o = magnitude(k, e) / 3;
				d = cos(o) * e / 5;
				q = x / 4 + k / cos(y / 9) * sin(d * 9 - t) + 25;
				c = d - t / 8;
				px = q * sin(c) + (moving / 2);
				py = (q * 2 + x + y / 2 + d * 90) / 4 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 11: // cheerful arrow
		for (y = 99; y < 300; y += 2)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 101; x < 300; x++) // 101..299 = 200 is center of line
			{
				// color
				alpha = 1 / (1 + abs(x - 200));
				glColor4f(y / 300, (float)((int)(x + y) % 100) / 100, 1, alpha * 3);
				// draw
				k = x / 8 - 25;
				e = y / 8 - 25;
				o = magnitude(k, e) / (4 + sin(k * e / 49 - t));
				d = cos(o);
				q = k / cos(e) * sin(d * 9) + x / 3;
				c = o - t / 8; // change the minus to plus and it will go the other way around
				px = q * cos(c);
				py = (y / 2 + q) / 2 * sin(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 12: // double nudibranch
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 1 / (1 + abs(x - 100));
				glColor4f(0.2f, (float)(abs(moving) / 400) + 0.2f, y / 200, alpha); // glColor4f(0.2f, 0.7f, y / 200, alpha);
				// draw
				k = x / 8 - 12;
				e = y / 13 - 14;
				o = magnitude(k, e) / 2;
				d = 5 * cos(o);
				q = x / 2 + 10 + 1 / k + k * cos(e) * sin(d * 8 - t);
				c = d / 3 + t / 8;
				px = q * sin(c) + sin(d * 2 + t) * k;
				py = (y / 4 + 5 * o * o + q * cos(c * 3)) / 2 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 13: // twins
		for (y = 0; y < 150; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 100; x++)
			{
				// color
				alpha = 2 / (1 + abs(x - 50));
				glColor4f(y / 200, 0.5f, x / 100, alpha);
				// draw
				k = x / 4 - 12.5;
				e = y / 9;
				o = magnitude(k, e) / 9;
				q = x + 99 + cos(9 / k) + o * k * (cos(e * 9) / 3 + cos(y / 9) / .7) * sin(o * 4 - t);
				c = o * e / 30 - t / 8;
				px = q * .7 * sin(c) + 100;
				py = y / 9 * cos(c * 4 - t / 2) - q / 2 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();

			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 100; x++)
			{
				// color
				alpha = 2 / (1 + abs(x - 50));
				glColor4f(y / 200, 0.5f, x / 100, alpha);
				// draw
				k = x / 4 - 12.5;
				e = y / 9 + 9;
				o = magnitude(k, e) / 9;
				q = x + 99 + tan(1 / k) + o * k * (cos(e * 9) / 2 + cos(y / 9) / .7) * sin(o * 4 - t * 2);
				c = o * e / 30 - t / 8;
				px = q * .7 * sin(c) - 100;
				py = y / 9 * cos(c * 4 - t / 2) - q / 2 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 14: // splash
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				glColor4f(y / 200, 0.6f, x / 200, 0.1f);
				// draw
				k = x / 8 - 12.5;
				e = y / 8 - 12.5;
				o = magnitude(k, e) / 12;
				o *= cos(sin(k / 2) * cos(e / 2));
				d = 5 * cos(o);
				px = x + d * k * (sin(d * 2 + t) + sin(y * o * o) / 8) - 100;
				py = y / 3 - d * 40 + 19 * cos(d + t) * -3 + 50;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 15: // sea flower
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				glColor4f(y / 200, 0.3f, 0.7f, 0.2f);
				// draw
				k = x / 8 - 12;
				e = y / 8 - 12;
				o = 2 - magnitude(k, e) / 3;
				d = -5 * abs(sin(k / 2) * cos(e * .8));
				px = (x - d * k * 4 + d * k * sin(d + t)) + k * o * 2 - 95;
				py = (y - d * y / 5 + d * e * cos(d + t + o) * sin(t + d)) + e * o - 190;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 16: // storm waves
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				glColor4f(x / 200, y / 200, 0.7f, 0.1f);
				// draw
				k = x / 8 - 12.5;
				e = y / 8 - 12;
				o = 3 - magnitude(k, e) / 3;
				d = -4 * (sin(k / 2) * cos(e));
				px = 3 * (x + e * cos(t) + d * k * sin(d + t)) + k * o - 300;
				py = (y - d * 19 + d * e * cos(d + t)) - 120;
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	case 17: // scurfy
		for (y = 0; y < 200; y++)
		{
			glBegin(GL_LINE_STRIP);
			for (x = 0; x < 200; x++)
			{
				// color
				alpha = 2 / (1 + abs(x - 100));
				glColor4f(y / 100, 0.6f, 0.8f, alpha + .06f);
				// draw
				k = x / 8 - 12.5;
				d = abs(cos(k / 2) + sin(y / 4));
				q = x / 4 + 90 + (d + .1) * k * cos(d - t * 2 + y / 24);
				c = y / 99 - t / 8 + d * d / 32;
				px = (q + y) * .7 * sin(c) + (q - y) / 2 * cos(c * 2);
				py = q * .7 * cos(c);
				glVertex2d(px, py);
			}
			glEnd();
		}
		break;
	default:
		break;
	}

	glFlush(); // flush render pipeline
	SwapBuffers(hDC);
}

static void glResize(int width, int height)
{
	if (height == 0)
		height = 1; // Prevent A Divide By Zero
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height); // Set Viewport to window dimensions

	// GL_PROJECTION - Perspective Projection  Render object affected by the distance, a car is seen smaller when it move away.
	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity();			 // Reset The Projection Matrix
	// gluPerspective(60.0f, aspectRatio, 0.0f, 5.0f);//Aspect Ratio (zNear & zFar are always positive !!!)

	// GL_MODELVIEW - Orthographic Projection Render object NOT affected by the distance like a menu, a text on the screen, 2D objects ...
	// glOrtho(0, width, 0, height, 0, 1);//Establish clipping volume (left, right, bottom, top, near, far)
	if (width <= height)
		glOrtho(-viewVolume, viewVolume, -viewVolume / aspectRatio, viewVolume / aspectRatio, 0.0, 5.0);
	else
		glOrtho(-viewVolume * aspectRatio, viewVolume * aspectRatio, -viewVolume, viewVolume, 0.0, 5.0);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity();			// Reset The Modelview Matrix
}

static void glResize(HWND hWnd, int width = 0, int height = 0)
{
	if (width == 0 || height == 0)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
	}
	glResize(width, height);
}

static void ShowMouseCursor(BOOL bShow) // connect mouse reset ShowCursor counter to 0 / disconnect mouse reset ShowCursor counter to -1
{
	CURSORINFO ci;
	ci.cbSize = sizeof(ci);
	if (GetCursorInfo(&ci))
	{
		if (ci.flags == CURSOR_SHOWING) // mouse cursor is showing
		{
			if (!bShow) // hide mouse cursor
				while (ShowCursor(FALSE) > -1)
					;
		}
		else
		{
			if (bShow) // show mouse cursor
				while (ShowCursor(TRUE) < 0)
					;
		}
	}
}

static void SetFullscreen(HWND hWnd, BOOL bFullscreen)
{
	if (bFullscreen)
	{
		LONG_PTR style = (LONG_PTR)GetClassLongPtr(hWnd, GCL_STYLE);
		SetClassLongPtr(hWnd, GCL_STYLE, style | CS_NOCLOSE); // disable Alt+F4

		ShowWindow(hWnd, SW_MAXIMIZE); // maximize window
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_MAXIMIZE);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		// SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		ShowWindow(hWnd, SW_MAXIMIZE); // send WM_SIZE
		ShowMouseCursor(FALSE);
	}
	else
	{
		LONG_PTR style = (LONG_PTR)GetClassLongPtr(hWnd, GCL_STYLE);
		SetClassLongPtr(hWnd, GCL_STYLE, style & ~CS_NOCLOSE); // enable Alt+F4

		ShowWindow(hWnd, SW_RESTORE); // restore window
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
		// SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		ShowWindow(hWnd, SW_RESTORE); // send WM_SIZE
		ShowMouseCursor(TRUE);
	}
}

static void SceneSpeedUp()
{
	if (speed > 20)
		speed -= 10;
}

static void SceneSpeedDown()
{
	if (speed < 200)
		speed += 10;
}

static void SceneNext()
{
	if (scene == SCENE_COUNT - 1)
		scene = 0;
	else
		scene++;
}

static void ScenePrev()
{
	if (scene == 0)
		scene = SCENE_COUNT - 1;
	else
		scene--;
}

static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;	 // request an RGBA format
		pfd.cColorBits = 24;			 // number of bits per pixel excluding alpha
		pfd.cDepthBits = 16;			 // 16Bit Z-Buffer (Depth Buffer)
		pfd.iLayerType = PFD_MAIN_PLANE; // main drawing layer
		SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

		hRC = wglCreateContext(hDC); // create render context
		wglMakeCurrent(hDC, hRC);	 // enable render context

		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // implement transparency - rendering antialiased points and lines in arbitrary order
		glEnable(GL_BLEND);								   // enable transparency
		glShadeModel(GL_SMOOTH);						   // smooth shading
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // quality of color and texture coordinate interpolation
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);		   // sampling quality of antialiased points
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);			   // sampling quality of antialiased lines
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);		   // sampling quality of antialiased polygons
		glEnable(GL_POINT_SMOOTH);						   // enable points antialiasing
		glEnable(GL_LINE_SMOOTH);						   // enable lines antialiasing
		glEnable(GL_POLYGON_SMOOTH);					   // enable polygons antialiasing

		glClearColor(.1f, .1f, .1f, .0f); // background color

		SetTimer(hWnd, TIMER_INDEX_FRAME, TIMER_TIME_FRAME, NULL);
		SetTimer(hWnd, TIMER_INDEX_SCENE, TIMER_TIME_SCENE, NULL);
		break;
	case WM_ERASEBKGND: // window background must be erased (for example, when a window is resized)
		return 0;		// disable background erase (remove flickering)
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) // exit app
			DestroyWindow(hWnd);
		else if (wParam == VK_UP) // speed up
			SceneSpeedUp();
		else if (wParam == VK_DOWN) // speed down
			SceneSpeedDown();
		else if (wParam == VK_LEFT) // prev scene
			ScenePrev();
		else if (wParam == VK_RIGHT) // next scene
			SceneNext();
		else if (wParam == VK_SPACE) // random scene
			scene = rand() % SCENE_COUNT;
		else if (wParam == VK_F1)
		{
			ShowMouseCursor(TRUE);
			wchar_t message[1000];
			int i = swprintf_s(message, 1000, L"Current scene: %i of [0..%i]\n", scene, SCENE_COUNT - 1);
			i += swprintf_s(message + i, 1000 - (size_t)i, L"\nShortcuts:\nEscape - exit application\nSPACE - select random scene\nLeft/Right - select prev or next scene\nUp/Down - speed up or down animation\nF11 - toggle fullscreen\nF12 - toggle scene change lock\n");
			i += swprintf_s(message + i, 1000 - (size_t)i, L"\nhttps://github.com/KamilR82/Artificial-Life-2D");
			MessageBox(hWnd, message, lpWindowTitle, MB_OK | MB_ICONINFORMATION);
			if (fullscreen)
				ShowMouseCursor(FALSE);
		}
		else if (wParam == VK_F11) // toogle fullscreen
			SetFullscreen(hWnd, fullscreen = !fullscreen);
		else if (wParam == VK_F12) // toogle scene lock
		{
			if (KillTimer(hWnd, TIMER_INDEX_SCENE) == 0)				   // disable scene timer
				SetTimer(hWnd, TIMER_INDEX_SCENE, TIMER_TIME_SCENE, NULL); // or enable if not exists
		}
		break;
	case WM_MOUSEWHEEL:
		if (LOWORD(wParam) & MK_CONTROL)
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				SceneSpeedUp();
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
				SceneSpeedDown();
		}
		else
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				SceneNext();
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
				ScenePrev();
		}
		break;
	case WM_TIMER:
		if (wParam == TIMER_INDEX_FRAME)
		{
			t += GL_PI / speed;
			if (t > GL_PI * 16) // full circle animation
				t = 0;			// reset
			glSceneUpdate();
		}
		else if (wParam == TIMER_INDEX_SCENE)
			scene = rand() % SCENE_COUNT; // random scene
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) // SIZE_RESTORED or SIZE_MAXIMIZED
			glResize(hWnd, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE && fullscreen) // executes the screen saver application
			return 0;							   // disable
		break;
	case WM_DESTROY:
		KillTimer(hWnd, TIMER_INDEX_FRAME);
		KillTimer(hWnd, TIMER_INDEX_SCENE);
		// clean up and exit
		if (hRC)
		{
			wglMakeCurrent(NULL, NULL); // release rendering context
			wglDeleteContext(hRC);		// delete rendering context
		}
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// only one instance allowed
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, lpClassName); // instance already exists?
	if (hMutex)													 // second instance
	{
		HWND hWnd = FindWindow(lpClassName, lpWindowTitle); // find window
		if (hWnd)
		{
			if (IsIconic(hWnd)) // window is minimized?
			{
				if (IsZoomed(hWnd)) // get prev state
					ShowWindow(hWnd, SW_MAXIMIZE);
				else
					ShowWindow(hWnd, SW_RESTORE);
			}
			SetForegroundWindow(hWnd);
		}
		return 0; // exit this instance
	}
	else											   // first instance
		hMutex = CreateMutex(NULL, TRUE, lpClassName); // create mutex

	srand((UINT)time(NULL));	  // random seed
	scene = rand() % SCENE_COUNT; // random scene

	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_NOCLOSE = disable Alt+F4
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_LOADTRANSPARENT);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADTRANSPARENT);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpfnWndProc = (WNDPROC)WindowProcedure;
	wcex.lpszClassName = lpClassName;
	if (!RegisterClassEx(&wcex))
	{
		ReleaseMutex(hMutex);
		return -1;
	}

	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, lpClassName, lpWindowTitle, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, hInstance, NULL);
	if (!hWnd)
	{
		UnregisterClass(lpClassName, hInstance);
		ReleaseMutex(hMutex);
		return -2;
	}
	SetFullscreen(hWnd, fullscreen);
	SetForegroundWindow(hWnd); // slightly higher priority
	ShowWindow(hWnd, SW_SHOW); // show window
	SetFocus(hWnd);			   // sets keyboard focus to the window

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(lpClassName, hInstance);
	ReleaseMutex(hMutex);
	return (int)msg.wParam;
}
