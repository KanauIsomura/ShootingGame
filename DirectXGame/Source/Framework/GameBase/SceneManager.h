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

#ifndef ______SCENE_MANAGER_CALSS_H_INCLUDE_____
#define ______SCENE_MANAGER_CALSS_H_INCLUDE_____

#include<stack>
#include<map>
#include<string>

#include"../System/CSmartPointer.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//SceneBaseクラス
//フレーム用抽象クラス
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneBase{
private:
	bool m_MakeFlag;							//Initが呼び出されたか

protected:
	//純粋仮想関数
	virtual bool InitMain() = 0;				//初期化
	virtual void ReleaseMain() = 0;				//開放

public:
	SceneBase() { m_MakeFlag = false;}			//コンストラクタ
	virtual ~SceneBase(){};

	virtual bool Init() { m_MakeFlag = true; return InitMain();}
	virtual void Uninit() { m_MakeFlag = false; ReleaseMain();}
	
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual bool ReStart() { return true;}
	virtual void PreRelease() { return;}

	bool GetMakeFlag() { return m_MakeFlag;}		//SceneManagerクラスで使用。

};

/////////////////////////////////////////////////////////////////////////////////////////////
//SceneManagerクラス
//フレーム制御クラス
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneManagerBase{
protected:
	//定義
	enum{
		FMANAGERCOM_NO,				//変更イベントなし
		FMANAGERCOM_POP,				//popする
		FMANAGERCOM_CHANGE,			//変更する
		FMANAGERCOM_PUSH,				//pushする
		FMANAGERCOM_RESTART,			//フレーム再起動
		FMANAGERCOM_HRESTART,			//フレーム再起動(データ呼び出し含め)
		FMANAGERCOM_POPNORELEASE,		//popするがReleaseではなくPreRelease
		FMANAGERCOM_CHANGE_NOLOADING,	//変更する。読み込み画面なし
		FMANAGERCOM_PUSH_NOLOADING,		//pushする。読み込み画面なし
		FMANAGERCOM_HRESTART_NOLOADING,	//フレーム再起動(データ呼び出し含め)。読み込み画面なし
		FMANAGERCOM_POP_NOLOADING		//popする。読み込み画面なし
	};
	
	typedef std::map< std::string, CSmartPointer<SceneBase>>	SCENEDATAMAP;
	typedef std::pair< std::string, CSmartPointer<SceneBase>>	SCENEDATAPAIR;
	typedef std::stack< CSmartPointer<SceneBase>>				SCENESTACK;
	typedef std::map< std::string, void *>					MSGMAP;
	typedef std::map< std::string, void *>::iterator			MSGITE;

	//メンバ変数
	SCENEDATAMAP	m_SceneDataMap;		//シーンデータ配列
	SCENESTACK	m_Stack;				//シーンのスタック
	int			m_Command;			//シーン制御コマンド
	std::string	m_TmpSceneName;		//移動先シーン番号
	std::string	m_Error;				//エラー情報
	MSGMAP		m_MassageMap;			//メッセージマップ
	
	//メンバ関数
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在Sceneの取得
	CSmartPointer<SceneBase> GetTopScene() { return ( ( m_Stack.empty() ) ? NULL : m_Stack.top() );}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在Sceneを消す
	void ScenePop();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//指定したシーンを現在シーンに
	bool ScenePush(CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在のシーンを開放して指定したシーンに変更
	bool SceneChange(CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//登録されたシーンの全削除
	void RegistedAllSceneDelete();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//指定したシーンを一番後ろに入れる
	std::string RegistSceneData(std::string SceneName, CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//指定したシーンが何番目に入っているか
	std::string GetSceneNum(CSmartPointer<SceneBase> spScene){
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//メッセージの確認
	void *FindMsgData(char *key){
		std::string MessageKey = key;
		MSGITE MassageIte = m_MassageMap.find(MessageKey);
		return ( MassageIte == m_MassageMap.end() ) ? NULL : MassageIte->second;
	}

public:
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//コンストラクタ
	SceneManagerBase();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//デストラクタ
	virtual ~SceneManagerBase(){Release();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//開放
	void Release();
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//シーンの登録
	template <typename T> std::string RegistSceneDataMakeInstance(std::string SceneName){
		CSmartPointer<SceneBase> spDataTemplate(new T); return RegistSceneData( SceneName, spDataTemplate);}

	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在シーンの取得
	CSmartPointer<SceneBase> GetScene();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//IDで指定したシーンの開放
	void SceneRelease(std::string SceneName) { m_SceneDataMap.find(SceneName)->second->Uninit();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在登録されてるシーンの数
	int GetRegistedSceneNum() { return m_SceneDataMap.size();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//IDで指定したシーンの削除
	void DeleteRegistedScene(std::string SceneName);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//現在のシーンのID
	int GetTopSceneNum() { return -1;}//GetSceneNum(GetTopScene());}

	//メッセージ
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//メッセージの登録
	template <typename T> void SetMsg(char *key,T *val){
		DeleteMsg(key);
		string ss = key;
		T *msg;
		msg = new T;
		if(!msg)
			return;
		*msg = *val;
		m_MassageMap.insert(make_pair( ss, (void *)msg) );
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//指定したメッセージの削除
	void DeleteMsg(char *key){
		std::string ss = key;
		MSGITE pp = m_MassageMap.find(ss);
		if(pp == m_MassageMap.end())
			return;
		delete pp->second;
		m_MassageMap.erase(pp);
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//全てのメッセージの削除
	void ClearMsg();
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//メッセージの更新
	template <typename T> bool UpdateMsg(char *key,T *val){
		string ss = key;
		MSGITE pp = m_MassageMap.find(ss);
		
		if(pp == m_MassageMap.end())
			return false;
		T *msg = (T *)(pp->second);
		*msg = *val;

		return true;
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//メッセージの取得
	template <typename T> bool GetMsg(char *key,T *out){
		T *Massage = (T *)FindMsgData(key);
		if(!Massage)
			return FALSE;
		*out = *Massage;
		return TRUE;
	}

	//イベント。実際の処理はGetSceneが呼ばれた時に行われる
	void PopScene() { m_Command = FMANAGERCOM_POP;}
	void PopSceneNoRelease() { m_Command = FMANAGERCOM_POPNORELEASE;}
	void ChangeScene(std::string SceneName) { m_TmpSceneName = SceneName; m_Command = FMANAGERCOM_CHANGE;}
	void PushScene(std::string SceneName) { m_TmpSceneName = SceneName; m_Command = FMANAGERCOM_PUSH;}
	char *GetErrMsg() { return (char *)m_Error.c_str();}
	void RestartScene() { m_Command = FMANAGERCOM_RESTART;}
	void HeavyRestartScene() { m_Command = FMANAGERCOM_HRESTART;}
	void CancelEvent() { m_Command = FMANAGERCOM_NO;}
};

class SceneManager : public SceneManagerBase{
};

//手軽に使えるように
//-------------------------------------------------------
//inline SceneManager *GetSceneManager(void){
//	return SceneManager::GetSingletonPointer();
//}
//-------------------------------------------------------

#endif //__SceneMANAGER_H__