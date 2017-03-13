//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-0
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______TEXTURE_MANAGER_CLASS_H_INCLUDE_____
#define ______TEXTURE_MANAGER_CLASS_H_INCLUDE_____

//=== インクルード ===
#include<d3d9.h>
#include<d3dx9.h>

#include<tchar.h>
#include<string>
#include<map>

#include"../System/CSmartPointer.h"

typedef struct{
	D3DXVECTOR3 Vertex;		// 頂点座標
	float		rhw;		// 同次座標変換用の成分(右下の1)
	D3DCOLOR	Diffuse;	// 反射光
	D3DXVECTOR2	Texture;	// テクスチャ座標
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
	
	//読み込み
	LPDIRECT3DTEXTURE9 LoadTexture( const std::wstring& FileName);

	//描画
	void Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size);
	void Draw( const std::wstring& FileName, D3DXVECTOR3& Position, D3DXVECTOR3& Size, int Alpha);
};

inline CSmartPointer<CTextureManager> GetTextureManager(){
	return CTextureManager::GetSingletonPointer();
}

#endif
