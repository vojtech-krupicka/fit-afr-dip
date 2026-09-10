/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Primitive.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Primitive.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_PRIMITIVE_H
#define _AFR_PRIMITIVE_H


// Including project libraries
#include "../materials/Material.h"
#include "../types/Array.h"
#include "../types/Color.h"
#include "../types/Vector2.h"
#include "../types/Vector3.h"
#include "../types/Matrix.h"
#include "../types/Vertex.h"
#include "../types/Edge.h"


// Predefinition of classes
class Ray;
class Matrix;
class Vector2;
class Vector3;
class Texture;
class Primitive;
class BBox;


/**
 * @brief Structure s_hitinfo for store infomations about intersect between ray and 
 * primitive surface in scene. Defines new type THitInfor.
 */ 
typedef struct s_hitinfo {
	float 		t;
	Vector3 	hit;
	Vector3 	normal;
	Vector2 	coords;
	Primitive 	*primitive;
	float		sign;
} THitInfo;	



/**
 * Class Primitive
 * @brief Abstract class for definitions of all primitives in the scene.
 */ 
class Primitive
{
public:
	///< Implicit constructors
	Primitive();
	Primitive(Material *m, Texture *t);

	///< Implicit destructors
	virtual ~Primitive();

	///< Getters
	inline Texture *GetTexture() const { return this->texture; };
	inline Material *GetMaterial() const { return this->material; };

	///< Primitive operations
	virtual Primitive *Hit(const Ray &r, const float &tmin, float &tmax, const bool &cull = true) = 0;
	virtual bool GetHitInfo(const Ray &r, THitInfo &info) = 0;
	
	virtual BBox *GetBBox() = 0;
	virtual bool IntersectBBox(const BBox &box) const = 0;
	
	virtual Primitive *Transform(const Matrix &m) = 0;
	virtual void Render() const = 0;
	
protected:
	Texture 	*texture;
	Material	*material;
	BBox		*bbox;
}; // class Primitive



/**
 * Defines new type for list of primitives in the scene as dynamic array of 
 * pointers at primitive instance.
 */
typedef Array<Primitive*> TPrimitiveList;



#endif // #ifndef _AFR_PRIMITIVE_H



/** ***************************************************************************/
/** end of file Primitive.h													  */
/** ***************************************************************************/
