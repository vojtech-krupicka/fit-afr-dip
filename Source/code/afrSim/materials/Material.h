/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Material.h														  *	
 * @date	29. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Material.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_MATERIAL_H
#define _AFR_MATERIAL_H



// Including project libraries
#include "Texture.h"
#include "../types/Color.h"
#include "../types/Array.h"
#include "../types/Vector2.h"
#include "../types/Vector3.h"



/**
 * Class Material
 * @brief Class for defining material and its settings.
 */ 
class Material
{
public:
	///< Default constructors
	Material();
	Material(const Color &clr, const Color &amb, const float &kd, const float &ks, const float &sh, const float &tr, const float &ior);
	Material(const Color &clr, const Color &amb, const Color &dif, const Color &spc, const float &sh, const float &tr, const float &ior);
	
	///< Default destructors
	~Material() {};
	
	///< Getters
	inline Color 	GetColor() const { return this->color; };
	inline Color 	GetAmbient() const { return this->ambient; };
	inline Color 	GetDiffuse() const { return this->diffuse; };
	inline Color 	GetSpecular() const { return this->specular; };
	inline Color 	GetReflection() const { return this->reflection; };
	inline float	GetShine() const { return this->shine; };
	inline float	GetRefraction() const { return this->refraction; };
	inline float	GetIor() const { return this->ior; };
	
	///< Setters
	inline Material *SetColor(const Color &clr) { this->color = clr; return this; };
	inline Material *SetAmbient(const float &amb) { this->ambient = Color(amb, amb, amb); return this; };
	inline Material *SetAmbient(const Color &amb) { this->ambient = amb; return this; };
	inline Material *SetDiffuse(const float &dif) { this->diffuse = Color(dif, dif, dif); this->reflection = this->SetReflection(); return this; };
	inline Material *SetDiffuse(const Color &dif) { this->diffuse = dif; return this; };
	inline Material *SetSpecular(const float &spc) { this->specular = Color(spc, spc, spc); this->reflection = this->SetReflection(); return this; };
	inline Material *SetSpecular(const Color &spc) { this->specular = spc; return this; };
	inline Material *SetShine(const float &sh) { this->shine = sh; return this; };
	inline Material *SetRefraction(const float &tr) { this->refraction = tr; return this; };
	inline Material *SetIor(const float &ior) { this->ior = ior; return this; };
	
	inline bool		HasReflections() const { return this->reflection != Color::Black(); };
	inline bool		HasRefractions() const { return this->refraction > 0.0f; };
	
	///< Material operations
	inline Color	GetBaseColor(Texture *t, const Vector2 &c, const Vector3 &h);
	inline Color	GetDiffuseColor(Texture *t, const Vector2 &c, const Vector3 &h, const Color &lc, const float &ln);
	inline Color	GetSpecularColor(const Color &lc, const float &rv);
	inline Color	GetRefractedColor(Texture *t, const Vector2 &c, const Vector3 &h);
	
private:
	inline Color	GetSolidColor(Texture *t, const Vector2 &c, const Vector3 &h);
	inline Color 	SetReflection();

	Color	color;
	Color 	ambient;
	Color	diffuse;
	Color	specular;
	Color	reflection;
	float	shine;
	float 	refraction;
	float	ior;
}; // class Material



/**
 * Define new type for list of materials as dynamic array of materials where 
 * material is a pointer on a class Material.
 */ 
typedef Array<Material*> TMaterialList;



/**
 * Method for calculate real base material color from texture or solid color.
 * @param Texture t Texture object, if any,
 * @param Vector2 c Texture coordinates,
 * @param Vector3 h Hit point for procedural textures.
 * @return Color Returns real base material color.
 */ 
inline Color Material::GetBaseColor(Texture *t, const Vector2 &c, const Vector3 &h)
{
	Color temp = this->GetSolidColor(t, c, h);
	return (this->ambient * temp);
} // GetBaseColor()



/**
 * Method for calculate diffuse material color.
 * @param Texture t Texture object, if any,
 * @param Vector2 c Texture coordinates,
 * @param Vector3 h Hit point for procedural textures.
 * @param Color lc Light color,
 * @param float ln Dot product between surface normal and light vector.
 * @return Color Returns real base material color.
 */
inline Color Material::GetDiffuseColor(Texture *t, const Vector2 &c, const Vector3 &h, const Color &lc, const float &ln)
{
	Color temp = this->GetSolidColor(t, c, h);
	return (lc * ln * this->diffuse * temp);
} // GetDiffuseColor()



/**
 * Method for calculate specular material color.
 * @param Color lc Light color,
 * @param float rv Dot product between eye position and reflected ray.
 * @return Color Returns real base material color.
 */
inline Color Material::GetSpecularColor(const Color &lc, const float &rv)
{
	return (lc * pow(rv, this->shine) * this->specular);
} // GetSpecularColor()



/**
 * Method for calculate refracted material color.
 * @param Texture t Texture object, if any,
 * @param Vector2 c Texture coordinates,
 * @param Vector3 h Hit point for procedural textures.
 * @return Color Returns transparency color.
 */ 
inline Color Material::GetRefractedColor(Texture *t, const Vector2 &c, const Vector3 &h)
{
	Color absorbance = this->GetSolidColor(t, c, h) * this->refraction;
	return absorbance;
	return Color(expf(absorbance.r()), expf(absorbance.g()), expf(absorbance.b()));	
} // GetRefractedColor()



/**
 * Auxiliary method for get material color at given point.
 * @param Texture t Texture object, if any,
 * @param Vector2 c Texture coordinates,
 * @param Vector3 h Hit point for procedural textures.
 * @return Color Returns real base material color.
 */ 
inline Color Material::GetSolidColor(Texture *t, const Vector2 &c, const Vector3 &h)
{
	// If there is a given texture, use color at the specific position in the bitmap
	if(t != NULL) {
		return t->GetTexel(c, h);
	}
	// Else use solid material color
	else {
		return this->color;
	}
	
	return Color::Black();
} // GetSolidColor()



/**
 * Auxiliary method for set right value to reflection power.
 * @return Color Returns reflection power as three link color.
 */ 
inline Color Material::SetReflection()
{
	if(this->diffuse == Color::Black()) {
		return this->specular;
	}
	else {
		return (this->diffuse * this->specular);
	}
	return Color::Black();
} // SetReflection()



#endif // #ifndef _AFR_MATERIAL_H



/** ***************************************************************************/
/** end of file Material.h													  */
/** ***************************************************************************/
