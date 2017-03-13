//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Transform.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-05-04
//更新日:2016-05-04
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______TRANSFORM_CLASS_H_INCLUDE_____
#define ______TRANSFORM_CLASS_H_INCLUDE_____

//=== インクルード ===
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
	
	//座標の移動
	void AddPosition(D3DXVECTOR3 AddPos);
	//回転
	void Rotation(D3DXVECTOR3 vAngle);					//全軸回転
	void RotationX(float fAngleX);						//X軸回転
	void RotationY(float fAngleY);						//Y軸回転
	void RotationZ(float fAngleZ);						//Z軸回転
	void RotationAxis(D3DXVECTOR3 vAxis, float fAngle);	//軸を指定して回転

	void LockAtTransform(D3DXVECTOR3& Pos);
	
	//--- セッター ---
	void SetWorld(D3DXMATRIX World){m_World = World;}
	void SetPosition(D3DXVECTOR3 Position){
		m_World._41 = Position.x; m_World._42 = Position.y; m_World._43 = Position.z;}
	void SetParent(CSmartPointer<Transform> spParent){
		m_spParent = spParent;}

	//--- ゲッター ---
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
