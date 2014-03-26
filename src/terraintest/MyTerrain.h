#ifndef TESTTERRAIN_H
#define TESTTERRAIN_H

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainQuadTreeNode.h>
#include <OgreTerrainMaterialGenerator.h>
#include <OgreTerrainPaging.h>

#define TERRAIN_SIZE 2000

class MyTerrain
{
public:
    void setup(Ogre::SceneManager* pSceneMgr, Ogre::Light* l);

    void defineTerrain(long x, long y, bool flat = false);
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
    void configureTerrainDefaults(Ogre::Light* l);
    Ogre::Terrain* getTerrain();
    Ogre::TerrainGroup* getTerrainGroup()
    {
        return mTerrainGroup;
    }

    void initBlendMaps(Ogre::Terrain* terrain);

private:
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::SceneManager* mSceneMgr;
};

#endif