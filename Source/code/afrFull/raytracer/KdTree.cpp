/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    KdTree.cpp														  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class KdTree.									  *
 **************************************************************************** */


// Including system libraries
#include <limits>


// Including project libraries
#include "KdTree.h"
#include "KdTreeNode.h"

#include "../scene/Scene.h"
#include "../types/Ray.h"



/**
 * Default KdTree constructor.
 * @return KdTree.
 */ 
KdTree::KdTree()
{
	this->scene = NULL;
	this->root = NULL;
	
	this->depth = 0;
} // KdTree()



/**
 * Default KdTree constructor.
 * @return KdTree.
 */ 
KdTree::KdTree(Scene *s, const short &d)
{
	// Init Kd Tree
	this->scene = s;
	this->depth = d;
	
	// Creates new root node
	this->root = new KdTreeNode(s, this, NULL);
	
	// Create main scene bounding box
	float pInf = numeric_limits<float>::max();
	float mInf = numeric_limits<float>::min();
	Vector3 min(pInf, pInf, pInf);
	Vector3 max(mInf, mInf, mInf);
	
	TPrimitiveList geometry = this->scene->GetPrimitiveList();
	for(unsigned i = 0; i < geometry.Size(); i++) {
		BBox *bbox = geometry[i]->GetBBox();
		min = Min(min, bbox->GetMin());
		max = Max(max, bbox->GetMax());
		
		// Adds all primitives into root node
		if(this->root) {
			this->root->AddPrimitive(geometry[i]);
		}
	}
	
	min -= 10.0f;
	max += 10.0f;
	this->sceneBox = BBox(min, max);
	
	// Adds all primitives into root node
	if(this->root) {
		this->root->Subdivide(this->sceneBox, this->depth);
	}
	
} // KdTree()



/**
 * Default KdTree destructor.
 * @return KdTree.
 */ 
KdTree::~KdTree()
{
	this->scene = NULL;
	
	if(this->root) {
		delete this->root;
		this->root = NULL;
	}
	
	this->depth = 0;
} // ~KdTree()



/** ***************************************************************************/
/** end of file KdTree.cpp													  */
/** ***************************************************************************/
