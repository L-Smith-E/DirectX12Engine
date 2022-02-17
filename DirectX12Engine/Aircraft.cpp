#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
, m_Type(type)
{
	switch (type)
	{
	case (Type::Eagle):
		m_Sprite = "Eagle";
		break;
	case (Type::Raptor):
		m_Sprite = "Raptor";
		break;
	default:
		m_Sprite = "Eagle";
		break;
	}
}

void Aircraft::drawCurrent() const
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (m_AircraftRender!= nullptr)
	{
		game->getCmdList()->IASetVertexBuffers(0, 1, &m_AircraftRender->Geo->VertexBufferView());
		game->getCmdList()->IASetIndexBuffer(&m_AircraftRender->Geo->IndexBufferView());
		game->getCmdList()->IASetPrimitiveTopology(m_AircraftRender->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(m_AircraftRender->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)m_AircraftRender->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)m_AircraftRender->Mat->MatCBIndex * matCBByteSize;

		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);

		game->getCmdList()->DrawIndexedInstanced(m_AircraftRender->IndexCount, 1, m_AircraftRender->StartIndexLocation, m_AircraftRender->BaseVertexLocation, 0);
	}
}

//void Aircraft::buildCurrent()
//{
//	auto render = std::make_unique<RenderItem>();
//	renderer = render.get();
//	renderer->World = getTransform();
//	renderer->ObjCBIndex = (UINT)game->getRenderItems().size();
//	renderer->Mat = game->getMaterials()[m_Sprite].get();
//	renderer->Geo = game->getGeometries()["boxGeo"].get();
//	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
//	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
//	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
//	m_AircraftRender = render.get();
//	game->getRenderItems().push_back(std::move(render));
//}