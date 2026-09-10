/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Raytracer.cpp													  *	
 * @date	24. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Raytracer.								  *
 **************************************************************************** */



// Including project libraries
#include "Raytracer.h"

#include "KdTree.h"
#include "KdTreeNode.h"

#include "../scene/Camera.h"
#include "../scene/Scene.h"
#include "../types/Image.h"
#include "../types/Ray.h"
#include "../types/Vector2.h"
#include "../types/Vector3.h"
#include "../utils/Random.h"



// Define some macros and constants
#define TMIN 	0.001f
#define TMAX 	1000000.0f
#define EPS 	0.00001f



/**
 * Default constructor.
 * @return Raytracer.
 */ 
Raytracer::Raytracer()
{
	this->Init();
} // Raytracer()



/**
 * Default constructor. Sets scene, kd tree and prepare properties.
 * @param Scene s Object of scene.
 * @param Tree t Object of scene kd tree.
 * @return Raytracer.
 */ 
Raytracer::Raytracer(Scene *s, KdTree *t)
{
	// Check if scene is set
	if(s == NULL) {
		throw InvalidStateException("<Raytracer>: Scene is NULL!");
	}
	
	// First init raytracer with defaults;
	this->Init();
	
	// Sets scene, tree, camera and properties derivered from scene
	this->scene = s;
	this->tree = t;
	this->camera = s->GetCamera();
	this->width = this->camera->GetWidth();
	this->height = this->camera->GetHeight();
	this->ior = s->GetEnvIor();
	this->bgColor = s->GetBackgroundColor();	
} // Raytracer()



/**
 * Default desctructor. Destroy raytracer.
 */ 
Raytracer::~Raytracer() 
{
	// Delete KD Tree
	if(this->tree != NULL) {
		delete this->tree;
		this->tree = NULL;
	}
	
	// All object has been set to NULL
	this->Init();	
} // ~Raytracer()



/**
 * Initialize raytracer properties.
 * @return Raytracer Provides fluent interface.
 */ 
Raytracer *Raytracer::Init()
{
	// Init all properties
	this->scene = NULL;
	this->tree = NULL;
	this->useTree = false;
	
	this->camera = NULL;
	this->bgColor = Color::Black();
	this->ior = 1.0f;
	this->depth = 1;
	this->msaa = 1;
	this->subSamples = 1;
	this->useMsaa = false;
	
	// Init MC Grid with random values
	Random rnd;
	for(int i = 0; i < MC_GRID_SIZE; i++) {
		for(int j = 0; j < MC_GRID_SIZE; j++) {
			this->mcGrid[i][j] = Vector2(rnd(), rnd());
		}	
	}
	
	return this;
} // Init()



/**
 * Main operation for start raytracer and do all maths.
 * @param Image image Image which will be computed.
 * @return bool Return true, if rendering is succesfull, false otherwise.
 */ 
bool Raytracer::Run(Image *(&image))
{
	// Init auxiliary variables
	Color final;
	
	// Image is NULL, create new blank one
	if(image == NULL) {
		image = new Image(this->width, this->height, this->bgColor);
	}
	// Image is already exists, resize and fill with bg color
	else {
		image->Clear(this->width, this->height, this->bgColor);
	}
	
	
	// Loop over all lines and all columns and compute color for all pixels
	for(unsigned y = 0; y < this->height; y++) {
		// Debug messages
		if ((y % 10) == 0) {
			cout << endl << "Rendering line: ";
		}
		printf("\t%5d", (y + 1));
		
		for(unsigned x = 0; x < this->width; x++) {
			// Compute pixel color
			final = this->RunOnce(x, y);
			
			// Write color into image
			image->SetPixel(x, y, final);
		}
	}
	
	cout << endl << endl;
	
	return true;
} // Run()



/**
 * Method for trace only one ray through one specified point at screen plane. 
 * Both dimensions should be in range <0, width), <0, height). Method calculate 
 * one color at this position and return it.
 * @param unsigned x X position at screen plane,
 * @param unsigned y Y position at screen plane.
 * @return Color Returns calculated color at specified point.
 */ 
Color Raytracer::RunOnce(const float &x, const float &y)
{
	// Init color and get camera position
	Color final = this->bgColor;
	Vector3 camPos = this->camera->GetPosition();
	
	// Prepare absolute and relative position of pixel
	unsigned absPosX = (unsigned)x;
	unsigned absPosY = (unsigned)y;
	float diffX = Abs(x - (float)absPosX);
	float diffY = Abs(y - (float)absPosY);
	Vector3 increment(diffX / this->width, diffY / this->height, 0.0f);
	
	// Calculate offset at screen plane for current sample
	Vector3 offset = this->camera->GetOffset(x, y) + increment;


	// Trace ray, or use multisampling
	if(this->useMsaa && this->subSamples > 1) {
		final = this->Multisample(offset);
	}
	else {
		// Create ray between camera position and offset on the screen plane
		Vector3 dir = (offset - camPos).Normalize();
		Ray r(camPos, dir);
		
		// Trace ray
		this->Trace(r, final, this->ior, 0);
	}
	
	// Return final color
	return final;
} // RunOnce()



/**
 * Main method for trace one ray in scene and calculate final color.
 * @param Ray ray Actualy traced ray in scene,
 * @param Color color Final color of sample for given ray,
 * @param float ior Actual index of refraction,
 * @param unsigned level Actual depth of recursion for reflected rays. 
 * @return Primitive Returns the closes primitive.
 */ 
Primitive *Raytracer::Trace(const Ray &ray, Color &color, const float &ior, const unsigned &level, const bool &cull)
{
	// Test, if recursion level is exceeded
	if(level >= this->depth) {
		return NULL;
	}
	
	
	float tmax = TMAX;
	Primitive *prim = this->Hit(ray, NULL, tmax, cull);
	if(prim == NULL) {
		return NULL;
	}
	
	
	THitInfo info;
	info.t = tmax;
	info.hit = ray.PointAt(tmax);
	if(!prim->GetHitInfo(ray, info)) {
		return NULL;
	}
	
	
	color = this->CalcShading(ray, info);
	color += this->CalcReflection(ray, info, level);
	color += this->CalcRefraction(ray, info, ior, level);
	
	
	return prim;
} // Trace()



/**
 * Method for find closes primitive in scene to given ray.
 * @param Ray ray Actualy traced ray in scene,
 * @param Primitive p Actualy hited primitive to avoid hit the same primitive twice (for shading purposes),
 * @param float tmax Actualy 't' parameter at the ray.
 * @return Primitive Retuns the closes primitive in scene if any, or NULL.
 */ 
Primitive *Raytracer::Hit(const Ray &ray, Primitive *p, float &tmax, const bool &cull)
{
	// If KD Tree is defined and should be used
	if(this->useTree && this->tree != NULL) {
		// Init auxiliary variables
		float tNear = 0.0f;
		float tFar = tmax;
		float tSplit = 0.0f;

		Vector3 bMin = this->tree->GetSceneBox().GetMin();
		Vector3 bMax = this->tree->GetSceneBox().GetMax();
		Vector3 rO = ray.Origin();
		Vector3 rD = ray.Direction();
		
		
		// Find tNear and tMax - 't' params where ray intersect bounding box (in and out)
		for(int i = 0; i < 3; i++) {
			if (rD[i] <= 0.0f) {
				if(rO[i] < bMin[i]) return NULL;
			} 
			else {
				if(rO[i] > bMax[i]) return NULL;
			}
		}

		for(int i = 0; i < 3; i++) {
			float pos = ray.PointAt(tFar)[i];
			if(rD[i] < 0.0f) {
				if(pos < bMin[i]) {
					tFar = tNear + (tFar - tNear) * ((rO[i] - bMin[i]) / (rO[i] - pos));
				}
				if(rO[i] > bMax[i]) {
					tNear += (tFar - tNear) * ((rO[i] - bMax[i]) / (tFar * rD[i]));
				} 
			} 
			else {
				if(pos > bMax[i]) {
					tFar = tNear + (tFar - tNear) * ((bMax[i] - rO[i]) / (pos - rO[i]));
				}
				if(rO[i] < bMin[i]) {
					tNear += (tFar - tNear) * ((bMin[i] - rO[i]) / (tFar * rD[i]));
				}
			}

			if(tNear > tFar) return NULL;
		}
		
		
		// Init KdStack
		TKdStackItemList stack;
		for(int i = 0; i < 64; i++) {
			TKdStackItem tmp;
			tmp.node = NULL;
			tmp.t = 0.0f;
			tmp.prev = (i == 0) ? 0 : (i-1);
			
			stack.Add(tmp);
		}
		
		short in = 0; 
		short out = 1;
		stack[out].t = tFar;
		stack[out].hit = ray.PointAt(tFar);
		stack[out].node = NULL;
		
		stack[in].t = tNear;
		if(tNear > 0.0f) {
			stack[in].hit = ray.PointAt(tNear);
		}
		else {
			stack[in].hit = rO;
		}

		// Auxiliary pointers to nodes
		KdTreeNode *far = NULL;
		KdTreeNode *near = NULL;
		KdTreeNode *node = this->tree->GetRoot();
		
		
		// Traverse tree from root
		while(node != NULL) {
			// Until the node is leaf (ie it hasn't childes)
			while(!node->IsLeaf()) {
				float sPos = node->GetSplitPosition();
				short sPlane = node->GetSplitPlane();
				
				if(stack[in].hit[sPlane] < sPos) {
					if(stack[out].hit[sPlane] < sPos) {
						node = node->GetLeftChild();
						continue;
					}

					if(stack[out].hit[sPlane] == sPos) {
						node = node->GetRightChild();
						continue;
					}

					near = node->GetLeftChild();
					far = node->GetRightChild();
					node = near;
				}
				else {
					if(stack[out].hit[sPlane] >= sPos) {
						node = node->GetRightChild();
						continue;
					}

					near = node->GetRightChild();
					far = node->GetLeftChild();
					node = near;
				}
				
				tSplit = (sPos - rO[sPlane]) / rD[sPlane];
				short tmp = out++;
				if(out == in) {
					out++;
				}

				stack[out].t = tSplit;
				stack[out].node = far;
				stack[out].hit[sPlane] = sPos;
				stack[out].prev = tmp;
				
				short nextPlane = (sPlane + 1) % 3;
				short prevPlane = (sPlane + 2) % 3;
				stack[out].hit[nextPlane] = ray.PointAt(tSplit)[nextPlane];
				stack[out].hit[prevPlane] = ray.PointAt(tSplit)[prevPlane];
			}
			
			// Try to find primitiv in acutal node
			Primitive *closest = NULL;
			Primitive *current = NULL;
			TPrimitiveList geometry = node->GetPrimitiveList();
			unsigned primCount = geometry.Size();
			float tact = stack[out].t;
			float tmin = TMIN;
			
			// Find nearest primitive in node
			for(unsigned i = 0; i < primCount; i++) {
				if((current = geometry[i]->Hit(ray, tmin, tact, cull)) != NULL && current != p) {
					closest = current;
					tmax = tact;
				}
			}
			
			// If any primitiv has been finded, return it
			if(closest != NULL) {
				return closest;
			}

			// Go to next node
			in = out;
			node = stack[out].node;
			out = stack[in].prev;
		}
		
		
		// No primitiv has been hit
		return NULL;

	}	
	// Else use the simple naive way to find the nearest primitive
	else {
		// Init auxiliary variables
		Primitive *closest = NULL;
		Primitive *current = NULL;
		TPrimitiveList geometry = this->scene->GetPrimitiveList();
		unsigned count = geometry.Size();
		float tact = tmax;
		float tmin = TMIN;
		
		// Find nearest primitive in node
		for(unsigned k = 0; k < count; k++) {
			if((current = geometry[k]->Hit(ray, tmin, tact, cull)) != NULL && current != p) {
				closest = current;
				tmax = tact;
			}
		}
		
		return closest;
	}

	return NULL;
} // Hit()



/**
 * Method for calculate sample color using bilinear antialing, sample divides to
 * subsamples and cast rays throught this new subsamples. This is for 
 * antialiasing using supersampling.
 * @param Vector3 col Actual sample position on screen in 3D space.
 * @return Color Returns average color for propriate sample.
 */ 
Color Raytracer::Multisample(const Vector3 &pos)
{
	// Get camera position
	Vector3 camPos = this->camera->GetPosition();
	
	// Store auxiliaty row and col pointers
	Vector3 hPos = pos;
	Vector3 vPos = pos;
	
	// Prepare auxiliary variables for MSAA
	float msaaPower = 1.0f / (float)this->subSamples;
	
	Vector3 aaVStep = this->camera->GetVStep() / (float)this->msaa;
	Vector3 aaHStep = this->camera->GetHStep() / (float)this->msaa;
	
	// Define auxiliart variables
	Color final, color;
	Vector2 offset;
	Vector3 dir, offsetPos;
	Random rnd;
	
	
	// Calculate MSAA for grid 4*4
	for(unsigned i = 0; i < this->msaa; i++) {
		for(unsigned j = 0; j < this->msaa; j++) {
			offset = this->mcGrid[i % MC_GRID_SIZE][j % MC_GRID_SIZE];
			offsetPos = hPos + (offset.x() * aaHStep) + (offset.y() * aaVStep);
			
			// Create propriate ray
			dir = (offsetPos - camPos);
			Ray ray(camPos, dir);
			
			// Trace ray to scene and acumulate color
			color = this->bgColor;
			this->Trace(ray, color, this->ior, 0);
			final += color;
			
			// Go to next column
			hPos += aaHStep;
		}
		
		// Go to next row
		vPos += aaVStep;
		hPos = vPos;
	}
	
	// Return final color times msaa power (1 / subsamples)
	return (final * msaaPower);
} // Multisample()



/**
 * Method for calculate shading - diffuse and specular and shadows.
 * @param Ray ray Actual trace ray,
 * @param THitInfo info Informations about actual intersection.
 * @return Color Returns fragment color.
 */ 
Color Raytracer::CalcShading(const Ray &ray, THitInfo &info)
{
	Material *mat = info.primitive->GetMaterial();
	Color temp = mat->GetBaseColor(info.primitive->GetTexture(), info.coords, info.hit);
	
	Vector3 V = ray.d().Normalize();
	
	// For all lights in scene
	TLightList lights = this->scene->GetLightList();
	for(unsigned i = 0; i < lights.Size(); i++) {
		if(this->CalcShadow(lights[i], info)) {
			Vector3 L = (lights[i]->position - info.hit).Normalize();
			float LdotN = L.Dot(info.normal);
			
			if(LdotN > 0.0f) {
				temp += mat->GetDiffuseColor(info.primitive->GetTexture(), info.coords, info.hit, lights[i]->color, LdotN);
				
				Vector3 R = L - (info.normal * L.Dot(info.normal) * 2.0);
				float RdotV = R.Dot(V);
				if(RdotV > 0.0) {
					temp += mat->GetSpecularColor(lights[i]->color, RdotV);
				}
			}
		}
	} 
	
	return temp.Clamp();
} // CalcShading()



/**
 * Method for calculate reflections - cast new secondary rays and recursively
 * calls Trace method for tracing secondary rays.
 * @param Ray ray Actual trace ray,
 * @param THitInfo info Informations about actual intersection,
 * @param unsigned level Actual recursion level.
 * @return Color Returns fragment color.
 */ 
Color Raytracer::CalcReflection(const Ray &ray, THitInfo &info, const unsigned &level)
{
	// Get material info and check, if there are any reflections
	Material *mat = info.primitive->GetMaterial();
	if(info.sign <= 0.0f || !mat->HasReflections()) {
		return Color::Black();
	}
	
	// If any, calculate base reflected color
	Color refl = mat->GetReflection();
	
	// Then prepare new reflected ray and cast it into scene
	Vector3 V = ray.d().Normalize();
	Vector3 R = V - (2.0f * V.Dot(info.normal) * info.normal);
	Vector3 origin = info.hit + (R * EPS);
	
	Color color = this->bgColor;
	Ray ndRay(origin, R);
	
	if(this->Trace(ndRay, color, this->ior, (level+1))) {
		return (color * refl);
	}
	
	return Color::Black();
} // CalcReflection()



/**
 * Method for calculate refractions.
 * @param Ray ray Actual trace ray,
 * @param THitInfo info Informations about actual intersection,
 * @param unsigned level Actual recursion level.
 * @return Color Returns fragment color.
 */ 
Color Raytracer::CalcRefraction(const Ray &ray, THitInfo &info, const float &ior, const unsigned &level)
{
	Material *mat = info.primitive->GetMaterial();
	if(!mat->HasRefractions()) {
		return Color::Black();
	}
	

	float refrN, sinI, sinT, cosT;
	float iorNext = mat->GetIor();
	
	if(info.sign < 0.0f) {
		info.normal = -info.normal;
		iorNext = this->ior;
	}
	
	refrN = ior / iorNext;
	
	Vector3 rayDir = ray.d().Normalize();
	float cosI = fabs(info.normal.Dot(rayDir));
	
	if(cosI >= 0.99999) {
		cosI = 1.0;
		sinI = 0.0;
		sinT = 0.0;
		cosT = 1.0;
	}
	else {
		sinI = sqrtf(1.0 - (cosI * cosI));
		sinT = refrN * sinI;
		if((sinT * sinT) > 0.9999) {
			return Color::Black();
		}
		else {
			cosT = sqrtf(1.0 - (sinT * sinT));
		}
	}
	
	Vector3 dir = (info.normal * (-cosT)) + (((info.normal * cosI) + rayDir) * refrN);
	Vector3 origin = info.hit + (dir * EPS);
	
	Ray refrRay(origin, dir);
	Color color = this->bgColor;
	
	if(this->Trace(refrRay, color, iorNext, (level+1)), true) {
		Color refr = mat->GetRefractedColor(info.primitive->GetTexture(), info.coords, info.hit);
		return (color * refr);
	}
	
	return Color::Black();
} // CalcRefraction()



/**
 * Auxiliary method for determine, if actual fragment is occluded or not for
 * given light.
 * @param TLight light One of lights in scene,
 * @param THitInfo info Informations about actual intersection,
 * @return bool Returns true, if fragment is not occluded, false otherwise.
 */ 
bool Raytracer::CalcShadow(TLight *light, THitInfo &info)
{	
	Vector3 dir = info.hit - light->position;
	float length = dir.Length();
	dir = dir * (1.0f / length);
	Vector3 origin = light->position + (dir * EPS);
	
	Ray ray(origin, dir);
	if(this->Hit(ray, info.primitive, length, false) == NULL) {
		return true;
	}
	
	return false;
} // CalcShadow()



/** ***************************************************************************/
/** end of file Raytracer.cpp												  */
/** ***************************************************************************/
