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
#include"Device.h"
#include"../API/Window.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//�R���X�g���N�^
CDevice::CDevice():
	m_pDirect3D(nullptr),
	m_pDirect3DDevice(nullptr),
	m_bWindowMode(false){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CDevice::~CDevice(){
	if( m_pDirect3D )
		m_pDirect3D->Release();
	if( m_pDirect3DDevice )
		m_pDirect3DDevice->Release();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�N���G�C�g�֐�
CSmartPointer<CDevice> CDevice::GetSingletonPointer(){
	static CSmartPointer<CDevice> pStaticDevice(new CDevice);
	return pStaticDevice;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�������֐�
HRESULT CDevice::InitDirect3D(bool WindowMode){
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE		d3ddm;
	m_bWindowMode = WindowMode;

	//Direct3D�I�u�W�F�N�g�̐���
	m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pDirect3D == NULL ){
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���K��
	if( FAILED( m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) ) ){
		return E_FAIL;
	}
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ���[�N���[���N���A
	d3dpp.BackBufferWidth			= GetWindowClass()->GetClientWidth();	// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= GetWindowClass()->GetClientHeight();	// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̐ݒ�
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����B
	d3dpp.Windowed				= m_bWindowMode;			// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;					// �f�v�X�o�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if( m_bWindowMode ){
		//�E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}else{
		//�t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	//�f�o�C�X�I�u�W�F�N�g�̐���
	//[�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
										D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
										GetWindowClass()->GetWindowHandle(),			// �t�H�[�J�X����E�B���h�E�ւ̃n���h��
										D3DCREATE_HARDWARE_VERTEXPROCESSING |
										D3DCREATE_MULTITHREADED,				// �f�o�C�X�쐬����̑g�ݍ��킹
										&d3dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
										&m_pDirect3DDevice)) )	{				// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^

			//��L�̐ݒ肪���s������
			//[�f�o�C�X�쐬����]<�`��>�ƃn�[�h�E�F�A�ōs���A<���_����>��CPU�ōs��
			if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
												D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
												GetWindowClass()->GetWindowHandle(),		// �t�H�[�J�X����E�B���h�E�ւ̃n���h��
												D3DCREATE_SOFTWARE_VERTEXPROCESSING |
												D3DCREATE_MULTITHREADED,				// �f�o�C�X�쐬����̑g�ݍ��킹
												&d3dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
												&m_pDirect3DDevice)) ){				// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^

					//�L�̐ݒ肪���s������
					//�f�o�C�X�쐬����]<�`��>�ƃn<���_����>��CPU�ōs��
					if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
														D3DDEVTYPE_REF,						// �f�o�C�X�^�C�v
														GetWindowClass()->GetWindowHandle(),		// �t�H�[�J�X����E�B���h�E�ւ̃n���h��
														D3DCREATE_SOFTWARE_VERTEXPROCESSING |
														D3DCREATE_MULTITHREADED,				// �f�o�C�X�쐬����̑g�ݍ��킹
														&d3dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
														&m_pDirect3DDevice)) ){				// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
							//���������s
							return E_FAIL;
					}
			}
	}
	InitRender();

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//��ʂ̃N���A
void CDevice::Clear(){
	m_pDirect3DDevice->Clear( 
		0, NULL, 
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 
		D3DCOLOR_RGBA( 125, 125, 125, 255), 1.0f, 0 );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//��ʂ̕`��J�n
HRESULT CDevice::BeginScene(){
	return m_pDirect3DDevice->BeginScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//��ʂ̕`��I���
void CDevice::EndScene(){
	m_pDirect3DDevice->EndScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//Present
void CDevice::Present(){
	m_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����_�����O�̏�����
void CDevice::InitRender(){	
	//�e�N�X�`���̐ݒ�
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//�����_�[�X�e�[�g�̃p�����[�^�̐ݒ�
	m_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// ���ʂ��J�����O
	m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );				// �A���t�@�u�����h���s��
	m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );			// �\�[�X�̐ݒ�
	m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );		// �ł��Ƃ̐ݒ�

	//�T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );		// �e�N�X�`��U�l�̌J��Ԃ��ݒ�
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );		// �e�N�X�`��V�l�̌J��Ԃ��ݒ�
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );		// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );		// �e�N�X�`���g�����̕�Ԑݒ�
}