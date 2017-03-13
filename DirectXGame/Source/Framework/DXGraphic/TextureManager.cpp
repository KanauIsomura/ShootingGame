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
#include"TextureManager.h"

#include"../System/Device.h"

//=== �萔��` ===

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CTextureManager::CTextureManager()
	:m_FilePath()
	,m_TextureList(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CTextureManager::~CTextureManager(){
	TEXTUREMAP::iterator Iterator;

	for( Iterator = m_TextureList.begin(); Iterator != m_TextureList.end(); Iterator ++ ){
		if(Iterator->second){
			Iterator->second->Release();
		}
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�V���O���g���|�C���^�̎擾
CSmartPointer<CTextureManager> CTextureManager::GetSingletonPointer(){
	static CSmartPointer<CTextureManager> pStaticTextureManager(new CTextureManager);

	return pStaticTextureManager;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�e�N�X�`���̓ǂݍ��݁A
//�ǂݍ��݂��s���Ă��e�N�X�`���Ȃ炻�̂܂ܕԂ�
//�s���Ă��Ȃ��Ȃ�ǂݍ��݂��s��
LPDIRECT3DTEXTURE9 CTextureManager::LoadTexture( const std::wstring& FileName){
	TEXTUREMAP::iterator Iterator = m_TextureList.find( FileName );

	if(Iterator != m_TextureList.end()){
		return Iterator->second;
	}else{
		std::wstring		TextureName = m_FilePath + FileName;
		LPDIRECT3DTEXTURE9	pTexture;

		if(FAILED(D3DXCreateTextureFromFile( GetDeviceClass()->GetDirect3DDevice(), TextureName.c_str(), &pTexture ))){
#ifdef _UNICODE
			printf("�e�N�X�`���F%ls�̓ǂݍ��݂Ɏ��s���܂���\n", TextureName.c_str());
#else
			printf("�e�N�X�`���F%hs�̓ǂݍ��݂Ɏ��s���܂���\n", FileName);
#endif
			return NULL;
		}
		m_TextureList.insert(std::make_pair( FileName, pTexture ));

		return pTexture;
	}
}

//-------------------------------------------------------------------------------------------------
//�`��
//-------------------------------------------------------------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�e�N�X�`���̕`��
void CTextureManager::Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size){
	VERTEX_2D PolygonData[4];
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDeviceClass()->GetDirect3DDevice();
	
	PolygonData[0].Vertex = D3DXVECTOR3(Position.x - Size.x, Position.y - Size.y, 0.0f);
	PolygonData[1].Vertex = D3DXVECTOR3(Position.x + Size.x, Position.y - Size.y, 0.0f);
	PolygonData[2].Vertex = D3DXVECTOR3(Position.x - Size.x, Position.y + Size.y, 0.0f);
	PolygonData[3].Vertex = D3DXVECTOR3(Position.x + Size.x, Position.y + Size.y, 0.0f);
	
	PolygonData[0].rhw =
	PolygonData[1].rhw =
	PolygonData[2].rhw =
	PolygonData[3].rhw = 1.0f;
	
	PolygonData[0].Diffuse = 
	PolygonData[1].Diffuse = 
	PolygonData[2].Diffuse = 
	PolygonData[3].Diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255);
	
	PolygonData[0].Texture = D3DXVECTOR2(0.0f, 0.0f);
	PolygonData[1].Texture = D3DXVECTOR2(1.0f, 0.0f);
	PolygonData[2].Texture = D3DXVECTOR2(0.0f, 1.0f);
	PolygonData[3].Texture = D3DXVECTOR2(1.0f, 1.0f);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	
	//�`��ݒ�
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetTexture(NULL, GetTextureManager()->LoadTexture( FileName.c_str() ));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, PolygonData, sizeof(VERTEX_2D));
}
void CTextureManager::Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size, int Alpha){
	VERTEX_2D PolygonData[4];
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDeviceClass()->GetDirect3DDevice();
	
	PolygonData[0].Vertex = D3DXVECTOR3(Position.x - Size.x, Position.y - Size.y, 0.0f);
	PolygonData[1].Vertex = D3DXVECTOR3(Position.x + Size.x, Position.y - Size.y, 0.0f);
	PolygonData[2].Vertex = D3DXVECTOR3(Position.x - Size.x, Position.y + Size.y, 0.0f);
	PolygonData[3].Vertex = D3DXVECTOR3(Position.x + Size.x, Position.y + Size.y, 0.0f);
	
	PolygonData[0].rhw =
	PolygonData[1].rhw =
	PolygonData[2].rhw =
	PolygonData[3].rhw = 1.0f;
	
	PolygonData[0].Diffuse = 
	PolygonData[1].Diffuse = 
	PolygonData[2].Diffuse = 
	PolygonData[3].Diffuse = D3DCOLOR_RGBA( 255, 255, 255, Alpha);
	
	PolygonData[0].Texture = D3DXVECTOR2(0.0f, 0.0f);
	PolygonData[1].Texture = D3DXVECTOR2(1.0f, 0.0f);
	PolygonData[2].Texture = D3DXVECTOR2(0.0f, 1.0f);
	PolygonData[3].Texture = D3DXVECTOR2(1.0f, 1.0f);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	
	//�`��ݒ�
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetTexture(NULL, GetTextureManager()->LoadTexture( FileName.c_str() ));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, PolygonData, sizeof(VERTEX_2D));
}