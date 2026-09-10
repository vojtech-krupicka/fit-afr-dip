/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech,	 xkrupi06 									  *
 * @file    afr.cpp															  *	
 * @date	07.03.2012														  *
 * @note  	ISO 8859-2														  *
 * @brief   Main file for application. Contains main() function.			  *
 **************************************************************************** */
 
 
// Including system libraries
#include <stdio.h>
#include <string.h>
#include <string>


// Including project libraries
#include "./exceptions/Exceptions.h"
#include "./application/Application.h"
#include "./application/Configurator.h"
#include "./application/GLController.h"
#include "./scene/Scene.h"
#include "./scene/Camera.h"
#include "./types/Image.h"

#include "./utils/MathUtils.h"
#include "./utils/StringUtils.h"



// Macros and constants definitions
#define _AFR_APP_VERSION 	"1.00"												///< Program version
#define _AFR_APP_NAME		"Hassa ARF (Sim)"									///< Program name
#define _AFR_APP_TITLE		"Hassa Adaptive Frameless Renderer (SIMULATOR)"		///< Program title



// Set namespaces
using namespace std;



/** ***************************************************************************/
/** Function prototypes														  */
/** ***************************************************************************/



/**
 * Function for print help into console into STDERR.
 * @return void.
 */ 
void PrintHelp();



/** ***************************************************************************/
/** Main function 															  */
/** ***************************************************************************/



/**
 * Main program function.
 * @param int argc Arguments count,
 * @param char argv Array of arguments.
 * @return int Returs EXIT_SUCCESS if program ends ok, EXIT_FAILURE otherwise.
 */
int main(int argc, char **argv)
{ 
	// Define auxiliary variables
	string configFile = "";
	string sceneFile = "";
	bool hasConfig = false;
	bool hasScene = false;
	
	
	// Test parameters
	if(argc < 2) {
		// To low parameters, cant continue
		cerr << "ERROR: To low parameters, use './afr -h' for display help." << endl;
		return EXIT_FAILURE;
	}
	else if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
		// Parameters are 2 - print HELP
		PrintHelp();
		return EXIT_SUCCESS;
	}
	else if(argc == 3) {
		if((strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--config") == 0)) {
			// Parameters are 3 - config file name
			configFile = argv[2];
			hasConfig = true;
		}
		else if((strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--scene") == 0)) {
			// Parameters are 3 - scene file name
			sceneFile = argv[2];
			hasScene = true;
		}
	}
	else if(
		argc == 5 && 
		(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--config") == 0) && 
		(strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--scene") == 0)
	) {
			// Parameters are 5 - config and scene file name
			configFile = argv[2];
			hasConfig = true;
			sceneFile = argv[4];
			hasScene = true;
	}
	
	
	// Declare application and configurator object
	Configurator *context = NULL;

	try {
		// If config is defined, creates application with config file
		if(hasConfig) {
			context = new Configurator(configFile);
			
			// After create container from config file, try set new scene file
			if(hasScene) {
				context->SetSceneFile(sceneFile);
			}
		}
		// Else if only scene file has been set
		else if(hasScene) {
			context = new Configurator();
			context->SetSceneFile(sceneFile);
		}
		// Else creates configurator manualy and creates application with context
		else {
			// Creates configurator and sets properties from argument line
			context = new Configurator();
			
			// Parse arguments from argmunet line, explode to key=value and set to context
			for(int i = 1; i < argc; i++) {
				string key, value;
				size_t pos = StrExplode(argv[i], key, value);
				
				if(pos != string::npos) {
					context->Set(key, value);
				}
			}
		}
		
		// Sets obejcts to GLController class
		context->Setup();
		GLController::context = context;
		GLController::application = context->GetApplicationService();
	}
	catch(Exception &e) {
		cout << "APPLICATION CREATION FAILED" << endl;
		cerr << e << endl;
		
		if(context != NULL) {
			delete context;
			context = NULL;
		}
		
		exit(EXIT_FAILURE);
	}
	
	// Get camera for resolution
	Camera *cam = GLController::application->GetScene()->GetCamera();
	
	string version = " v" + Dump(_AFR_APP_VERSION);
	string title = ".: "_AFR_APP_TITLE + version + " : <" + context->GetSceneFile() + "> :.";
	
	// Init GL controller and GLUT application
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(cam->GetWidth(), cam->GetHeight());
	glutInitWindowPosition(GLController::windowPos.x(), GLController::windowPos.y());
	glutCreateWindow(title.c_str());
	glutDisplayFunc(GLController::OnDisplay);
	glutReshapeFunc(GLController::OnResize);
	glutKeyboardFunc(GLController::OnKeyboard);
	glutSpecialFunc(GLController::OnSpecialKeys);
	glutMouseFunc(GLController::OnMouseClick);
	glutMotionFunc(GLController::OnMouseMotion);
	GLController::Init();
	
	// Run main GLUT loop
	glutMainLoop(); 
	
	
	
	return EXIT_SUCCESS;
} // main()



/** ***************************************************************************/
/** Functions declarations													  */
/** ***************************************************************************/



/**
 * Function for print help into console into STDERR.
 */ 
void PrintHelp()
{
	string version = "v" + Dump(_AFR_APP_VERSION);
	cout << "+==============================================================================+" << endl;
	cout << "|  " << _AFR_APP_TITLE << " " << version << "                                     |" << endl;
	GLController::PrintHelp();
	
	return;
} // PrintHelp()



/** ***************************************************************************/
/** end of file afr.cpp														  */
/** ***************************************************************************/
