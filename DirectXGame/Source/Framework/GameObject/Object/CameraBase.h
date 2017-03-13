//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CameraBase.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-06-01
//�X�V��:2016-06-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______CAMERA_BASE_CLASS_H_INCLUDE_____
#define ______CAMERA_BASE_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../3DObjectBase/3DObjectBase.h"
#include"../../System/Device.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�J�����x�[�X
class CCameraBase : public C3DObjectBase{
protected:
	D3DXMATRIX	m_MtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_MtxView;		// �r���[�}�g���b�N�X
	D3DXVECTOR3	m_Position;		// ���_
	D3DXVECTOR3	m_At;			// �����_
	D3DXVECTOR3	m_Up;			// ����x�N�g��
	float		m_fFovy;			// ����p
	float		m_fAspect;		// �A�X�y�N�g��
	float		m_fZNear;			// �O���N���b�v����
	float		m_fZFar;			// ����N���b�v����

public:
	CCameraBase();
	virtual ~CCameraBase();
	
	virtual void Initialize();
	virtual void Update(){};
	virtual void Finalize();

	virtual void PostUpdate(){};
	virtual void PreDraw();

	virtual D3DXMATRIX GetViewMatrix(){return m_MtxView;}
};

#endif
