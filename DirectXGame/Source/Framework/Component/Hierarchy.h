//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[SkinHierarchy.h]
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

#ifndef ______HIERARCHY_CLASS_H_INCLUDE_____
#define ______HIERARCHY_CLASS_H_INCLUDE_____

//=== インクルード ===
#include<d3dx9.h>

#include<string>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//フレーム構造体
struct FRAME : public D3DXFRAME{
	// 最終的なマトリックスを格納するための変数を追加.
	D3DXMATRIX	CombinedTransformationMatrix;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//メッシュコンテナクラス
struct MESHCONTAINER : public D3DXMESHCONTAINER{
	// テクスチャを保存するための変数を追加
	LPDIRECT3DTEXTURE9*		ppTextures;
	// 属性テーブル
	LPD3DXATTRIBUTERANGE		pAttributeTable;
	DWORD					NumAttributeGroups;
	// 以下はスキンメッシュ用
	DWORD					dwWeight;				// 重みの数
	DWORD					dwBone;				// ボーンの数
	LPD3DXBUFFER				pBoneBuffer;			// ボーン・テーブル
	LPD3DXMATRIX*				ppBoneMatrix;			// ボーンのワールド行列の先頭
	LPD3DXMATRIX				pBoneOffsetMatrix;		// フレームとしてのボーンのワールド行列
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//フレームの階層構造
class Hierarchy : public ID3DXAllocateHierarchy{
private:
	TCHAR m_szDirectryName[_MAX_PATH];	// Xファイルのあるディレクトリ

public:
	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerBase);

	Hierarchy();
	void SetDirectory(LPCTSTR pszDir);
};

#endif
