/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Material.cpp													  *	
 * @date	29. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Material.								  *
 **************************************************************************** */


// Including project libraries
#include "Material.h"



/**
 * Default constructor. Creates absolute black material.
 * @return Material.
 */ 
Material::Material()
{
	this->ambient = Color::White();
	this->diffuse = Color::White();
	this->specular = Color::Black();
	this->reflection = Color::Black();
	this->shine = 0.0f;
	this->refraction = 0.0f;
	this->ior = 1.0f;	
} // Material()



/**
 * Default constructor. Creates basic material with texture.
 * @param Texture tex Instance of given texture,
 * @param Color amb Ambient color component,
 * @param float kd Diffuse power,
 * @param float ks Specular power,
 * @param float sh Specular cosine exponent,
 * @param float tr Transmittance,
 * @param float ior Material index of refraction.
 * @return Material.
 */ 
Material::Material(const Color &clr, const Color &amb, const float &kd, const float &ks, const float &sh, const float &tr, const float &ior)
{
	this->color = clr;
	this->ambient = amb;
	this->diffuse = Color(kd, kd, kd);
	this->specular = Color(ks, ks, ks);
	this->reflection = this->SetReflection();
	this->shine = sh;
	this->refraction = tr;
	this->ior = ior;
} // Material()



/**
 * Default constructor. Creates basic material with texture.
 * @param Texture tex Instance of given texture,
 * @param Color amb Ambient color component,
 * @param Color dif Diffuse color component,
 * @param Color spc Specular color component,
 * @param float sh Specular cosine exponent,
 * @param float tr Transmittance,
 * @param float ior Material index of refraction.
 * @return Material.
 */ 
Material::Material(const Color &clr, const Color &amb, const Color &dif, const Color &spc, const float &sh, const float &tr, const float &ior)
{
	this->color = clr;
	this->ambient = amb;
	this->diffuse = dif;
	this->specular = spc;
	this->reflection = this->SetReflection();
	this->shine = sh;
	this->refraction = tr;
	this->ior = ior;
} // Material()	



/** ***************************************************************************/
/** end of file Material.cpp												  */
/** ***************************************************************************/
