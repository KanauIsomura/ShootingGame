//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CameraBase.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-06-01
//�X�V��:2016-06-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"CameraBase.h"

#include"../../API/Window.h"

#define FOVY			45.0f				//����p
#define NEAR_CLIP		1.0f					//�j�A�N���b�v���s������
#define FAR_CLIP		10000.0f				//�t�@�[�N���b�v���s������

// �Œ�J�����ʒu
#define CAMERA_POS_X	0.0f
#define CAMERA_POS_Y	0.0f
#define CAMERA_POS_Z	-500.0f

// �Œ�J���������_
#define LOOK_POS_X		0.0f
#define LOOK_POS_Y		0.0f
#define LOOK_POS_Z		0.0f

// �Œ�J����������x�N�g��
#define UP_DIR_X		0.0f
#define UP_DIR_Y		1.0f
#define UP_DIR_Z		0.0f

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
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
//�f�X�g���N�^
CCameraBase::~CCameraBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������
void CCameraBase::Initialize(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	// �r���[�}�g���b�N�X�ݒ�
	D3DXMatrixLookAtLH(&m_MtxView, &m_Position, &m_At, &m_Up);
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
	
	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(
		&m_MtxProjection, D3DXToRadian(FOVY),
		m_fAspect, m_fZNear, m_fZFar);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

	m_spTransform->SetPosition(m_Position);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���O�ݒ�
void CCameraBase::PreDraw(){
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();

	pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);
	pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�I������
void CCameraBase::Finalize(){
	C3DObjectBase::Finalize();
}
