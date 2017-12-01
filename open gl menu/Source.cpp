#include <GL\freeglut.h>
#include <GL\glut.h>
#include <GL\GLU.h>
#include <GL\GL.h>

HDC g_HDC;
bool done;

void DrawGame(float xPos, float yPos, float zPos){
	glPushMatrix();
	glBegin(GL_POLYGON);

		glVertex3f(0.0f,0.0f,0.0f);

	glEnd();
	glPopMatrix();

}


void Render(){

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
		glLoadIdentity();

	glPopMatrix();
	glFlush();

	SwapBuffers(g_HDC);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        /*      Rendering and Device Context
                variables are declared here.
        */
        static HGLRC hRC;
        static HDC hDC;

        /*      Width and Height for the
                window our robot is to be
                displayed in.
        */
        int width, height;

        switch(message)
        {
                case WM_CREATE: //window being created

                        hDC = GetDC(hwnd);  //get current windows device context
                        g_HDC = hDC;
                         //call our pixel format setup function

                        /*      Create rendering context and make it current
                        */
                        hRC = wglCreateContext(hDC);
                        wglMakeCurrent(hDC, hRC);

                        return 0;
                        break;

                case WM_CLOSE:  //window is closing

                        /*      Deselect rendering context and delete it*/
                        wglMakeCurrent(hDC, NULL);
                        wglDeleteContext(hRC);

                        /*      Send quit message to queue*/
                        PostQuitMessage(0);

                        return 0;
                        break;

                case WM_SIZE:

                        /*      Retrieve width and height*/
                        height = HIWORD(lParam);
                        width = LOWORD(lParam);

                        /*      Don't want a divide by 0*/
                        if (height == 0)
                        {
                                height = 1;
                        }

                        /*      Reset the viewport to new dimensions*/
                        glViewport(0, 0, width, height);

                        /*      Set current Matrix to projection*/
                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity(); //reset projection matrix

                        /*      Time to calculate aspect ratio of
                                our window.
                        */
                        gluPerspective(54.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f);

                        glMatrixMode(GL_MODELVIEW); //set modelview matrix
                        glLoadIdentity(); //reset modelview matrix

                        return 0;
                        break;

                default:

                        break;
        }

        return (DefWindowProc(hwnd, message, wParam, lParam));
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
        WNDCLASSEX windowClass; //window class
        HWND    hwnd;                   //window handle
        MSG             msg;                    //message
        bool    done;                   //flag for completion of app
        DWORD   dwExStyle;              //window extended style
        DWORD   dwStyle;                //window style
        RECT    windowRect;

        /*      Screen/display attributes*/
        int width = 800;
        int height = 600;
        int bits = 32;

        windowRect.left =(long)0;               //set left value to 0
        windowRect.right =(long)width;  //set right value to requested width
        windowRect.top =(long)0;                //set top value to 0
        windowRect.bottom =(long)height;//set bottom value to requested height

        /*      Fill out the window class structure*/
        windowClass.cbSize                      = sizeof(WNDCLASSEX);
        windowClass.style                       = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc         = WndProc;
        windowClass.cbClsExtra          = 0;
        windowClass.cbWndExtra          = 0;
        windowClass.hInstance           = hInstance;
        windowClass.hIcon                       = LoadIcon(NULL, IDI_APPLICATION);
        windowClass.hCursor                     = LoadCursor(NULL, IDC_ARROW);
        windowClass.hbrBackground       = NULL;
        windowClass.lpszMenuName        = NULL;
        windowClass.lpszClassName       = "MyClass";
        windowClass.hIconSm                     = LoadIcon(NULL, IDI_WINLOGO);

        /*      Register window class*/
        if (!RegisterClassEx(&windowClass))
        {
                return 0;
        }

        /*      Check if fullscreen is on*/
        

        /*      Check if fullscreen is still on*/
        
		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW; 
        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
		   
        /*      Class registerd, so now create our window*/
        hwnd = CreateWindowEx(NULL, "MyClass",  //class name
                                                  "OpenGL Robot",       //app name
                                                  dwStyle |
                                                  WS_CLIPCHILDREN |
                                                  WS_CLIPSIBLINGS,
                                                  0, 0,                         //x and y coords
                                                  windowRect.right - windowRect.left,
                                                  windowRect.bottom - windowRect.top,//width, height
                                                  NULL,                 //handle to parent
                                                  NULL,                 //handle to menu
                                                  hInstance,    //application instance
                                                  NULL);                //no xtra params

        /*      Check if window creation failed (hwnd = null ?)*/
        if (!hwnd)
        {
                return 0;
        }

        ShowWindow(hwnd, SW_SHOW);      //display window
        UpdateWindow(hwnd);                     //update window

        done = false;   //initialize loop condition variable

        /*      Main message loop*/
        while (!done)
        {
                PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

                        if (msg.message == WM_QUIT)     //did we receive a quit message?
                        {
                                done = true;
                        }

                        else
                        {
                                Render();
                                TranslateMessage(&msg);
                                DispatchMessage(&msg);
                        }
        }

        

        return msg.wParam;

}