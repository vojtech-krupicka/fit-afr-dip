/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    KdTree.h														  *	
 * @date	01. 12. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class KdTree.										  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_KD_TREE_H
#define _AFR_KD_TREE_H


// Including project libraries
#include "../primitives/BBox.h"


// Predefinition of classes
class KdTreeNode;
class Scene;
class Ray;



/**
 * Class KdTree
 * @brief Class for implement whole kd-tree structure and its creation.
 */ 
class KdTree
{
public:
	///< Implicit constructors
	KdTree();
	KdTree(Scene *s, const short &d);
	
	///< Implicit destructors
	~KdTree();
	
	///< Getters
	inline KdTreeNode	*GetRoot() const { return root; };
	inline BBox			GetSceneBox() const { return sceneBox; };
	
	///< Kd Tree operations
	
private:
	Scene		*scene;
	KdTreeNode 	*root;
	BBox 		sceneBox;
	
	short		depth;
	
}; // class KdTree



#endif // #ifndef _AFR_KD_TREE_H



/** ***************************************************************************/
/** end of file KdTree.h													  */
/** ***************************************************************************/
