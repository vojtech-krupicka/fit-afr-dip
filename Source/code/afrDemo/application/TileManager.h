/** ***************************************************************************
 * AFR - Adaptive Frameless Rendering 										  *	
 * @author	Krupicka Vojtech, xkrupi06	 									  *
 * @file    TileManager.h													  *	
 * @date	15. 03. 2012													  *
 * @note  	ISO 8859-2														  *
 * @brief 	Definition of class TileManager.								  *
 **************************************************************************** */
 

// Conditional compilation 
#ifndef _AFR_TILE_MANAGER_H
#define _AFR_TILE_MANAGER_H



// Including system libraries
#include <string>



// Including project libraries
#include "../types/Array.h"
#include "../types/Image.h"
#include "../types/Tile.h"
#include "../types/Vector2.h"
#include "../utils/Random.h"



// Predefinition of classes
class Image;
class Vector2;
class Tile;



/**
 * @brief Structure for store informations about tiles tree node.
 */ 
typedef struct s_ttnode {
	float 		variance;
	unsigned	index;
	Tile		*tile;
	struct 		s_ttnode *parent;
	struct 		s_ttnode *left;
	struct 		s_ttnode *right;
} TTileNodeInfo;



/**
 * Class TileManager
 * @brief Class for handling image tiling and choosing tile of interest. 
 * Managing tile tree and spread variance from tree leaf to its root. Its 
 * implement guiding sampling in adaptive variant.
 */ 
class TileManager
{
public:
	///< Implicit constructors
	TileManager();
	TileManager(const Vector2 &r, const unsigned &d, const unsigned &h, const unsigned &v);
	
	~TileManager();
	
	
	///< Tile manager operations
	Tile 		*GetTileWithinRange();
	TileManager *UpdateVariances(const Tile* const tile);
	float		GetStarvationThreshold() const { return (this->tilesTree[0]->variance * 0.1f); };
	Image		*GetTilesBuffer();
	Image		*GetDensityBuffer();
	TileManager *UpdateSamplingRate(const unsigned &timeSpan, const unsigned &now);
	
	
	///< Getters
	inline Tile		*GetTile(const unsigned &i) { 
		Tile *tile = this->tilesList[i]; 
		for(int i = 0; i < this->tilesTree.Size(); i++) { 
			if(tile == this->tilesTree[i]->tile) { 
				this->currNode = this->tilesTree[i];
			} 
		} 
		
		return tile; 
	};
	inline unsigned	GetHTilesNum() const { return this->hTilesNum; };
	inline unsigned	GetVTilesNum() const { return this->vTilesNum; };
	inline unsigned	GetTotalTilesNum() const { return this->tilesNum; };	
	inline Vector2	GetTileSize() const { return this->tileSize; };
	inline Vector2	GetTileSizeInPixels() const { return this->tileSize * this->resolution; };
	inline float	GetPixelsPerTile() const { return this->pixelsPerTile; };
	
	///< Setters
	TileManager		*SetSamplingMethod(const string &m);
	TileManager		*SetSamplingMethod(const unsigned &m);
	TileManager		*SetSampleCacheSize(const unsigned &s);
	
	
	
private:
	///< Private application operations
	TileManager		*Init();
	
	
	///< Private class properties
	unsigned		hTilesNum;			///< number of tiles horizontaly
	unsigned		vTilesNum;			///< number of tiles verticaly
	unsigned		tilesNum;			///< number of all tiles
	
	Vector2			tileSize;			///< size of tile as vector		
	float			pixelsPerTile;		///< number of pixels covered by each tile
	unsigned		depth;				///< depth of sample buffer
	
	Random			random;				///< pseundo random number generator
	
	Vector2			resolution;			///< full image plane size in pixel as 2D vector (w*h)
	Image			*tilesBuffer;		///< image buffer for store variance of all tiles
	Image			*densityBuffer;		///< image buffer for density of all tiles
	
	TTileNodeInfo	*currNode;			///< currently selected node from tiles tree
	TTileList		tilesList;			///< list of all tiles
	Array<TTileNodeInfo*> 	tilesTree;	///< binary tree of tiles info
	unsigned		treeHeight;			///< height of tile node info tree 

}; // class TileManager



#endif // #ifndef _AFR_TILE_MANAGER_H



/** ***************************************************************************/
/** end of file TileManager.h											  	  */
/** ***************************************************************************/
