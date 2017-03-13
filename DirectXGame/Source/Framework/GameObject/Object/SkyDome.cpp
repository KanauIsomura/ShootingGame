//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-00
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"SkyDome.h"

#include"../../System/Device.h"

#include"../ObjectManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
SkyDome::SkyDome(): CModelObject("SkyDome", _T("../Data/Model/Sky/Sky.x")),
	m_pCameraObj(NULL){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
SkyDome::~SkyDome(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化
void SkyDome::Initialize(){
	CModelObject::Initialize();

	m_pCameraObj = (C3DObjectBase *)GetObjectManager()->FindObject("Camera");
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//更新処理
void SkyDome::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//開放
void SkyDome::Finalize(){
	CModelObject::Finalize();
	m_pCameraObj = NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//後更新
void SkyDome::PostUpdate(){
	D3DXVECTOR3 Position(0.0f, 0.0f, 0.0f);

	if(m_pCameraObj)
		m_spTransform->SetPosition(m_pCameraObj->GetTransform()->GetPosition());
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//事前描画
void SkyDome::PreDraw(){
	if(!m_pCameraObj)
		return;
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	CModelObject::DrawNoAlpha();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
