/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    StringUtils.h													  *	
 * @date	09. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief   Contains macros, definitions and function for string operations.  *
 **************************************************************************** */
 
 
// Conditional compilation 
#ifndef _AFR_STRING_UTILS_H
#define _AFR_STRING_UTILS_H


// Including system libraries
#include <string>
#include <sstream>


// Used namespaces
using namespace std;



/**
 * Template function for dump (print) variable of basic type to string.
 * @param T t Variable with type T.
 * @return string Returns variable t as string.
 */ 
template <class T>
inline string Dump(const T& t)
{
	stringstream ss;
	ss << t;
	return ss.str();
} // Dump()



/**
 * Function for convert string to upper case.
 * @param string s Input string which will be converted.
 * @return string Returns string s as upper case.
 */ 
inline string StrToUpper(const string& s)
{
	string upper = s;
	string::iterator i = upper.begin();
	string::iterator end = upper.end();
 
	while (i != end) {
		*i = toupper((unsigned char)*i);
		++i;
	}
	
	return upper;
} // strToUpper()



/**
 * Function for convert string to lower case.
 * @param string s Input string which will be converted.
 * @return string Returns string s as lower case.
 */ 
inline string StrToLower(const string& s)
{
	string lower = s;
	string::iterator i = lower.begin();
	string::iterator end = lower.end();
 
	while (i != end) {
		*i = tolower((unsigned char)*i);
		++i;
	}
	
	return lower;
} // strToLower()



/**
 * Function for explode string to two strings, key and value with char '='.
 * @param string str Input string in format 'key=value' which will explode,
 * @param string key Output key name,
 * @param string value Output value.
 * @return size_t Return position of '=' character.
 */ 
inline size_t StrExplode(const string &str, string &key, string &value, const char &delim = '=')
{
	size_t pos = str.find(delim);
	if(pos != string::npos) { 
		key = str.substr(0, pos);
		value = str.substr(pos+1);
	}
	
	return pos;
} // StrExplode()


/**
 * Function for trim given string, erase empty spaces from begin and end of 
 * the string. This is analogical function for PHP trim() function.
 * @param string str String which will be trimmed.
 * @return string Returns trimmed string.
 */ 
inline string StrTrim(const string &str)
{
	size_t pos = str.find_last_not_of(' ');
	string trimmed = str;
	
	if(pos != string::npos) {
		trimmed.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) {
			trimmed.erase(0, pos);
		}
	}
	else {
		return "";
	}
	
	return trimmed;
} // StrTrim()



#endif // _AFR_STRING_UTILS_H



/** ***************************************************************************/
/** end of file StringUtils.h												  */
/** ***************************************************************************/
