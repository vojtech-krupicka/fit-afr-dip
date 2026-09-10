/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Scene.cpp														  *	
 * @date	14. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Scene.									  *
 **************************************************************************** */


// Including project libraries
#include "Scene.h"

#include "Animation.h"
#include "Camera.h"

#include "../materials/ImageTexture.h"
#include "../materials/SolidTexture.h"

#include "../primitives/Cone.h"
#include "../primitives/Cylinder.h"
#include "../primitives/Polygon.h"
#include "../primitives/Sphere.h"
#include "../primitives/Triangle.h"

#include "../types/Vector2.h"
#include "../types/Vector3.h"
#include "../types/Vector4.h"
#include "../types/Matrix.h"

#include "../utils/AffParser.h"
#include "../utils/MathUtils.h"
#include "../utils/StringUtils.h"



/**
 * Default constructor. Initialize scene to default state.
 * @return Scene.
 */ 
Scene::Scene()
{
	// Scene hasn't been created yet.
	this->created = false;
	
	// Init scene to default state
	this->Init();
} // Scene()



/**
 * Constructor with AFF file name. Initialize scene to default state and try
 * to construct scene from AFF file.
 * @param string f AFF file name.
 * @return Scene.
 */ 
Scene::Scene(const string &n)
{
	// Scene has'n been created yet.
	this->created = false;
	
	// Init scene to default state and create scene
	this->Init()->Create(n);
} // Scene()



/**
 * Default desctructor. Destroy scene and init itself to default state.
 */ 
Scene::~Scene() 
{
	// Destroy scene (delete all allocated dynamic structures)
	this->Destroy()->Init();
} // ~Scene()



/**
 * Initialize scene to default state - init all properties to default value.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::Init() throw(InvalidStateException)
{
	// If scene has been created, can't been created again (destroy it first)
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already initialized and created!");
	}
	
	
	// Init environment settings structure
	this->env = new TEnvironment;
	this->env->bgColor = Color::Black();
	this->env->ambientColor = Color::White();
	this->env->ior = 1.0f;
	this->env->camera = new Camera();
	
	// Init scene graph
	this->actualNode = NULL;
	this->rootNode = new SceneNode(this);
	if(this->rootNode != NULL) {
		this->actualNode = this->rootNode;
	}
	
	// Init animated triangle list
	this->animTriangles.Clear();
	
	// Init primitive list to empty
	this->primitives.Clear();
	
	// Init animation manager to NULL
	this->animManager = new(nothrow) AnimationManager();
	
	// Init materials list to empty
	this->materials.Clear();
	Material *mat = new Material();
	this->materials.Add(mat);
	
	// Init texture list to empty
	this->textures.Clear();
	
	// Init light list to empty
	this->lights.Clear();
	this->animLights.Clear();
	
	
	return this;
} // Init()



/**
 * Method for creating scene. Prepares all structures and parse AFF input file
 * and store all scene settings and geometry (primitives).
 * @param string f AFF file name.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when parsing fails or parser cannot be created.
 */ 
Scene *Scene::Create(const string &f) throw(InvalidStateException, RuntimeException)
{
	// If scene has been created, can't be created again (destroy it first)
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created!");
	}
	
	
	// Try to parse input AFF file
	AffParser *parser = NULL;
	try {
		parser = new(nothrow) AffParser(this, f);
		if(parser == NULL || !parser->Parse()) {
			// Can't parse AFF file, throw exception
			throw RuntimeException("Error: Can't create AFF Parser or can't parse AFF file '"+f+"'!");
		}
		
		// Delete parser
		delete parser;
	}
	// Forward exception up (it will be cought in upper level where Create method has been called)
	catch(...) {
		throw;
	}
	
	
	// Scene has been succefully created, update flag
	this->created = true;
	
	
	// Sets animation to start (first frame)
	this->GotoStart();
	
	
	return this;
} // Create()



/**
 * Method for destroying scene. Delete all structures and geometry.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has'n been created yet.
 */ 
Scene *Scene::Destroy() throw(InvalidStateException)
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// Destroy environment settings structure
	if(this->env) {
		if(this->env->camera) delete this->env->camera;
		
		delete this->env;
		this->env = NULL;
	}
	
	
	// Destroy scene graph
	if(this->rootNode != NULL) {
		delete this->rootNode;
		
		this->rootNode = NULL;
		this->actualNode = NULL;
		this->animNodes.Clear();
		this->rootAnimNodes.Clear();
		
		this->primitives.Clear();
		this->animTriangles.Clear();
	}
	
	
	// Delete animation list if exists
	if(this->animManager) {
		delete this->animManager;
		this->animManager = NULL;
	}
	
	
	// If there are some materials, delete it
	if(this->materials.Size() > 0) {
		for(unsigned i = 0; i < this->materials.Size(); i++) {
			delete this->materials[i];
		}
		this->materials.Clear();
	}
	
	
	// If there are some textures, delete it
	if(this->textures.Size() > 0) {
		for(unsigned i = 0; i < this->textures.Size(); i++) {
			delete this->textures[i];
		}
		this->textures.Clear();
	}
	
	
	// If there are some lights, delete it
	if(this->lights.Size() > 0) {
		for(unsigned i = 0; i < this->lights.Size(); i++) {
			delete this->lights[i];
		}
		this->lights.Clear();
		this->animLights.Clear();
	}
	
	
	// Scene has been succefully destroyed, update flag
	this->created = false;
	
	return this;
} // Destroy()



/**
 * Method for simulate shift to next frame in animation, if any animation 
 * exists and animation params are set or animation has'n ended yet.
 * @return bool Returns true, if shift to next frame could be done, false otherway.
 * @throw InvalidStateException Throw when scene has'n been created yet.
 */ 
bool Scene::Next() throw(InvalidStateException)
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// If playing animation is already ended
	if(this->animManager->IsEnded()) {
		return false;
	}
	
	
	// Go to next frame
	this->animManager->NextFrame();
	
	
	// Animate scene at this time
	this->Animate();
	
	
	return true;
} // Next()



/**
 * Method for simulate shift to previous frame in animation, if any animation 
 * exists and animation params are set or animation has'n ended yet.
 * @return bool Returns true, if shift to previous frame could be done, false otherway.
 * @throw InvalidStateException Throw when scene has'n been created yet.
 */ 
bool Scene::Prev() throw(InvalidStateException)
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If playing animation is already ended
	if(!this->animManager->IsStarted()) {
		return false;
	}
	
	
	// Go to next frame
	this->animManager->PrevFrame();
	
	
	// Animate scene at this time
	this->Animate();
	
	
	return true;
} // Prev()



/**
 * Method for sets new frame of animation and animate to this frame.
 * @param unsigned f Number of requested frame.
 * @return bool Returns true, if given frame is valid, false otherwise.
 */ 
bool Scene::GotoFrame(const unsigned &f)
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// Sets actual frame and animate
	this->animManager->GotoFrame(f);
	this->Animate();	
	
	
	return true;
} // GotoFrame()



/**
 * Method for sets new time of animation and animate to this time.
 * @param float t Requested time.
 * @return bool Returns true, if given time is valid, false otherwise.
 */ 
bool Scene::GotoTime(const float &t)
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// Sets actual frame and animate
	this->animManager->GotoTime(t);
	this->Animate();	
	
	
	return true;	
} // GotoTime()



/**
 * Method for reset animation to the start, i.e. first frame.
 * @return bool Returns true, if it is posible go to start, false otherwise.
 */ 
bool Scene::GotoStart()
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// Sets actual frame and animate
	this->animManager->Reset();
	this->Animate();	
	
	
	return true;
} // GotoStart()



/**
 * Method for reset animation to the end, i.e. last frame.
 * @return bool Returns true, if it is posible go to end, false otherwise.
 */ 
bool Scene::GotoEnd()
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// Sets actual frame and animate
	this->animManager->End();
	this->Animate();	
	
	
	return true;
} // GotoEnd()



/**
 * Auxiliary private method for do all maths for animate scene to given frame or
 * time which is set in animation manager.
 * @return Scene Provides fluent interface.
 */ 
Scene *Scene::Animate()
{
	// If scene has'n been created yet, crete it first
	if(!this->IsCreated()) {
		throw InvalidStateException("Scene has not been created yet!");
	}
	
	
	// If is there any animation (i.e. Animation Manager is instantiated)
	if(this->animManager == NULL) {
		return false;
	}
	
	
	// Calculate and set new position of all animated lights
	for(unsigned i = 0; i < this->animLights.Size(); i++) {
		this->animLights[i]->position = this->animManager->GetTranslation(this->animLights[i]->name);
	}
	
	
	// Calculate and set new position of all animated triagles
	for(unsigned i = 0; i < this->animTriangles.Size(); i++) {
		this->animTriangles[i]->Lerp(this->animManager->GetActualTime());
	}
	
	
	// Calculate and set new transformation matrix for all animated scene nodes
	for(unsigned i = 0; i < this->animNodes.Size(); i++) {
		Matrix m = this->animManager->GetMatrix(this->animNodes[i]->GetName());
		this->animNodes[i]->SetMatrix(m);
	}
	
	
	// Transform all animated nodes from root ones
	for(unsigned i = 0; i < this->rootAnimNodes.Size(); i++) {
		this->rootAnimNodes[i]->Transform(this->rootAnimNodes[i]);
	}
	
	
	// Animate camera (sets new position, direction and up vector)
	this->animManager->GetCamera(this->env->camera);
	
	return this;
} // Animate()



/**
 * Method for get texture with given name. If texture not exists in texture list
 * yet it will be created as new one and this new texture will be returns.
 * @param string dir Texture directory
 * @param string name Texture name
 * @return Texture Returns texture with given name or NULL.
 */ 
Texture *Scene::GetTexture(const string &dir, const string &name)
{
	string fname = dir + name;
	string lname = StrToLower(name);
	
	// If name is empty string or is NULL, nothing to find
	if(lname == "" || lname == "null") {
		return NULL;
	}
	
	// Else try to find testure by name 
	for(unsigned i = 0; i < this->textures.Size(); i++) {
		if(this->textures[i]->GetName() == fname) {
			return this->textures[i];
		}
	}
	
	// Texture with given name not exists yet
	Texture *texture = new ImageTexture(fname);
	if(texture != NULL) {
		this->textures.Add(texture);
		return texture;
	}
	
	return NULL;
} // GetTexture()
 


/** ***************************************************************************/
/** API for initialize scene properties										  */
/** ***************************************************************************/



/**
 * Initialize background color to given color 'c'.
 * @param Color c Given background color.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::InitBackgroundColor(const Color &c)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// Sets new background color to enviroment params
	this->env->bgColor = c;
	
	return this;
} // InitBackgroundColor()



/**
 * Initialize scene ambient light color to given color 'c'.
 * @param Color c Given ambient light color.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::InitAmbientLight(const Color &c)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// Sets new ambient light to environment params
	this->env->ambientColor = c;
	
	return this;
} // InitAmbientLight()



/**
 * Initialize scene camera - view point.
 * @param Vector3 from Camera position in the scene,
 * @param Vector3 at Camera direction vector (look at),
 * @param Vector3 up Camera up direction (mostly y axis),
 * @param float angle Angle between top and bottom bar of view - field of view,
 * @param float hither Distance between camera position and near plane,
 * @param Vector2 res Camera resolution - i.e. width and height of the final picture.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::InitCamera(const Vector3 &from, const Vector3 &at, const Vector3 &up, const float &angle, const float &hither, const Vector2 &res)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// If camera has been already initialized, delete it first
	if(this->env->camera) {
		delete this->env->camera;
		this->env->camera = NULL;
	}
	
	// Creates new camera object
	this->env->camera = new Camera(from, at, up, angle, hither, res);

	return this;
} // InitCamera()



/**
 * Initialize animation parameters (start, end and frames count).
 * @param float start Time when animation starts,
 * @param float end Time when animation ends,
 * @param unsigned frames Total count of animation frames (time step is (end-start) / (frames-1)).
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::InitAnimationParams(const float &start, const float &end, const unsigned &frames, const int &loops)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// If animation manager does't exist
	if(this->animManager == NULL) {
		this->animManager = new AnimationManager();
	}
	
	// Sets player settings
	this->animManager->SetPlayerSettings(start, end, frames, loops);
	
	return this;
} // InitAnimationParams()



/**
 * Creates new light entity with given name, pos and color and adds to scene.
 * @param string name Light name if light will be animated,
 * @param Vector3 pos Light position in scene,
 * @param Color c Light color.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddLight(const string &name, const Vector3 &pos, const Color &c)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	
	// Creates new light
	TLight *light = new(nothrow) TLight;
	if(light != NULL) {
		light->name = name;
		light->color = c;
		light->position = pos;
		
		// Adds new light to light list
		this->lights.Add(light);
		
		// If light has name, it will be animated, adds to list of animated lights to
		if(name != "") {
			this->animLights.Add(light);
		}
	}
	
	
	return this;
} // AddLight()



/**
 * Creates and store new material (basic material type).
 * @param Color c Material color,
 * @param float kd Diffuse constant multipliyer,
 * @param float ks Specular constant multipliyer,
 * @param float s Material shine multipliyer,
 * @param float ior Material index of refraction,
 * @param float t Material transmittance.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddMaterial(const Color &c, const float &kd, const float &ks, const float &s, const float &ior, const float &t)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	
	// Creates new material
	Material *mat = new(nothrow) Material(c, this->env->ambientColor, kd, ks, s, t, ior);
	if(mat != NULL) {
		// Adds new material to material list
		this->materials.Add(mat);
	}
	
	return this;
} // AddMaterial()



/**
 * Creates and store new material (extended material type).
 * @param Color ac Material ambient color component,
 * @param float dc Material diffuse color component,
 * @param float sc Material specular colo component,
 * @param float s Material shine multipliyer,
 * @param float ior Material index of refraction,
 * @param float t Material transmittance.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddMaterial(const Color &ac, const Color &dc, const Color &sc, const float &s, const float &ior, const float &t)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	
	// Creates new material
	Material *mat = new(nothrow) Material(Color::White(), ac, dc, sc, s, t, ior);
	if(mat != NULL) {
		// Adds new material to material list
		this->materials.Add(mat);
	}
	
	
	return this;
} // AddMaterial()



/**
 * Adds new animation to scene to animation manager.
 * @param Animation a The animation which will be added to list.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddAnimation(Animation *a)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// If animation list not initilized yet
	if(this->animManager == NULL) {
		this->animManager = new AnimationManager();
	}
	
	// Try to adds new animation to list
	if(!this->animManager->Add(a) && a != NULL) {
		delete a;
	}
	
	return this;
} // AddAnimation()



/**
 * Adds new animated transform (animated scene node) with given name 'n'.
 * @param string n Animated scene node name (equal to animation name).
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddTransform(const string &n)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// Creates new scene node and adds into actualy selected node
	SceneNode *node = new SceneNode(this, this->actualNode, n);
	if(node != NULL && this->actualNode != NULL) {
		this->actualNode->AddChild(node);
		
		// Add this node into animated nodes
		this->animNodes.Add(node);
		
		// If this node is in static branch, it must be a root animated node
		if(SceneNode::CalcNodeBranch(this->actualNode)) {
			this->rootAnimNodes.Add(node);
		}
		
		this->actualNode = node;
	}
	
	return this;
} // AddTransform()



/**
 * Adds new static transformation (static scene node).
 * @param Vector3 t Translation vector,
 * @param Vector3 r Rotation axis,
 * @param float a Rotation angle,
 * @param Vector3 s Scale vector.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddTransform(const Vector3 &t, const Vector3 &r, const float &a, const Vector3 &s)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// Init auxiliary variables
	Matrix m;
	Quaternion q;
	Vector4 rot(r.x(), r.y(), r.z(), a);
	
	// Creates rotation matrix
	rot >> q;
	m << q;

	// Add scales to matrix
	m[0][0] *= s.x(); m[0][1] *= s.y(); m[0][2] *= s.z();
	m[1][0] *= s.x(); m[1][1] *= s.y(); m[1][2] *= s.z();
	m[2][0] *= s.x(); m[2][1] *= s.y(); m[2][2] *= s.z();
	
	// Add translation to matrix
	m[0][3] = t.x();
	m[1][3] = t.y();
	m[2][3] = t.z();
	
	// Creates new scene node
	SceneNode *node = new SceneNode(this, this->actualNode, "", m);
	if(node != NULL && this->actualNode != NULL) {
		this->actualNode->AddChild(node);
		this->actualNode = node;
	}	
	
	return this;
} // AddTransform()



/**
 * Method which will to try close actual scene node.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::CloseTransform()
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	// Try to close actual transform, if transform cannot be closed, throw exception
	if(this->actualNode == NULL || this->actualNode->IsRoot()) {
		throw UnexpectedValueException("Root tranformation node cannot be closed!");
	}
	
	// Close acutal tranform
	this->actualNode = this->actualNode->GetParent();
	
	return this;
} // CloseTransform()



/**
 * Adds cone or cylinder primitive to scene.
 * @param string bp Base directory name for texture,
 * @param string tn Texture name,
 * @param Vector3 b Base center position,
 * @param float br Base radius,
 * @param Vector3 a Apex center position,
 * @param float ar Apes radius.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when actual scene node is NULL.
 */ 
Scene *Scene::AddCone(const string &bp, const string &tn, const Vector3 &b, const float &br, const Vector3 &a, const float &ar)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	else if(this->actualNode == NULL) {
		throw RuntimeException("There is no selected node in scene!"); 
	}
	
	
	// If both radii (br and ar) are equal, this is the cylinder
	if(br == ar) {
		Primitive *cylinder = new Cylinder(this->materials.Last(), this->GetTexture(bp, tn), b, a, br);
		if(cylinder != NULL) {
			this->actualNode->AddPrimitive(cylinder);
			this->primitives.Add(cylinder);
		}		
	}
	else {
		Primitive *cone = new Cone(this->materials.Last(), this->GetTexture(bp, tn), b, br, a, ar);
		if(cone != NULL) {
			this->actualNode->AddPrimitive(cone);
			this->primitives.Add(cone);
		}		
	}
	
	
	return this;
} // AddCone()



/**
 * Adds sphere primitive to scene.
 * @param string bp Base directory name for texture,
 * @param string tn Texture name,
 * @param Vector3 c Sphere center,
 * @param float r Sphere radius. 
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when actual scene node is NULL.
 */ 
Scene *Scene::AddSphere(const string &bp, const string &tn, const Vector3 &c, const float &r)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	else if(this->actualNode == NULL) {
		throw RuntimeException("There is no selected node in scene!"); 
	}
	
	
	// Creates new sphere
	Primitive *sphere = new(nothrow) Sphere(this->materials.Last(), this->GetTexture(bp, tn), c, r, !this->actualNode->IsStaticBranch());
	if(sphere != NULL) {
		this->actualNode->AddPrimitive(sphere);
		this->primitives.Add(sphere);
	}
	
	
	return this;
} // AddSphere()



/**
 * Adds polygon or polygonal patch primitive to scene.
 * @param Vector3 v Array of polygon's vertices,
 * @param Vector3 n Array of polygon's normals,
 * @param unsigned c Polygon vertices count.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when actual scene node is NULL.
 */ 
Scene *Scene::AddPolygon(Vector3 *v, Vector3 *n, const unsigned &c)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	else if(this->actualNode == NULL) {
		throw RuntimeException("There is no selected node in scene!"); 
	}
	else if(c < 3) {
		throw InvalidArgumentException("To low vertices in polygon!");
	}
	
	
	// Creates new polygon
	/*
	Primitive *poly = new(nothrow) Polygon(this->materials.Last(), NULL, v, n, c);
	if(poly != NULL) {
		this->actualNode->AddPrimitive(poly);
		this->primitives.Add(poly);
	}
	*/
	
	for(unsigned i = 0; i < c-2; i++) {
		Vector3 verts[3];
		verts[0] = v[0];
		verts[1] = v[i+1];
		verts[2] = v[i+2];
		
		Vector3 norms[3];
		if(n != NULL) {
			norms[0] = n[0];
			norms[1] = n[i+1];
			norms[2] = n[i+2];
			this->AddTriangle("", "", verts, norms, NULL);
		}
		else {
			this->AddTriangle("", "", verts, NULL, NULL);
		}
	}
	
	
	return this;
} // AddPolygon()



/**
 * Adds triangle to scene.
 * @param string bp Base directory name for texture,
 * @param string tn Texture name,
 * @param Vector3 v 3-link array of triangle's vertices,
 * @param Vector3 n Array of triangle's normals,
 * @param Vector2 c Array of triangle's texture coordinates.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when actual scene node is NULL.
 */ 
Scene *Scene::AddTriangle(const string &bp, const string &tn, const Vector3 v[3], Vector3 *n, Vector2 *c)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	else if(this->actualNode == NULL) {
		throw RuntimeException("There is no selected node in scene!"); 
	}
	
	
	// Creates new triangle
	Primitive *triangle = new(nothrow) Triangle(this->materials.Last(), this->GetTexture(bp, tn), v, n, c, !this->actualNode->IsStaticBranch());
	if(triangle != NULL) {
		this->actualNode->AddPrimitive(triangle);
		this->primitives.Add(triangle);
	}
	
	
	return this;
} // AddTriangle()



/**
 * Adds triangle mesh to scene.
 * @param Vector3 v Array of all mesh vertices,
 * @param unsigned vc Vertices count,
 * @param Vector3 n Array of all mesh normals, if any,
 * @param unsigned nc Normals count,
 * @param Vector2 c Array of all mesh texture coordinates, if any,
 * @param unsigned cc Texture coordinates count, 
 * @param string bp Base directory name for texture,
 * @param string tn Texture name,
 * @param unsigned short i Array of all triangle indices,
 * @param unsigned ic Count of all mesh triangles.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 */ 
Scene *Scene::AddMesh(Vector3 *v, const unsigned &vc, Vector3 *n, const unsigned &nc, Vector2 *c, const unsigned &cc, const string &bd, const string &tn, unsigned short *i, const unsigned &ic)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	
	
	// Init auxiliary variables
	unsigned index = 0;
	unsigned vi[3], ni[3], ci[3];
	Vector3 V[3];
	Vector3 *N = (nc > 0) ? new Vector3[3] : NULL;
	Vector2 *C = (cc > 0) ? new Vector2[3] : NULL;
	
	// Iterate over all indices in array
	for(unsigned j = 0; j < ic; j++) {		
		// Iterate over 3 triangle vertices
		for(unsigned k = 0; k < 3; k++) {
			if(cc > 0) {
				ci[k] = i[index++];
				C[k] = c[ci[k]];
			}
			if(nc > 0) {
				ni[k] = i[index++];
				N[k] = n[ni[k]];
			}
			if(vc > 0) {
				vi[k] = i[index++];
				V[k] = v[vi[k]];
			}
		}		
		
		// Adds new triangle to scene
		this->AddTriangle(bd, tn, V, N, C);
	}
	
	// Delete auxiliary arrays
	if(N != NULL) delete [] N;
	if(C != NULL) delete [] C;
	
	
	return this;
} // AddMesh()



/**
 * Adds special animated triangle primitive to scene.
 * @param unsigned c Key frames count,
 * @param float t Array of times for each key frame,
 * @param Vector3 v Array of all vertices (3 for each triangle at one key frame),
 * @param Vector3 n Array of all normals.
 * @return Scene Provides fluent interface.
 * @throw InvalidStateException Throw when scene has been already created.
 * @throw RuntimeException Throw when actual scene node is NULL.
 */ 
Scene *Scene::AddAnimatedTriangle(const unsigned &c, float *t, Vector3 *v, Vector3 *n)
{
	// If scene has been already created, can't be modified!
	if(this->IsCreated()) {
		throw InvalidStateException("Scene has been already created! Cannot modified scene, destroy it first."); 
	}
	else if(this->actualNode == NULL) {
		throw RuntimeException("There is no selected node in scene!"); 
	}
	
	
	// Creates new animated triangle and adds to selected node
	Primitive *atri = new AnimatedTriangle(this->materials.Last(), NULL, c, t, v, n);
	if(atri != NULL) {
		Primitive *triangle = static_cast<AnimatedTriangle*>(atri)->GetTriangle();
		
		this->actualNode->AddPrimitive(triangle);
		this->primitives.Add(triangle);
		this->animTriangles.Add(static_cast<AnimatedTriangle*>(atri));
	}
	
	
	return this;
} // AddAnimatedTriangle()



/** ***************************************************************************/
/** end of file Scene.cpp													  */
/** ***************************************************************************/
