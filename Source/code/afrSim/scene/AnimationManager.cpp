/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AnimationManager.cpp											  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class AnimationManager.						  *
 **************************************************************************** */


// Including project libraries
#include "AnimationManager.h"

#include "Camera.h"

#include "../types/Vector3.h"
#include "../types/Vector4.h"
#include "../types/Matrix.h"



/**
 * Default constructor.
 * @return AnimationManager.
 */ 
AnimationManager::AnimationManager()
{
	this->camera = NULL;
	this->list.Clear();
	
	this->start = 0.0f;
	this->end = 0.0f;
	this->time = 0.0f;
	this->step = 0.0f;
	this->total = 0;
	this->frame = 0;	
	
	this->loop = 0;
	this->loops = 0;
} // AnimationManager()



/**
 * Default constructor.
 * @param float start Time when animation starts,
 * @param float end Time when animation ends,
 * @param unsigned frames Total count of animation frames (time step is (end-start) / (frames-1)).
 * @param Vector2 res Camera resolution - i.e. width and height of the final picture.
 * @return ScenePlayer.
 * @throw InvalidArgumentException Thrown when total count of animation frames are less than 2.
 */ 
inline AnimationManager::AnimationManager(const float &start, const float &end, const unsigned &frames, const int &loops)
{
	this->camera = NULL;
	this->list.Clear();

	this->SetPlayerSettings(start, end, frames);
	
	// Minimal frames count is 2
	if(frames < 2) {
		throw InvalidArgumentException("Minimal total animation frames must be 2 or higher! '"+Dump(frames)+"' given.");
	}
	
	if(this->start > this->end) {
		this->start = end;
		this->end = start;	
	}
	else {
		this->start = start;
		this->end = end;	
	}
	
	this->time = this->start;
	this->frame = 0;
	this->total = frames;	
	this->step = Abs((this->end - this->start) / ((float)this->total - 1.0f));
	
	this->loop = 0;
	this->loops = loops;
} // AnimationManager()



/**
 * Default destructor.
 */ 
AnimationManager::~AnimationManager()
{
	// Delete camera animation if exists
	if(this->camera) {
		delete this->camera;
		this->camera = NULL;
	}
	
	// Delete all animations stored in list
	for(unsigned i = 0; i < this->list.Size(); i++) {
		delete this->list[i];
	}
	this->list.Clear();
	
} // AnimationManager()



/**
 * Method for add new animation into list.
 * @param Animation a An animation which will we be added to list.
 * @return bool Returns true, if animation has been added succesfully, false otherwise.
 */ 
bool AnimationManager::Add(Animation *a)
{
	// If animation 'a' is NULL do nothing
	if(a == NULL) {
		return false;
	}
	// If animation's name is "camera" store into a special property
	else if(a->GetName() == "camera") {
		if(this->camera) {
			delete this->camera;
			this->camera = NULL;
		}
		
		this->camera = a;
		return true;
	}
	// If animation with the same name is does't exists, add to list
	else if(this->Find(a->GetName()) == NULL) {
		this->list.Add(a);
		return true;
	}
	// Else delete animation
	else {
		return false;
	}
	
	return false;
} // Add()



/**
 * Method for find animation by name.
 * @param string name Animation's name.
 * @return Animation Returns found animation or NULL.
 */ 
Animation *AnimationManager::Find(const string &name) const
{
	// No name, nothing to find
	if(name.empty()) {
		return NULL;
	}
	
	// Find animation over list of animations
	for(unsigned i = 0; i < this->list.Size(); i++) {
		if(this->list[i]->GetName() == name) {
			return this->list[i];
		}
	}
	
	return NULL;
} // Find()



/**
 * Return translation for animation with name 'name' at actual animation time.
 * @param string name Animation's name.
 * @return Vector3 Returns translation vector for transformations.
 */ 
Vector3	AnimationManager::GetTranslation(const string &name)
{
	Animation *a = this->Find(name);
	if(a != NULL) {
		return a->GetTranslation(this->time);
	}
	
	return Vector3(0.0, 0.0, 0.0);
} // GetTranslation()



/**
 * Return rotation for animation with name 'name' at given time 'time'.
 * @param string name Animation's name.
 * @return Vector4 Returns rotation vector for transformations.
 */ 
Vector4	AnimationManager::GetRotation(const string &name)
{
	Animation *a = this->Find(name);
	if(a != NULL) {
		return a->GetRotation(this->time);
	}
	
	return Vector4(1.0, 0.0, 0.0, 0.0);
} // GetRotation()



/**
 * Return scale for animation with name 'name' at given time 'time'.
 * @param string name Animation's name.
 * @return Vector3 Returns scale vector for transformations.
 */ 
Vector3	AnimationManager::GetScale(const string &name)
{
	Animation *a = this->Find(name);
	if(a != NULL) {
		return a->GetTranslation(this->time);
	}
	
	return Vector3(1.0, 1.0, 1.0);
} // GetScale()



/**
 * Return visibility for animation with name 'name' at given time 'time'.
 * @param string name Animation's name.
 * @return bool Returns true, if object is visible at given time, false otherwise.
 */ 
bool AnimationManager::GetVisibility(const string &name)
{
	Animation *a = this->Find(name);
	if(a != NULL) {
		return a->GetVisibility(this->time);
	}
	
	return true;
} // GetVisibility()



/**
 * Return transformation matrix for animation with name 'name' at given 
 * time 'time'. Combines GetTranslation(), GetRotation() and GetScale() methods.
 * @param string name Animation's name.
 * @return Matrix Returns transformation matrix for animation.
 */ 
Matrix AnimationManager::GetMatrix(const string &name)
{
	Animation *a = this->Find(name);
	if(a != NULL) {
		return a->GetMatrix(this->time);
	}
	
	// Jinak vraci matici identity
	return Matrix::Identity();
} // GetMatrix()



/**
 * Calculates new camera position and directions at given time.
 * @param Camera camera Camera object.
 * @return bool Return true, if camera animation exists and camera object has been affected, false otherwise.
 * @see AnimationManager::GetCamera().
 */ 
bool AnimationManager::GetCamera(Camera *camera)
{
	bool hasFrom = false;
	bool hasDirection = false;
	Vector3 from, at, up;
	
	bool hasCamera = this->GetCamera(from, hasFrom, at, up, hasDirection);
	if(hasFrom) {
		camera->SetPosition(from);
	}
	if(hasDirection) {
		camera->SetLookAt(at);
		camera->SetUp(up);
	}
	if(hasCamera) {
		camera->Transform();
	}
	
	//cout << this->time << ": " << from << "; " << at << ", " << up << endl;
	return hasCamera;
} // GetCamera()



/**
 * Calculate look at vector for camera at given time. Calculates from, at and up
 * components of look at structure. Sets hasFrom flag if camera animation has
 * translations and sets hasDirection flag if camera animation has rotations.
 * TODO: maybe refactor this method with Camera object or ViewPort structure.
 * @param Vector3 from New position of camera in scene at given time,
 * @param bool hasFrom Flag if new position has been set,
 * @param Vector3 at New direction of camera in scene at given time,
 * @param Vector3 up New UP direction of camera in scene at given time,
 * @param bool hasDirection Flag if new directions of camera has been set.
 * @return bool Retursn true, if camera animation exists, false otherwise.
 */ 
bool AnimationManager::GetCamera(Vector3 &from, bool &hasFrom, Vector3 &at, Vector3 &up, bool &hasDirection)
{
	hasFrom = false;
	hasDirection = false;
	
	// If camera animation exists
	if(this->HasCamera()) {
		// If camera has some translations
		if(this->camera->HasTranslations()) {
			hasFrom = true;
			from = this->camera->GetTranslation(this->time);
		}
		
		// If camera has some rotations
		if(this->camera->HasRotations()) {
			hasDirection = true;
			Matrix m = this->camera->GetMatrix(this->time);
			
			// Set AT vector and mult by 200 (FIX unknown bug)
			at.x(-m[0][2]).y(-m[1][2]).z(-m[2][2]);
			at *= 200.0f;
			
			// Set UP vector
			up.x(m[0][1]).y(m[1][1]).z(m[2][1]);
		}
		
		return true;
	}
	
	return false;
} // GetCamera()



/** ***************************************************************************/
/** end of file AnimationManager.cpp										  */
/** ***************************************************************************/

