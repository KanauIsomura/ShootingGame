//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[LightBase.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-09-12
//�X�V��:2015-09-12
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"LightBase.h"

#include"../../System/Device.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
LightBase::LightBase() : C3DObjectBase("Light"){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
LightBase::~LightBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������
void LightBase::Initialize(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	
	//���C�g�̐ݒ�
	D3DXVECTOR3	vecDir;									// �������w�肷��x�N�g��
	D3DLIGHT9		Light;									// ���C�g�I�u�W�F�N�g
	ZeroMemory(&Light, sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;						// ���s����
	Light.Diffuse.r = Light.Diffuse.g = 
	Light.Diffuse.b = Light.Diffuse.a = 1.0f;
	Light.Ambient.r = Light.Ambient.g = 
	Light.Ambient.b = Light.Ambient.a = 1.0f;
	Light.Specular = Light.Diffuse;
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 1.0f);					// ���̌��������߂�
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vecDir);	// ���K�������x�N�g����Ԃ�
	pDevice->SetLight(0, &Light);
	pDevice->LightEnable(0, TRUE);							// ���C�g0��L��
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);		// ���ʔ��˂�L��

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
//�X�V
void LightBase::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���
void LightBase::Finalize(){
}
