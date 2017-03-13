#include "SceneManagerThread.h"

#include <windows.h>

//ChangeSceneFuncの引数用
typedef struct{
	CSmartPointer<SceneBase> ChangeScene;
	CSmartPointer<SceneBase> TopScene;
}SCENDDATA;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	LoadingFunc
//	処理概要	：	読み込み処理
//	入力		：
//	出力		：
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void _cdecl LoadingSceneFunc(void *lpThreadParameter){
	SceneManagerThread* pSceneInstans = SceneManagerThread::GetSingletonPointer();

	if(!pSceneInstans)_endthread();
	pSceneInstans->SetLoadingFlag(1);

	SceneBase *Temp = (SceneBase*)lpThreadParameter;

	if(Temp->Init())//初期化成功
		pSceneInstans->SetLoadSuccess(true);
	else//初期化失敗
		pSceneInstans->SetLoadSuccess(false);
	
	//読み込み終了
	pSceneInstans->SetLoadingFlag(2);

	_endthread();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	ReleaseFunc
//	処理概要：	読み込み処理
//	入力	：
//	出力	：
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void _cdecl ReleaseSceneFunc(void *lpThreadParameter){
	SceneManagerThread* pSceneInstans = SceneManagerThread::GetSingletonPointer();

	if(!pSceneInstans) _endthread();
	
	pSceneInstans->SetLoadingFlag(1);
	SceneBase *Temp = (SceneBase*) lpThreadParameter;
	Temp->Uninit();

	pSceneInstans->SetLoadSuccess(true);
	pSceneInstans->SetLoadingFlag(0);

	_endthread();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	ChangeFunc
//	処理概要：	フレーム変更処理
//	入力	：
//	出力	：
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void _cdecl ChangeSceneFunc(void *lpThreadParameter){
	//フレームマネージャのインスタンス取得
	SceneManagerThread* pSceneInstans = SceneManagerThread::GetSingletonPointer();
	//念のため
	if(!pSceneInstans) _endthread();
	pSceneInstans->SetLoadingFlag(1);

	//引数取得
	SCENDDATA *Temp = (SCENDDATA*)lpThreadParameter;

	//開放＆読み込み開始
	Temp->TopScene->Uninit();

	if(Temp->ChangeScene->Init())//初期化成功
		pSceneInstans->SetLoadSuccess(true);
	else//初期化失敗
		pSceneInstans->SetLoadSuccess(false);

	delete Temp;

	//読み込み終了
	pSceneInstans->SetLoadingFlag(2);
	_endthread();
}


//------------------------------------------
//シーンマネージャースレッド
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread::SceneManagerThread(){
	m_LoadingFlag		= 0;
	m_spLoading		= NULL;
	m_Thread			= NULL;
	m_LoadSuccess		= true;
	m_LoadWaitTime	= 10;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread::~SceneManagerThread(){
	Release();
	//delete this->GetSingletonPointer();
}
	
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//シングルトン生成
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread* SceneManagerThread::GetSingletonPointer(){
	static SceneManagerThread* pStaticSceneManager = new SceneManagerThread;

	return pStaticSceneManager;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	Release
//	処理概要：	開放関数
//	入力	：
//	出力	：
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
volatile void SceneManagerThread::Release(){
	//子スレッドが開放処理などを終わらせるとm_loadingflagは0になる。
	//最適化されるとwhile(1)になりそうなのでvolatile修飾子付き

	//読み込み処理が終わるまで待つ
	while(m_LoadingFlag != 0){
		if(m_LoadingFlag == 2) break;
	}

	LoadingRelease();
	//親クラスのリリース処理
	SceneManagerBase::Release();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	LoadingRelease
//	処理概要：	読み込み画面開放関数
//	入力	：
//	出力	：
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void SceneManagerThread::LoadingRelease(){
	if(!m_LoadingFlag)return;

	m_spLoading->Uninit();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	GetScene
//	処理概要：	topフレームを取得する
//	入力	：
//	出力	：	topフレームのポインタ。ない場合はNULLを返す
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
CSmartPointer<SceneBase> SceneManagerThread::GetScene(){
	if(m_LoadingFlag != 0){//読み込み中
		Sleep(m_LoadWaitTime);
		if(m_LoadingFlag == 2){//読み込み終了
			if(!m_LoadSuccess){//読み込み失敗
				m_Error="GetScene:フレーム初期化に失敗";
				return NULL;
			}
			//フラグを通常時に戻す
			m_LoadingFlag = 0;
		}
		m_Command = FMANAGERCOM_NO;
		return m_spLoading;
	}
	SCENEDATAMAP::iterator Iterator;

	switch( m_Command ){
		case FMANAGERCOM_NO://イベントなし
			return GetTopScene();

		case FMANAGERCOM_POP://pop
			ScenePopL();
			m_Command  =FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_CHANGE://変更
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//ない場合

			SceneChangeL(Iterator->second);
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_PUSH://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//ない場合
			ScenePushL(Iterator->second);
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_POPNORELEASE://Releaseなしpop
			GetTopScene()->PreRelease();
			m_Stack.pop();
			break;

		case FMANAGERCOM_RESTART://リスタート
			GetTopScene()->PreRelease();
			GetTopScene()->ReStart();
			break;

		case FMANAGERCOM_HRESTART://ヘビーリスタート
			SceneChangeL(GetTopScene());
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_POP_NOLOADING://pop
			ScenePop();
			break;

		case FMANAGERCOM_CHANGE_NOLOADING://変更
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//ない場合
			SceneChange(Iterator->second);
			break;

		case FMANAGERCOM_PUSH_NOLOADING://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//ない場合
			ScenePush(Iterator->second);
			break;

		case FMANAGERCOM_HRESTART_NOLOADING://ヘビーリスタート
			SceneChange(GetTopScene());
			break;

		default://例外
			m_Error="GetScene:不明なコマンドが入力されました";
			break;
	}

	m_Command = FMANAGERCOM_NO;
	return GetTopScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	RegistNowLoadingSceneData
//	処理概要：	NowLoading画面用フレーム読み込みを行う
//	入力	：	*pScene
//	出力	：	成功すればtrue失敗すればfalse
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::RegistNowLoadingSceneData(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){//フレームオブジェクトなし
		m_Error = "RegistNowLoadingSceneData:登録フレームが無効です";
		return false;
	}

	//既に登録されてるなら開放する。
	if(m_spLoading.GetPointer()){
		m_spLoading->Uninit();
		//delete m_loading;
	}
	//初期化処理
	m_spLoading = spScene;
	
	return m_spLoading->Init();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	ScenePopL
//	処理概要：	フレームをpopする。読み込み画面あり
//	入力	：	
//	出力	：	
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void SceneManagerThread::ScenePopL(void){
	if(m_Stack.empty())return;
	CSmartPointer<SceneBase> spScene = m_Stack.top();
	m_spLoading->ReStart();
	//topフレームをリリース
	m_LoadingFlag = 1;
	m_Thread = _beginthread(ReleaseSceneFunc, 0, spScene.GetPointer());
	m_Stack.pop();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	ScenePushL
//	処理概要：	フレームをpushする。読み込み画面あり
//	入力	：	pScene	pushするフレームのポインタ
//	出力	：	成功すればtrue失敗すればfalse
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::ScenePushL(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "ScenePushL:Pushフレームが設定されていません";
		return false;
	}
	m_spLoading->ReStart();
	if(spScene->GetMakeFlag()){
		if(!spScene->ReStart()){
			m_Error = "ScenePushL:読み込みフレームの再起動に失敗";
			spScene->Uninit();
			return false;
		}
	}else{
		m_LoadingFlag = 1;
		m_Thread = _beginthread( LoadingSceneFunc, 0, spScene.GetPointer());
	}
	m_Stack.push(spScene);
	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	関数名	：	SceneChangeL
//	処理概要：	topフレームを変更する。読み込み画面あり
//	入力	：	pScene	変更するフレームのポインタ
//	出力	：	成功すればtrue失敗すればfalse
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::SceneChangeL(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "SceneChangeL:変更フレームが設定されていません";
		return false;
	}
	//読み込み中フラグ
	m_LoadingFlag = 1;
	//読み込み画面リスタート
	m_spLoading->ReStart();

	if(spScene->GetMakeFlag() && spScene.GetPointer() != GetTopScene().GetPointer()){//変更先が既にInit呼ばれている
		if(!m_Stack.empty()){
			m_Thread = _beginthread( ReleaseSceneFunc, 0, GetTopScene().GetPointer());
			m_Stack.pop();
			if(!spScene->ReStart())
				return false;
		}
		
		if(!spScene->ReStart())
			return false;
	}else{//変更先はまだInit呼ばれていない
		if(!m_Stack.empty()){
			SCENDDATA *pSceneData;
			pSceneData = new(_NORMAL_BLOCK, __FILE__, __LINE__) SCENDDATA;
			pSceneData->ChangeScene	= spScene;
			pSceneData->TopScene		= GetTopScene();
			m_Thread = _beginthread( ChangeSceneFunc, 0, pSceneData);
			m_Stack.pop();
		}else{
			m_Thread = _beginthread( LoadingSceneFunc, 0, spScene.GetPointer());
		}
	}
	
	m_Stack.push(spScene);
	return true;
}