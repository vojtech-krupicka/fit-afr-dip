/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Vertex.h														  *	
 * @date	24. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief   Type definition of vertex.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_VERTEX_H
#define _AFR_VERTEX_H


// Including project libraries
#include "Vector2.h"
#include "Vector3.h"



// Used namespaces
using namespace std;



// Predefinition of classes
class Vector2;
class Vector3;



/**
 * @brief Structure Vertex for definition vectex as object with its position, normal 
 * and texturing coordinates.
 */ 
typedef struct s_vrtx {
	Vector3		position;		///< vertex position in 3D space
	Vector3 	normal;			///< vertex normal vector
	Vector2		coords;			///< vertex texture coordinates
} TVertex;



#endif // #ifndef _AFR_VERTEX_H



/** ***************************************************************************/
/** end of file Vertex.h													  */
/** ***************************************************************************/

