/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    Edge.h															  *	
 * @date	24. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief   Definition of edge between two vertices.						  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_EDGE_H
#define _AFR_EDGE_H


// Including project libraries
#include "Vertex.h"
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
typedef struct s_edge {
	TVertex		from;		///< edge's start vertex
	TVertex	 	to;			///< edge's ending vertex
	Vector3		edge;		///< real edge vector as subtraction of to - from
} TEdge;



#endif // #ifndef _AFR_EDGE_H



/** ***************************************************************************/
/** end of file Edge.h													 	  */
/** ***************************************************************************/

