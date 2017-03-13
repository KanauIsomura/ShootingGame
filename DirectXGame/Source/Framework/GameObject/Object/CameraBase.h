//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CameraBase.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-06-01
//更新日:2016-06-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______CAMERA_BASE_CLASS_H_INCLUDE_____
#define ______CAMERA_BASE_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"../3DObjectBase/3DObjectBase.h"
#include"../../System/Device.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//カメラベース
class CCameraBase : public C3DObjectBase{
protected:
	D3DXMATRIX	m_MtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_MtxView;		// ビューマトリックス
	D3DXVECTOR3	m_Position;		// 視点
	D3DXVECTOR3	m_At;			// 注視点
	D3DXVECTOR3	m_Up;			// 上方ベクトル
	float		m_fFovy;			// 視野角
	float		m_fAspect;		// アスペクト比
	float		m_fZNear;			// 前方クリップ距離
	float		m_fZFar;			// 後方クリップ距離

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
