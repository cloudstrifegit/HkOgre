#include "MyTerrain.h"
#include <XkDebugStr.h>
#include <OgreMath.h>

void MyTerrain::setup(Ogre::SceneManager* pSceneMgr, Ogre::Light* l)
{
    mSceneMgr = pSceneMgr;
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    //Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Terrain");

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(pSceneMgr,
        Ogre::Terrain::ALIGN_X_Z, 513, TERRAIN_SIZE);
    mTerrainGroup->setFilenameConvention("myTerrain", "dat");
    mTerrainGroup->setOrigin(Ogre::Vector3(0, 0, 0));
    //mTerrainGroup->setResourceGroup("Terrain");

    configureTerrainDefaults(l);
}

void MyTerrain::defineTerrain(long x, long y, bool flat)
{
    // if a file is available, use it
	// if not, generate file from import

	// Usually in a real project you'll know whether the compact terrain data is
	// available or not; I'm doing it this way to save distribution size

	if (flat)
	{
		mTerrainGroup->defineTerrain(x, y, 0.0f);
	}
	else
	{
		Ogre::String filename = mTerrainGroup->generateFilename(x, y);
		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
		{
			mTerrainGroup->defineTerrain(x, y);
		}
		else
		{
            
			Ogre::Image img;
			getTerrainImage(x % 2 != 0, y % 2 != 0, img);
            
            /*
            Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource("terrain.raw");
            size_t size = stream.get()->size();
            XkDebugString("size: %d", size);

            float* buffer = OGRE_ALLOC_T(float, size, Ogre::MEMCATEGORY_GENERAL);
            stream->read(buffer, size);

            mTerrainGroup->defineTerrain(x, y, 200);
            */
			mTerrainGroup->defineTerrain(x, y, &img);
		}
	}
}

void MyTerrain::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("myterrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

void MyTerrain::configureTerrainDefaults(Ogre::Light* l)
{
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
    mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(Ogre::ColourValue(0.8, 0.8, 0.8));
    mTerrainGlobals->setCompositeMapDiffuse(Ogre::ColourValue(0.2, 0.2, 0.2));
    
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = TERRAIN_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	defaultimp.layerList[1].worldSize = 30;
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	defaultimp.layerList[2].worldSize = 200;
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");

    defineTerrain(0, 0, false);

    mTerrainGroup->loadAllTerrains(true);

    initBlendMaps(getTerrain());
}

Ogre::Terrain* MyTerrain::getTerrain()
{
    Ogre::Terrain* pTerrain = mTerrainGroup->getTerrain(0, 0);

    return pTerrain;
}

void MyTerrain::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;


		}
	}
	blendMap0->dirty();
	blendMap1->dirty();
	//blendMap0->loadImage("blendmap1.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	blendMap0->update();
	blendMap1->update();

}