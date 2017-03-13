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

#ifndef ______SCENE_MANAGER_THREAD_CALSS_H_INCLUDE_____
#define ______SCENE_MANAGER_THREAD_CALSS_H_INCLUDE_____

#include "SceneManager.h"
#include <process.h>

#include"../System/CSmartPointer.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//SceneManagerWin�N���X
//Windows�œǂݍ��݉�ʑΉ��t���[�������{�N���X
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneManagerThread : public SceneManagerBase{
protected:
	CSmartPointer<SceneBase>	m_spLoading;		//�ǂݍ��ݒ��\���p�t���[��
	int						m_LoadingFlag;	//�ǂݍ��ݒ��t���O�B0�͓ǂݍ��ݏI��1�͓ǂݍ��ݒ�2�͓ǂݍ��݊���
	uintptr_t					m_Thread;			//�ǂݍ��ݒ��X���b�h�n���h��
	bool						m_LoadSuccess;	//�ǂݍ��ݐ���
	unsigned int				m_LoadWaitTime;	//�ǂݍ��ݒ��\�����ɓǂݍ��ݑ��X���b�h�ɏ�����n������

protected:
	SceneManagerThread();
	
	bool SceneChangeL(CSmartPointer<SceneBase> spScene);
	void ScenePopL();
	bool ScenePushL(CSmartPointer<SceneBase> spScene);
	bool RegistNowLoadingSceneData(CSmartPointer<SceneBase> spScene);
	
public:
	virtual ~SceneManagerThread();

	static SceneManagerThread* GetSingletonPointer();

	void SetLoadSuccess(bool bSuccess) { m_LoadSuccess = bSuccess;}
	void SetLoadingFlag(int flag) { m_LoadingFlag = flag;}
	CSmartPointer<SceneBase> GetScene();
	void SetLoadingWait(unsigned int time) { m_LoadWaitTime = time;}
	//���[�h��ʂ̓o�^
	template <typename T> bool RegistNowLoadingSceneDataMakeInstance() { CSmartPointer<SceneBase> spData(new T); return RegistNowLoadingSceneData(spData);}
	void LoadingRelease();
	volatile void Release();

	void PopSceneNoLoading() { m_Command = FMANAGERCOM_POP_NOLOADING;}
	void ChangeSceneNoLoading(std::string SceneName) { m_TmpSceneName = SceneName; m_Command = FMANAGERCOM_CHANGE_NOLOADING;}
	void PushSceneNoLoading(std::string SceneName) { m_TmpSceneName =  SceneName; m_Command = FMANAGERCOM_PUSH_NOLOADING;}
	void HeavyRestartSceneNoLoading() { m_Command = FMANAGERCOM_HRESTART_NOLOADING;}
};

inline SceneManagerThread* GetSceneManagerThread(){
	return SceneManagerThread::GetSingletonPointer();
}

#endif

