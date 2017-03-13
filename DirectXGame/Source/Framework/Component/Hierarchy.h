//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[SkinHierarchy.h]
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

#ifndef ______HIERARCHY_CLASS_H_INCLUDE_____
#define ______HIERARCHY_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<d3dx9.h>

#include<string>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�t���[���\����
struct FRAME : public D3DXFRAME{
	// �ŏI�I�ȃ}�g���b�N�X���i�[���邽�߂̕ϐ���ǉ�.
	D3DXMATRIX	CombinedTransformationMatrix;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���b�V���R���e�i�N���X
struct MESHCONTAINER : public D3DXMESHCONTAINER{
	// �e�N�X�`����ۑ����邽�߂̕ϐ���ǉ�
	LPDIRECT3DTEXTURE9*		ppTextures;
	// �����e�[�u��
	LPD3DXATTRIBUTERANGE		pAttributeTable;
	DWORD					NumAttributeGroups;
	// �ȉ��̓X�L�����b�V���p
	DWORD					dwWeight;				// �d�݂̐�
	DWORD					dwBone;				// �{�[���̐�
	LPD3DXBUFFER				pBoneBuffer;			// �{�[���E�e�[�u��
	LPD3DXMATRIX*				ppBoneMatrix;			// �{�[���̃��[���h�s��̐擪
	LPD3DXMATRIX				pBoneOffsetMatrix;		// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�t���[���̊K�w�\��
class Hierarchy : public ID3DXAllocateHierarchy{
private:
	TCHAR m_szDirectryName[_MAX_PATH];	// X�t�@�C���̂���f�B���N�g��

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
