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

#ifndef ______GAME_MAIN_CLASS_H_INCLUDE_____
#define ______GAME_MAIN_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"Framework\GameBase\GameBase.h"

#include"Framework\System\CSmartPointer.h"
#include"Framework\GameBase\SceneManagerThread.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CGameMain : public CGameBase{
private:
	CSmartPointer<SceneBase> m_spNowScene;

public:
	//�R���X�g���N�^
	CGameMain();
	
	// �������֐�
	bool InitGame();

	// �X�V�֐�
	void Update();

	// �`��֐�
	void Draw();

	// ��n���֐�
	void UnInitAll();
};

#endif
