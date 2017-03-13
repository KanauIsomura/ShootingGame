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

#ifndef ______BOX_COLLIDER_H_INCLUDE_____
#define ______BOX_COLLIDER_H_INCLUDE_____

//=== �C���N���[�h ===
#include"ColliderBase.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�{�b�N�X�R���C�_�[
class BoxCollider : public ColliderBase{
private:
	D3DXVECTOR3	m_BoundingBoxSize;		//���E�{�b�N�X(�n�[�t�T�C�Y)
	LPD3DXMESH	m_pBoundingBoxMesh;	//���E�{�b�N�X���b�V��
	
public:
	BoxCollider();
	virtual ~BoxCollider();
	
	void Create(CSmartPointer<Transform> spTransform, D3DXVECTOR3& Center, D3DXVECTOR3& Size);
	bool CollisionOBB(CSmartPointer<BoxCollider> spOtherCollider);

	void Draw();
	
	//--- �Q�b�^�[ ---
	D3DXVECTOR3 GetCenter(){return m_Center;}
	D3DXVECTOR3 GetBoundingBox(){return m_BoundingBoxSize;}
};

#endif
