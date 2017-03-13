//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-0
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______TEXTURE_MANAGER_CLASS_H_INCLUDE_____
#define ______TEXTURE_MANAGER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<d3d9.h>
#include<d3dx9.h>

#include<tchar.h>
#include<string>
#include<map>

#include"../System/CSmartPointer.h"

typedef struct{
	D3DXVECTOR3 Vertex;		// ���_���W
	float		rhw;		// �������W�ϊ��p�̐���(�E����1)
	D3DCOLOR	Diffuse;	// ���ˌ�
	D3DXVECTOR2	Texture;	// �e�N�X�`�����W
} VERTEX_2D;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CTextureManager{
private:
	typedef std::map<std::wstring, LPDIRECT3DTEXTURE9> TEXTUREMAP;

	std::wstring			m_FilePath;
	TEXTUREMAP			m_TextureList;
	
private:
	CTextureManager();
public:
	~CTextureManager();

	static CSmartPointer<CTextureManager> GetSingletonPointer();
	
	//�ǂݍ���
	LPDIRECT3DTEXTURE9 LoadTexture( const std::wstring& FileName);

	//�`��
	void Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size);
	void Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size, int Alpha);
};

inline CSmartPointer<CTextureManager> GetTextureManager(){
	return CTextureManager::GetSingletonPointer();
}

#endif
