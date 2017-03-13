//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-05-25
//更新日:2015-05-25
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"3DObjectBase.h"

//=== 定数定義 ===

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
