/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    GLController.h													  *	
 * @date	07. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class GLController.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_GL_CONTROLLER_H
#define _AFR_GL_CONTROLLER_H



// Including system libraries
#include <GL/gl.h>
#ifndef _WCHAR_T_DEFINED
	#define _WCHAR_T_DEFINED
#endif
#include <GL/glut.h>



// Including project libraries
#include "../exceptions/Exceptions.h"
#include "../types/Color.h"



// Predefinition of classes
class Application;
class Configurator;
class Vector2;



/**
 * Class GLController
 * @brief Class for manage rendering to window and implement all OpenGL 
 * callbacks. Its only a layer between application and OpenGL.
 * Here will be refactoring - implementation of parent class (RenderController) 
 * with virtual methods for replace this controller with another one (with 
 * wxWidgets or Qt).
 */ 
class GLController
{
public:
	///< Implicit constructors
	GLController() throw(StaticClassException);
	
	///< Implicit destructors
	~GLController() {};
	
	
	///< Static methods
	static void Init();
	static void Destroy();
	static void PrintHelp();
	
	
	///< Static methods for handle OpenGL callbacks
	static void OnResize(int w, int h);
	static void OnKeyboard(unsigned char key, int x, int y);
	static void OnSpecialKeys(int key, int x, int y);
	static void OnMouseClick(int button, int state, int x, int y);
	static void OnMouseMotion(int x, int y);
	static void OnDisplay();
	
	
	///< Public static properties
	static Configurator *context;
	static Application  *application;
	
	static Vector2		windowPos;
	static Vector2		windowSize;
	static Vector2		oldWindowSize;
	static bool			fullscreenState;
	static unsigned		displayBufferType;
	
	
private:
	///< Private drawing methods
	static void PrintFPS();
	static void PrintAnimTime();
	
	static void DrawString(const string &str, const Vector2 &pos, const Color &clr);
	static void SetOrthoProjection();
	static void UnsetOrthoProjection();
	
	
	///< Private properties
	static unsigned 	frameCnt;
	static int			curTime;
	static int			prevTime;
	static string		fpsStr;
	static string		animTimeStr;
	
	static Vector2		screenOffset;
	
}; // class GLController



#endif // #ifndef _AFR_GL_CONTROLLER_H



/** ***************************************************************************/
/** end of file GLController.h												  */
/** ***************************************************************************/
