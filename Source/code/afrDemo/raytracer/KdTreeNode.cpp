/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    KdTreeNode.cpp													  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class KdTreeNode.								  *
 **************************************************************************** */


// Including project libraries
#include "KdTreeNode.h"

#include "KdTree.h"

#include "../primitives/BBox.h"
#include "../scene/Scene.h"
#include "../types/Ray.h"



/**
 * Default KdTreeNode constructor.
 * @return KdTreeNode.
 */ 
KdTreeNode::KdTreeNode()
{
	this->scene = NULL;
	this->tree = NULL;
	this->parent = NULL;
	
	this->left = NULL;
	this->right = NULL;
	
	this->splitPlane = -1;
	this->splitPos = 0.0f;
	this->primitives.Clear();
} // KdTreeNode()



/**
 * Default KdTreeNode constructor.
 * @param Scene s Scene object, should be not NULL,
 * @param KdTree t Kd Tree object, should be not NULL,
 * @param KdTreeNode p Node parent, if is NULL, this node will be root.
 * @return KdTreeNode.
 */ 
KdTreeNode::KdTreeNode(Scene *s, KdTree *t, KdTreeNode *p)
{
	this->scene = s;
	this->tree = t;
	this->parent = p;
	
	this->left = NULL;
	this->right = NULL;
	
	this->splitPlane = -1;
	this->splitPos = 0.0f;
	this->primitives.Clear();
} // KdTreeNode()



/**
 * Default destructor
 */
KdTreeNode::~KdTreeNode()
{
	this->scene = NULL;
	this->tree = NULL;
	this->parent = NULL;
	
	if(this->left) {
		delete this->left;
		this->left = NULL;
	}
	if(this->right) {
		delete this->right;
		this->right = NULL;
	}
	
	this->splitPlane = -1;
	this->splitPos = 0.0f;
	
	this->primitives.Clear();
} // ~KdTreeNode()



/**
 * Method for inserting primitive into this node.
 * @param Primitive p Primitive which will be added.
 * @return KdTreeNode Provides fluent interface.
 */ 
KdTreeNode *KdTreeNode::AddPrimitive(Primitive *p)
{
	if(p != NULL) {
		this->primitives.Add(p);
	}
	
	return this;
} // AddPrimitive()



/**
 * Method for subdivide actual tree node into two new nodes, left and right.
 * This method is recursively called for all node's children.
 * @param BBox box Surrounding bounding box,
 * @param short depth Actual level of nesting.
 * @return bool Returns true, if subdivision is ok, false otherwise.
 */ 
bool KdTreeNode::Subdivide(const BBox &box, const short &depth)
{
	cout << "Subdividing box (" << box.GetMin() << ", " << box.GetMax() << "), depth: " << depth << ", count: " << this->primitives.Size();
	
	// Get new splitting plane from box size
	this->splitPlane = box.GetSize().MaxAbsComponentIndex();
	if(this->splitPlane < 0) {
		return false;
	}
	
	// Init auxiliary variables for boundaries on splitting plane
	float bMin = box.GetMin()[this->splitPlane];
	float bMax = box.GetMax()[this->splitPlane];
	
	// Init storages for store minimal and maximal bound of primitives
	unsigned primCount = this->primitives.Size();
	float *eLeft = new float[primCount];
	float *eRight = new float[primCount];
	
	float pMin = 0.0;	// minimum at the plane axis
	float pMax = 0.0;	// maximum at the plane axis
	TSplitItemList splitItems;
	
	// Iterate over all primitives in this node and get boundaries on actual 
	// splitting plane and create split positions	
	for(unsigned i = 0; i < primCount; i++) {
		BBox *pBox = this->primitives[i]->GetBBox();
		pMin = pBox->GetMin()[this->splitPlane];
		pMax = pBox->GetMax()[this->splitPlane];
		
		eLeft[i] = pMin;
		eRight[i] = pMax;
		
		if(bMin <= pMin && pMin <= bMax) {
			this->AddSplitPosition(pMin, splitItems);
		}
		if(bMin <= pMax && pMax <= bMax) {
			this->AddSplitPosition(pMax, splitItems);
		}
	}
	
	// Sort split positions in list
	unsigned splitCount = splitItems.Size();
	for(unsigned i = 1; i < splitCount; i++) {
		for(unsigned j = 0; j < splitCount - i; j++) {
			if(splitItems[j].position > splitItems[j + 1].position) {
				TSplitItem pom = splitItems[j];
				splitItems[j] = splitItems[j + 1];
				splitItems[j + 1] = pom;
			}  
		}
	}
	
	
	
	// Boxes for left and right child nodes
	BBox leftBox;
	BBox rightBox;
	
	// Init auxiliary variables for SAH algorithm for choose best split position
	Vector3 bSize = box.GetSize();
	float sah = 0.5f / (bSize.x() * bSize.y() + bSize.x() * bSize.z() + bSize.y() * bSize.z());
	float cleaf = primCount;
	float lowCost = 10000.0f;
	float bestPos = bMin + bSize[this->splitPlane] * 0.5f;
	
	// Choose best split position using SAH (surface area heuristic) algorithm
	for(unsigned i = 0; i < splitCount; i++) {
		BBox tmpLBox = box;
		BBox tmpRBox = box;
	
		// Split main boundary box to two boxes
		Vector3 pos = tmpRBox.GetMin();
		pos[this->splitPlane] = splitItems[i].position;
		tmpRBox.SetMin(pos);
		
		pos = tmpRBox.GetMax();
		pos[this->splitPlane] = bMax;
		tmpRBox.SetMax(pos);
		
		pos = tmpLBox.GetMax();
		pos[this->splitPlane] = splitItems[i].position;
		tmpLBox.SetMax(pos);
		
		// Get min and max points of inner boxes at actual splitting plane
		float lbMin = tmpLBox.GetMin()[this->splitPlane];
		float lbMax = tmpLBox.GetMax()[this->splitPlane];
		float rbMin = tmpRBox.GetMin()[this->splitPlane];
		float rbMax = tmpRBox.GetMax()[this->splitPlane];
		
		// Check all primitiv in this node, where they lies
		for(unsigned j = 0; j < primCount; j++) {
			if(eRight[j] >= lbMin && eLeft[j] <= lbMax) {
				if(this->primitives[j]->IntersectBBox(tmpLBox)) {
					splitItems[i].leftCount++;
				}
			}
			
			if(eRight[j] >= rbMin && eLeft[j] <= rbMax) {
				if(this->primitives[j]->IntersectBBox(tmpRBox)) {
					splitItems[i].rightCount++;
				}
			}
		}
		
		// Calculate cost for this split position and if this cost good enought
		// save it for future usage
		Vector3 lbSize = tmpLBox.GetSize();
		Vector3 rbSize = tmpRBox.GetSize();
		
		float SAL = 2.0f * (lbSize.x() * lbSize.y() + lbSize.x() * lbSize.z() + lbSize.y() * lbSize.z());
		float SAR = 2.0f * (rbSize.x() * rbSize.y() + rbSize.x() * rbSize.z() + rbSize.y() * rbSize.z());
		float splitCost = 0.3f + (SAL * sah * splitItems[i].leftCount + SAR * sah * splitItems[i].rightCount);
		if (splitCost < lowCost) {
			lowCost = splitCost;
			bestPos = splitItems[i].position;
			leftBox = tmpLBox;
			rightBox = tmpRBox;
		}
	}
	
	
	// Best cost is not goot enought - stop partitiong
	if(lowCost > cleaf) {
		cout << endl;
		return false;
	}
	
	// Else create new child nodes and add primitives in it
	this->splitPos = bestPos;
	float lbMin = leftBox.GetMin()[this->splitPlane];
	float lbMax = leftBox.GetMax()[this->splitPlane];
	float rbMin = rightBox.GetMin()[this->splitPlane];
	float rbMax = rightBox.GetMax()[this->splitPlane];
	
	this->left = new KdTreeNode(this->scene, this->tree, this);
	this->right = new KdTreeNode(this->scene, this->tree, this);
	
	for(unsigned i = 0; i < primCount; i++) {
		if(eRight[i] >= lbMin && eLeft[i] <= lbMax) {
			if(this->primitives[i]->IntersectBBox(leftBox)) {
				this->left->AddPrimitive(this->primitives[i]);
			}
		}
		
		if(eRight[i] >= rbMin && eLeft[i] <= rbMax) {
			if(this->primitives[i]->IntersectBBox(rightBox)) {
				this->right->AddPrimitive(this->primitives[i]);
			}
		}
	}
	
	cout << ", LC: " << this->left->GetPrimitiveCount() << ", RC: " << this->right->GetPrimitiveCount() << endl;
	
	// Delete auxiliary arrays
	delete [] eLeft;
	delete [] eRight;
	
	// If depth is grather than zero, subdivide both child nodes
	if(depth > 0) {
		if(this->left->GetPrimitiveCount() > 2) {
			this->left->Subdivide(leftBox, (depth-1));
		}

		if(this->right->GetPrimitiveCount() > 2) {
			this->right->Subdivide(rightBox, (depth-1));
		}
	}
	
	return false;
} // Subdivide()



/**
 * Auxiliary method for insert new split position into temporary list, where 
 * all split positions are stored. New split position will be inserted only
 * if in list does't exists.
 * @param TSplitItemList list List of all already stored split positions,
 * @param float p New split position.
 * @return bool Returns true, if position does't exists in list, false otherwise.
 */ 
bool KdTreeNode::AddSplitPosition(const float &p, TSplitItemList &list)
{
	// Check if given position is in list
	unsigned size = list.Size();
	for(unsigned i = 0; i < size; i++) {
		if(list[i].position == p) {
			return false;
		}
	}
	
	// Or create new split item and stored it into list
	TSplitItem spItem;
	spItem.position = p;
	spItem.leftCount = 0;
	spItem.rightCount = 0;
	
	list.Add(spItem);
	
	return true;
} // AddSplitPosition()



/** ***************************************************************************/
/** end of file KdTreeNode.cpp												  */
/** ***************************************************************************/
