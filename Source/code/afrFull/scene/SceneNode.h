/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SceneNode.h														  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class SceneNode.									  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_SCENE_NODE_H
#define _AFR_SCENE_NODE_H


// Including system libraries
#include <string>



// Including project libraries
#include "../exceptions/Exceptions.h"
#include "../primitives/Primitive.h"
#include "../types/Array.h"


// Predefinition of classes
class Scene;
class Matrix;
class SceneNode;



/**
 * Type definition for list of scene nodes.
 */ 
typedef Array<SceneNode *> TSceneNodeList;



/**
 * Class SceneNode
 * @brief One node of scene graph for transformations its geometry.
 */ 
class SceneNode
{
public:
	///< Default constructors
	SceneNode();
	SceneNode(Scene *s, SceneNode *p = NULL, const string &n = "", const Matrix &m = Matrix::Identity()) throw (InvalidArgumentException);
	
	///< Default destructors
	~SceneNode();	
	
	///< Scene node operations
	SceneNode *Transform(SceneNode *root);
	
	///< Getters
	inline string			GetName() const { return this->name; };
	inline Scene			*GetScene() const { return this->scene; };
	inline SceneNode 		*GetParent() const { return this->parent; };
	inline Matrix			GetInitMatrix() const { return this->initMatrix; };
	inline Matrix			GetFinalMatrix() const { return this->finalMatrix; };
	inline TPrimitiveList	&GetPrimitives() { return this->primitives; };
	inline TSceneNodeList	&GetChildren() { return this->children; };
	
	///< Setters
	inline SceneNode *SetMatrix(const Matrix &m) { this->initMatrix = m; return this; };
	SceneNode *AddChild(SceneNode *n);
	SceneNode *AddPrimitive(Primitive *p);
	
	///< Others
	inline bool IsRoot() const { return (this->parent == NULL); };
	inline bool IsLeaf() const { return this->children.Empty(); };
	inline bool IsStatic() const { return (this->name == ""); };
	inline bool IsStaticBranch() const { return this->staticBranch; };
	
	static Matrix	CalcMatrix(SceneNode *node);
	static Matrix	CalcMatrix(SceneNode *node, SceneNode *root);
	static bool		CalcNodeBranch(SceneNode *node);
	
private:
	
	///< Properties
	TPrimitiveList	primitives;		///< list of all primitives in this node
	TSceneNodeList	children;		///< list of all children in this node
	
	Matrix			initMatrix;		///< initialization transformation matrix for this node
	Matrix 			finalMatrix;	///< final transformation matrix for this node
	string			name;			///< animated node name
	bool			staticBranch;	///< flag which indicates, that this node is in static branch
	
	Scene			*scene;			///< scene object (scene graph)
	SceneNode		*parent;		///< node's parent (if node is root, parent is NULL)
	
}; // class SceneNode



#endif // #ifndef _AFR_SCENE_NODE_H



/** ***************************************************************************/
/** end of file SceneNode.h													  */
/** ***************************************************************************/
	
