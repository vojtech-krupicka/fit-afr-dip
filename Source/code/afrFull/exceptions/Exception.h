/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Exception.h														  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class Exception.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_EXCEPTION_H
#define _AFR_EXCEPTION_H


// Including system libraries
#include <string>
#include <iostream>


// Used namespaces
using namespace std;


/**
 * Class Exception
 * @brief Base class for definition an exception, that can be thrown from 
 * application. All other exception classes should be inherited 
 * from this base class.
 */ 
class Exception
{
public:
	///< Default constructors
	Exception(const string &m = "", const unsigned &c = 0, const string &f = "", const unsigned &l = 0) throw();
	Exception(const Exception &e) throw();
	
	///< Default destructors
	~Exception() throw() {};
	
	///< Getters
	inline string GetMessage() const { return this->message; };
	inline unsigned GetCode() const { return this->code; };
	inline string GetFile() const { return this->file; };
	inline unsigned GetLine() const { return this->line; };
	
	///< IO operators
	friend ostream &operator<<(ostream &os, const Exception &e);
	
	///< Basic operators
	Exception &operator=(const Exception &e) throw();

protected:
	string 		message;
	unsigned	code;
	string		file;
	unsigned	line;
}; // class Exception



/**
 * Operator for assignement.
 * @param Exception v An original exception (right value).
 * @return Exception Return a left value (exception) after assignement.
 */
inline Exception &Exception::operator=(const Exception &e) throw()
{
	this->message = e.GetMessage();
	this->code = e.GetCode();
	this->file = e.GetFile();
	this->line = e.GetLine();
	
	return *this;
} // operator=



#endif // #ifndef _AFR_EXCEPTION_H



/** ***************************************************************************/
/** end of file Exception.h													  */
/** ***************************************************************************/
