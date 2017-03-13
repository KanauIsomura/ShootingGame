//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-0
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______BOX_COLLIDER_H_INCLUDE_____
#define ______BOX_COLLIDER_H_INCLUDE_____

//=== インクルード ===
#include"ColliderBase.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ボックスコライダー
class BoxCollider : public ColliderBase{
private:
	D3DXVECTOR3	m_BoundingBoxSize;		//境界ボックス(ハーフサイズ)
	LPD3DXMESH	m_pBoundingBoxMesh;	//境界ボックスメッシュ
	
public:
	BoxCollider();
	virtual ~BoxCollider();
	
	void Create(CSmartPointer<Transform> spTransform, D3DXVECTOR3& Center, D3DXVECTOR3& Size);
	bool CollisionOBB(CSmartPointer<BoxCollider> spOtherCollider);

	void Draw();
	
	//--- ゲッター ---
	D3DXVECTOR3 GetCenter(){return m_Center;}
	D3DXVECTOR3 GetBoundingBox(){return m_BoundingBoxSize;}
};

#endif
