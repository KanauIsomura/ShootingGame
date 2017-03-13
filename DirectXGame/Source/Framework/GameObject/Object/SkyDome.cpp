//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-00
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"SkyDome.h"

#include"../../System/Device.h"

#include"../ObjectManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
SkyDome::SkyDome(): CModelObject("SkyDome", _T("../Data/Model/Sky/Sky.x")),
	m_pCameraObj(NULL){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
SkyDome::~SkyDome(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������
void SkyDome::Initialize(){
	CModelObject::Initialize();

	m_pCameraObj = (C3DObjectBase *)GetObjectManager()->FindObject("Camera");
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�X�V����
void SkyDome::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�J��
void SkyDome::Finalize(){
	CModelObject::Finalize();
	m_pCameraObj = NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//��X�V
void SkyDome::PostUpdate(){
	D3DXVECTOR3 Position(0.0f, 0.0f, 0.0f);

	if(m_pCameraObj)
		m_spTransform->SetPosition(m_pCameraObj->GetTransform()->GetPosition());
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���O�`��
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
