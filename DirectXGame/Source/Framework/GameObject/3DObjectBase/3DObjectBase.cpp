//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-05-25
//�X�V��:2015-05-25
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"3DObjectBase.h"

//=== �萔��` ===

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
C3DObjectBase::C3DObjectBase(std::string ObjName):
	CObjectBase(ObjName){
	m_spTransform.SetPointer(new Transform);
}

C3DObjectBase::~C3DObjectBase(){
}

void C3DObjectBase::Initialize(){
	CObjectBase::Initialize();
}

void C3DObjectBase::Update(){
}

void C3DObjectBase::DrawAlpha(){
}

void C3DObjectBase::DrawNoAlpha(){
}

void C3DObjectBase::Finalize(){
	CObjectBase::Finalize();
}
