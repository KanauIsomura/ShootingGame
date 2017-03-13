//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelObject.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-05-31
//�X�V��:2016-05-31
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
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
