//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CameraBase.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-06-01
//更新日:2016-06-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"CameraBase.h"

#include"../../API/Window.h"

#define FOVY			45.0f				//視野角
#define NEAR_CLIP		1.0f					//ニアクリップを行う距離
#define FAR_CLIP		10000.0f				//ファークリップを行う距離

// 固定カメラ位置
#define CAMERA_POS_X	0.0f
#define CAMERA_POS_Y	0.0f
#define CAMERA_POS_Z	-500.0f

// 固定カメラ注視点
#define LOOK_POS_X		0.0f
#define LOOK_POS_Y		0.0f
#define LOOK_POS_Z		0.0f

// 固定カメラ上方向ベクトル
#define UP_DIR_X		0.0f
#define UP_DIR_Y		1.0f
#define UP_DIR_Z		0.0f

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CCameraBase::CCameraBase():
	C3DObjectBase("Camera"){
		
	m_fFovy		= FOVY;
	m_fAspect		= (float)GetWindowClass()->GetClientWidth() / GetWindowClass()->GetClientHeight();
	m_fZNear		= NEAR_CLIP;
	m_fZFar		= FAR_CLIP;
	m_Position	= D3DXVECTOR3(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z);
	m_At			= D3DXVECTOR3(LOOK_POS_X, LOOK_POS_Y, LOOK_POS_Z);
	m_Up			= D3DXVECTOR3(UP_DIR_X, UP_DIR_Y, UP_DIR_Z);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
CCameraBase::~CCameraBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化
void CCameraBase::Initialize(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&m_MtxView, &m_Position, &m_At, &m_Up);
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
	
	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(
		&m_MtxProjection, D3DXToRadian(FOVY),
		m_fAspect, m_fZNear, m_fZFar);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

	m_spTransform->SetPosition(m_Position);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//事前設定
void CCameraBase::PreDraw(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();

	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//終了処理
void CCameraBase::Finalize(){
	C3DObjectBase::Finalize();
}
