#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"

#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

class Game;

struct RenderItem
{
	RenderItem() = default;
	// World matrix of the shape that describes the object's local space
	// relative to the world space, which defines the position, orientation,
	// and scale of the object in the world.
	XMFLOAT4X4 World = MathHelper::Identity4x4();
	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
	Material* Mat = nullptr;
	int NumFramesDirty = gNumFrameResources;
	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;
	MeshGeometry* Geo = nullptr;
	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
	
	
};

class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;


public:
	SceneNode(Game* game);

	void					attachChild(Ptr child);
	Ptr						detachChild(const SceneNode& node);

	void					update(const GameTimer& gt);
	void					draw() const;
	void					build();

	XMFLOAT3				getWorldPosition() const;
	void					setPosition(float x, float y, float z);
	XMFLOAT3				getWorldRotation() const;
	void					setWorldRotation(float x, float y, float z);
	XMFLOAT3				getWorldScale() const;
	void					setScale(float x, float y, float z);

	XMFLOAT4X4				getWorldTransform() const;
	XMFLOAT4X4				getTransform() const;

	void					move(float x, float y, float z);
private:
	virtual void			updateCurrent(const GameTimer& gt);
	void					updateChildren(const GameTimer& gt);

	virtual void			drawCurrent() const;
	void					drawChildren() const;
	virtual void			buildCurrent();
	void					buildChildren();

protected:
	Game* game;
	RenderItem* renderer;
private:
	XMFLOAT3				mWorldPosition;
	XMFLOAT3				mWorldRotation;
	XMFLOAT3				mWorldScaling;
	std::vector<Ptr>		mChildren;
	SceneNode* mParent;
};
//// List of all the render items.
//std::vector<std::unique_ptr<RenderItem>> mAllRitems;
//// Render items divided by PSO.
//std::vector<RenderItem*> mOpaqueRitems;


