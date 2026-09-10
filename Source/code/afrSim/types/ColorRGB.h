/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    ColorRGB.h														  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class ColorRGB.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_COLOR_RGB_H
#define _AFR_COLOR_RGB_H


// Including system libraries
#include <iostream>
#include <string>
#include <cmath>


// Including project libraries
#include "../utils/MathUtils.h"


// Used namespaces
using namespace std;


/**
 * Class ColorRGB
 * @brief Class for three color structure. Color's components 
 * are Red, Green and Blue.
 */ 
class ColorRGB
{
public:
	///< Implicit constructors
	inline ColorRGB();
	inline ColorRGB(const float data[3]);
	inline ColorRGB(const float &g);
	inline ColorRGB(const float &r, const float &g, const float &b);
	inline ColorRGB(const ColorRGB &c);
	
	///< Implicit destructors
	~ColorRGB() {};
	
	///< Accessors methods
	inline float r() const { return this->data[0]; };
	inline float g() const { return this->data[1]; };
	inline float b() const { return this->data[2]; };
	
	inline float operator[]	(const unsigned &i) const { return this->data[i]; };
	inline float &operator[] (const unsigned &i) { return this->data[i]; };
	
	///< Setters
	inline ColorRGB& operator() (const float &r, const float &g, const float &b);
	inline ColorRGB& r(const float &f) { this->data[0] = f; return *this; };
	inline ColorRGB& g(const float &f) { this->data[1] = f; return *this; };
	inline ColorRGB& b(const float &f) { this->data[2] = f; return *this; };
	
	///< Boolean operators
	friend bool operator==(const ColorRGB &c1, const ColorRGB &c2);
	friend bool operator!=(const ColorRGB &c1, const ColorRGB &c2);
	
	///< IO operators
	friend istream &operator>>(istream &is, ColorRGB &c);
	friend ostream &operator<<(ostream &os, const ColorRGB &c);
	
	///< Basic operators
	friend ColorRGB operator+(const ColorRGB &c1, const ColorRGB &c2);
	friend ColorRGB operator+(const ColorRGB &c, const float &s);
	friend ColorRGB operator+(const float &s, const ColorRGB &c);
	friend ColorRGB operator-(const ColorRGB &c1, const ColorRGB &c2);
	friend ColorRGB operator-(const ColorRGB &c, const float &s);
	friend ColorRGB operator-(const float &s, const ColorRGB &c);
	friend ColorRGB operator*(const ColorRGB &c, const ColorRGB &c2);
	friend ColorRGB operator*(const ColorRGB &c, const float &s);
	friend ColorRGB operator*(const float &s, const ColorRGB &c);
	friend ColorRGB operator/(const ColorRGB &c1, const ColorRGB &c2);
	friend ColorRGB operator/(const ColorRGB &c, const float &s);
	
	ColorRGB operator+() const;
	ColorRGB operator-() const;
	
	ColorRGB &operator=(const ColorRGB &c);
	ColorRGB &operator+=(const ColorRGB &c);
	ColorRGB &operator+=(const float &s);
	ColorRGB &operator-=(const ColorRGB &c);
	ColorRGB &operator-=(const float &s);
	ColorRGB &operator*=(const ColorRGB &c);
	ColorRGB &operator*=(const float &s);
	ColorRGB &operator/=(const ColorRGB &c);
	ColorRGB &operator/=(const float &s);
	
	///< Vector operations
	ColorRGB Clamp(const float &min = 0.0f, const float &max = 1.0f) const;
	ColorRGB Grayscale(const float &r = 0.299f, const float &g = 0.587f, const float &b = 0.114f) const;
	ColorRGB Channel(const float &r = 1.0f, const float &g = 1.0f, const float &b = 1.0f) const;
	float	 Intensity(const float &r = 0.299f, const float &g = 0.587f, const float &b = 0.114f) const;
	
	friend ColorRGB Min(const ColorRGB &c1, const ColorRGB &c2);
	friend ColorRGB Max(const ColorRGB &c1, const ColorRGB &c2);
	friend ColorRGB Abs(const ColorRGB &c);
	
	///< Static methods, used for creating common vectors
	static ColorRGB Black() { return ColorRGB(0.0, 0.0, 0.0); };
	static ColorRGB DarkGray() { return ColorRGB(0.25, 0.25, 0.25); };
	static ColorRGB Gray() { return ColorRGB(0.5, 0.5, 0.5); };
	static ColorRGB LightGray() { return ColorRGB(0.75, 0.75, 0.75); };
	static ColorRGB White() { return ColorRGB(1.0, 1.0, 1.0); };
	static ColorRGB Red() { return ColorRGB(1.0, 0.0, 0.0); };
	static ColorRGB Green() { return ColorRGB(0.0, 1.0, 0.0); };
	static ColorRGB Blue() { return ColorRGB(0.0, 0.0, 1.0); };	
	static ColorRGB Cyan() { return ColorRGB(0.0, 1.0, 1.0); };
	static ColorRGB Magenta() { return ColorRGB(1.0, 0.0, 1.0); };
	static ColorRGB Yellow() { return ColorRGB(1.0, 1.0, 0.0); };

private:
	float data[3];
}; // class ColorRGB



/**
 * Implicit constructor. All components are zero.
 * @return ColorRGB.
 */ 
inline ColorRGB::ColorRGB()
{
	this->data[0] = 0.0;
	this->data[1] = 0.0;
	this->data[2] = 0.0;
} // ColorRGB()



/**
 * Implicit constructor from three-link float array.
 * @param float data[3] Three-link float array.
 * @return ColorRGB.
 */ 
inline ColorRGB::ColorRGB(const float data[3])
{
	this->data[0] = data[0];
	this->data[1] = data[1];
	this->data[2] = data[2];
} // ColorRGB()



/**
 * Implicit constructor from one float. Create color in grayscale.
 * @param float g Gray scale component.
 * @return ColorRGB.
 */ 
inline ColorRGB::ColorRGB(const float &g)
{
	this->data[0] = g;
	this->data[1] = g;
	this->data[2] = g;
} // ColorRGB()



/**
 * Implicit constructor from three floats.
 * @param float r Red component,
 * @param float g Green component,
 * @param float b Blue component.
 * @return ColorRGB.
 */ 
inline ColorRGB::ColorRGB(const float &r, const float &g, const float &b)
{
	this->data[0] = r;
	this->data[1] = g;
	this->data[2] = b;
} // ColorRGB()



/**
 * Copy contructor. Creates a copy of the vector 'c'.
 * @param ColorRGB c Original color, which will be copied.
 * @return ColorRGB.
 */ 
inline ColorRGB::ColorRGB(const ColorRGB &c)
{
	this->data[0] = c.r();
	this->data[1] = c.g();
	this->data[2] = c.b();
} // ColorRGB()



/**
 * Setter operator, which sets all component of color.
 * @param float r Red component,
 * @param float g Green component,
 * @param float b Blue component.
 * @return ColorRGB.
 */ 
inline ColorRGB &ColorRGB::operator() (const float &r, const float &g, const float &b)
{
	this->data[0] = r;
	this->data[1] = g;
	this->data[2] = b;
	return *this;
} // operator()



/**
 * Method for normalizating color. All components are set in min-max range.
 * @param float min Minimal component value,
 * @param float max Maximal component value.
 * @return ColorRGB Returns new clamped color.
 */ 
inline ColorRGB ColorRGB::Clamp(const float &min, const float &max) const
{
	ColorRGB temp(*this);
	
	temp[0] = Max(min, Min(temp[0], max));
	temp[1] = Max(min, Min(temp[1], max));
	temp[2] = Max(min, Min(temp[2], max));
	
	return temp;
} // Clamp()



/**
 * Method to convert color to grayscale.
 * @param float r Red multiplier,
 * @param float g Green multiplier,
 * @param float b Blue multiplier.
 * @return ColorRGB Returns converted color in grayscale.
 */
inline ColorRGB ColorRGB::Grayscale(const float &r, const float &g, const float &b) const
{
	float gray = r * this->r() + g * this->g() + b * this->b();	
	return ColorRGB(gray, gray, gray);
} // Grayscale()



/**
 * Method to mask color's channels.
 * @param float r Red multiplier,
 * @param float g Green multiplier,
 * @param float b Blue multiplier.
 * @return ColorRGB Returns converted color in grayscale.
 */
inline ColorRGB ColorRGB::Channel(const float &r, const float &g, const float &b) const
{
	return ColorRGB(r * this->r(), g * this->g(), b * this->b());
} // Channel()



/**
 * Method for compute color intensity.
 * @param float r Red multiplier,
 * @param float g Green multiplier,
 * @param float b Blue multiplier.
 * @return ColorRGB Returns converted color in grayscale.
 */
inline float ColorRGB::Intensity(const float &r, const float &g, const float &b) const
{
	return (r * this->r() + g * this->g() + b * this->b());
} // Intensity()



/**
 * Function for create a new color with minimal components of two color.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Returns a minimal color.
 */ 
inline ColorRGB Min(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(Min(c1.r(), c2.r()), Min(c1.g(), c2.g()), Min(c1.b(), c2.b()));
} // Min()



/**
 * Function for create a new color with maximal components of two color.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Returns a maximal color.
 */ 
inline ColorRGB Max(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(Max(c1.r(), c2.r()), Max(c1.g(), c2.g()), Max(c1.b(), c2.b()));
} // Max()



/**
 * Function for create a new color with absolute all components.
 * @param ColorRGB c An operand.
 * @return ColorRGB Returns a absolute color.
 */ 
inline ColorRGB Abs(const ColorRGB &c)
{
	return ColorRGB(Abs(c.r()), Abs(c.g()), Abs(c.b()));
} // Abs()



/**
 * Operator for comparation of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return bool Return true, if all compoments are equal, false otherwise.
 */ 
inline bool operator==(const ColorRGB &c1, const ColorRGB &c2) 
{
	return ((c1.r() == c2.r()) && (c1.g() == c2.g()) && (c1.b() == c2.b()));
} // operator==



/**
 * Operator for comparation of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return bool Return true, if one of the compoments are not equal, false otherwise.
 */ 
inline bool operator!=(const ColorRGB &c1, const ColorRGB &c2)
{
	return ((c1.r() != c2.r()) || (c1.g() != c2.g()) || (c1.b() != c2.b()));
} // operator!=



/**
 * Operator for addition of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Return new color as a result of additioning.
 */ 
inline ColorRGB operator+(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(c1.r() + c2.r(), c1.g() + c2.g(), c1.b() + c2.b());
} // operator+



/**
 * Operator for addition color with scalar.
 * @param ColorRGB c A first operand (color),
 * @param float s A second operand (scalar).
 * @return ColorRGB Return new color as a result of additioning.
 */ 
inline ColorRGB operator+(const ColorRGB &c, const float &s)
{
	return ColorRGB(c.r() + s, c.g() + s, c.b() + s);
} // operator+



/**
 * Operator for addition scalar with color.
 * @param float s A second operand (scalar).
 * @param ColorRGB c A first operand (color),
 * @return ColorRGB Return new color as a result of additioning.
 */ 
inline ColorRGB operator+(const float &s, const ColorRGB &c)
{
	return ColorRGB(c.r() + s, c.g() + s, c.b() + s);
} // operator+



/**
 * Operator for subtration of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Return new color as a result of subtrationing.
 */ 
inline ColorRGB operator-(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(c1.r() - c2.r(), c1.g() - c2.g(), c1.b() - c2.b());
} // operator-



/**
 * Operator for subtration of color and scalar.
 * @param ColorRGB c A first operand (color),
 * @param float s A second operand (scalar).
 * @return ColorRGB Return new color as a result of subtrationing.
 */ 
inline ColorRGB operator-(const ColorRGB &c, const float &s)
{
	return ColorRGB(c.r() - s, c.g() - s, c.b() - s);
} // operator-



/**
 * Operator for subtration of scalar with color.
 * @param float s A second operand (scalar).
 * @param ColorRGB c A first operand (color),
 * @return ColorRGB Return new color as a result of additioning.
 */ 
inline ColorRGB operator-(const float &s, const ColorRGB &c)
{
	return ColorRGB(c.r() - s, c.g() - s, c.b() - s);
} // operator-



/**
 * Operator for multiplication of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Return new color as a result of multiplication.
 */ 
inline ColorRGB operator*(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(c1.r() * c2.r(), c1.g() * c2.g(), c1.b() * c2.b());
} // operator*



/**
 * Operator for multiplication of color and scalar.
 * @param ColorRGB c A first operand (color),
 * @param float s A second operand (scalar).
 * @return ColorRGB Return new color as a result of multiplication.
 */ 
inline ColorRGB operator*(const ColorRGB &c, const float &s)
{
	return ColorRGB(c.r() * s, c.g() * s, c.b() * s);
} // operator*



/**
 * Operator for multiplication of scalar and color.
 * @param float s A second operand (scalar).
 * @param ColorRGB c A first operand (color),
 * @return ColorRGB Return new color as a result of multiplication.
 */ 
inline ColorRGB operator*(const float &s, const ColorRGB &c)
{
	return ColorRGB(c.r() * s, c.g() * s, c.b() * s);
} // operator*



/**
 * Operator for division of two colors.
 * @param ColorRGB c1 A first operand,
 * @param ColorRGB c2 A second operand.
 * @return ColorRGB Return new color as a result of division.
 */
inline ColorRGB operator/(const ColorRGB &c1, const ColorRGB &c2)
{
	return ColorRGB(c1.r() / c2.r(), c1.g() / c2.g(), c1.b() / c2.b());
} // operator/	



/**
 * Operator for division of color and scalar.
 * @param ColorRGB c A first operand (color),
 * @param float s A second operand (scalar).
 * @return ColorRGB Return new color as a result of division.
 */ 
inline ColorRGB operator/(const ColorRGB &c, const float &s)
{
	return ColorRGB(c.r() / s, c.g() / s, c.b() / s);
} // operator/



/**
 * Operator for get the same color.
 * @return ColorRGB Returns new color, which has same components as the original.
 */ 
inline ColorRGB ColorRGB::operator+() const
{
	return ColorRGB(this->r(), this->g(), this->b());
} // operator+



/**
 * Operator for negation of color's components.
 * @return ColorRGB Returns new color, which has negate components as the original.
 */ 
inline ColorRGB ColorRGB::operator-() const
{
	return ColorRGB(-this->r(), -this->g(), -this->b());
} // operator-



/**
 * Operator for assignement.
 * @param ColorRGB c An original color (right value).
 * @return ColorRGB Return a left value (color) after assignement.
 */
inline ColorRGB &ColorRGB::operator=(const ColorRGB &c)
{
	this->data[0] = c.r();
	this->data[1] = c.g();
	this->data[2] = c.b();	
	return *this;
} // operator=



/**
 * Operator for addition with color.
 * @param ColorRGB c An original color (right value), which will be added to this.
 * @return ColorRGB Return a left value (color) after addition.
 */
inline ColorRGB &ColorRGB::operator+=(const ColorRGB &c)
{
	*this = *this + c;
	return *this;
} // operator+=



/**
 * Operator for addition color with scalar.
 * @param float s An original color (right value), which will be added to this.
 * @return ColorRGB Return a left value (color) after addition.
 */
inline ColorRGB &ColorRGB::operator+=(const float &s)
{
	*this = *this + s;
	return *this;
} // operator+=



/**
 * Operator for subtraction with color.
 * @param ColorRGB c An original color (right value), which will be subtract from this.
 * @return ColorRGB Return a left value (color) after subtraction.
 */
inline ColorRGB &ColorRGB::operator-=(const ColorRGB &c)
{
	*this = *this - c;
	return *this;
} // operator-=



/**
 * Operator for subtraction with scalar.
 * @param float s An original color (right value), which will be subtract from this.
 * @return ColorRGB Return a left value (color) after subtraction.
 */
inline ColorRGB &ColorRGB::operator-=(const float &s)
{
	*this = *this - s;
	return *this;
} // operator-=



/**
 * Operator for multiplication with color.
 * @param ColorRGB c An original color (right value), which will be mult to this.
 * @return ColorRGB Return a left value (color) after multiplication.
 */
inline ColorRGB &ColorRGB::operator*=(const ColorRGB &c)
{
	*this = *this * c;
	return *this;
} // operator*=



/**
 * Operator for multiplication with scalar.
 * @param float s An right scalar value, which will be multiplied with this.
 * @return ColorRGB Return a left value (color) after multiplication.
 */
inline ColorRGB &ColorRGB::operator*=(const float &s)
{
	*this = *this * s;
	return *this;
} // operator*=



/**
 * Operator for division with color.
 * @param ColorRGB c Denominator (right value).
 * @return ColorRGB Return a left value (color) after division.
 */
inline ColorRGB &ColorRGB::operator/=(const ColorRGB &c)
{
	*this = *this / c;
	return *this;
} // operator/=



/**
 * Operator for division with scalar.
 * @param float s Denominator (right value). No zero division control!
 * @return ColorRGB Return a left value (color) after division.
 */
inline ColorRGB &ColorRGB::operator/=(const float &s)
{
	*this = *this / s;
	return *this;
} // operator/=



#endif // #ifndef _AFR_COLOR_RGB_H



/** ***************************************************************************/
/** end of file ColorRGB.h													  */
/** ***************************************************************************/
