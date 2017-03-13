//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Transform.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-05-04
//�X�V��:2016-05-04
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______TRANSFORM_CLASS_H_INCLUDE_____
#define ______TRANSFORM_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"d3dx9.h"

#include"../System/CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class Transform{
private:
	D3DXMATRIX m_World;
	CSmartPointer<Transform> m_spParent;

public:
	Transform();
	virtual ~Transform();
	
	//���W�̈ړ�
	void AddPosition(D3DXVECTOR3 AddPos);
	//��]
	void Rotation(D3DXVECTOR3 vAngle);					//�S����]
	void RotationX(float fAngleX);						//X����]
	void RotationY(float fAngleY);						//Y����]
	void RotationZ(float fAngleZ);						//Z����]
	void RotationAxis(D3DXVECTOR3 vAxis, float fAngle);	//�����w�肵�ĉ�]

	void LockAtTransform(D3DXVECTOR3& Pos);
	
	//--- �Z�b�^�[ ---
	void SetWorld(D3DXMATRIX World){m_World = World;}
	void SetPosition(D3DXVECTOR3 Position){
		m_World._41 = Position.x; m_World._42 = Position.y; m_World._43 = Position.z;}
	void SetParent(CSmartPointer<Transform> spParent){
		m_spParent = spParent;}

	//--- �Q�b�^�[ ---
	D3DXMATRIX GetLocalWorld(){return m_World;}
	D3DXMATRIX GetWorld();

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	D3DXVECTOR3 GetUp();

	D3DXVECTOR3 GetLocalPosition();
	D3DXVECTOR3 GetLocalForward();
	D3DXVECTOR3 GetLocalRight();
	D3DXVECTOR3 GetLocalUp();
};

#endif
