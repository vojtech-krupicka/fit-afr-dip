/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Scene.h															  *	
 * @date	14. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Scene.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SCENE_H
#define _AFR_SCENE_H



// Including system libraries
#include <string>



// Including project libraries
#include "SceneNode.h"
#include "AnimationManager.h"
#include "../exceptions/Exceptions.h"
#include "../materials/Material.h"
#include "../materials/Texture.h"
#include "../primitives/Light.h"
#include "../primitives/Primitive.h"
#include "../primitives/AnimatedTriangle.h"
#include "../types/Color.h"


// Predefinition of classes
class Animation;
class Camera;
class SceneNode;
class Vector2;
class Vector3;
class Vector4;



/**
 * @brief Structure for keep global informations about scene and environment like 
 * background color, index of refraction or global ambient light.
 */ 
typedef struct s_env {
	Color 			bgColor;
	Color 			ambientColor;
	float 			ior;
	Camera 			*camera;
} TEnvironment;



/**
 * Class Scene
 * @brief Implementation of scene.
 */ 
class Scene
{
public:
	///< Implicit constructors
	Scene();
	Scene(const string &f);

	///< Implicit destructors
	~Scene();	
	
	///< Scene main operations
	Scene	*Create(const string &f) throw (InvalidStateException, RuntimeException);
	Scene	*Destroy() throw (InvalidStateException);
	
	bool 	Next() throw (InvalidStateException);
	bool 	Prev() throw (InvalidStateException);
	bool	GotoFrame(const unsigned &f);
	bool	GotoTime(const float &t);
	bool	GotoStart();
	bool	GotoEnd();
	
	///< API for initialize scene from parser
	Scene	*InitBackgroundColor(const Color &c);
	Scene	*InitAmbientLight(const Color &c);
	Scene   *InitCamera(const Vector3 &from, const Vector3 &at, const Vector3 &up, const float &angle, const float &hither, const Vector2 &res);
	Scene   *InitAnimationParams(const float &start, const float &end, const unsigned &frames, const int &loops = 0);
	
	Scene	*AddLight(const string &name, const Vector3 &pos, const Color &c);
	Scene 	*AddMaterial(const Color &c, const float &kd, const float &ks, const float &s, const float &ior, const float &t);
	Scene 	*AddMaterial(const Color &ac, const Color &dc, const Color &sc, const float &s, const float &ior, const float &t);
	
	Scene	*AddAnimation(Animation *a);
	Scene	*AddTransform(const string &n);
	Scene	*AddTransform(const Vector3 &t, const Vector3 &r, const float &a, const Vector3 &s);
	Scene	*CloseTransform();
	
	Scene	*AddCone(const string &bd, const string &tn, const Vector3 &b, const float &br, const Vector3 &a, const float &ar);
	Scene	*AddSphere(const string &bd, const string &tn, const Vector3 &c, const float &r);
	Scene	*AddPolygon(Vector3 *v, Vector3 *n, const unsigned &c);
	Scene	*AddTriangle(const string &bd, const string &tn, const Vector3 v[3], Vector3 *n, Vector2 *c);
	Scene	*AddMesh(Vector3 *v, const unsigned &vc, Vector3 *n, const unsigned &nc, Vector2 *c, const unsigned &cc, const string &bd, const string &tn, unsigned short *i, const unsigned &ic);
	Scene	*AddAnimatedTriangle(const unsigned &c, float *t, Vector3 *v, Vector3 *n);
	
	///< Getters
	inline bool IsCreated() const { return this->created; };
	
	inline Color 			GetBackgroundColor() const { return this->env->bgColor; };
	inline Color 			GetAmbientColor() const { return this->env->ambientColor; };
	inline float 			GetEnvIor() const { return this->env->ior; };
	inline Camera 			*GetCamera() const { return this->env->camera; };
	
	inline AnimationManager	*GetAnimationManager() const { return this->animManager; };
	inline bool				HasAnimations() const { return (this->animManager != NULL && this->animManager->HasAnimations()); };
	
	inline TPrimitiveList 	&GetPrimitiveList() { return this->primitives; };
	inline unsigned			GetPrimitiveCount() const { return this->primitives.Size(); };
	
	inline TLightList 		&GetLightList() { return this->lights; };
	inline unsigned			GetLightCount() const { return this->lights.Size(); };
	
	
	Texture	*GetTexture(const string &dir, const string &name);
	
private:
	///< Private scene operations
	Scene 	*Init() throw(InvalidStateException);
	Scene	*Animate();

	bool				created; 	///< flat, that scene has been succefully created.
	TEnvironment		*env; 		///< global enviroment settings and parameters.
	
	AnimationManager	*animManager;	///< List of all animations in scene.
	TMaterialList		materials;		///< List of all materials in scene.
	TLightList			lights;			///< List of all lights in scene.
	TLightList			animLights;		///< List of all animated lights in scene, i.e. lights with names.
	TTextureList		textures;		///< List of all textures in scene.
	
	TPrimitiveList		primitives;		///< List of all primitives in scene.
	TAnimatedTriangleList animTriangles;///< List of all animated triangle in scene. 
	
	SceneNode			*rootNode;		///< Static root node of scene graph.
	SceneNode			*actualNode;	///< Actualy selected node.
	TSceneNodeList		animNodes;		///< List of all animated nodes.
	TSceneNodeList		rootAnimNodes;	///< List of root animated nodes only.
	
}; // class Scene



#endif // #ifndef _AFR_SCENE_H



/** ***************************************************************************/
/** end of file Scene.h														  */
/** ***************************************************************************/
