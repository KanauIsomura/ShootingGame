#include "SceneManager.h"

#include <windows.h>

////////////////////////////////////////////////////////////////////////////
//	コンストラクタ
////////////////////////////////////////////////////////////////////////////
SceneManagerBase::SceneManagerBase(){
	//スタックを空に
	while(!m_Stack.empty()){
		m_Stack.pop();
	}
	m_Command			= FMANAGERCOM_NO;
	m_Error			= "エラーなし";
	m_TmpSceneName.clear();

	m_SceneDataMap.clear();
	m_MassageMap.clear();
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	RegistSceneData
//	処理概要	：	フレームオブジェクトを登録する
//	入力		：	pScene	登録するフレームオブジェクト
//	出力		：	登録されたフレーム番号。失敗時は-1を返す
////////////////////////////////////////////////////////////////////////////
std::string SceneManagerBase::RegistSceneData(std::string SceneName, CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error ="RegistSceneData:フレームが設定されていません";
		return "";
	}
	SCENEDATAMAP::iterator Iterator;

	Iterator = m_SceneDataMap.find(SceneName);

	if(Iterator != m_SceneDataMap.end())
		return SceneName;	//すでに登録されている
	//登録する
	SCENEDATAPAIR NewScene(SceneName, spScene);
	m_SceneDataMap.insert(NewScene);

	return SceneName;
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	DeleteRegistedScene
//	処理概要：	フレームオブジェクトを削除する
//	入力	：	SceneNum	フレーム番号
//	出力	：
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::DeleteRegistedScene(std::string SceneName){
	SCENEDATAMAP::iterator Iterator;

	Iterator = m_SceneDataMap.find(SceneName);

	if(Iterator == m_SceneDataMap.end())
		return;	//すでにシーンがない
	//delete m_SceneData.at(SceneNum);

	//Sceneの削除
	m_SceneDataMap.erase(SceneName);
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：RegistedAllSceneDelete
//	処理概要	：全ての登録されたフレームオブジェクトを削除する
//	入力		：	
//	出力		：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::RegistedAllSceneDelete(){
	SCENEDATAMAP::iterator Iterator = m_SceneDataMap.begin();

	//中身を全て初期化
	for( ; Iterator != m_SceneDataMap.end() ; Iterator ++){
		Iterator->second->Uninit();
		//delete m_SceneData.at(i);
	}

	m_SceneDataMap.clear();
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	ResetMsg
//	処理概要：	メッセージデータを全消去
//	入力	：	
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::ClearMsg(){
	for(MSGITE pp = m_MassageMap.begin(); pp != m_MassageMap.end(); ++ pp){
		delete pp->second;
	}

	m_MassageMap.clear();
}



////////////////////////////////////////////////////////////////////////////
//	関数名	：	ScenePop
//	処理概要	：	フレームをpopする
//	入力		：	
//	出力		：	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::ScenePop(){
	if( m_Stack.empty())
		return;
	//topフレームをリリース
	m_Stack.top()->Uninit();
	m_Stack.pop();
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：ScenePush
//	処理概要	：フレームをpushする
//	入力		：pScene	pushするフレームのポインタ
//	出力		：成功すればtrue失敗すればfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::ScenePush(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "ScenePush:Pushフレームが設定されていません";
		return false;
	}

	if(spScene->GetMakeFlag()){
		if(!spScene->ReStart()){
			m_Error = "フレームの初期化に失敗";
			spScene->Uninit();
			return false;
		}
	}else{
		if(!spScene->Init()){
			m_Error = "フレームの初期化に失敗";
			spScene->Uninit();
			return false;
		}
	}

	m_Stack.push(spScene);
	
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：SceneChange
//	処理概要	：topフレームを変更する
//	入力		：pScene	変更するフレームのポインタ
//	出力		：成功すればtrue失敗すればfalse
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::SceneChange(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "SceneChange:変更フレームが設定されていません";
		return false;
	}

	ScenePop();

	if(!ScenePush(spScene))
		return false;
	return true;
}


////////////////////////////////////////////////////////////////////////////
//	関数名	：GetScene
//	処理概要	：topフレームを取得する
//	入力		：
//	出力		：topフレームのポインタ。ない場合はNULLを返す
////////////////////////////////////////////////////////////////////////////
CSmartPointer<SceneBase> SceneManagerBase::GetScene(){
	
	bool bSearch = true;
	SCENEDATAMAP::iterator Iterator;

	switch(m_Command){
		case FMANAGERCOM_NO://イベントなし
			return GetTopScene();

		case FMANAGERCOM_POP://pop
			ScenePop();
			break;

		case FMANAGERCOM_CHANGE://変更
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end()){
				bSearch = false;
				break;//ない場合
			}
			bSearch = SceneChange(Iterator->second);
			break;

		case FMANAGERCOM_PUSH://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end()){
				bSearch = false;
				break;//ない場合
			}
			bSearch = ScenePush(Iterator->second);
			break;

		case FMANAGERCOM_POPNORELEASE://Releaseなしpop
			GetTopScene()->PreRelease();
			m_Stack.pop();
			break;

		case FMANAGERCOM_RESTART://リスタート
			GetTopScene()->PreRelease();
			GetTopScene()->ReStart();
			break;

		case FMANAGERCOM_HRESTART://ヘビーリスタート
			bSearch = SceneChange(GetTopScene());
			break;
		default://例外
			m_Error="GetScene:不明なコマンドが入力されました";
			break;
	}

	m_Command = FMANAGERCOM_NO;
	if(bSearch == false){
		return NULL;
	}

	return GetTopScene();
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	Release
//	処理概要	：	開放関数
//	入力		：
//	出力		：
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::Release(){
	//スタックの開放処理
	if(!m_Stack.empty()){
		m_Stack.pop();
	}

	//登録情報の削除
	RegistedAllSceneDelete();
	m_Command	 = FMANAGERCOM_NO;
	m_TmpSceneName.clear();
	ClearMsg();
}