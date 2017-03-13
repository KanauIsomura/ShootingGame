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

#ifndef ______MODEL_MANAGER_CLASS_H_INCLUDE_____
#define ______MODEL_MANAGER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"d3dx9.h"
#include<string>
#include<map>
#include<tchar.h>

#include"../System/CSmartPointer.h"
#include"../Component/Hierarchy.h"

//���_�t�H�[�}�b�g
typedef struct{
	D3DXVECTOR3	Vertex; //���_���W
	D3DXVECTOR3	Normal; //�@���x�N�g��
	D3DXVECTOR2	Tex;    //�e�N�X�`�����W
	D3DCOLOR    Color;  //�F
}VERTEX_3D;

#define FVF_MVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

class AnimationController;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���f���N���X
class CModel{
private:
	Hierarchy                 m_Hierarchy;             //�K�w�\���������̊m��
	LPD3DXFRAME               m_pFrameRoot;            //�t���[���̃��[�g���
	LPD3DXANIMATIONCONTROLLER m_pAnimationController;  //�A�j���[�V�����R���g���[��
	UINT                      m_uNumAnimationSet;      //�A�j���[�V������
	LPD3DXANIMATIONSET*       m_ppAnimationSet;        //�A�j���[�V�����Z�b�g

public:
	CModel();
	virtual ~CModel();

	//�ǂݍ���
	bool LoadModel(LPCWSTR FileName);

	//�A�j���[�V�����R���g���[���[�̃R�s�[
	void CopyAnimationController(AnimationController* pAnimationController);
	//�A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet);

	//�����_�[
	//void RenderModel(D3DXMATRIX& World);
	void DrawNoAlpha(D3DXMATRIX& World); // �s���������`��
	void DrawAlpha(D3DXMATRIX& World);   // �����������`��
	
	//���
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
//���f���}�l�[�W��
class CModelManager{
private:
	std::map<LPCWSTR, CModel> m_ModelMap;
	
private:
	CModelManager();

public:
	virtual ~CModelManager();

	//�V���O���g���̎擾
	static CSmartPointer<CModelManager> GetSingletonPointer();
	//���f���̓ǂݍ���
	void Load(LPCWSTR FileName);
	
	//�A�j���[�V�����R���g���[���[�̃R�s�[
	void CopyAnimationController(LPCWSTR FileName, AnimationController* pAnimationController);
	//�A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(LPCWSTR FileName, LPD3DXANIMATIONCONTROLLER* ppAnimationController, UINT uAnimationSet);
	
	//���f���̕`��
	//void RenderModel(LPCWSTR FileName, D3DXMATRIX& World);
	//�s�������f���̕`��
	void RenderModelNoAlpha(LPCWSTR FileName, D3DXMATRIX& World);
	//���������f���̕`��
	void RenderModelAlpha(LPCWSTR FileName, D3DXMATRIX& World);

	//�w�肵�����f���̍폜
	void Release(LPCWSTR FileName );
	//���ׂẴ��f���̍폜
	void AllRelease();
};

inline CSmartPointer<CModelManager> GetModelManager(){
	return CModelManager::GetSingletonPointer();
}

#endif
