#include <irrlicht.h>
#include <driverChoice.h>
#include<cmath>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
	//------------------Create devece  and window

	E_DRIVER_TYPE driverType = driverChoiceConsole();
	IrrlichtDevice* device = createDevice(driverType,dimension2d<u32>(1024,720),16,false,false,false,0);
	if(!device)return 1;
	device->setWindowCaption(L"Irrlicht LOD");


	//---------------------- Parametrs ----------

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//------------------ Camera ----------------

	ICameraSceneNode *  camera = smgr->addCameraSceneNodeFPS(0,50.0f,0.2f);
	camera ->setPosition(vector3df(0,0,45)); //position camera
	camera ->setTarget(vector3df(0,0,0)); // Look up on object
	device->getCursorControl()->setVisible(false);

	//----------------- LOW ----------------LOD

	IAnimatedMesh* low_tree_mesh = smgr->getMesh("resources/meshes/tree_trunk_lod2.obj");
	if(!low_tree_mesh)
	{
		device->drop();
		return 1;
	}

	ISceneNode* low_tree_node = smgr->addAnimatedMeshSceneNode( low_tree_mesh );
	if(low_tree_node)
	{
		low_tree_node->setMaterialFlag(EMF_LIGHTING, false);
		low_tree_node->setPosition(vector3df(0,0,0));
		low_tree_node->setMaterialTexture( 0, driver->getTexture("resources/textures/tree_trunk.tga") );
		low_tree_node->setScale(vector3df(7.0f,7.0f,7.0f));
	}

	//--------------  Medium ----------------LOD

	IAnimatedMesh* medium_tree_mesh = smgr->getMesh("resources/meshes/tree_trunk_lod1.obj");
	if(!medium_tree_mesh)
	{
		device->drop();
		return 1;
	}

	ISceneNode* medium_tree_node = smgr->addAnimatedMeshSceneNode( medium_tree_mesh );
	if(medium_tree_node)
	{
		medium_tree_node->setMaterialFlag(EMF_LIGHTING, false);
		medium_tree_node->setPosition(vector3df(0,0,0));
		medium_tree_node->setMaterialTexture( 0, driver->getTexture("resources/textures/tree_trunk.tga") );
		medium_tree_node->setScale(vector3df(7.0f,7.0f,7.0f));
	}

	//-------------- FULL -----------------LOD

	IAnimatedMesh* full_tree_mesh = smgr->getMesh("resources/meshes/tree_trunk_lod1.obj");
	if(!full_tree_mesh)
	{
		device->drop();
		return 1;
	}

	ISceneNode* full_tree_node = smgr->addAnimatedMeshSceneNode( full_tree_mesh );
	if(full_tree_node)
	{
		full_tree_node->setMaterialFlag(EMF_LIGHTING, false);
		full_tree_node->setPosition(vector3df(0,0,0));
		full_tree_node->setMaterialTexture( 0, driver->getTexture("resources/textures/tree_trunk.tga") );
		full_tree_node->setScale(vector3df(7.0f,7.0f,7.0f));
	}

	//--------------- Flore ---------------- LOD

	IAnimatedMesh* full_flore_mesh = smgr->getMesh("resources/meshes/tree_foliage_lod0.obj");
	if(!full_flore_mesh)
	{
		device->drop();
		return 1;
	}

	ISceneNode* full_flore_node = smgr->addAnimatedMeshSceneNode( full_flore_mesh );
	if(full_flore_node)
	{
		full_flore_node->setMaterialFlag(EMF_LIGHTING, false);
		full_flore_node->setPosition(vector3df(0,0,0));
		full_flore_node->setMaterialTexture( 0, driver->getTexture("resources/textures/tree_foliage.tga") );
		full_flore_node->setScale(vector3df(7.0f,7.0f,7.0f));
	}

	//-------------------------------------- General 

    //scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
    //    driver->getTexture("tex/skybox_top.tga"),
    //    driver->getTexture("tex/terrain.tga"),
    //    driver->getTexture("tex/skybox_west.tga"),
    //    driver->getTexture("tex/skybox_east.tga"), 
    //    driver->getTexture("tex/skybox_north.tga"), 
    //    driver->getTexture("tex/skybox_south.tga"));
    //skybox->setVisible(true);

	int lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true,SColor(255,255,255,255));
		f32 distance = camera->getPosition().getDistanceFrom(full_tree_node->getPosition());
		if (distance<=50)
		{
			full_tree_node->setVisible(true);
			medium_tree_node->setVisible(false);
			low_tree_node->setVisible(false);
			full_flore_node->setVisible(true);
		}
		else if(distance>50 && distance<=100)
		{
			full_tree_node->setVisible(false);
			medium_tree_node->setVisible(true);
			low_tree_node->setVisible(false);
			full_flore_node->setVisible(true);
		}
		else
		{
			full_tree_node->setVisible(false);
			medium_tree_node->setVisible(false);
			low_tree_node->setVisible(true);
			full_flore_node->setVisible(true);
		}

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht LOD [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;
			str += " distance: ";
			str += distance;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();
	return 0;
}