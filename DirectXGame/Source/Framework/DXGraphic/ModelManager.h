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

#ifndef ______MODEL_MANAGER_CLASS_H_INCLUDE_____
#define ______MODEL_MANAGER_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"d3dx9.h"
#include<string>
#include<map>
#include<tchar.h>

#include"../System/CSmartPointer.h"
#include"../Component/Hierarchy.h"

//頂点フォーマット
typedef struct{
	D3DXVECTOR3	Vertex; //頂点座標
	D3DXVECTOR3	Normal; //法線ベクトル
	D3DXVECTOR2	Tex;    //テクスチャ座標
	D3DCOLOR    Color;  //色
}VERTEX_3D;

#define FVF_MVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

class AnimationController;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//モデルクラス
class CModel{
private:
	Hierarchy                 m_Hierarchy;             //階層構造メモリの確保
	LPD3DXFRAME               m_pFrameRoot;            //フレームのルート情報
	LPD3DXANIMATIONCONTROLLER m_pAnimationController;  //アニメーションコントローラ
	UINT                      m_uNumAnimationSet;      //アニメーション数
	LPD3DXANIMATIONSET*       m_ppAnimationSet;        //アニメーションセット

public:
	CModel();
	virtual ~CModel();

	//読み込み
	bool LoadModel(LPCWSTR FileName);

	//アニメーションコントローラーのコピー
	void CopyAnimationController(AnimationController* pAnimationController);
	//アニメーションの切り替え
	void ChangeAnimation(LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet);

	//レンダー
	//void RenderModel(D3DXMATRIX& World);
	void DrawNoAlpha(D3DXMATRIX& World); // 不透明部分描画
	void DrawAlpha(D3DXMATRIX& World);   // 半島名部分描画
	
	//解放
	void Release();
	
private:
	void RenderNoAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void RenderAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);

	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	void DrawAlphaFrame(LPD3DXFRAME pFrame);
	void DrawNoAlphaFrame(LPD3DXFRAME pFrame);
	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME pFrameBase);
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//モデルマネージャ
class CModelManager{
private:
	std::map<LPCWSTR, CModel> m_ModelMap;
	
private:
	CModelManager();

public:
	virtual ~CModelManager();

	//シングルトンの取得
	static CSmartPointer<CModelManager> GetSingletonPointer();
	//モデルの読み込み
	void Load(LPCWSTR FileName);
	
	//アニメーションコントローラーのコピー
	void CopyAnimationController(LPCWSTR FileName, AnimationController* pAnimationController);
	//アニメーションの切り替え
	void ChangeAnimation(LPCWSTR FileName, LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet);
	
	//モデルの描画
	//void RenderModel(LPCWSTR FileName, D3DXMATRIX& World);
	//不透明モデルの描画
	void RenderModelNoAlpha(LPCWSTR FileName, D3DXMATRIX& World);
	//半透明モデルの描画
	void RenderModelAlpha(LPCWSTR FileName, D3DXMATRIX& World);

	//指定したモデルの削除
	void Release(LPCWSTR FileName );
	//すべてのモデルの削除
	void AllRelease();
};

inline CSmartPointer<CModelManager> GetModelManager(){
	return CModelManager::GetSingletonPointer();
}

#endif
