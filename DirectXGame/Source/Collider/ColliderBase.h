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

#ifndef ______COLLIDER_BASE_H_INCLUDE_____
#define ______COLLIDER_BASE_H_INCLUDE_____

//=== インクルード ===
#include"../Framework/System/CSmartPointer.h"
#include"../Framework/Component/Transform.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class ColliderBase{
protected:
	CSmartPointer<Transform> m_spTransform;	//親の座標
	D3DXVECTOR3	m_Center;					//境界球中心座標
	D3DCOLORVALUE	m_MeshColor;				//境界ボックスの色
	bool			m_bHit;					//あたっているか

public:
	ColliderBase(){};
	virtual ~ColliderBase(){};

	CSmartPointer<Transform> GetTransform(){return m_spTransform;}
	void SetHit(){m_bHit = true;}
	bool GetHit(){return m_bHit;}
};

#endif
