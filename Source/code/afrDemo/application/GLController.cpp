/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    GLController.cpp												  *	
 * @date	07. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class GLController.							  *
 **************************************************************************** */


// Including project libraries
#include "GLController.h"

#include "Application.h"
#include "Configurator.h"
#include "Sampler.h"

#include "../scene/Scene.h"
#include "../scene/Camera.h"
#include "../raytracer/Raytracer.h"

#include "../types/Image.h"
#include "../types/Vector2.h"

#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"
#include "../utils/TimeUtils.h"



// Initialize GLController static public properties
Configurator 	*GLController::context = NULL;
Application  	*GLController::application = NULL;

Vector2			GLController::screenOffset = Vector2(-1.0f, -1.0f);

unsigned 		GLController::frameCnt = 0;
int				GLController::curTime = 0;
int				GLController::prevTime = 0;
string			GLController::fpsStr = "FPS: 0";
string			GLController::animTimeStr = "Time: 0.0";

Vector2			GLController::windowPos = Vector2(512, 256);
Vector2			GLController::windowSize = Vector2::Zero();
Vector2			GLController::oldWindowSize = Vector2::Zero();
bool			GLController::fullscreenState = false;
unsigned		GLController::displayBufferType = Sampler::BUFFER_NORMAL;


/**
 * Default constructor. Static class, throw exception.
 * @throw StaticClassException Static class, cannot be instantiated.
 * @return GLController.
 */ 
GLController::GLController() throw(StaticClassException)
{
	throw StaticClassException("Class 'GLController' is static. Cannot be instantiated!");
} // GLController()



/** ***************************************************************************/
/** Static auxiliary methods												  */
/** ***************************************************************************/
	


/**
 * Method for initialize OpenGL application.
 */ 
void GLController::Init()
{
	Color cls = GLController::application->GetScene()->GetBackgroundColor();
	glClearColor(cls.r(), cls.g(), cls.b(), 1.0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	return;
} // Init()



/**
 * Method for destroy OpenGL application.
 */ 
void GLController::Destroy()
{
	// Delete context and all instantiated objects
	if(context != NULL) {
		delete context;
		context = NULL;
	}
	
	// Log message
	cout << "QUITING APPLICATION" << endl;
	exit(EXIT_SUCCESS);			
	
	return;
} // Destroy()



/**
 * Method for display application help.
 */ 
void GLController::PrintHelp()
{
	cout << "+==============================================================================+" << endl;
	cout << "|  START APPLICATION (command line parameters)                                 |" << endl;
	cout << "+==============================================================================+" << endl << endl;
	cout << "\t./afr -c config.ini" << endl;
    cout << "\t> Run app and sets parameters according to 'config.ini' file which must contain definition of scene file." << endl << endl;
	cout << "\t./afr -c config.ini -s scene.aff" << endl;
    cout << "\t> Run app and sets parameters according to 'config.ini' with scene file 'scene.aff'." << endl << endl;
	cout << "\t./afr -s scene.aff" << endl;
    cout << "\t> Run app and use default settings with scene file 'scene.aff'." << endl << endl;
	cout << "\t./afr redraw_mode=fixed redraw_fixed=4500 scene_file=scene.aff" << endl;
	cout << "\t> Run app and use manual settings from command line with scene file 'scene.aff'." << endl << endl;
	cout << "\t./afr -h" << endl;
	cout << "\t> Show this help page." << endl << endl;
	cout << "+==============================================================================+" << endl;
	cout << "|  SHORTCUTS                                                                   |" << endl;
	cout << "+==============================================================================+" << endl << endl;
    cout << "\t> F - toggle fullscreen," << endl;
    cout << "\t> G - toggle guiding sampling," << endl;
    cout << "\t> R - toggle reconstruction," << endl;
    cout << "\t> B - select auxiliary buffer from list," << endl;
    cout << "\t> C - capture actual frame into image and save it to disk," << endl;
    cout << "\t> A - increase value of aging factor about 50," << endl;
    cout << "\t> SHIFT+A - decrease value of aging factor about 50," << endl;
    cout << "\t> S - increase value of sigma factor obout 1," << endl;
    cout << "\t> SHIFT+S - decrease value of sigma factor about 1," << endl;
    cout << "\t> 1 - set random metod for choosing sample position," << endl;
    cout << "\t> 2 - set Halton sequence metod for choosing sample position," << endl;
    cout << "\t> 3 - set Jittered sampling metod for choosing sample position," << endl;
    cout << "\t> 4 - set Multi jittered sampling metod for choosing sample position," << endl;
    cout << "\t> 5 - set Poisson discs metod for choosing sample position," << endl;
    cout << "\t> 6 - set N rooks metod for choosing sample position," << endl;
    cout << "\t> SPACE - start/pause animation," << endl;
    cout << "\t> HOME - goto animation start," << endl;
    cout << "\t> END - goto animation end," << endl;
    cout << "\t> PAGE_DOWN - pause animation and go to the next frame," << endl;
    cout << "\t> PAGE_UP - pause animation and go to the previous frame," << endl;
    cout << "\t> F1 - show this help in console," << endl;
    cout << "\t> F2 - show/hide actual FPS in window," << endl;
    cout << "\t> F3 - show/hide actual animation time in window," << endl;
    cout << "\t> Q|ESC - quit application." << endl << endl;	
	cout << "+==============================================================================+" << endl;
	cout << "|  CONFIGURATION                                                               |" << endl;
	cout << "+==============================================================================+" << endl;
    cout << "|  [common]                                                                    |" << endl;
	cout << "+==============================================================================+" << endl << endl;
	cout << "\tredraw_mode (string, [auto|fps|fixed]):" << endl;
    cout << "\t> application mode for determine when actualization of new pixel will be stopped:" << endl;
    cout << "\t\t> auto: if scene has animations, step will the as step in animation, otherwise it the same as fps," << endl;
	cout << "\t\t> fps: set step to actual FPS (see redraw_fps)," << endl;
	cout << "\t\t> fixed: update specified amount of samples (see redraw_fixed)," << endl << endl;
	cout << "\tredraw_fps (integer):" << endl;
    cout << "\t> if mode 'fps', this is FPS, i. e. 1 / fps = time step." << endl << endl;
	cout << "\tredraw_fixed (integer):" << endl;
    cout << "\t> if mode 'fixed', this is amount of updated samples." << endl << endl;
	cout << "\tredraw_min_pixels (integer):" << endl;
    cout << "\t> minimum pixels which will be actualize in each step." << endl << endl;
	cout << "\tdraw_fps (boolean):" << endl;
    cout << "\t> if true, FPS will be drawn on display." << endl << endl;
	cout << "\tpredraw_screen (boolean):" << endl;
    cout << "\t> if true, first whole screen image will be precomputed before animation starts via raytracer." << endl << endl;
	cout << "\tuse_bg_image (boolean):" << endl;
    cout << "\t> if true, background image will be used." << endl << endl;
	cout << "\tbg_image (string):" << endl;
    cout << "\t> path and name of image background file - should be first screen image." << endl << endl;
	cout << "\tcapture_frame (boolean):" << endl;
    cout << "\t> if true, each frame will captured." << endl << endl;
	cout << "\tcapture_nth_frame (integer):" << endl;
    cout << "\t> number of frame which will be captured." << endl << endl;
	cout << "\tframe_name_mask (string):" << endl;
    cout << "\t> if frames are captured, this is file name mask:" << endl;
    cout << "\t\t> %%: escape sequence for %," << endl;
    cout << "\t\t> %N: unique number (id)," << endl;
	cout << "\t\t> %T: actual animation time," << endl;
	cout << "\t\t> %F: actual animation frame," << endl;
	cout << "\t\t> %G: global time since application has been started," << endl;
	cout << "\t\t> %A: actual time of current application's step," << endl;
	cout << "\t\t> %M: sampling method name (halton, random, poisson, jitter, rooks)," << endl;
	cout << "\t\t> %B: auxiliary buffer name (normal, tiles, deriv, temp)," << endl << endl;
	cout << "+==============================================================================+" << endl;
    cout << "|  [application]                                                               |" << endl;
	cout << "+==============================================================================+" << endl << endl;
	cout << "\tauto_start (boolean):" << endl;
    cout << "\t> auto start animation playback flag." << endl << endl;
	cout << "\tsampling_method (string, [halton|random|jitter|poisson|rooks]):" << endl;
    cout << "\t> sampling method." << endl << endl;
	cout << "\tsamples_cache_size (integer):" << endl;
    cout << "\t> number of samples which will be precomputed when its neccessary (for jitter, poisson and rooks)." << endl << endl;
	cout << "\tuse_guiding_sampling (boolean):" << endl;
    cout << "\t> use guiding sampling." << endl << endl;
	cout << "\thorizontal_tiles (integer):" << endl;
    cout << "\t> number of horizontal tiles." << endl << endl;
	cout << "\tvertical_tiles (integer):" << endl;
    cout << "\t> number of vertical tiles." << endl << endl;
	cout << "\tuse_recontruction (boolean):" << endl;
    cout << "\t> use reconstruction." << endl << endl;
	cout << "\tsigma_factor (integer):" << endl;
    cout << "\t> sigma factor for gaussian filter." << endl << endl;
	cout << "\taging_factor (integer):" << endl;
    cout << "\t> scale factor s for aging function lambda in guiding sampling and resonstruction temporal filter function. " << endl << endl;	
    cout << "+==============================================================================+" << endl;
    cout << "|  [scene]                                                                     |" << endl;
	cout << "+==============================================================================+" << endl << endl;
	cout << "\tscene_file (string):" << endl;
    cout << "\t> path and name of AFF file with scene description. MANDATORY!!!" << endl << endl;
	cout << "\tuse_scene_kdtree (boolean):" << endl;
    cout << "\t> kd tree usage." << endl << endl;
	cout << "\tscene_kdtree_depth (integer|string [smart]):" << endl;
    cout << "\t> kd tree depth." << endl << endl;
	cout << "\tscene_kdtree_min_primitives (integer):" << endl;
    cout << "\t> min primitives in scene to use kd tree." << endl << endl;
	cout << "\tscene_kdtree_min_depth (integer):" << endl;
    cout << "\t> OR min depth to use kd tree in scene. " << endl << endl;
	cout << "+==============================================================================+" << endl;
    cout << "|  [raytracer]                                                                 |" << endl;
	cout << "+==============================================================================+" << endl << endl;
	cout << "\tuse_tracer_msaa (boolean):" << endl;
    cout << "\t> flag if supersampling method will be use." << endl << endl;
	cout << "\ttracer_msaa (integer):" << endl;
    cout << "\t> number of samples used in supersampling method for suppression antialiasing." << endl << endl;
	cout << "\ttracer_depth (integer):" << endl;
    cout << "\t> max level depth of ray for refractions and reflections ." << endl << endl;
	
	return;
} // PrintHelp()



/**
 * Helper method for print actual FPS on screen.
 * @return void.
 */ 
void GLController::PrintFPS()
{	
	GLController::frameCnt++;
	GLController::curTime = Now();
	if(GLController::curTime - GLController::prevTime > 1000.0f) {
		GLController::fpsStr = "FPS: " + Dump(GLController::frameCnt * 1000.0f / (GLController::curTime - GLController::prevTime));
		GLController::prevTime = GLController::curTime;
		GLController::frameCnt = 0;
	}

	if(GLController::context->IsDrawFPS()) {
		GLController::DrawString(GLController::fpsStr, Vector2(15, 15), Color::Red());
	}
	
	return;
} // PrintFPS()



/**
 * Helper function for print actual animation and application time on screen.
 * @return void.
 */ 
void GLController::PrintAnimTime()
{
	if(GLController::application->GetScene()->HasAnimations()) {
		char tt[11];
		sprintf(tt, "%6.4f", GLController::application->GetScene()->GetAnimationManager()->GetActualTime());
		string timeStr = "Time: " + string(tt);
		sprintf(tt, "%6.4f", GLController::application->GetAppTime() / 1000.0f);
		timeStr += ", App time: " + string(tt);
		
		if(GLController::context->IsDrawAnimTime()) {
			GLController::DrawString(timeStr, Vector2(15, GLController::windowSize.y() - 10), Color::Red());
		}
	}
	
	return;
} // PrintAnimTime()



/**
 * Helper function for sets orthographical projection.
 * @return void.
 */ 
void GLController::SetOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, GLController::windowSize.x(), 0, GLController::windowSize.y());
	glScalef(1, -1, 1);
	glTranslatef(0, -GLController::windowSize.y(), 0);
	glMatrixMode(GL_MODELVIEW);
	
	return;
} // SetOrthoProjection()



/**
 * Helper function for unsets orthographical projection.
 * @return void.
 */ 
void GLController::UnsetOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glRasterPos2f(GLController::screenOffset.x(), GLController::screenOffset.y());
	
	return;
} // ResetOrthoProjection()



/**
 * Helper function draw given string with specified color on screen at given
 * position.
 * @param string str String which will be printed,
 * @param Vector2 pos Position on screen,
 * @param Color clr Color of text.
 * @return void.
 */ 
void GLController::DrawString(const string &str, const Vector2 &pos, const Color &clr)
{
	glRasterPos2f(pos.x(), pos.y());
	glColor4f(clr.r(), clr.g(), clr.b(), 1.0f);
	for(char *c = (char *)str.c_str(); *c != '\0'; c++) {
    	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
	
	return;
} // DrawText()



/** ***************************************************************************/
/** Static methods for OpenGL callbacks										  */
/** ***************************************************************************/



/**
 * Callback method for handle resizing window.
 * @return void.
 */
void GLController::OnResize(int w, int h)
{
	glViewport(0, 0, w, h);                       
	
	GLController::windowSize.x(w).y(h);	
	GLController::screenOffset = Vector2(
		Max(-1.0f, -1.0f + ((2.0f / w) * ((w - (int)GLController::application->GetScene()->GetCamera()->GetWidth()) * 0.5f))),
		Max(-1.0f, -1.0f + ((2.0f / h) * ((h - (int)GLController::application->GetScene()->GetCamera()->GetHeight()) * 0.5f)))
	);
	
	glRasterPos2f(GLController::screenOffset.x(), GLController::screenOffset.y());
	glutPostRedisplay();
	
	//cout << "GLController::OnResize( " << offsetX << " x " << offsetY << " ): " << w << " x " << h << ", " << GLController::scene->GetCamera()->GetWidth() << " x " << GLController::scene->GetCamera()->GetHeight() << endl;
	return;
} // OnResize()



/**
 * Callback method for handle inputs from keyboard.
 * @return void.
 */
void GLController::OnKeyboard(unsigned char key, int x, int y)
{
	// Switch to lowercase
	if(key >= 'A' && key <= 'Z') {
		key += 'a' - 'A';
	}
	
	// Check pressed key
	switch(key)	{
		// key 'Q' - quit
		case 27: // maybe ESC
		case 'q':
			GLController::Destroy();
			break;
			
		case 'f':
			// Toggle fullscreen state
			GLController::fullscreenState = !GLController::fullscreenState;
			
			// Fullscreen mode
			if(GLController::fullscreenState) {
				GLController::oldWindowSize = GLController::windowSize;
				GLController::windowPos.x(glutGet(GLUT_WINDOW_X))
									   .y(glutGet(GLUT_WINDOW_Y));
				glutFullScreen();
			}
			// Window mode
			else {
				if(GLController::oldWindowSize.x() > 0.0) {
					glutPositionWindow(GLController::windowPos.x(), GLController::windowPos.y());
					glutReshapeWindow(GLController::oldWindowSize.x(), GLController::oldWindowSize.y());
				}
				else {
					Camera *cam = GLController::application->GetScene()->GetCamera();
					glutReshapeWindow(cam->GetWidth(), cam->GetHeight());
				}
			}
			
			break;
			
		// key ' ' or '32' - space bar
		case 32:
			GLController::context->TogglePlayback();
			cout << ((GLController::context->IsPlaying()) ? "Animations is playing..." : "Animation has been stopped!") << endl;
			break;
		
		// key '1'
		case '1':
			cout << "RANDOM method" << endl;
			GLController::context->SetSamplingMethod("random");
			break;
			
		// key '2'
		case '2':
			cout << "HALTON sequence" << endl;
			GLController::context->SetSamplingMethod("halton");
			break;
			
		// key '3'
		case '3':
			cout << "JITTER (or stratified) sampling method" << endl;
			GLController::context->SetSamplingMethod("jitter");
			break;
		
		// key '4'
		case '4':
			cout << "MULTI JITTER sampling method" << endl;
			GLController::context->SetSamplingMethod("multijitter");
			break;
		
		// key '5'
		case '5':
			cout << "POISSON DISCS sampling method" << endl;
			GLController::context->SetSamplingMethod("poisson");
			break;
	
		// key '6'
		case '6':
			cout << "N-ROOKS sampling method" << endl;
			GLController::context->SetSamplingMethod("rooks");
			break;
			
		// key 'a' - aging factor
		case 'a': {
			float af = GLController::context->GetAgingFactor();
			int mod = glutGetModifiers();
			if(mod == GLUT_ACTIVE_SHIFT) {
				af = Max(50.0f, af-50.0f);
			}
			else {
				af += 50.0f;
			}
			
			GLController::context->SetAgingFactor(af);
			cout << "Aging factor has been changed. New value is " << af << "." << endl;
			break;
		}	

		// key 's' - sigma factor
		case 's': {
			float sf = GLController::context->GetSigmaFactor();
			int mod = glutGetModifiers();
			if(mod == GLUT_ACTIVE_SHIFT) {
				sf = Max(1.0f, sf-1.0f);
			}
			else {
				sf += 1.0f;
			}
			
			GLController::context->SetSigmaFactor(sf);
			cout << "Sigma factor has been changed. New value is " << sf << "." << endl;
			break;
		}
			
		// key 'c'
		case 'c': {
			string fn = GLController::application->CaptureFrame(true, GLController::displayBufferType);
			cout << "Frame '" << fn << "' captured" << endl;
			
			break;
		}
		// key 'g'
		case 'g': {
			GLController::context->ToggleGuidingSampling();
			if(!GLController::context->UseGuidingSampling()) {
				GLController::displayBufferType = Sampler::BUFFER_NORMAL;
			}
			cout << ((GLController::context->UseGuidingSampling()) ? "Guiding sampling is ON..." : "Guiding sampling is OFF!") << endl;
			break;
		}
		// key 'r'
		case 'r': {
			GLController::context->ToggleReconstruction();
			if(!GLController::context->UseReconstruction()) {
				GLController::displayBufferType = Sampler::BUFFER_NORMAL;
			}
			cout << ((GLController::context->UseReconstruction()) ? "Reconstruction process is ON..." : "Reconstruction process is OFF!") << endl;
			break;
		}
		// key 'b'
		case 'b': {
			GLController::displayBufferType = (GLController::displayBufferType + 1) % Sampler::BUFFER_MAX;
			if(GLController::displayBufferType == Sampler::BUFFER_TILES && !GLController::context->UseGuidingSampling()) {
				GLController::displayBufferType = Sampler::BUFFER_NORMAL;
			}
			if((GLController::displayBufferType == Sampler::BUFFER_DERIVATIVES || GLController::displayBufferType == Sampler::BUFFER_TEMPORAL) && !GLController::context->UseReconstruction()) {
				GLController::displayBufferType = Sampler::BUFFER_NORMAL;
			}
			
			switch(GLController::displayBufferType) {
				case Sampler::BUFFER_TILES: cout << "Tiling mode is on" << endl; break;
				case Sampler::BUFFER_DERIVATIVES: cout << "Derivatives mode is on" << endl; break;
				case Sampler::BUFFER_TEMPORAL: cout << "Temporal mode is on" << endl; break;
				default: cout << "Normal mode is on" << endl; break;
			}
			break;
		}
		default:
			break;
	}
	
	glutPostRedisplay();
	return;
} // OnKeyboard()



/**
 * Callback method for handle special inputs from keyboard (special buttons).
 * @return void.
 */
void GLController::OnSpecialKeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_F1:
			GLController::PrintHelp();
			break;
		case GLUT_KEY_F2:
			GLController::context->ToggleDrawFPS();
			break;
		case GLUT_KEY_F3:
			GLController::context->ToggleDrawAnimTime();
			break;
		case GLUT_KEY_HOME:
			GLController::context->SetPlayback(false);
			GLController::context->GetSceneService()->GotoStart();
			break;
		case GLUT_KEY_END:
			GLController::context->SetPlayback(false);
			GLController::context->GetSceneService()->GotoEnd();
			break;
		case GLUT_KEY_PAGE_UP:
			GLController::context->SetPlayback(false);
			GLController::context->GetSceneService()->Prev();
			break;
		case GLUT_KEY_PAGE_DOWN:
			GLController::context->SetPlayback(false);
			GLController::context->GetSceneService()->Next();
			break;
	}
	
	glutPostRedisplay();
	return;
} // OnSpecialKeys()



/**
 * Callback method for handle mouse clicks.
 * @return void.
 */
void GLController::OnMouseClick(int button, int state, int x, int y)
{
	glutPostRedisplay();
	return;
} // OnMouseClick()



/**
 * Callback method for handle mouse motion.
 * @return void.
 */
void GLController::OnMouseMotion(int x, int y)
{
	glutPostRedisplay();
	return;
} // OnMouseMotion() 



/**
 * Callback method for handle redraw display content.
 * @return void.
 */
void GLController::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int pixels = GLController::application->Run();
	if(pixels > 0) {		
		float *buffer = NULL;
		Image *fb = NULL;
		
		// Select proper frame buffer according to display buffer type
		switch(GLController::displayBufferType) {
			case Sampler::BUFFER_TILES: 
				fb = GLController::application->GetSampler()->GetTilesBuffer(); 
				break;
			case Sampler::BUFFER_DERIVATIVES: 
				fb = GLController::application->GetSampler()->GetDerivativesBuffer(); 
				break;
			case Sampler::BUFFER_TEMPORAL: 
				fb = GLController::application->GetSampler()->GetTemporalBuffer(); 
				break;
			default: 
				fb = GLController::application->GetFrameBuffer(); 
				break;
		}
		
		// Convert image to float array
		fb->ToFloat(buffer);
		
		glDrawPixels(
			fb->GetWidth(),
			fb->GetHeight(),
			GL_RGB,
			GL_FLOAT,
			buffer
		);
		
		delete buffer;		
	}
	
	
	// Calculate and print FPS
	GLController::SetOrthoProjection();
	GLController::PrintFPS();
	GLController::PrintAnimTime();
	GLController::UnsetOrthoProjection();
	
	
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
	
	return;
} // OnDisplay()



/** ***************************************************************************/
/** end of file GLController.cpp											  */
/** ***************************************************************************/
