/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Exceptions.cpp													  *	
 * @date	13. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of all exceptions.								  *
 **************************************************************************** */


// Including project libraries
#include "Exceptions.h"



/**
 * Output operator. Write exception message to output stream.
 * @param ostream os Output stream,
 * @param LogicException e Exception which has been thrown.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const LogicException &e)
{
	os << "ERROR<LogicException #" << e.GetCode() << ">: " << e.GetMessage();
	return os;
} // operator<<



/**
 * Output operator. Write exception message to output stream.
 * @param ostream os Output stream,
 * @param LogicException e Exception which has been thrown.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const RuntimeException &e)
{
	os << "ERROR<RuntimeException #" << e.GetCode() << ">: " << e.GetMessage();
	return os;
} // operator<<



/**
 * Output operator. Write exception message to output stream.
 * @param ostream os Output stream,
 * @param StaticClassException e Exception which has been thrown.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const StaticClassException &e)
{
	os << "ERROR<StaticClassException #" << e.GetCode() << ">: " << e.GetMessage();
	return os;
} // operator<<



/**
 * Output operator. Write exception message to output stream.
 * @param ostream os Output stream,
 * @param LogicException e Exception which has been thrown.
 * @return ostream Returns output stream for fluent interface.
 */ 
ostream &operator<<(ostream &os, const FileNotFoundException &e)
{
	os << "ERROR<FileNotFoundException>: File '" << e.GetFile() << "' not found on disk! " << e.GetMessage();
	return os;
} // operator<<



/** ***************************************************************************/
/** end of file Exceptions.cpp												  */
/** ***************************************************************************/
