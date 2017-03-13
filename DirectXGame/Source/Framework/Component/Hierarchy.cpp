//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[SkinHierarchy.cpp]
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
#include"Hierarchy.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �t���[������
HRESULT Hierarchy::CreateFrame(LPCSTR pMesh, LPD3DXFRAME* ppNewFrame){
	// ��肠����NULL�ŃN���A
	*ppNewFrame = NULL;

	// �t���[���p�������m��
	FRAME* pFrame = new FRAME;
	if (pFrame == NULL) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pFrame, sizeof(FRAME));

	// �t���[�����̃R�s�[
	if (pMesh) {
		// �t���[��������
		pFrame->Name = new char[lstrlenA(pMesh) + 1];
		if (pFrame->Name == NULL) {
			delete pFrame;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pFrame->Name, pMesh);
	} else {
		// �t���[�����Ȃ�
		pFrame->Name = NULL;
	}

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	// �m�ۂ����t���[����Ԃ�
	*ppNewFrame = pFrame;
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���b�V���R���e�i����
HRESULT Hierarchy::CreateMeshContainer(
	LPCSTR					Name,				//�f�B���N�g���l�[��
	CONST D3DXMESHDATA*		pMeshData,			//���b�V���f�[�^
	CONST D3DXMATERIAL*		pMaterials,			//�}�e���A���f�[�^
	CONST D3DXEFFECTINSTANCE*	pEffectInstances,		//
	DWORD					NumMaterials,			//�}�e���A����
	CONST DWORD*				pAdjacency,			//�א�
	LPD3DXSKININFO			pSkinInfo,			//�X�L�����
	LPD3DXMESHCONTAINER*		ppNewMeshContainer)	//�V�������b�V���R���e�i
{
	// ��肠����NULL���Z�b�g���Ă���
	*ppNewMeshContainer = NULL;

	// ����ȃ��b�V���ȊO�̓G���[�Ŗ߂�
	if (pMeshData->Type != D3DXMESHTYPE_MESH || pMeshData->pMesh->GetFVF() == 0) {
		return E_FAIL;
	}

	// ���b�V���I�u�W�F�N�g�擾
	LPD3DXMESH pMesh = pMeshData->pMesh;

	// ���b�V���R���e�i�p�������̊m��
	MESHCONTAINER* pMeshContainer = new MESHCONTAINER;
	if (pMeshContainer == NULL) {
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MESHCONTAINER));

	// ���b�V�����̃R�s�[
	if (Name) {
		pMeshContainer->Name = new char[lstrlenA(Name) + 1];
		if (pMeshContainer->Name == NULL) {
			delete pMeshContainer;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pMeshContainer->Name, Name);
	} else
		pMeshContainer->Name = NULL;

	// D3D�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMesh->GetDevice(&pDevice);

	// �|���S��(�O�p�`)���擾
	int iFacesAmount = pMesh->GetNumFaces();

	// �@���L���̃`�F�b�N
	if (pMesh->GetFVF() & D3DFVF_NORMAL) {
		pMeshContainer->MeshData.pMesh = pMesh;
		pMesh->AddRef();
	} else {
		// �@���������Ȃ��ꍇ�͒ǉ�
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pMeshContainer->MeshData.pMesh))) {
			pDevice->Release();
			DestroyMeshContainer(pMeshContainer);
			return E_FAIL;
		}
		pMesh = pMeshContainer->MeshData.pMesh;
		D3DXComputeNormals(pMesh, NULL);
	}

	// �}�e���A���p�������m��
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

	// �}�e���A���̐ݒ�
	if (NumMaterials > 0) {
		// �J�����g�f�B���N�g����ύX
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
				printf("�e�N�X�`���F%ls�̓ǂݍ��݂Ɏ��s���܂���\n", pMeshContainer->ppTextures[i]);
#else
				printf("�e�N�X�`���F%hs�̓ǂݍ��݂Ɏ��s���܂���\n", pMeshContainer->ppTextures[i]);
#endif
				pMeshContainer->ppTextures[i] = NULL;
			}
		}

		// �J�����g�f�B���N�g�������ɖ߂�
		if (m_szDirectryName[0])
			SetCurrentDirectory(szCurrentDir);
	} else {
		ZeroMemory(pMeshContainer->pMaterials, sizeof(D3DXMATERIAL));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// �����e�[�u�����擾
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

	// D3D�f�o�C�X���
	pDevice->Release();

	// �X�L����񂪂��邩
	if (pSkinInfo) {
		// �X�L������ۑ�(�Q�ƃJ�E���^���Z)
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// ���[���h�E�}�g���b�N�X�̏ꏊ���m��
		DWORD dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrix = new D3DXMATRIX[dwBoneNum];
		if (pMeshContainer->pBoneOffsetMatrix == NULL)
			return E_OUTOFMEMORY;

		// ���[���h�E�}�g���b�N�X��������
		for (DWORD i = 0; i < dwBoneNum; ++i) {
			pMeshContainer->pBoneOffsetMatrix[i] =
				*pSkinInfo->GetBoneOffsetMatrix(i);
		}

		// �X�L���p�ɕϊ�
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

	// �m�ۂ������b�V���R���e�i��Ԃ�
	*ppNewMeshContainer = pMeshContainer;
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�t���[���j��
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
//���b�V���R���e�i�j��
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

	// �S�e�N�X�`���̉��
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
//�R���X�g���N�^
Hierarchy::Hierarchy()
{
	m_szDirectryName[0] = 0;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//X�t�@�C���̂���f�B���N�g����ݒ�
void Hierarchy::SetDirectory(LPCTSTR pszDirectryName){
	if (pszDirectryName)
		lstrcpy(m_szDirectryName, pszDirectryName);
	else
		m_szDirectryName[0] = 0;
}
