#include <fstream>

#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"


extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    // read the map tile definitions from the .map file
    std::fstream mapFile;
    mapFile.open(filePath);

    // below logic basically chooses which segment of the map catalogue to load based on a txt file
    // txt file is a 2d matrix, whose values represent the index of the tile to choose from the catalogue --> srcRect (what to draw)
    // position of the value within this map file represents where the tile should be drawn --> dstRect (how to draw)
    for(int y = 0; y < mapSizeY; y++) {
        for(int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = atoi(&ch) * tileSize;
            
            AddTile(srcRectX, srcRectY, x * (scale*tileSize), y * (scale*tileSize));

            mapFile.ignore(); // skipping the comma
        }
    }
    mapFile.close();

}

void Map::AddTile(int sourceRectX, int sourceRectY, int destRectx, int destRecty) {
    // Add a new tile entity in the game scene
    Entity& newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, destRectx, destRecty, tileSize, scale, textureId);

}