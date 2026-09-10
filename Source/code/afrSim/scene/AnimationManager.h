/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AnimationManager.h												  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class AnimationManager.							  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_ANIMATION_MANAGER_H
#define _AFR_ANIMATION_MANAGER_H


// Including project libraries
#include "Animation.h"
#include "../exceptions/Exceptions.h"
#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"


// Predefinition of classes
class Camera;



/**
 * Class AnimationManager
 * @brief Class for storing all animations in scene and handle global scene 
 * animation.
 */ 
class AnimationManager
{
public:
	///< Implicit constructors
	AnimationManager();
	AnimationManager(const float &start, const float &end, const unsigned &frames, const int &loops = 0);
	
	///< Implicit destructors
	~AnimationManager();	
	
	///< Animation list operation
	bool			Add(Animation *a);
	Animation 		*Find(const string &name) const;
	Vector3			GetTranslation(const string &name);
	Vector4			GetRotation(const string &name);
	Vector3			GetScale(const string &name);
	bool			GetVisibility(const string &name);
	Matrix			GetMatrix(const string &name);
	bool			GetCamera(Camera *camera);
	bool			GetCamera(Vector3 &from, bool &hasFrom, Vector3 &at, Vector3 &up, bool &hasDirection);
	
	///< Getters
	inline float 	GetStartTime() const { return this->start; };
	inline float 	GetEndTime() const { return this->end; };
	inline float 	GetActualTime() const { return this->time; };
	inline float 	GetTimeStep() const { return this->step; };
	inline unsigned	GetActualFrame() const { return this->frame; };
	inline unsigned	GetFrames() const { return this->total; };
	inline int		GetActualLoop() const { return this->loop; };
	inline int		GetLoops() const { return this->loops; };
	
	inline bool		HasCamera() const { return (this->camera != NULL); };
	inline bool		HasAnimations() const { return !this->list.Empty() || this->camera != NULL; };
	
	///< Setters
	inline AnimationManager *SetPlayerSettings(const float &start, const float &end, const unsigned &frames, const int &loops = 0) throw (InvalidArgumentException);
	
	///< Animation player operations
	inline bool IsStarted() const;
	inline bool IsEnded();
	
	inline bool NextFrame();
	inline bool PrevFrame();
	
	inline bool GotoFrame(const unsigned &f);
	inline bool GotoTime(const float &t);
	inline bool Reset();
	inline bool End();
	
private:
	Animation		*camera;	///< special camera animation
	TAnimationList 	list;		///< list of all animation without camera animation
	
	float			start;		///< animation start time
	float			end;		///< animation end time
	float			time;		///< actual animation time
	float 			step;		///< animation step
	unsigned		frame;		///< actual animation frames
	unsigned 		total;		///< total animation frames
	
	int				loop;		///< loops counter
	int				loops;		///< total loops per animation	
}; // class AnimationManager



/**
 * Sets all settings to animation player such as animation start and end time 
 * and total count of animation frames.
 * @param float start Time when animation starts,
 * @param float end Time when animation ends,
 * @param unsigned frames Total count of animation frames (time step is (end-start) / (frames-1)).
 * @return AnimationManager.
 * @throw InvalidArgumentException Thrown when total count of animation frames are less than 2.
 */ 
inline AnimationManager *AnimationManager::SetPlayerSettings(const float &start, const float &end, const unsigned &frames, const int &loops) throw (InvalidArgumentException)
{
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
	
	return this;
} // SetPlayerSettings()



/**
 * Method for determine if simulation has been started, i.e. actual frame is
 * greater than zero.
 * @return bool Return true, if animation has been started, false otherwise.
 */ 
inline bool AnimationManager::IsStarted() const 
{ 
	return (this->frame > 0); 
} // IsStarted()



/**
 * Method for determine if simulation has been ended, i.e. actual time is
 * less than total simulation time.
 * @return bool Return true, if animation has been ended, false otherwise.
 */ 
inline bool AnimationManager::IsEnded() 
{
	if(this->loops < 0) {
		if(this->frame >= this->total) { 
			this->Reset();
		}
		return false;
	}
	else if(this->loop > this->loops) {
		return true;
	}
	else {
		if(this->frame >= this->total) { 
			this->loop++;
			
			this->frame = 0;
			this->time = this->start;
			return (this->loop > this->loops);
		}
		else {
			return false;
		}
	}
	
	return false;
} // IsEnded() 



/**
 * Method for simulate transition to next frame and update all 
 * appropriate properties. If transition is posible and next frame exists, 
 * return true, false otherwise.
 * @return bool Return true, if next frame exists, false otherwise.
 */ 
inline bool AnimationManager::NextFrame()
{
	// If animation ended, cannot go to next frame
	if(this->IsEnded()) {
		return false;
	}
	
	this->frame++;
	this->time += this->step; 
	
	return true;
} // NextFrame()



/**
 * Method for simulate transition to previous frame and update all 
 * appropriate properties. If transition is posible and previous frame exists, 
 * return true, false otherwise.
 * @return bool Return true, if previous frame exists, false otherwise.
 */ 
inline bool AnimationManager::PrevFrame()
{
	// If animation doesnt started, cannot go to previous frame
	if(!this->IsStarted()) {
		return false;
	}
	
	this->frame--;
	this->time -= this->step; 
	
	return true;
} // PrevFrame()



/**
 * Method for simulate transition to specific frame and update all 
 * appropriate properties if frame exists.
 * @param unsigned f Specific animation frame.
 * @return bool Return true, if specific frame exists, false otherwise.
 */ 
inline bool AnimationManager::GotoFrame(const unsigned &f)
{
	// If required frame is greater than total frames, return false
	if(f >= this->total - 1) {
		return false;
	}
	
	this->frame = f;
	this->time = this->start + (this->step*this->frame);
	
	return true;
} // GotoFrame()



/**
 * Method for simulate transition to specific time and update all 
 * appropriate properties if animation at this time is not finish yet.
 * @param float t Specific animation time.
 * @return bool Return true, if specific time exists, false otherwise.
 */ 
inline bool AnimationManager::GotoTime(const float &t)
{
	// If time is lower that start or greater than end, return false
	if(t < this->start || t > this->end) {
		return false;
	}
	
	this->frame = (unsigned)(t / this->step);
	this->time = this->start + (this->step*this->frame);
	
	return true;
} // GotoTime()



/**
 * Method for reset animation to begin (first frame).
 * @return AnimationManager Provides fluent interface.
 */ 
inline bool AnimationManager::Reset()
{
	this->time = this->start;
	this->frame = 0;
	this->loop = 0;
	
	return true;
} // Reset()



/**
 * Method for set animation to end (last frame).
 * @return AnimationManager Provides fluent interface
 */ 
inline bool AnimationManager::End()
{
	this->time = this->end;
	this->frame = this->total - 1;
	
	return true;
} // End()




#endif // #ifndef _AFR_ANIMATION_MANAGER_H



/** ***************************************************************************/
/** end of file AnimationManager.h											  */
/** ***************************************************************************/
