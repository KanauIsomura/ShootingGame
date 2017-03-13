//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[SkinHierarchy.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-00
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"Hierarchy.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// フレーム生成
HRESULT Hierarchy::CreateFrame(LPCSTR pMesh, LPD3DXFRAME* ppNewFrame){
	// 取りあえずNULLでクリア
	*ppNewFrame = NULL;

	// フレーム用メモリ確保
	FRAME* pFrame = new FRAME;
	if (pFrame == NULL) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pFrame, sizeof(FRAME));

	// フレーム名のコピー
	if (pMesh) {
		// フレーム名あり
		pFrame->Name = new char[lstrlenA(pMesh) + 1];
		if (pFrame->Name == NULL) {
			delete pFrame;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pFrame->Name, pMesh);
	} else {
		// フレーム名なし
		pFrame->Name = NULL;
	}

	// マトリックス初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	// 確保したフレームを返す
	*ppNewFrame = pFrame;
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//メッシュコンテナ生成
HRESULT Hierarchy::CreateMeshContainer(
	LPCSTR					Name,				//ディレクトリネーム
	CONST D3DXMESHDATA*		pMeshData,			//メッシュデータ
	CONST D3DXMATERIAL*		pMaterials,			//マテリアルデータ
	CONST D3DXEFFECTINSTANCE*	pEffectInstances,		//
	DWORD					NumMaterials,			//マテリアル数
	CONST DWORD*				pAdjacency,			//隣接
	LPD3DXSKININFO			pSkinInfo,			//スキン情報
	LPD3DXMESHCONTAINER*		ppNewMeshContainer)	//新しいメッシュコンテナ
{
	// 取りあえずNULLをセットしておく
	*ppNewMeshContainer = NULL;

	// 正常なメッシュ以外はエラーで戻す
	if (pMeshData->Type != D3DXMESHTYPE_MESH || pMeshData->pMesh->GetFVF() == 0) {
		return E_FAIL;
	}

	// メッシュオブジェクト取得
	LPD3DXMESH pMesh = pMeshData->pMesh;

	// メッシュコンテナ用メモリの確保
	MESHCONTAINER* pMeshContainer = new MESHCONTAINER;
	if (pMeshContainer == NULL) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MESHCONTAINER));

	// メッシュ名のコピー
	if (Name) {
		pMeshContainer->Name = new char[lstrlenA(Name) + 1];
		if (pMeshContainer->Name == NULL) {
			delete pMeshContainer;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pMeshContainer->Name, Name);
	} else
		pMeshContainer->Name = NULL;

	// D3Dデバイス取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMesh->GetDevice(&pDevice);

	// ポリゴン(三角形)数取得
	int iFacesAmount = pMesh->GetNumFaces();

	// 法線有無のチェック
	if (pMesh->GetFVF() & D3DFVF_NORMAL) {
		pMeshContainer->MeshData.pMesh = pMesh;
		pMesh->AddRef();
	} else {
		// 法線を持たない場合は追加
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pMeshContainer->MeshData.pMesh))) {
			pDevice->Release();
			DestroyMeshContainer(pMeshContainer);
			return E_FAIL;
		}
		pMesh = pMeshContainer->MeshData.pMesh;
		D3DXComputeNormals(pMesh, NULL);
	}

	// マテリアル用メモリ確保
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	if (pMeshContainer->pAdjacency == NULL
		|| pMeshContainer->ppTextures == NULL
		|| pMeshContainer->pMaterials == NULL) {
		DestroyMeshContainer(pMeshContainer);
		pDevice->Release();
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	CopyMemory(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);

	// マテリアルの設定
	if (NumMaterials > 0) {
		// カレントディレクトリを変更
		TCHAR szCurrentDir[_MAX_PATH];
		if (m_szDirectryName[0]) {
			GetCurrentDirectory(_MAX_PATH, szCurrentDir);
			SetCurrentDirectory(m_szDirectryName);
		}

		CopyMemory(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
		for (DWORD i = 0; i < NumMaterials; ++i) {
			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMeshContainer->pMaterials[i].MatD3D.Diffuse;
			if (FAILED(D3DXCreateTextureFromFileA(
				pDevice,
				pMeshContainer->pMaterials[i].pTextureFilename,
				&pMeshContainer->ppTextures[i]))){
#ifdef _UNICODE
				printf("テクスチャ：%lsの読み込みに失敗しました\n", pMeshContainer->ppTextures[i]);
#else
				printf("テクスチャ：%hsの読み込みに失敗しました\n", pMeshContainer->ppTextures[i]);
#endif
				pMeshContainer->ppTextures[i] = NULL;
			}
		}

		// カレントディレクトリを元に戻す
		if (m_szDirectryName[0])
			SetCurrentDirectory(szCurrentDir);
	} else {
		ZeroMemory(pMeshContainer->pMaterials, sizeof(D3DXMATERIAL));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// 属性テーブルを取得
	HRESULT hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(
		D3DXMESH_MANAGED, pMeshContainer->MeshData.pMesh->GetFVF(), pDevice, &pMesh);
	if (FAILED(hr)) {
		pDevice->Release();
		return hr;
	}
	if(pMeshContainer->MeshData.pMesh){
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = NULL;
	}

	pMeshContainer->MeshData.pMesh = pMesh;
	hr = pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
	if (FAILED(hr)) {
		pDevice->Release();
		return hr;
	}
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
	if (pMeshContainer->pAttributeTable == NULL) {
		pMeshContainer->NumAttributeGroups = 0;
		pDevice->Release();
		return E_OUTOFMEMORY;
	}
	hr = pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
	if (FAILED(hr)) {
		pDevice->Release();
		return hr;
	}

	// D3Dデバイス解放
	pDevice->Release();

	// スキン情報があるか
	if (pSkinInfo) {
		// スキン情報を保存(参照カウンタ加算)
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// ワールド・マトリックスの場所を確保
		DWORD dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrix = new D3DXMATRIX[dwBoneNum];
		if (pMeshContainer->pBoneOffsetMatrix == NULL)
			return E_OUTOFMEMORY;

		// ワールド・マトリックスを初期化
		for (DWORD i = 0; i < dwBoneNum; ++i) {
			pMeshContainer->pBoneOffsetMatrix[i] =
				*pSkinInfo->GetBoneOffsetMatrix(i);
		}

		// スキン用に変換
		hr = pSkinInfo->ConvertToBlendedMesh(
			pMesh, NULL, pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->dwWeight,
			&pMeshContainer->dwBone,
			&pMeshContainer->pBoneBuffer,
			&pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			return hr;
	}

	// 確保したメッシュコンテナを返す
	*ppNewMeshContainer = pMeshContainer;
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//フレーム破棄
HRESULT Hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree){
	FRAME* pFrame = (FRAME*)pFrameToFree;
	if(pFrame->Name){
		delete[] pFrame->Name;
		pFrame->Name = NULL;
	}

	if(pFrame){
		delete pFrame;
		pFrame = NULL;
	}
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//メッシュコンテナ破棄
HRESULT Hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerToFree;

	if(pMeshContainer->Name){
		delete[] pMeshContainer->Name;
		pMeshContainer->Name = NULL;
	}
	if(pMeshContainer->pAdjacency){
		delete[] pMeshContainer->pAdjacency;
		pMeshContainer->pAdjacency = NULL;
	}
	if(pMeshContainer->pMaterials){
		delete[] pMeshContainer->pMaterials;
		pMeshContainer->pMaterials = NULL;
	}

	// 全テクスチャの解放
	if (pMeshContainer->ppTextures) {
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			if(pMeshContainer->ppTextures[i]){
				pMeshContainer->ppTextures[i]->Release();
				pMeshContainer->ppTextures[i] = NULL;
			}
		}
	}
	if(pMeshContainer->ppTextures){
		delete[] pMeshContainer->ppTextures;
		pMeshContainer->ppTextures = NULL;
	}

	if(pMeshContainer->MeshData.pMesh){
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = NULL;
	}

	if(pMeshContainer->pAttributeTable){
		delete[] pMeshContainer->pAttributeTable;
		pMeshContainer->pAttributeTable = NULL;
	}

	pMeshContainer->NumAttributeGroups = 0;

	if(pMeshContainer->pBoneBuffer){
		pMeshContainer->pBoneBuffer->Release();
		pMeshContainer->pBoneBuffer = NULL;
	}
	if(pMeshContainer->pBoneOffsetMatrix){
		delete[] pMeshContainer->pBoneOffsetMatrix;
		pMeshContainer->pBoneOffsetMatrix = NULL;
	}
	if(pMeshContainer->ppBoneMatrix){
		delete[] pMeshContainer->ppBoneMatrix;
	}

	if(pMeshContainer){
		delete pMeshContainer;
		pMeshContainer = NULL;
	}
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
Hierarchy::Hierarchy()
{
	m_szDirectryName[0] = 0;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//Xファイルのあるディレクトリを設定
void Hierarchy::SetDirectory(LPCTSTR pszDirectryName){
	if (pszDirectryName)
		lstrcpy(m_szDirectryName, pszDirectryName);
	else
		m_szDirectryName[0] = 0;
}
