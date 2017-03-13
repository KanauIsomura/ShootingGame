//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelObject.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-05-31
//更新日:2016-05-31
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"ModelObject.h"
#include"../../DXGraphic/ModelManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
CModelObject::CModelObject(std::string ObjName, std::wstring FilePath):
	C3DObjectBase(ObjName){
	m_MeshFilePath = FilePath;
}

CModelObject::~CModelObject(){
	m_MeshFilePath.clear();
}

void CModelObject::Initialize(){
	C3DObjectBase::Initialize();
	GetModelManager()->Load(m_MeshFilePath.c_str());
}

void CModelObject::Update(){
}

void CModelObject::DrawAlpha(){
	GetModelManager()->RenderModelAlpha(m_MeshFilePath.c_str(), m_spTransform->GetWorld());
}

void CModelObject::DrawNoAlpha(){
	GetModelManager()->RenderModelNoAlpha(m_MeshFilePath.c_str(), m_spTransform->GetWorld());
}

void CModelObject::Finalize(){
	GetModelManager()->Release(m_MeshFilePath.c_str());
	C3DObjectBase::Finalize();
}
