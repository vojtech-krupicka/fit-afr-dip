/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06 										  *
 * @file    SceneNode.cpp													  *	
 * @date	16. 11. 2011													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class SceneNode.								  *
 **************************************************************************** */


// Including project libraries
#include "SceneNode.h"

#include "Scene.h"
#include "../types/Matrix.h"



/**
 * Default constructor. Initialize scene to default state.
 * @return SceneNode.
 */ 
SceneNode::SceneNode()
{
	// Sets properties
	this->scene = NULL;
	this->parent = NULL;
	this->name = "";
	this->staticBranch = true;
	this->initMatrix = Matrix::Identity();	
	this->finalMatrix = Matrix::Identity();
	
	this->primitives.Clear();
	this->children.Clear();
} // SceneNode()



/**
 * Default constructor. Initialize scene to default state.
 * @return SceneNode.
 */ 
SceneNode::SceneNode(Scene *s, SceneNode *p, const string &n, const Matrix &m) throw (InvalidArgumentException)
{
	// Test, if scene is object and it does't created
	if(s == NULL || s->IsCreated()) {
		throw InvalidArgumentException("Scene is NULL or it has been already created!");
	}
	
	// Sets properties
	this->scene = s;
	this->parent = p;
	this->name = n;
	this->staticBranch = SceneNode::CalcNodeBranch(this);
	this->initMatrix = m;	
	this->finalMatrix = SceneNode::CalcMatrix(this);
	
	this->primitives.Clear();
	this->children.Clear();	
} // SceneNode()



/**
 * Default destructor.
 */ 
SceneNode::~SceneNode()
{
	// Delete all primitives in this node
	for(unsigned i = 0; i < this->primitives.Size(); i++) {
		delete this->primitives[i];
	}
	this->primitives.Clear();
	
	// Delete all child nodes in this node
	for(unsigned i = 0; i < this->children.Size(); i++) {
		delete this->children[i];
	}
	this->children.Clear();
	
	// Sets defaults
	this->scene = NULL;
	this->parent = NULL;
	this->name = "";
	this->initMatrix = Matrix::Identity();
	this->finalMatrix = Matrix::Identity();
} // ~SceneNode()



/**
 * Method for transformation all primitives in actual node.
 * @param SceneNode root Root node.
 * @return SceneNode Provides fluent interface.
 */ 
SceneNode *SceneNode::Transform(SceneNode *root)
{
	// Transform all child nodes
	for(unsigned i = 0; i < this->children.Size(); i++) {
		this->children[i]->Transform(root);
	}
	
	// Calculate matrix for transformation all primitives
	Matrix m = SceneNode::CalcMatrix(this, root);
	
	// Transform all primitives in this node
	for(unsigned i = 0; i < this->primitives.Size(); i++) {
		this->primitives[i]->Transform(m);
	}
	
	return this;
} // Transform()



/**
 * Method for add new child node into this node.
 * @param SceneNode n New child node, which will be added.
 * @return SceneNode Provides fluent interface.
 */ 
SceneNode *SceneNode::AddChild(SceneNode *n)
{
	if(n != NULL) {
		this->children.Add(n);
	}
	
	return this;	
} // AddChild()



/**
 * Method for add new primitive into this node.
 * @param Primitive p New primitive, which will be added.
 * @return SceneNode Provides fluent interface.
 */ 
SceneNode *SceneNode::AddPrimitive(Primitive *p)
{
	if(p != NULL) {
		if(this->staticBranch) {
			p->Transform(this->finalMatrix);
		}
		this->primitives.Add(p);
	}
	
	return this;	
} // AddPrimitive()



/**
 * Method for determine if this node is in static branch.
 * @param SceneNode node Scene node, for which will be calculate if is in static branch.
 * @return bool Returns true, if this node is in static branch, false otherwise.
 */ 
bool SceneNode::CalcNodeBranch(SceneNode *node)
{
	// If this node is dynamic, return false
	if(node == NULL || !node->IsStatic()) {
		return false;
	}
	
	// Node's parent
	SceneNode *parent = node->GetParent();
	
	// While parent node is not a root node
	while(parent != NULL) {
		if(!parent->IsStatic()) {
			return false;
		}
		
		parent = parent->GetParent();
	}
	
	return true;
} // CalcNodeBranch()



/**
 * Static method for calculate final tranformation matrix of 'node' in 
 * scene graph.
 * @param SceneNode node Base node for calculate matrix.
 * @return Matrix Returns transformation matrix for 'node'.
 */ 
Matrix SceneNode::CalcMatrix(SceneNode *node)
{
	Matrix m = node->GetInitMatrix();
	SceneNode *parent = node->GetParent();
	
	while(parent != NULL) {
		m = parent->GetInitMatrix() * m;
		parent = parent->GetParent();
	}

	return m.Transpose();
} // CalcMatrix()



/**
 * Static method for calculate final transformation matrix of node 'node' to 
 * 'root' node.
 * @param SceneNode node Starting node,
 * @param SceneNode root Ending root node.
 * @return Matrix Returns transformation matrix.  
 */ 
Matrix SceneNode::CalcMatrix(SceneNode *node, SceneNode *root)
{
	Matrix m = node->GetInitMatrix();	
	SceneNode *parent = node->GetParent();
	
	while(parent != NULL) {
		m = parent->GetInitMatrix() * m;
		if(parent == root) {
			break;
		}
		
		parent = parent->GetParent();
	}
	
	return m.Transpose();
} // CalcMatrix()



/** ***************************************************************************/
/** end of file SceneNode.cpp												  */
/** ***************************************************************************/
