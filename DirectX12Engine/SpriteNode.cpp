#include "Game.hpp"
#include "SpriteNode.hpp"

SpriteNode::SpriteNode(Game* game) : Entity(game)
{

}

void SpriteNode::drawCurrent() const
{
	renderer->World = getTransform();
	renderer->NumFramesDirty++;

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (SpriteNodeRender != nullptr)
	{
		game->getCmdList()->IASetVertexBuffers(0, 1, &SpriteNodeRender->Geo->VertexBufferView());
		game->getCmdList()->IASetIndexBuffer(&SpriteNodeRender->Geo->IndexBufferView());
		game->getCmdList()->IASetPrimitiveTopology(SpriteNodeRender->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(SpriteNodeRender->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + SpriteNodeRender->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + SpriteNodeRender->Mat->MatCBIndex * matCBByteSize;

		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->getCmdList()->DrawIndexedInstanced(SpriteNodeRender->IndexCount, 1, SpriteNodeRender->StartIndexLocation, SpriteNodeRender->BaseVertexLocation, 0);
	}
}

void SpriteNode::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(50.0f, 50.0f, 1.0f));
	renderer->ObjCBIndex = (UINT)game->getRenderItems().size();
	renderer->Mat = game->getMaterials()["Desert"].get();
	renderer->Geo = game->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
	SpriteNodeRender = render.get();
	game->getRenderItems().push_back(std::move(render));
}
