/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Exceptions.h													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of all exceptions.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_EXCEPTIONS_H
#define _AFR_EXCEPTIONS_H


// Including project libraries
#include "Exception.h"



/**
 * @brief Exception that represents error in the program logic. This kind of exceptions
 * should directly lead to a fix in source code. 
 */ 
class LogicException : public Exception
{
public:
	///< Default constructors
	LogicException(const string &m = "", const unsigned &c = 0) throw() : Exception(m, c) {};
	LogicException(const LogicException &e) throw() : Exception(e) {};

	///< IO operators
	friend ostream &operator<<(ostream &os, const LogicException &e);
	
}; // class LogicException



/**
 * @brief Exception thrown if an error which can only be found on runtime occurs.  
 */ 
class RuntimeException : public Exception
{
public:
	///< Default constructors
	RuntimeException(const string &m = "", const unsigned &c = 0) throw() : Exception(m, c) {};
	RuntimeException(const RuntimeException &e) throw() : Exception(e) {};
	
	///< IO operators
	friend ostream &operator<<(ostream &os, const RuntimeException &e);
	
}; // class RuntimeException



/**
 * @brief Exception thrown if an argument does not match with the expected value. 
 */ 
class InvalidArgumentException : public LogicException
{
public:
	///< Default constructors
	InvalidArgumentException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	InvalidArgumentException(const InvalidArgumentException &e) throw() : LogicException(e) {};
	
}; // class InvalidArgumentException



/**
 * @brief The exception that is thrown when the value of an argument is outside the 
 * allowable range of values as defined by the invoked method.
 */ 
class OutOfRangeException : public LogicException
{
public:
	///< Default constructors
	OutOfRangeException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	OutOfRangeException(const OutOfRangeException &e) throw() : LogicException(e) {};
	
}; // class OutOfRangeException



/**
 * @brief Exception thrown if a length is invalid. 
 */ 
class LengthException : public LogicException
{
public:
	///< Default constructors
	LengthException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	LengthException(const LengthException &e) throw() : LogicException(e) {};
	
}; // class LengthException



/**
 * @brief Exception that is thrown when a requested method or operation 
 * is not implemented.
 */ 
class NotImplementedException : public LogicException
{
public:
	///< Default constructors
	NotImplementedException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	NotImplementedException(const NotImplementedException &e) throw() : LogicException(e) {};
	
}; // class NotImplementedException 



/**
 * @brief Exception that is thrown when an invoked method is not supported. For 
 * scenarios where it is sometimes possible to perform the requested operation. 
 * @see InvalidStateException.
 */ 
class NotSupportedException : public LogicException
{
public:
	///< Default constructors
	NotSupportedException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	NotSupportedException(const NotSupportedException &e) throw() : LogicException(e) {};
	
}; // class NotSupportedException 



/**
 * @brief The exception that is thrown when a requested method or operation 
 * is deprecated.
 */ 
class DeprecatedException : public NotSupportedException
{
public:
	///< Default constructors
	DeprecatedException(const string &m = "", const unsigned &c = 0) throw() : NotSupportedException(m, c) {};
	DeprecatedException(const DeprecatedException &e) throw() : NotSupportedException(e) {};
	
}; // class DeprecatedException 



/**
 * @brief The exception that is thrown when static class is instantiated.
 */ 
class StaticClassException : public LogicException
{
public:
	///< Default constructors
	StaticClassException(const string &m = "", const unsigned &c = 0) throw() : LogicException(m, c) {};
	StaticClassException(const StaticClassException &e) throw() : LogicException(e) {};
	
}; // class StaticClassException 



/**
 * @brief Exception thrown if a value does not match with a set of values. 
 * Typically this happens when a function calls another function and expects 
 * the return value to be of a certain type or value not including arithmetic 
 * or buffer related errors. 
 */ 
class UnexpectedValueException : public RuntimeException
{
public:
	///< Default constructors
	UnexpectedValueException(const string &m = "", const unsigned &c = 0) throw() : RuntimeException(m, c) {};
	UnexpectedValueException(const UnexpectedValueException &e) throw() : RuntimeException(e) {};
	
}; // class UnexpectedValueException



/**
 * @brief Exception that is thrown when a method call is invalid for the 
 * object's current state, method has been invoked at an illegal or 
 * inappropriate time. 
 */ 
class InvalidStateException : public RuntimeException
{
public:
	///< Default constructors
	InvalidStateException(const string &m = "", const unsigned &c = 0) throw() : RuntimeException(m, c) {};
	InvalidStateException(const InvalidStateException &e) throw() : RuntimeException(e) {};
	
}; // class InvalidStateException



/**
 * @brief The exception that is thrown when an I/O error occurs.
 */ 
class IOException : public RuntimeException
{
public:
	///< Default constructors
	IOException(const string &m = "", const unsigned &c = 0) throw() : RuntimeException(m, c) {};
	IOException(const IOException &e) throw() : RuntimeException(e) {};
	
}; // class IOException



/**
 * @brief The exception that is thrown when accessing a file that does not 
 * exist on disk.
 */ 
class FileNotFoundException : public IOException
{
public:
	///< Default constructors
	FileNotFoundException(const string &m = "", const unsigned &c = 0, const string &f = "") throw() 
		: IOException(m, c) { this->file = f; };
	FileNotFoundException(const FileNotFoundException &e) throw() 
		: IOException(e) {};
	
	///< IO operators
	friend ostream &operator<<(ostream &os, const FileNotFoundException &e);
	
}; // class FileNotFoundException



#endif // #ifndef _AFR_EXCEPTIONS_H



/** ***************************************************************************/
/** end of file Exceptions.h												  */
/** ***************************************************************************/
