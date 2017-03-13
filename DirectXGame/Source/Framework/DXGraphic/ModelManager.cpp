
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelManager.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-08-10
//�X�V��:2016-08-10
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"ModelManager.h"

#include"../System/Device.h"
#include"../Component/AnimationController.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ModelClass
//�R���X�g���N�^
CModel::CModel(){
	m_pFrameRoot			= NULL;
	m_pAnimationController	= NULL;
	m_uNumAnimationSet		= 0;
	m_ppAnimationSet		= NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CModel::~CModel(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�ǂݍ���
bool CModel::LoadModel(LPCTSTR FileName){
	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(FileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	m_Hierarchy.SetDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
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
		printf("���f���F%ls�̓ǂݍ��݂Ɏ��s���܂���\n", FileName);
#else
		printf("���f���F%hs�̓ǂݍ��݂Ɏ��s���܂���\n", FileName);
#endif
		return false;
	}

	// �{�[���ƃt���[���̊֘A�t��
	hr = AllocAllBoneMatrix(m_pFrameRoot);
	if (FAILED(hr)) return false;

	// �A�j���[�V�����Z�b�g�擾
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
//�A�j���[�V�����R���g���[���[�̃R�s�[
void CModel::CopyAnimationController(AnimationController* pAnimationController){
	pAnimationController->SetAnimation(m_pAnimationController, m_ppAnimationSet, m_uNumAnimationSet);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�A�j���[�V�����̐؂�ւ�
void CModel::ChangeAnimation(LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet){
	if(uAnimationSet >= m_uNumAnimationSet || !*ppAnimationController) return;

	(*ppAnimationController)->SetTrackAnimationSet(0, m_ppAnimationSet[uAnimationSet]);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�s�����`��
void CModel::DrawNoAlpha(D3DXMATRIX& World){
	// �}�g���b�N�X�X�V
	GetDeviceClass()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &World);
	if (m_pFrameRoot) {
		UpdateFrameMatrices(m_pFrameRoot, &World);
		// �t���[���`��
		DrawNoAlphaFrame(m_pFrameRoot);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�������`��
void CModel::DrawAlpha(D3DXMATRIX& World){
	// �}�g���b�N�X�X�V
	GetDeviceClass()->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &World);
	if (m_pFrameRoot) {
		UpdateFrameMatrices(m_pFrameRoot, &World);
		// �t���[���`��
		DrawAlphaFrame(m_pFrameRoot);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���
void CModel::Release(){
	// �A�j���[�V�����j��
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

	// ���b�V���j��
	if (m_pFrameRoot) {
		D3DXFrameDestroy(m_pFrameRoot, &m_Hierarchy);
		m_pFrameRoot = NULL;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���e�i���̕s�������b�V���̕`��
void CModel::RenderNoAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	FRAME* pFrame = (FRAME*)pFrameBase;
	if(pMeshContainer->pSkinInfo){
		// �X�L�����b�V�����f��
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;

		//�X�L�����b�V�����胂�f���̕`��
		//�s������
		for (DWORD i = 0; i < pMeshContainer->dwBone; ++i) {
			dwPrevBoneID = pBoneCombi[i].AttribId;	//�A�g���r���[�e�[�u��ID�̎擾
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
	// �X�L���������f��
	//�s���������̕`��
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
//�R���e�i���̔��������b�V���̕`��
void CModel::RenderAlphaMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	FRAME* pFrame = (FRAME*)pFrameBase;
	if(pMeshContainer->pSkinInfo){
		// �X�L�����b�V�����f��
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;

		//�X�L�����b�V�����胂�f���̕`��
		//��������
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
	// �X�L���������f��
	//���������`��
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
//�t���[���}�g���b�N�X�̍X�V
void CModel::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix){
	FRAME* pFrame = (FRAME*)pFrameBase;
	// �e�̃}�g���b�N�X���|�����킹��
	if (pParentMatrix) {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix * (*pParentMatrix);
	} else {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix;
	}

	// �Z��t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameSibling) {
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	// �q�t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameFirstChild) {
		// �������e�ƂȂ�̂ŁA�����̃}�g���b�N�X��n��
		UpdateFrameMatrices(pFrame->pFrameFirstChild,
			&pFrame->CombinedTransformationMatrix);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�������t���[���`��
void CModel::DrawAlphaFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// ���b�V���R���e�i�̕`��
		RenderAlphaMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		DrawAlphaFrame(pFrame->pFrameSibling);

	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		DrawAlphaFrame(pFrame->pFrameFirstChild);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�s�����t���[���`��
void CModel::DrawNoAlphaFrame(LPD3DXFRAME pFrame){
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// ���b�V���R���e�i�̕`��
		RenderNoAlphaMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		DrawNoAlphaFrame(pFrame->pFrameSibling);

	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		DrawNoAlphaFrame(pFrame->pFrameFirstChild);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�{�[���p���[���h�}�g���b�N�X�̃������m��
HRESULT CModel::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase){
	MESHCONTAINER* pMeshContainer = (MESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�
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
//�R���X�g���N�^
CModelManager::CModelManager(){
	m_ModelMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CModelManager::~CModelManager(){
	AllRelease();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�V���O���g���̎擾
CSmartPointer<CModelManager> CModelManager::GetSingletonPointer(){
	static CSmartPointer<CModelManager> StaticModelManager(new CModelManager);

	return StaticModelManager;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���f���̓ǂݍ���
void CModelManager::Load(LPCWSTR FileName){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//���݂��Ȃ����f���Ȃ�}�b�v�ɒǉ�
	if( Iterator == m_ModelMap.end() ){ 
		CModel AddModel;										// �ǉ��p��CModel�N���X�쐬
		AddModel.LoadModel(FileName);							//���f���ǂݍ���
		m_ModelMap.insert(std::make_pair( FileName, AddModel ));	//�}�b�v�ɒǉ�
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�A�j���[�V�����R���g���[���[�̃R�s�[
void CModelManager::CopyAnimationController(LPCWSTR FileName, AnimationController* pAnimationController){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	if( Iterator != m_ModelMap.end() ){//�}�b�v�ɂ��łɒǉ�����Ă��郂�f���Ȃ̂ł��̂܂�
		Iterator->second.CopyAnimationController(pAnimationController);
	}else{ //�Ȃ��̂Œǉ����ĕ`��
		CModel AddModel;											// �ǉ��p��CModel�N���X�쐬
		AddModel.LoadModel(FileName);								//���f���ǂݍ���
		m_ModelMap.insert(std::make_pair( FileName, AddModel));		//�}�b�v�ɒǉ�
		AddModel.CopyAnimationController(pAnimationController);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�A�j���[�V�����̐؂�ւ�
void CModelManager::ChangeAnimation(LPCWSTR FileName, LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//�}�b�v�ɂ��łɒǉ�����Ă��郂�f���Ȃ̂ł��̂܂ܕ`��
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.ChangeAnimation(ppAnimationController, uAnimationSet);
	}else{ //�Ȃ��̂Œǉ����ĕ`��
		CModel AddModel;										// �ǉ��p��CModel�N���X�쐬
		AddModel.LoadModel(FileName);							//���f���ǂݍ���
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//�}�b�v�ɒǉ�
		AddModel.ChangeAnimation(ppAnimationController, uAnimationSet);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�s���������f���̕`��
void CModelManager::RenderModelNoAlpha(LPCWSTR FileName, D3DXMATRIX& World){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//�}�b�v�ɂ��łɒǉ�����Ă��郂�f���Ȃ̂ł��̂܂ܕ`��
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.DrawNoAlpha(World);
	}else{ //�Ȃ��̂Œǉ����ĕ`��
		CModel AddModel;										// �ǉ��p��CModel�N���X�쐬
		AddModel.LoadModel(FileName);							//���f���ǂݍ���
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//�}�b�v�ɒǉ�
		AddModel.DrawNoAlpha(World);							//����̕���`��
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����������f���̕`��
void CModelManager::RenderModelAlpha(LPCWSTR FileName, D3DXMATRIX& World){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );

	//�}�b�v�ɂ��łɒǉ�����Ă��郂�f���Ȃ̂ł��̂܂ܕ`��
	if( Iterator != m_ModelMap.end() ){
		Iterator->second.DrawAlpha(World);
	}else{ //�Ȃ��̂Œǉ����ĕ`��
		CModel AddModel;										// �ǉ��p��CModel�N���X�쐬
		AddModel.LoadModel(FileName);							//���f���ǂݍ���
		m_ModelMap.insert(std::make_pair( FileName, AddModel));	//�}�b�v�ɒǉ�
		AddModel.DrawAlpha(World);								//����̕���`��
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�w�肵�����f���̍폜
void CModelManager::Release(LPCWSTR FileName ){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.find( FileName );
	
	// �ΏۂƂȂ郂�f��������΍폜
	if( Iterator != m_ModelMap.end()){
		Iterator->second.Release();
		m_ModelMap.erase(Iterator);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���ׂẴ��f���̍폜
void CModelManager::AllRelease(){
	std::map<LPCWSTR, CModel>::iterator Iterator = m_ModelMap.begin();
	
	 while( Iterator != m_ModelMap.end()){
		Iterator->second.Release();
		Iterator ++;
	}
	if( !m_ModelMap.empty())
		m_ModelMap.clear();
}