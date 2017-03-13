
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelManager.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-08-10
//更新日:2016-08-10
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"ModelManager.h"

#include"../System/Device.h"
#include"../Component/AnimationController.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ModelClass
//コンストラクタ
CModel::CModel(){
	m_pFrameRoot			= NULL;
	m_pAnimationController	= NULL;
	m_uNumAnimationSet		= 0;
	m_ppAnimationSet		= NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
CModel::~CModel(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//読み込み
bool CModel::LoadModel(LPCTSTR FileName){
	// ディレクトリ抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(FileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	m_Hierarchy.SetDirectory(szDir);

	// 階層構造メッシュの読み込み
	HRESULT hr = D3DXLoadMeshHierarchyFromX(
		FileName, 
		D3DXMESH_MANAGED,
		GetDeviceClass()->GetDirect3DDevice(),
		&m_Hierarchy,
		NULL,
		&m_pFrameRoot,
		&m_pAnimationController);

	if(FAILED(hr)){
#ifdef _UNICODE
		printf("モデル：%lsの読み込みに失敗しました\n", FileName);
#else
		printf("モデル：%hsの読み込みに失敗しました\n", FileName);
#endif
		return false;
	}

	// ボーンとフレームの関連付け
	hr = AllocAllBoneMatrix(m_pFrameRoot);
	if (FAILED(hr)) return false;

	// アニメーションセット取得
	m_uNumAnimationSet = 0;
	if (m_pAnimationController) {
		m_uNumAnimationSet = m_pAnimationController->GetNumAnimationSets();
		if (m_uNumAnimationSet > 0) {
			m_ppAnimationSet = new LPD3DXANIMATIONSET[m_uNumAnimationSet];
			for (DWORD u = 0; u < m_uNumAnimationSet; ++u) {
				m_pAnimationController->GetAnimationSet(u, &m_ppAnimationSet[u]);
			}
		}
	}
	
	return SUCCEEDED(hr);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーションコントローラーのコピー
void CModel::CopyAnimationController(AnimationController* pAnimationController){
	pAnimationController->SetAnimation(m_pAnimationController, m_ppAnimationSet, m_uNumAnimationSet);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーションの切り替え
void CModel::ChangeAnimation(LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet){
	if(uAnimationSet >= m_uNumAnimationSet || !*ppAnimationController) return;

	(*ppAnimationController)->SetTrackAnimationSet(0, m_ppAnimationSet[uAnimationSet]);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//不透明描画
void CModel::DrawNoAlpha(D3DXMATRIX& World){
	// マトリックス更新
	GetDeviceClass()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &World);
	if (m_pFrameRoot) {
		UpdateFrameMatrices(m_pFrameRoot, &World);
		// フレーム描画
		DrawNoAlphaFrame(m_pFrameRoot);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//半透明描画
void CModel::DrawAlpha(D3DXMATRIX& World){
	// マトリックス更新
	GetDeviceClass()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &World);
	if (m_pFrameRoot) {
		UpdateFrameMatrices(m_pFrameRoot, &World);
		// フレーム描画
		DrawAlphaFrame(m_pFrameRoot);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//解放
void CModel::Release(){
	// アニメーション破棄
	if (m_ppAnimationSet) {
		for (DWORD u = 0; u < m_uNumAnimationSet; ++u) {
			if(m_ppAnimationSet[u]){
				m_ppAnimationSet[u]->Release();
				m_ppAnimationSet[u] = NULL;
			}
		}
		if(m_ppAnimationSet){
			delete[] m_ppAnimationSet;
			m_ppAnimationSet = NULL;
		}
	}
	if(m_pAnimationController){
		m_pAnimationController->Release();
		m_pAnimationController = NULL;
	}

	// メッシュ破棄
	if (m_pFrameRoot) {
		D3DXFrameDestroy(m_pFrameRoot, &m_Hierarchy);
		m_pFrameRoot = NULL;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンテナ内の不透明メッシュの描画
void CModel::RenderNoAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	FRAME* pFrame = (FRAME*)pFrameBase;
	if(pMeshContainer->pSkinInfo){
		// スキンメッシュモデル
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;

		//スキンメッシュありモデルの描画
		//不透明部
		for (DWORD i = 0; i < pMeshContainer->dwBone; ++i) {
			dwPrevBoneID = pBoneCombi[i].AttribId;	//アトリビューテーブルIDの取得
			if(pMeshContainer->pMaterials[dwPrevBoneID].MatD3D.Diffuse.a < 1.0f)
				continue;
			dwBlendMatrix = 0;
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				if (pBoneCombi[i].BoneId[k] != UINT_MAX)
					dwBlendMatrix = k;
			}
			GetDeviceClass()->GetDirect3DDevice()->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrix);
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				DWORD id = pBoneCombi[i].BoneId[k];
				if (id != UINT_MAX) {
					GetDeviceClass()->GetDirect3DDevice()->SetTransform(
						D3DTS_WORLDMATRIX(k), &(pMeshContainer->pBoneOffsetMatrix[id] * *pMeshContainer->ppBoneMatrix[id]));
				}
			}
			GetDeviceClass()->GetDirect3DDevice()->SetMaterial(&pMeshContainer->pMaterials[dwPrevBoneID].MatD3D);
			GetDeviceClass()->GetDirect3DDevice()->SetTexture(0, pMeshContainer->ppTextures[dwPrevBoneID]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		return;
	}
	// スキン無しモデル
	//不透明部分の描画
	GetDeviceClass()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
	for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; ++iAttrib) {
		DWORD dwAttrib = pMeshContainer->pAttributeTable[iAttrib].AttribId;
		if(pMeshContainer->pMaterials[dwAttrib].MatD3D.Diffuse.a < 1.0f)
			continue;
		GetDeviceClass()->GetDirect3DDevice()->SetMaterial(&pMeshContainer->pMaterials[dwAttrib].MatD3D);
		GetDeviceClass()->GetDirect3DDevice()->SetTexture(0, pMeshContainer->ppTextures[dwAttrib]);
		pMeshContainer->MeshData.pMesh->DrawSubset(dwAttrib);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンテナ内の半透明メッシュの描画
void CModel::RenderAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	FRAME* pFrame = (FRAME*)pFrameBase;
	if(pMeshContainer->pSkinInfo){
		// スキンメッシュモデル
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;

		//スキンメッシュありモデルの描画
		//透明部分
		for (DWORD i = 0; i < pMeshContainer->dwBone; ++i) {
			dwPrevBoneID = pBoneCombi[i].AttribId;
			if(pMeshContainer->pMaterials[dwPrevBoneID].MatD3D.Diffuse.a >= 1.0f)
				continue;
			dwBlendMatrix = 0;
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				if (pBoneCombi[i].BoneId[k] != UINT_MAX)
					dwBlendMatrix = k;
			}
			GetDeviceClass()->GetDirect3DDevice()->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrix);
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				DWORD id = pBoneCombi[i].BoneId[k];
				if (id != UINT_MAX) {
					GetDeviceClass()->GetDirect3DDevice()->SetTransform(
						D3DTS_WORLDMATRIX(k), &(pMeshContainer->pBoneOffsetMatrix[id] * *pMeshContainer->ppBoneMatrix[id]));
				}
			}
			GetDeviceClass()->GetDirect3DDevice()->SetMaterial(&pMeshContainer->pMaterials[dwPrevBoneID].MatD3D);
			GetDeviceClass()->GetDirect3DDevice()->SetTexture(0, pMeshContainer->ppTextures[dwPrevBoneID]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		return;
	}
	// スキン無しモデル
	//透明部分描画
	for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; ++iAttrib) {
		DWORD dwAttrib = pMeshContainer->pAttributeTable[iAttrib].AttribId;
		if(pMeshContainer->pMaterials[dwAttrib].MatD3D.Diffuse.a >= 1.0f)
			continue;
		GetDeviceClass()->GetDirect3DDevice()->SetMaterial(&pMeshContainer->pMaterials[dwAttrib].MatD3D);
		GetDeviceClass()->GetDirect3DDevice()->SetTexture(0, pMeshContainer->ppTextures[dwAttrib]);
		pMeshContainer->MeshData.pMesh->DrawSubset(dwAttrib);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//フレームマトリックスの更新
void CModel::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix){
	FRAME* pFrame = (FRAME*)pFrameBase;
	// 親のマトリックスを掛け合わせる
	if (pParentMatrix) {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix * (*pParentMatrix);
	} else {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix;
	}

	// 兄弟フレームがあればマトリックスを更新
	if (pFrame->pFrameSibling) {
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	// 子フレームがあればマトリックスを更新
	if (pFrame->pFrameFirstChild) {
		// 自分が親となるので、自分のマトリックスを渡す
		UpdateFrameMatrices(pFrame->pFrameFirstChild,
			&pFrame->CombinedTransformationMatrix);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//半透明フレーム描画
void CModel::DrawAlphaFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// メッシュコンテナの描画
		RenderAlphaMeshContainer(pMeshContainer, pFrame);
		// 次のメッシュコンテナ
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
		DrawAlphaFrame(pFrame->pFrameSibling);

	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
		DrawAlphaFrame(pFrame->pFrameFirstChild);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//不透明フレーム描画
void CModel::DrawNoAlphaFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// メッシュコンテナの描画
		RenderNoAlphaMeshContainer(pMeshContainer, pFrame);
		// 次のメッシュコンテナ
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
		DrawNoAlphaFrame(pFrame->pFrameSibling);

	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
		DrawNoAlphaFrame(pFrame->pFrameFirstChild);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ボーン用ワールドマトリックスのメモリ確保
HRESULT CModel::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
		return S_OK;	// スキン情報が無ければ何もしない
	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new LPD3DXMATRIX[dwBoneNum];
	for (DWORD i = 0; i < dwBoneNum; ++i) {
		FRAME* pFrame = (FRAME*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
			return E_FAIL;
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}
HRESULT CModel::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase){
	FRAME* pFrame = (FRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer) {
		hr = AllocBoneMatrix(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling) {
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild) {
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild);
	}
	return hr;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ModelManagerClass
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CModelManager::CModelManager(){
	m_ModelMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
CModelManager::~CModelManager(){
	AllRelease();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//シングルトンの取得
CSmartPointer<CModelManager> CModelManager::GetSingletonPointer(){
	static CSmartPointer<CModelManager> StaticModelManager(new CModelManager);

	return StaticModelManager;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//モデルの読み込み
void CModelManager::Load(LPCWSTR FileName){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//存在しないモデルならマップに追加
	if( Iterator == m_ModelMap.end() ){ 
		CModel AddModel;										// 追加用のCModelクラス作成
		AddModel.LoadModel(FileName);							//モデル読み込み
		m_ModelMap.insert(std::make_pair( FileName, AddModel ));	//マップに追加
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーションコントローラーのコピー
void CModelManager::CopyAnimationController(LPCWSTR FileName, AnimationController* pAnimationController){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	if( Iterator != m_ModelMap.end() ){//マップにすでに追加されているモデルなのでそのまま
		Iterator->second.CopyAnimationController(pAnimationController);
	}else{ //ないので追加して描画
		CModel AddModel;											// 追加用のCModelクラス作成
		AddModel.LoadModel(FileName);								//モデル読み込み
		m_ModelMap.insert(std::make_pair( FileName, AddModel));		//マップに追加
		AddModel.CopyAnimationController(pAnimationController);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーションの切り替え
void CModelManager::ChangeAnimation(LPCWSTR FileName, LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//マップにすでに追加されているモデルなのでそのまま描画
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.ChangeAnimation(ppAnimationController, uAnimationSet);
	}else{ //ないので追加して描画
		CModel AddModel;										// 追加用のCModelクラス作成
		AddModel.LoadModel(FileName);							//モデル読み込み
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//マップに追加
		AddModel.ChangeAnimation(ppAnimationController, uAnimationSet);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//不透明部モデルの描画
void CModelManager::RenderModelNoAlpha(LPCWSTR FileName, D3DXMATRIX& World){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//マップにすでに追加されているモデルなのでそのまま描画
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.DrawNoAlpha(World);
	}else{ //ないので追加して描画
		CModel AddModel;										// 追加用のCModelクラス作成
		AddModel.LoadModel(FileName);							//モデル読み込み
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//マップに追加
		AddModel.DrawNoAlpha(World);							//今回の分を描画
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//半透明部モデルの描画
void CModelManager::RenderModelAlpha(LPCWSTR FileName, D3DXMATRIX& World){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//マップにすでに追加されているモデルなのでそのまま描画
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.DrawAlpha(World);
	}else{ //ないので追加して描画
		CModel AddModel;										// 追加用のCModelクラス作成
		AddModel.LoadModel(FileName);							//モデル読み込み
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//マップに追加
		AddModel.DrawAlpha(World);								//今回の分を描画
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//指定したモデルの削除
void CModelManager::Release(LPCWSTR FileName ){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );
	
	// 対象となるモデルがあれば削除
	if( Iterator != m_ModelMap.end()){
		Iterator->second.Release();
		m_ModelMap.erase(Iterator);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//すべてのモデルの削除
void CModelManager::AllRelease(){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.begin();
	
	 while( Iterator != m_ModelMap.end()){
		Iterator->second.Release();
		Iterator ++;
	}
	if( !m_ModelMap.empty())
		m_ModelMap.clear();
}