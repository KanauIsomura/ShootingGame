//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-0
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______COLLIDER_BASE_H_INCLUDE_____
#define ______COLLIDER_BASE_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../Framework/System/CSmartPointer.h"
#include"../Framework/Component/Transform.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class ColliderBase{
protected:
	CSmartPointer<Transform> m_spTransform;	//�e�̍��W
	D3DXVECTOR3	m_Center;					//���E�����S���W
	D3DCOLORVALUE	m_MeshColor;				//���E�{�b�N�X�̐F
	bool			m_bHit;					//�������Ă��邩

public:
	ColliderBase(){};
	virtual ~ColliderBase(){};

	CSmartPointer<Transform> GetTransform(){return m_spTransform;}
	void SetHit(){m_bHit = true;}
	bool GetHit(){return m_bHit;}
};

#endif
