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

#ifndef ______GAME_BASE_CALSS_H_INCLUDE_____
#define ______GAME_BASE_CALSS_H_INCLUDE_____

//=== �C���N���[�h ===
#pragma comment(lib, "d3d9.lib")
#include<d3d9.h>

#include"../System/CSmartPointer.h"
#include"../System/FPSManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�Q�[���̃x�[�X�v���O����
class CGameBase{
private:
	CSmartPointer<FPSManager>	m_spFPSManager;	//FPS�}�l�[�W��

public:
	//�R���X�g���N�^
	CGameBase();
	//�f�X�g���N�^
	~CGameBase(){};
	
	//�N��
	int Run();
	
	////--	�Q�b�^�[
	//DirectX9device�擾
	LPDIRECT3DDEVICE9	GetDirectXDevice();
	//WindowHandle�擾
	HWND	GetWindowHandle();

private:
	//--	�V�X�e��������
	bool InitSystem();
	//--	�Q�[��������
	virtual bool InitGame() = 0;
	//--	�X�V
	virtual void Update() = 0;
	//--	�`��
	virtual void Draw() = 0;
	//--	�I��
	virtual void UnInitAll() = 0;
};

#endif
