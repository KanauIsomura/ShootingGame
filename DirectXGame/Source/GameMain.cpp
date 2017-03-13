
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[GameMain]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2017-03-01
//�X�V��:2017-03-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"GameMain.h"

#include"Framework\System\Device.h"

#include"Framework\GameObject\ObjectManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CGameMain::CGameMain(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�������֐�
bool CGameMain::InitGame(){
	GetObjectManager()->Initialize();

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�X�V�֐�
void CGameMain::Update(){
	m_spNowScene = GetSceneManagerThread()->GetScene();

	GetObjectManager()->ObjUpdate();

	if(!m_spNowScene.GetPointer())
		return;
	m_spNowScene->Update();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�`��֐�
void CGameMain::Draw(){
	GetObjectManager()->ObjDraw();

	if(!m_spNowScene.GetPointer())
		return;
	m_spNowScene->Draw();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//��n���֐�
void CGameMain::UnInitAll(){
	GetObjectManager()->Finalize();

	delete GetSceneManagerThread();
}
