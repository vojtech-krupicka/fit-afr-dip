/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    AffParser.cpp													  *	
 * @date	14. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class AffParser.								  *
 **************************************************************************** */


// Including project libraries
#include "AffParser.h"

#include "../scene/Animation.h"
#include "../scene/Scene.h"
#include "../types/Color.h"
#include "../types/Vector2.h"
#include "../types/Vector3.h"
#include "../types/Vector4.h"
#include "../types/Matrix.h"
#include "MathUtils.h"
#include "PositionInfo.h"
#include "RotationInfo.h"
#include "StringUtils.h"



/**
 * Default constructor.
 * @return AffParser.
 */ 
AffParser::AffParser()
{
	this->fileName = "";
	this->baseDir = "";
	this->detailLevel = 0;
	
	this->scene = NULL;
} // AffParser()



/**
 * Default constructor. Initialize parser with given scene, aff file name and
 * detail level which is zero in default.
 * @param Scene s The SCENE,
 * @param string f AFF file name, or rather relative path to file,
 * @param unsigned l Default detail level to suppress including nested AFF files.
 * @return AffParser.
 */ 
AffParser::AffParser(Scene *s, const string &f, const unsigned &l)
{
	// Sets known components
	this->SetScene(s);
	this->SetDetailLevel(l);
	this->SetFileName(f);
} // AffParser()



/**
 * Default destructor. Clear pointer to scene instance and close input stream
 * if is open.
 */ 
AffParser::~AffParser() 
{
	// Pointer to Scene instance sets to NULL
	this->scene = NULL;
	
	// Reinitialize all components
	this->fileName = "";
	this->baseDir = "";
	this->detailLevel = 0;
	
	// If open AFF file, close it
	if(this->ifs.is_open()) {
		this->ifs.close();
	}
} // ~AffParser()



/**
 * Main method for parsing AFF file at the given name. File must be opened 
 * before calling this method, also scene must be sets. Otherwise 
 * InvalidStateException will be thrown.
 * @return bool Returns true, if parse has been successfuly finished, false otherwise.
 */ 
bool AffParser::Parse()
{
	// Auxiliary variable to store actual token from input stream
	string token;
	
	// Infinite loop for do all parsing
	while(true) {
		// Check integrity
		if(!this->CheckParserState()) {
			return false;
		}
		
		// Read token from input stream
		this->ifs >> token;
		
		// Check if is EOF - end loop
		if(this->ifs.eof()) {
			break;
		}
		
		// Final machine to parse each NFF/AFF primitive in special method
		if(this->IsComment(token)) {	// comment
			this->ParseComment();
		}
		else if(token == "v") { 		// viewing vectors and angles
			this->ParseViewPoint();
		}
		else if(token == "b") { 		// background color
			this->ParseBackgroundColor();
		}
		else if(token == "l") { 		// positional light location
			this->ParseLight();
		}
		else if(token == "f") { 		// object material properties
			this->ParseFillColor();
		}
		else if(token == "c") { 		// cone or cylinder primitive
			this->ParseCone();
		}
		else if(token == "s") { 		// sphere primitive
			this->ParseSphere();
		}
		else if(token == "p") { 		// polygon primitive
			this->ParsePolygon();
		}
		else if(token == "pp") { 		// polygonal patch primitive
			this->ParsePolyPatch();
		}
		else if(token == "i") { 		// included file
			this->ParseIncludeFile();
		}
		else if(token == "d") { 		// detail level
			this->ParseDetailLevel();
		}
		else if(token == "tt") { 		// textured triangle
			this->ParseTriangle();
		}
		else if(token == "ttp") { 		// textured triangle patch
			this->ParseTrianglePatch();
		}
		else if(token == "tpa") { 		// animated triangle patch
			this->ParseAnimatedTriangle();
		}
		else if(token == "k") { 		// keyframe
			this->ParseKeyFrames();
		}
		else if(token == "xs") { 		// static transform
			this->ParseStaticTransform();
		}
		else if(token == "x") { 		// keyframe animated transform
			this->ParseAnimatedTransform();
		}
		else if(token == "a") { 		// animation parameters
			this->ParseAnimationParams();
		}
		else if(token == "am") { 		// global ambient light
			this->ParseAmbientLight();
		}
		else if(token == "fm") { 		// material with shading properties
			this->ParseFillMaterial();
		}
		else if(token == "la") { 		// animated point light
			this->ParseAnimatedLight();
		}
		else if(token == "m") { 		// triangle mesh (with optional normals/vertex and texture coords)
			this->ParseMesh();
		}
		else if(token == "}") { 		// triangle mesh (with optional normals/vertex and texture coords)
			this->CloseTranform();
		}
		else {							// unknown
			throw UnexpectedValueException("AffParser<"+this->fileName+">: Unknown primitive code '"+token+"'. Can't continue!");
		}
	}
	
	return true;
} // Parse()



/**
 * Method for parsing comments.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseComment()
{
	// Test parser state
	this->CheckParserState();
	
	// Read whole line with comment
	string comment;
	getline(this->ifs, comment);
	
	return true;
} // ParseComment()



/**
 * Method for parsing scene view port (camera position, look at and up vecotrs).
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseViewPoint()
{
	// Test parser state
	this->CheckParserState();
	
	// Initializing auxiliary variables
	string token;
	Vector3 from, at, up;
	Vector2 res;
	float angle, hither;
	
	// Read token 'from'
	this->ifs >> token;
	if(token == "from") {
		this->ifs >> from;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'from' statement!");
		}
	}
	
	// Read token 'at'
	this->ifs >> token;
	if(token == "at") {
		this->ifs >> at;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'at' statement!");
		}
	}
	
	// Read token 'up'
	this->ifs >> token;
	if(token == "up") {
		this->ifs >> up;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'up' statement!");
		}
	}
	
	// Read token 'angle'
	this->ifs >> token;
	if(token == "angle") {
		this->ifs >> angle;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'angle' statement!");
		}
	}
	
	// Read token 'hither'
	this->ifs >> token;
	if(token == "hither") {
		this->ifs >> hither;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'hither' statement!");
		}
		
		hither = (hither < 0.0001) ? 1.0 : hither;
	}
	
	// Read token 'resolution'
	this->ifs >> token;
	if(token == "resolution") {
		this->ifs >> res;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: View Point parse error, can't parse 'resolution' statement!");
		}
	}
	
	// Init view point in scene
	this->scene->InitCamera(from, at, up, angle, hither, res);
	
	
	return true;
} // ParseViewPoint()



/**
 * Method for parsing background color.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseBackgroundColor()
{
	// Test parser state
	this->CheckParserState();
	
	// Read background color and store to variable
	Color bgColor;
	this->ifs >> bgColor;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Background Color parse error!");
	}
	
	// Set background color to scene
	this->scene->InitBackgroundColor(bgColor);
	
	return true;
} // ParseBackgroundColor()



/**
 * Method for parsing light entity.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseLight()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 pos;
	Color color;
	
	// Precte pozici svetla
	this->ifs >> pos;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Light parse error!");
	}
	
	// Pokusi se precist barvu svetla - volitelny parametr, pokud neexistuje, nastavi vychozi
	this->ifs >> color;
	if(this->ifs.fail()) {
		this->ifs.clear();
		color = Color::White();
	}
	
	// Adds light entity to scene
	this->scene->AddLight("", pos, color);
	
	return true;
} // ParseLight()



/**
 * Method for fill color (basic material type).
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseFillColor()
{
	// Test parser state
	this->CheckParserState();
	
	// Read material color
	Color color;
	this->ifs >> color;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill color syntax error.");
	}
	
	// Read material others parameters
	float kd, ks, shine, transmit, ior;
	this->ifs >> kd >> ks >> shine >> transmit >> ior;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill material syntax error.");
	}
	
	// Adds material to scene
	this->scene->AddMaterial(color, kd, ks, 4.0f*shine, ior, transmit);
	
	return true;
} // ParseFillColor()



/**
 * Method for parsing cone or cylinder primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseCone()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 base, apex;
	float r1, r2;
	string name;
	
	
	// Read all properties
	this->ifs >> base >> r1 >> apex >> r2;
	if(this->ifs.fail()) {
		this->ifs.clear();
		this->ifs >> name >> base >> r1 >> apex >> r2;
		
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Cylinder or cone parse error!");
		}
	}
	
	// If radius is lower then zero, negate both radii
	if(r1 < 0.0) {
		r1 = -r1;
		r2 = -r2;
	}
	
	// Adds cone primitive to scene
	this->scene->AddCone(this->baseDir, name, base, r1, apex, r2);
	
	
	return true;
} // ParseCone()



/**
 * Method for parsing sphere primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseSphere()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 center;
	float radius;
	string name;
	
	// Read sphere components
	this->ifs >> center >> radius;
	if(this->ifs.fail()) {
		this->ifs.clear();
		this->ifs >> name >> center >> radius;
		
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Sphere parse error!");
		}
	}
	
	// Adds sphere primitive to scene
	this->scene->AddSphere(this->baseDir, name, center, radius);
	
	return true;
} // ParseSphere()



/**
 * Method for parsing polygon primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParsePolygon()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	unsigned count = 0;
	Vector3 *vertices = NULL;
	
	// Read vertices count
	this->ifs >> count;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Polygon syntax error!");
	}
	
	// Alloc memory for polygon's vertices
	vertices = new(nothrow) Vector3[count];
	if(vertices == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Can't allocate memory for polygon!");
	}
	
	// Read all vertices
	for(unsigned i = 0; i < count; i++) {
		this->ifs >> vertices[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Polygon syntax error as "+Dump(i)+"th vertex!");
		}
	}
	
	// Adds polygon to scene
	this->scene->AddPolygon(vertices, NULL, count);
	
	// Delete temporaly vertices array
	if(vertices) delete [] vertices;
	
	return true;
} // ParsePolygon()



/**
 * Method for parsing polygonal patch primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParsePolyPatch()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	unsigned count = 0;
	Vector3 *vertices = NULL;
	Vector3 *normals = NULL;
	
	// Read vertices count
	this->ifs >> count;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Polygonal patch syntax error!");
	}
	
	// Alloc memory for polygon's vertices and normals
	vertices = new(nothrow) Vector3[count];
	normals = new(nothrow) Vector3[count];
	if(vertices == NULL || normals == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Can't allocate memory for polygonal patch!");
	}
	
	// Read all vertices with normals
	for(unsigned i = 0; i < count; i++) {
		this->ifs >> vertices[i] >> normals[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Polygonal patch syntax error as "+Dump(i)+"th vertex or normal!");
		}
	}
	
	// Adds polygon to scene
	this->scene->AddPolygon(vertices, normals, count);	
	
	// Delete vertices and normals array 
	if(vertices) delete [] vertices;
	if(normals) delete [] normals;
	
	return true;
} // ParsePolyPatch()



/**
 * Method for parsing inluding file.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseIncludeFile()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	string name;
	unsigned level;
	
	// Read detail level and file name
	this->ifs >> level >> name;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Include File <"+name+"> parse error!");
	}
	
	// If detail level is lower then default detail level, parse included file
	if(level <= this->detailLevel) {
		AffParser *parser = NULL;
		try {
			parser = new AffParser(this->scene, this->baseDir+name, this->detailLevel);
			if(!parser->Parse()) {
				throw IOException("AffParser<"+this->fileName+">: Could not parse include file <"+this->baseDir+name+">!");
			}
		}
		catch(IOException &e) {
			cerr << e << endl;
			delete parser;
			throw;
		}
		
		// Delete parser
		delete parser;
	}
	
	return true;
} // ParseIncludeFile()



/**
 * Method for parsing detail level settings.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseDetailLevel()
{
	// Test parser state
	this->CheckParserState();
	
	// Read detail level
	this->ifs >> this->detailLevel;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Detail Level parse error!");
	}
	
	return true;
} // ParseDetailLevel()



/**
 * Method for parsing triangle primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseTriangle()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 vertices[3];
	Vector2 coords[3];
	string name;
	
	// Read texture name
	this->ifs >> name;
	
	// Read all vertices and texture's coords
	for(int i = 0; i < 3; i++) {
		// Read vertex
		this->ifs >> vertices[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse textured triangle "+Dump(i)+"th vertex!");
		}
		
		// Read texture's coords
		this->ifs >> coords[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse textured triangle "+Dump(i)+"th texture coords!");
		}	
	}
	
	// Adds triangle primitive to scene
	this->scene->AddTriangle(this->baseDir, name, vertices, NULL, coords);
	
	return true;
} // ParseTriangle()



/**
 * Method for parsing triangle patch primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseTrianglePatch()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 vertices[3];
	Vector3 normals[3];
	Vector2 coords[3];
	string name;
	
	// Read texture name
	this->ifs >> name;
	
	// Read all vertices and texture's coords
	for(int i = 0; i < 3; i++) {
		// Read vertex
		this->ifs >> vertices[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse textured triangle "+Dump(i)+"th vertex!");
		}
		
		// Precte normalu ve vrcholu
		this->ifs >> normals[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse textured triangle "+Dump(i)+"th normal!");
		}
		
		// Read texture's coords
		this->ifs >> coords[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse textured triangle "+Dump(i)+"th texture coords!");
		}	
	}
	
	// Adds triangle primitive to scene
	this->scene->AddTriangle(this->baseDir, name, vertices, normals, coords);
	
	return true;
} // ParseTrianglePatch()



/**
 * Method for parsing animated triangle primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseAnimatedTriangle()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	unsigned count = 0;
	Vector3 *vertices = NULL;
	Vector3 *normals = NULL;
	float	*times = NULL;
	
	// Read animation frames count
	this->ifs >> count;
	
	// Alloc memory
	times = new(nothrow) float[count];
	vertices = new(nothrow) Vector3[3*count];
	normals = new(nothrow) Vector3[3*count];
	
	// Check if allocation is OK
	if(times == NULL || vertices == NULL || normals == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Could not allocate memory form animated triangle patch!");
	}
	
	// Loop over all keyframes
	for(unsigned i = 0; i < count; i++) {
		// Read actual keyframe time
		this->ifs >> times[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not parse animated triangle (tpa)!");
		}
		
		// Read actual triangle at the given time
		for(int j = 0; j < 3; j++) {
			// Read vertex
			this->ifs >> vertices[i*3+j];
			if(this->ifs.fail()) {
				throw IOException("AffParser<"+this->fileName+">: Could not parse "+Dump(j)+"th vertex of "+Dump(i+1)+"th animated triangle patch in time "+Dump(times[i])+"!");
			}
			
			// Read normal
			this->ifs >> normals[i*3+j];
			if(this->ifs.fail()) {
				throw IOException("AffParser<"+this->fileName+">: Could not parse "+Dump(j)+"th normal of "+Dump(i+1)+"th animated triangle patch in time "+Dump(times[i])+"!");
			}
		}
	}
	
	// Adds animated triangle primitive to scene
	this->scene->AddAnimatedTriangle(count, times, vertices, normals);
	
	// Delete all temporary arrays
	if(times) delete [] times;
	if(vertices) delete [] vertices;
	if(normals) delete [] normals;
	
	return true;
} // ParseAnimatedTriangle()



/**
 * Method for parsing animation key frames.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseKeyFrames()
{
	// Test parser state
	this->CheckParserState();

	// Init auxiliary variables
	string name, transform, token;
	bool visibility;
	unsigned frames;
	float time, x, y, z, angle, tension, continuity, bias;
	Animation *animation = NULL;
	TPositionKey *pKeys = NULL;
	TRotationKey *rKeys = NULL;
	
	// Try to read name of animation and opening '{'
	this->ifs >> name >> token;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not read animated transform!");
	}
	else if(token != "{") {
		throw IOException("AffParser<"+this->fileName+">: Expected '{', get '"+token+"'");
	}
	
	// Create new animation object
	animation = new Animation(name);	
	
	// Read next token
	this->ifs >> token;
	while(token.compare("}") != 0) {
		// Token should be name of transformation
		transform = token;
		
		// Read frames count
		this->ifs >> frames;
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not read name of motion or number of keyframes for animation!");
		}
		
		// Minimum of keyframes must be 4 or higher if transformation is not 'visibility'
		if(frames < 4 && transform.compare("visibility") != 0) {
			throw IOException("AffParser<"+this->fileName+">: There must be at least 4 keyframes for '"+name+"'!");
		}
		
		// If actual transformation is translation - 'transl'
		if(transform == "transl") {
			// Allocates array for position keys
			pKeys = new(nothrow) TPositionKey[frames];
			if(pKeys == NULL) {
				throw IOException("AffParser<"+this->fileName+">: can't alloc memory for translation for animation '"+name+"'!");
			}
			
			// Read all frames for translation
			for(unsigned i = 0; i < frames; i++) {
				// Read actual frame
				this->ifs >> time >> x >> y >> z >> tension >> continuity >> bias;
				if(this->ifs.fail()) {
					throw IOException("AffParser<"+this->fileName+">: Error in parsing translations keyframes for '"+name+"' animation!");
				}
				
				// Init keyframe
				pKeys[i].time = time;
				pKeys[i].pos(x, y, z);
				pKeys[i].tension = tension;
				pKeys[i].continuity = continuity;
				pKeys[i].bias = bias;
			}
			
			// Adds all translation key frames to an animation and delete temporary array
			animation->SetTranslations(pKeys, frames);
			delete [] pKeys;
			pKeys = NULL;
		}
		// If actual transformation is rotation - 'rot'
		else if(transform == "rot") {
			// Allocates array for position keys
			rKeys = new(nothrow) TRotationKey[frames];
			if(rKeys == NULL) {
				throw IOException("AffParser<"+this->fileName+">: can't alloc memory for rotations for animation '"+name+"'!");
			}
			
			// Read all frames for translation
			for(unsigned i = 0; i < frames; i++) {
				// Read actual frame
				this->ifs >> time >> x >> y >> z >> angle >> tension >> continuity >> bias;
				if(this->ifs.fail()) {
					throw IOException("AffParser<"+this->fileName+">: Error in parsing rotation keyframes for '"+name+"' animation!");
				}
				
				// Init keyframe
				rKeys[i].time = time;
				rKeys[i].rot(x, y, z, DEG_TO_RAD(angle));
				rKeys[i].tension = tension;
				rKeys[i].continuity = continuity;
				rKeys[i].bias = bias;
			}
			
			// Adds all translation key frames to an animation and delete temporary array
			animation->SetRotations(rKeys, frames);
			delete [] rKeys;
			rKeys = NULL;
		}
		// If actual transformation is scale - 'scale'
		else if(transform == "scale") {
			// Allocates array for position keys
			pKeys = new(nothrow) TPositionKey[frames];
			if(pKeys == NULL) {
				throw IOException("AffParser<"+this->fileName+">: can't alloc memory for scales for animation '"+name+"'!");
			}
			
			// Read all frames for translation
			for(unsigned i = 0; i < frames; i++) {
				// Read actual frame
				this->ifs >> time >> x >> y >> z >> tension >> continuity >> bias;
				if(this->ifs.fail()) {
					throw IOException("AffParser<"+this->fileName+">: Error in parsing scale keyframes for '"+name+"' animation!");
				}
				
				// Init keyframe
				pKeys[i].time = time;
				pKeys[i].pos(x, y, z);
				pKeys[i].tension = tension;
				pKeys[i].continuity = continuity;
				pKeys[i].bias = bias;
			}
			
			// Adds all translation key frames to an animation and delete temporary array
			animation->SetScales(pKeys, frames);
			delete [] pKeys;
			pKeys = NULL;
		}
		// If actual transformation is visibility - 'visibility'
		else if(transform == "visibility") {
			// Read all frames for translation
			for(unsigned i = 0; i < frames; i++) {
				// Read actual frame
				this->ifs >> time >> visibility;
				if(this->ifs.fail()) {
					throw IOException("AffParser<"+this->fileName+">: Error in parsing visibility keyframes for '"+name+"' animation!");
				}
				
				// Adds visibility key to an animation
				animation->AddVisibilityKey(time, visibility);
			}
		}
		else {
			throw IOException("AffParser<"+this->fileName+">: Unknown keyframe type ('"+transform+"'). Must be 'transl', 'rot', 'scale' or 'visibility'!");
		}
		
		// Read next token
		this->ifs >> token;
	}
	
	// Adds animation to scene
	this->scene->AddAnimation(animation);
	
	return true;
} // ParseKeyFrames()



/**
 * Method for parsing static transformation node.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseStaticTransform()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Vector3 scale, rot, trans;
	float angle;
	string token;
	
	// Read all tranformation components in right order and opening '{'
	this->ifs >> scale >> rot >> angle >> trans >> token;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not read static transform!");
	}
	else if(token != "{") {
		throw IOException("AffParser<"+this->fileName+">: Expected '{', get '"+token+"'");
	}
	
	// Adds static transformation to scene
	this->scene->AddTransform(trans, rot, DEG_TO_RAD(angle), scale);
	
	return true;
} // ParseStaticTransform()



/**
 * Method for parsing animated transformation node.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseAnimatedTransform()
{
	// Test parser state
	this->CheckParserState();
	
	// Read transformation name and opening '{'
	string name, token;
	this->ifs >> name >> token;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not read animated transform!");
	}
	else if(token != "{") {
		throw IOException("AffParser<"+this->fileName+">: Expected '{', get '"+token+"'");
	}
	
	// Adds animated transformation to scene
	this->scene->AddTransform(name);	
	
	return true;
} // ParseAnimatedTransform()



/**
 * Method for parsing animation paramaters.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseAnimationParams()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	float start, end;
	unsigned frames;
	int loops;
	
	// Read animation's parameters - start, end and frames count
	this->ifs >> start >> end >> frames >> loops;
	
	if(this->ifs.fail()) {
		this->ifs.clear();
		loops = 0;
	}
	
	// Sets animation's parameters in scene
	this->scene->InitAnimationParams(start, end, frames, loops);
	
	return true;
} // ParseAnimationParams()



/**
 * Method for parsing scene ambient light color.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseAmbientLight()
{
	// Test parser state
	this->CheckParserState();
	
	// Read ambient light color
	Color ambientColor;
	this->ifs >> ambientColor;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Ambient Light parse error!");
	}
	
	// Sets ambient light entity
	this->scene->InitAmbientLight(ambientColor);
	
	return true;
} // ParseAmbientLight()



/**
 * Method for parsing material properties (extenden material).
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseFillMaterial()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	Color amb, dif, spc;
	float shine, transmit, ior;
	
	// Read ambient component
	this->ifs >> amb;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill Material parse error! Can't read ambient component.");
	}
	
	// Read diffuse component
	this->ifs >> dif;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill Material parse error! Can't read diffuse component.");
	}
	
	// Read specular component
	this->ifs >> spc;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill Material parse error! Can't read specular component.");
	}
	
	// Read others components
	this->ifs >> shine >> transmit >> ior;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Fill Material parse error! Can't read shine, transmittance or index of reflection.");
	}
	
	// Adds material to scene
	this->scene->AddMaterial(amb, dif, spc, 4.0f*shine, ior, transmit);
	
	return true;
} // ParseFillMaterial()



/**
 * Method for parsing animated light entity.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseAnimatedLight()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	string name;
	Vector3 pos;
	Color color;

	// Read light entity name
	this->ifs >> name;
	
	// Precte pozici svetla
	this->ifs >> pos;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Animated Light parse error!");
	}
	
	// Try read light color (optional parametr)
	this->ifs >> color;
	if(this->ifs.fail()) {
		this->ifs.clear();
		color = Color::White();
	}
	
	// Adds animated light to scene
	this->scene->AddLight(name, pos, color);
	
	return true;
} // ParseAnimatedLight()



/**
 * Method for parsing triangle mesh primitive.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::ParseMesh()
{
	// Test parser state
	this->CheckParserState();
	
	// Init auxiliary variables
	string type, textureName;		// item type (vertices, normals, texturecoords, triangles) and texture name
	Vector3 *vertices = NULL;		// array of vertices
	Vector3 *normals = NULL;		// array of normals
	Vector2 *coords = NULL;			// array of texture coords
	unsigned short *indices = NULL;	// array of triangles indices 
	unsigned numVertices = 0;		// vertices count
	unsigned numNormals = 0; 		// normals count
	unsigned numCoords = 0;			// texture coords count
	unsigned numTriangles = 0;		// triangles count
	
	// Read type - 'vertices'
	this->ifs >> type;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (could not find 'vertices')!");
	}
	if(type != "vertices") {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (expected 'vertices')!");
	}
	
	// Read all vectors and store in array
	this->ReadMeshVectors("vertices", vertices, numVertices);
	
	// Read type - 'normals'? (optional)
	this->ifs >> type;
	if(type == "normals") {
		// Read all normals and store in array
		this->ReadMeshVectors("normals", normals, numNormals);
		this->ifs >> type;
	}
	
	// Token 'texturecoords' has been already read
	if(type == "texturecoords") {
		// Read all texture coords and store in array
		this->ReadMeshCoords(textureName, coords, numCoords);
		this->ifs >> type;
	}
	
	// Token 'triangles' has been already read, so read all triangles and store in array
	if(type == "triangles") {
		this->ReadMeshTriangles(vertices, normals, coords, indices, numTriangles);
	}
	else {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (expected 'triangles')!");
	}
	
	// Add mesh primitive to scene
	this->scene->AddMesh(vertices, numVertices, normals, numNormals, coords, numCoords, this->baseDir, textureName, indices, numTriangles);
	
	// Clear all temporary arrays
	if(vertices) delete [] vertices;
	if(normals) delete [] normals;
	if(coords) delete [] coords;
	if(indices) delete [] indices;
	
	return true;
} // ParseMesh()



/**
 * Method for closing opened transformation node.
 * @return bool Returns true if parsing is OK, false otherwise.
 */ 
bool AffParser::CloseTranform()
{
	// Test parser state
	this->CheckParserState();
	
	// Close transformation
	this->scene->CloseTransform();
	
	return true;
} // CloseTranform()



/**
 * Auxiliary method for read mesh vector like vertices or normals from input.
 * @param string type Vector's type - vertices or normals,
 * @param Vector3 vectors Array of vectors which will be filled,
 * @param unsigned count Count of read vectors in array.
 * @return bool Returns true, if read is OK, false othrewise.
 */ 
bool AffParser::ReadMeshVectors(const string &type, Vector3 *(&vectors), unsigned &count)
{
	// Test parser state
	this->CheckParserState();
	
	// Read vectors count
	this->ifs >> count;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (expected 'num_"+type+"')!");
	}
	
	// Allocate memory to store all vectors in array
	vectors = new(nothrow) Vector3[count];
	if(vectors == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Could not allocate memory for vertices of mesh!");
	}
	
	// Read all vectors from input file (vertices or normals)
	for(unsigned i = 0; i < count; i++) {
		this->ifs >> vectors[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not read "+Dump(i)+"th "+type+" (total "+Dump(count)+") of mesh.!");
		}
	}
	
	return true;
} // ReadMeshVectors() 



/**
 * Auxiliary method for read mesh texture coordinates from input.
 * @param string name Texture name,
 * @param Vector3 coords Array of coordinates which will be filled,
 * @param unsigned count Count of read coordinates in array.
 * @return bool Returns true, if read is OK, false othrewise.
 */ 
bool AffParser::ReadMeshCoords(string &name, Vector2 *(&coords), unsigned &count)
{
	// Test parser state
	this->CheckParserState();
	
	// Read texture coordinates count and texture name
	this->ifs >> count >> name;
	name = this->baseDir+name;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (expected 'num_txts' or 'texturename')!");
	}
	
	// Allocate memory to store all vectors in array
	coords = new(nothrow) Vector2[count];
	if(coords == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Could not allocate memory for texturecoords of mesh!");
	}
	
	// Read all texture coordinates
	for(unsigned i = 0; i < count; i++) {
		this->ifs >> coords[i];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not read "+Dump(i)+"th texturecoords (total "+Dump(count)+") of mesh.!");
		}
	}
	
	return true;
} // ReadMeshCoords() 



/**
 * Auxiliary method for read mesh triangles defined by indices from input.
 * @param Vector3 vertices Array of vertices,
 * @param Vector3 normals Array of normals,
 * @param Vector2 coords Array of texture coordinates,
 * @param unsigned short indices Array of triangle indices,
 * @param unsigned count Count of read triangles in array.
 * @return bool Returns true, if read is OK, false othrewise.
 */ 
bool AffParser::ReadMeshTriangles(Vector3 *vertices, Vector3 *normals, Vector2 *coords, unsigned short *(&indices), unsigned &count)
{
	// Test parser state
	this->CheckParserState();
	
	// Test, if there are any vertices
	if(vertices == NULL) {
		throw InvalidArgumentException("AffParser<"+this->fileName+">: Theres no vertices in mesh!");
	}
	
	// Init auxiliary variables
	unsigned index = 0;
	unsigned v[3], n[3], c[3];
	unsigned allocSize = 3;
	if(normals) allocSize += 3;
	if(coords) allocSize += 3;
	
	// Read triangles count
	this->ifs >> count;
	if(this->ifs.fail()) {
		throw IOException("AffParser<"+this->fileName+">: Could not parse mesh (expected 'num_triangles')!");
	}
	
	// Allocate memory to store all indices in array
	indices = new(nothrow) unsigned short[allocSize*count];
	if(indices == NULL) {
		throw IOException("AffParser<"+this->fileName+">: Could not allocate memory for indices of mesh!");
	}
	
	// Read all triagles
	for(unsigned i = 0; i < count; i++) {
		// Read indices for triangle vertices
		this->ifs  >> v[0] >> v[1] >> v[2];
		if(this->ifs.fail()) {
			throw IOException("AffParser<"+this->fileName+">: Could not read "+Dump(i)+"th vertex indices of mesh!");
		}
		
		// If there are any normals (they are optional)
		if(normals) {
			// Read indices for triangle normals
			this->ifs >> n[0] >> n[1] >> n[2];
			if(this->ifs.fail()) {
				throw IOException("AffParser<"+this->fileName+">: Could not read "+Dump(i)+"th set of normal indices of mesh!");
			}
		}
		
		// If there are any texture coordinates (they are optional)
		if(coords) {
			// Read indices for triangle texture coordinates
			this->ifs >> c[0] >> c[1] >> c[2];
			if(this->ifs.fail()) {
				throw IOException("AffParser<"+this->fileName+">: Could not read "+Dump(i)+"th texturecoord indices of mesh!");
			}
		}
		
		// Copy indices from temporary arrays to one array in order [coords] [normals] vertices
		for(int j = 0; j < 3; j++) {
			if(coords) indices[index++] = c[j];
			if(normals) indices[index++] = n[j];
			indices[index++] = v[j];
		}
	}
	
	return true;
} // ReadMeshTriangles() 
	


/** ***************************************************************************/
/** end of file AffParser.cpp												  */
/** ***************************************************************************/
