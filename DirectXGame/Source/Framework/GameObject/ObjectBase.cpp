//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-05-26
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"ObjectBase.h"
#include"ObjectManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CObjectBase::CObjectBase(std::string ObjectName){
	m_ObjectName = ObjectName;
	m_bDelete = false;
	GetObjectManager()->SetObject(ObjectName, this);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CObjectBase::~CObjectBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������
void CObjectBase::Initialize(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�X�V
void CObjectBase::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�s������
void CObjectBase::DrawNoAlpha(){
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//��������
void CObjectBase::DrawAlpha(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�J��
void CObjectBase::Finalize(){
	//m_bDelete = true;
}