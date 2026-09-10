/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Exception.cpp													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class Exception.							  	  *
 **************************************************************************** */


// Including project libraries
#include "Exception.h"



/**
 * Default constructor which sets all components.
 * @param string m Exception's message which will be displayed,
 * @param unsigned c Exception's code,
 * @param string f Source file, where exception has been thrown,
 * @param unsigned l Line in source file, where exception has been thrown.
 * @return Exception.
 */ 
Exception::Exception(const string &m, const unsigned &c, const string &f, const unsigned &l) throw()
{
	this->message = m;
	this->code = c;
	this->file = (f != "") ? f : __FILE__;
	this->line = (l != 0) ? l : __LINE__;
} // Exception()



/**
 * Copy constructor. Creates new exception from original exception 'e'.
 * @param Exception e Original exception instance which will be copied.
 * @return Exception.
 */ 
Exception::Exception(const Exception &e) throw()
{
	this->message = e.GetMessage();
	this->code = e.GetCode();
	this->file = e.GetFile();
	this->line = e.GetLine();
} // Exception()
	


/**
 * Output operator. Write exception message to output stream.
 * @param ostream os Output stream,
 * @param Exception e Exception which has been thrown.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const Exception &e)
{
	os << "ERROR<'" << e.GetFile() << "' at line '" << e.GetLine() << "'>: " << e.GetMessage() << " [" << e.GetCode() << "]";
	return os;
} // operator<<



/** ***************************************************************************/
/** end of file Exception.cpp												  */
/** ***************************************************************************/
