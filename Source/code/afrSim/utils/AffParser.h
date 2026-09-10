/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AffParser.h														  *	
 * @date	14. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class AffParser.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_AFF_PARSER_H
#define _AFR_AFF_PARSER_H


// Including system libraries
#include <string>
#include <fstream>
#include <iostream>


// Including project libraries
#include "../exceptions/Exceptions.h"


// Predefinition of classes
class Scene;
class Vector2;
class Vector3;



/**
 * Class AffParser
 * @brief Class for parse input scene file in AFF format. Throught scene API 
 * creates new primitives, lights, materials and textures and create scene.
 */ 
class AffParser
{
public:
	///< Implicit constructors
	AffParser();
	AffParser(Scene *s, const string &f, const unsigned &l = 0);
	
	///< Implicit destructors
	~AffParser();	
	
	///< Parser operations
	bool Parse();
	
	///< Setters
	inline AffParser *SetDetailLevel(const unsigned &l) { this->detailLevel = l; return this; };
	inline AffParser *SetFileName(const string &f);
	inline AffParser *SetScene(Scene *s) throw(InvalidArgumentException);
	
	///< Getters
	inline unsigned GetDetailLevel() const { return this->detailLevel; };
	inline string GetFileName() const { return this->fileName; };
	inline string GetBaseDir() const { return this->baseDir; };
	inline Scene *GetScene() const { return this->scene; };
	
	///< Others methods
	
private:
	///< Auxiliary private methods
	inline AffParser *OpenFile() throw(FileNotFoundException);
	inline bool CheckParserState() const throw(InvalidStateException, IOException);
	inline bool IsComment(const string &token) const;
	
	///< Methods for parsing NFF primitives
	bool ParseComment();			///< '#' nebo '%'
	bool ParseViewPoint();			///< 'v'
	bool ParseBackgroundColor();	///< 'b'
	bool ParseLight();				///< 'l'
	bool ParseFillColor();			///< 'f'
	bool ParseCone();				///< 'c'
	bool ParseSphere();				///< 's'
	bool ParsePolygon();			///< 'p'	
	bool ParsePolyPatch();			///< 'pp'
	
	///< Methods for parsing extended AFF primitives
	bool ParseIncludeFile();		///< 'i'
	bool ParseDetailLevel();		///< 'd'	 
	bool ParseTriangle();			///< 'tt'
	bool ParseTrianglePatch();		///< 'ttp'
	bool ParseAnimatedTriangle();	///< 'tpa'	
	bool ParseKeyFrames();			///< 'k'
	bool ParseStaticTransform();	///< 'xs'
	bool ParseAnimatedTransform();	///< 'x'
	bool ParseAnimationParams();	///< 'a'
	bool ParseAmbientLight();		///< 'am'
	bool ParseFillMaterial();		///< 'fm'
	bool ParseAnimatedLight();		///< 'la'
	bool ParseMesh();				///< 'm'	
	bool CloseTranform();			///< '}'
	
	
	///< Auxiliary methods for parsing triangle mesh
	bool ReadMeshVectors(const string &type, Vector3 *(&vectors), unsigned &count);
	bool ReadMeshCoords(string &name, Vector2 *(&coords), unsigned &count);
	bool ReadMeshTriangles(Vector3 *vertices, Vector3 *normals, Vector2 *coords, unsigned short *(&indices), unsigned &count);
	

	Scene		*scene;			///< Scene object
	
	ifstream	ifs;			///< Input stream (input AFF file with scene)
	string		fileName;		///< Input AFF file name
	string 		baseDir;		///< Directory name with AFF file
	
	unsigned	detailLevel;	///< Default detail level
}; // class AffParser



/**
 * Method for set file name. After set file name, the base path will be 
 * determined from full relativ path and set too. The file will be opened.
 * @param string f File name, or rather relative path to AFF file.
 * @return AffParser Provides fluent interface.
 */ 
inline AffParser *AffParser::SetFileName(const string &f)
{
	// Prepare file name and path
	this->fileName = f;
	this->baseDir = "";
	
	size_t pos;
	if((pos = f.find_last_of("/\\")) != string::npos) {
		this->baseDir = f.substr(0, pos) + "/";
	}
	
	// Otevre novy soubor
	this->OpenFile();
	
	return this;
} // SetFileName()



/**
 * Method for set scene object. 
 * @param Scene s Instance of scene.
 * @return AffParser Provides fluent interface.
 * @throw InvalidArgumentException If scene is NULL.
 */ 
inline AffParser *AffParser::SetScene(Scene *s) throw(InvalidArgumentException)
{
	// Check if scene is really instantiated
	if(s == NULL) {
		throw InvalidArgumentException("AffParser: Scene is not instantiated or is NULL");
		return NULL;
	}
	this->scene = s;
	
	return this;
} // SetScene()
	


/**
 * Method for open file. The file name is already set in fileName property. 
 * @return AffParser Provides fluent interface.
 * @throw FileNotFoundException If file does not exists.
 */ 
inline AffParser *AffParser::OpenFile() throw(FileNotFoundException)
{
	// If some file has been opened before, close it first
	if(this->ifs.is_open()) {
		this->ifs.close();
	}
	
	// Try open file
	this->ifs.open(this->fileName.c_str(), ios_base::in);
	
	// If file doesn't exists, throw exception
	if(!this->ifs.is_open()) {
		throw FileNotFoundException("", 0, this->fileName);
	}
	
	return this;
} // OpenFile()



/**
 * Method for check parser actual state. Checks if file is open and if file is 
 * readable. This method does't check if file cursor is at the end of file 
 * (EOF). This method also check if scene is instantiated or if is NULL
 * @return bool Returns true, if file state is OK, false otherwise.
 * @throw InvalidStateException Throws when file is not open or scene is NULL.
 * @throw IOException Throw when file is not readable.
 */ 
inline bool AffParser::CheckParserState() const throw(InvalidStateException, IOException)
{
	// Check if scene has been instantiated
	if(this->scene == NULL) {
		throw InvalidStateException("AffParser: Scene has not been instantiated!");
	}	
	// Check if file has been opened
	if(!this->ifs.is_open()) {
		throw InvalidStateException("AffParser: File '"+this->fileName+"' has not been open!");
		return false;
	}
	// Check if file is readable
	if(this->ifs.fail()) {
		throw IOException("AffParser: File '"+this->fileName+"' can't be read!");
		return false;
	}
	
	return true;
} // CheckParserState()



/**
 * Method for determine if actual token is comment or not.
 * @param string token Aktual token read from input stream.
 * @return bool Returns true, if token might be a comment, false otherwise.
 */ 
inline bool AffParser::IsComment(const string &token) const
{
	return (token == "#" || token == "%" || token.find("#") == 0 || token.find("%") == 0);
} // IsComment()
	


#endif // #ifndef _AFR_AFF_PARSER_H



/** ***************************************************************************/
/** end of file AffParser.h													  */
/** ***************************************************************************/
