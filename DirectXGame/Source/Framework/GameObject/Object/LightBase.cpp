//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[LightBase.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-09-12
//更新日:2015-09-12
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"LightBase.h"

#include"../../System/Device.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
LightBase::LightBase() : C3DObjectBase("Light"){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
LightBase::~LightBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化
void LightBase::Initialize(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	
	//ライトの設定
	D3DXVECTOR3	vecDir;									// 方向を指定するベクトル
	D3DLIGHT9		Light;									// ライトオブジェクト
	ZeroMemory(&Light, sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;						// 平行光源
	Light.Diffuse.r = Light.Diffuse.g = 
	Light.Diffuse.b = Light.Diffuse.a = 1.0f;
	Light.Ambient.r = Light.Ambient.g = 
	Light.Ambient.b = Light.Ambient.a = 1.0f;
	Light.Specular = Light.Diffuse;
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 1.0f);					// 光の向きを決める
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);	// 正規化したベクトルを返す
	pDevice->SetLight(0, &Light);
	pDevice->LightEnable(0, TRUE);							// ライト0を有効
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);		// 鏡面反射を有効

	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//更新
void LightBase::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//解放
void LightBase::Finalize(){
}
