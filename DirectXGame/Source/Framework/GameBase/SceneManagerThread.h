//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-0
//更新日:
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
//SceneManagerWinクラス
//Windows版読み込み画面対応フレーム制御基本クラス
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneManagerThread : public SceneManagerBase{
protected:
	CSmartPointer<SceneBase>	m_spLoading;		//読み込み中表示用フレーム
	int						m_LoadingFlag;	//読み込み中フラグ。0は読み込み終了1は読み込み中2は読み込み完了
	uintptr_t					m_Thread;			//読み込み中スレッドハンドル
	bool						m_LoadSuccess;	//読み込み成功
	unsigned int				m_LoadWaitTime;	//読み込み中表示時に読み込み側スレッドに処理を渡す時間

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
	//ロード画面の登録
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

