/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    KdTreeNode.h													  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class KdTreeNode.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_KD_TREE_NODE_H
#define _AFR_KD_TREE_NODE_H


// Including project libraries
#include "../primitives/Primitive.h"
#include "../types/Array.h"


// Predefinition of classes
class Scene;
class KdTree;
class BBox;
class Ray;
class Vector3;



/**
 * @brief New type definition of split item for keep informations about split position
 * for kd tree node.
 */ 
typedef struct s_splititem {
	float 		position;	///< position on spliting plane (axis)
	unsigned 	leftCount;	///< posible count of primitives on the left side from split position
	unsigned	rightCount;	///< posible count of primitives on the right side from split position
} TSplitItem;



/**
 * @brief Type of array for store all split items into list.
 */ 
typedef Array<TSplitItem> TSplitItemList;



// Predefinition of classes
class KdTreeNode;



/**
 * @brief Type for stact item for traversing tree and nodes in right order when node
 * is find for find closest node for intersecting primitives with ray.
 */ 
typedef struct s_kdstackitem {
	KdTreeNode	*node;	///< kd tree node in the stack
	float		t;		///< 't' param on the ray - tmax
	Vector3		hit;	///< hit point on the bounded box
	short		prev;	///< index of previous node
} TKdStackItem;



/**
 * @brief Type of array for store all stack items into list. 
 */ 
typedef Array<TKdStackItem> TKdStackItemList;



/**
 * Class KdTreeNode
 * @brief Implementation of one node of kd-tree structure.
 */ 
class KdTreeNode
{
public:
	///< Implicit constructors
	KdTreeNode();
	KdTreeNode(Scene *s, KdTree *t, KdTreeNode *p = NULL);
	
	///< Implicit destructors
	~KdTreeNode();
	
	///< Kd Tree operations
	inline bool IsLeaf() const { return (left == NULL && right == NULL); };
	inline bool IsRoot() const { return (parent == NULL); };
	
	KdTreeNode 	*AddPrimitive(Primitive *p);
	bool		Subdivide(const BBox &box, const short &depth);
	
	///< Getters
	inline Scene			*GetScene() const { return scene; };
	inline KdTree	 		*GetTree() const { return tree; };
	inline KdTreeNode 		*GetParent() const { return parent; };
	inline KdTreeNode 		*GetLeftChild() const { return left; };
	inline KdTreeNode 		*GetRightChild() const { return right; };
	
	inline Primitive		*GetPrimitive(const unsigned &i) const { return primitives[i]; };
	inline TPrimitiveList 	&GetPrimitiveList() { return primitives; };
	inline unsigned			GetPrimitiveCount() const { return primitives.Size(); };
	
	inline float 			GetSplitPosition() const { return splitPos; };
	inline short			GetSplitPlane() const { return splitPlane; };
	
	///< Setters
	inline KdTreeNode	*SetScene(Scene *s) { scene = s; return this; };
	inline KdTreeNode	*SetTree(KdTree *t) { tree = t; return this; };
	inline KdTreeNode 	*SetParent(KdTreeNode *p) { parent = p; return this; };
	inline KdTreeNode 	*SetLeftChild(KdTreeNode *l) { left = l; return this; };
	inline KdTreeNode 	*SetRightChild(KdTreeNode *r) { right = r; return this; };
	
	inline KdTreeNode 	*SetSplitPosition(const float &p) { splitPos = p; return this; };
	inline KdTreeNode	*SetSplitPlane(const short &p) { splitPlane = p; return this; };
	
private:
	bool 				AddSplitPosition(const float &p, TSplitItemList &list);


	Scene		*scene;		///< Scene object
	KdTree		*tree;		///< Kd Tree parent object
	
	KdTreeNode 	*parent;	///< Parent node
	KdTreeNode	*left;		///< Left child
	KdTreeNode	*right;		///< Right child
	
	float 		splitPos;	///< Position of spliting plane
	short		splitPlane;	///< Spliting plane
	
	TPrimitiveList	primitives;	///< List of all primitives		
	
}; // class KdTreeNode



#endif // #ifndef _AFR_KD_TREE_NODE_H



/** ***************************************************************************/
/** end of file KdTreeNode.h												  */
/** ***************************************************************************/
