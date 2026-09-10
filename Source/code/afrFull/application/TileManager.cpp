/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06	 									  *
 * @file    TileManager.cpp													  *	
 * @date	15. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Implementation of class TileManager.							  *
 **************************************************************************** */



// Including project libraries
#include "TileManager.h"
#include "Sampler.h"



/**
 * Default constructor.
 * @return TileManager.
 */ 
TileManager::TileManager()
{
	this->hTilesNum = 1;
	this->vTilesNum = 1;
	
	this->Init();
} // TileManager()



/**
 * Constructor with number of tiles on horizontal and vertical direction.
 * @param unsigned h Number of tiles horizontaly,
 * @param unsigned v Number of tiles verticaly.
 * @return TileManager.
 */ 
TileManager::TileManager(const Vector2 &r, const unsigned &d, const unsigned &h, const unsigned &v)
{
	this->hTilesNum = h;
	this->vTilesNum = v;
	this->resolution = r;
	this->depth = d;
	
	this->Init();
} // TileManager()



/**
 * Default desctructor. Destroy tile manager and delete all tiles and tiles info.
 */ 
TileManager::~TileManager() 
{	
	// Destroy tiles tree
	this->currNode = NULL;
	if(!this->tilesTree.Empty()) {
		for(unsigned i = 0; i < this->tilesTree.Size(); i++) {
			delete this->tilesTree[i];
		}
		this->tilesTree.Clear();
	}
	
	// Destroy tile list
	if(!this->tilesList.Empty()) {
		for(unsigned i = 0; i < this->tilesList.Size(); i++) {
			delete this->tilesList[i];
		}
		this->tilesList.Clear();
	}
	
	// Destroy tile buffer
	if(this->tilesBuffer != NULL) {
		delete this->tilesBuffer;
		this->tilesBuffer = NULL;
	}
	
	// Destroy tile buffer
	if(this->densityBuffer != NULL) {
		delete this->densityBuffer;
		this->densityBuffer = NULL;
	}
	
} // ~TileManager() 



/** ***************************************************************************/
/** Tile manager operations												      */
/** ***************************************************************************/



/**
 * Initialize tile manager's properties into default state.
 * @return TileManager.
 */ 
TileManager *TileManager::Init()
{	
	// Check if number of horizontal tiles is even or 1
	if(this->hTilesNum > 1 && this->hTilesNum & 1 == 1) {
		this->hTilesNum -= 1;
	}
	// Check if number of horizontal tiles is even or 1
	if(this->vTilesNum > 1 && this->vTilesNum & 1 == 1) {
		this->vTilesNum -= 1;
	}
	
	// Calc number of all tiles
	this->tilesNum = this->hTilesNum * this->vTilesNum;
	
	// Calc tile size in each direction
	this->tileSize.x(1.0f / (float)this->hTilesNum)
				  .y(1.0f / (float)this->vTilesNum);
				  
	// Calc pixel per tile
	this->pixelsPerTile = (this->resolution.Mult() * this->tileSize.Mult());
	

	// Create image buffer to store tiles variances for output
	this->tilesBuffer = new Image(this->resolution.x(), this->resolution.y());
	this->densityBuffer = new Image(this->resolution.x(), this->resolution.y());
	
	
	// Create tiles and store it to list
	float variance = 1.0f / (float)this->tilesNum;
	unsigned maxSamples = this->pixelsPerTile * this->depth;
	for(unsigned r = 0; r < this->vTilesNum; r++) {
		for(unsigned c = 0; c < this->hTilesNum; c++) {
			Vector2 offset(c*this->tileSize.x(), r*this->tileSize.y());
			Tile *tile = new Tile(variance, maxSamples, Vector2(c, r), offset, this->tileSize);
			
			this->tilesList.Add(tile);
		}		
	}
	
	// Currently selected node
	this->currNode = NULL;
	
	// Create root node
	TTileNodeInfo *root = new TTileNodeInfo;
	root->variance = 1.0f;
	root->index = 0;
	root->tile = NULL;
	root->parent = NULL;
	root->left = NULL;
	root->right = NULL;
	
	if(this->tilesNum == 1) {
		root->tile = this->tilesList[0];
	}
	this->tilesTree.Add(root);
	
	
	// Creates all tree
	unsigned index = 0;
	unsigned tileIndex = 0;
	unsigned nodesNum = 2*this->tilesNum - 1;
	while(index < nodesNum - this->tilesNum) {
		TTileNodeInfo *node = this->tilesTree[index];
		
		// Compute variance for both childs
		float variance = node->variance / 2.0f;
		variance = (variance <= TILE_STARV_TRESH) ? TILE_STARV_TRESH : variance;
		
		// Create left child
		TTileNodeInfo *left = new TTileNodeInfo;
		left->variance = variance;
		left->index = 2*index + 1;
		left->parent = node;
		left->left = NULL;
		left->right = NULL;
		left->tile = NULL;
		if(left->index >= nodesNum - this->tilesNum) {
			left->tile = this->tilesList[tileIndex++];
			left->variance = left->tile->GetVariance();
		}
		node->left = left;
		this->tilesTree.Add(left);
		
		// Create right child
		TTileNodeInfo *right = new TTileNodeInfo;
		right->variance = variance;
		right->index = 2*index + 2;
		right->tile = NULL;
		right->parent = node;
		right->left = NULL;
		right->right = NULL;
		if(right->index >= nodesNum - this->tilesNum) {
			right->tile = this->tilesList[tileIndex++];
			right->variance = right->tile->GetVariance();
		}
		node->right = right;
		this->tilesTree.Add(right);
		
		index++;
	}
	
	
	// Tree height
	this->treeHeight = floor(log2(this->tilesTree.Size()));
	
	
	return this;
} // Init()



/**
 * Method for get tile from binary tree in range between zero and R.
 * @return Tile Returns selected tile.
 */
Tile *TileManager::GetTileWithinRange()
{
	// Check if tree is not empty
	if(this->tilesTree.Empty()) {
		return NULL;
	}
	
	
	// Init auxiliary variables
	this->currNode = this->tilesTree[0];					// current node from tile
	float r = this->random() * this->currNode->variance;	// max variance	
	
	
	// Find and return propriate tile
	for(unsigned i = 0; i <= this->treeHeight; i++) {
		if(this->currNode->tile != NULL) {
			return this->currNode->tile;
		}
		
		// Go to left child
		if(r < this->currNode->left->variance) {
			this->currNode = this->currNode->left;
		}
		else {
			r -= this->currNode->left->variance;
			this->currNode = this->currNode->right;
		}
	}
	
	
	// If tile has not been found, return NULL
	return NULL;
} // GetTileWithinRange()



/**
 * Method for reacumulating variances for all nodes in tree.
 * @param Tile t Currently selected tile.
 * @return TileManager Provides fluent interface.
 */ 
TileManager *TileManager::UpdateVariances(const Tile* const tile)
{
	// Assert
	if(tile == NULL || this->currNode == NULL) {
		cerr << "Tile or current node is NULL! Can't update variances" << endl;
		return this;
	}


	// Update variance for currently selected node in previous iteration
	this->currNode->variance = tile->GetVariance();
	
	// Get currently selected node's parent and propaget variace to root
	TTileNodeInfo *node = this->currNode->parent;
	while(node != NULL) {
		// Calculate new variance as sum of variance of all childs
		node->variance = node->left->variance + node->right->variance;
		
		// Go to upper level
		node = node->parent;
	}
	
	return this;
} // ReacumullateVariances()



/**
 * Method for get tiles buffer, which will be updated to current tiles variance.
 * @return Image Returns tiles buffer as image.
 */ 
Image *TileManager::GetTilesBuffer()
{
	// Check if tiles buffer exists
	if(this->tilesBuffer == NULL) {
		this->tilesBuffer = new Image(this->resolution.x(), this->resolution.y());
	}
	
	
	// Get max variance for 'simple proportion' to maximize max variance (to 255R color)
	float maxVar = 0.0f;
	for(unsigned i = 0; i < this->tilesList.Size(); i++) {
		maxVar = Max(maxVar, this->tilesList[i]->GetVariance());
	}
	
	
	// Update buffer
	for(unsigned i = 0; i < this->tilesList.Size(); i++) {
		Tile *tile = this->tilesList[i];
		Vector2 offset = tile->GetOffset();
		Color clr = tile->ToColor() / maxVar;
		this->tilesBuffer->Fill(offset.x(), offset.y(), this->tileSize.x(), this->tileSize.y(), clr);
	}
	
	return this->tilesBuffer;
} // GetTilesBuffer()



/** ***************************************************************************/
/** Setters													          		  */
/** ***************************************************************************/



/**
 * Method for update sample rate of all tiles in tile manager.
 * @param unsigned timeSpan Time span for evaluate which samples are valid,
 * @param unsigned now Current animation time.
 * @return TileManager Provides fluent interface.
 */ 
TileManager	*TileManager::UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now)
{
	// Sets to all tiles
	for(unsigned i = 0; i < this->tilesList.Size(); i++) {
		this->tilesList[i]->UpdateSamplingRate(timeSpan, now);
	}
	
	return this;
} // UpdateSamplingRate()
 


/** ***************************************************************************/
/** end of file TileManager.cpp												  */
/** ***************************************************************************/
