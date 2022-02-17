#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"
#include "../../Common/UploadBuffer.h"

#include "SceneNode.hpp"
#include "SpriteNode.hpp"

#include "FrameResource.h"
#include <ctime>
#include <vector>

#include "Aircraft.hpp"


class World
{
public:
	explicit			World(Game* game);
	void				update(const GameTimer& gt);
	void				draw();
	void				buildScene();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Game* mGame;

	SceneNode* mSceneGraph;
	

	XMFLOAT4							mWorldBounds;
	XMFLOAT2		    				mSpawnPosition;
	float								mScrollSpeed;
	Aircraft* mPlayerAircraft;
	Aircraft* plane1;
	Aircraft* plane2;
	SpriteNode* mBackground;
	Aircraft* mEnemy;
	std::array<SceneNode*, 2>	mSceneLayers;
	
};