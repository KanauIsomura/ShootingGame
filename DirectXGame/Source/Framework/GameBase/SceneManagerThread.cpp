#include "SceneManagerThread.h"

#include <windows.h>

//ChangeSceneFunc�̈����p
typedef struct{
	CSmartPointer<SceneBase> ChangeScene;
	CSmartPointer<SceneBase> TopScene;
}SCENDDATA;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	LoadingFunc
//	�����T�v	�F	�ǂݍ��ݏ���
//	����		�F
//	�o��		�F
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void _cdecl LoadingSceneFunc(void *lpThreadParameter){
	SceneManagerThread* pSceneInstans = SceneManagerThread::GetSingletonPointer();

	if(!pSceneInstans)_endthread();
	pSceneInstans->SetLoadingFlag(1);

	SceneBase *Temp = (SceneBase*)lpThreadParameter;

	if(Temp->Init())//����������
		pSceneInstans->SetLoadSuccess(true);
	else//���������s
		pSceneInstans->SetLoadSuccess(false);
	
	//�ǂݍ��ݏI��
	pSceneInstans->SetLoadingFlag(2);

	_endthread();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	ReleaseFunc
//	�����T�v�F	�ǂݍ��ݏ���
//	����	�F
//	�o��	�F
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
//	�֐���	�F	ChangeFunc
//	�����T�v�F	�t���[���ύX����
//	����	�F
//	�o��	�F
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void _cdecl ChangeSceneFunc(void *lpThreadParameter){
	//�t���[���}�l�[�W���̃C���X�^���X�擾
	SceneManagerThread* pSceneInstans = SceneManagerThread::GetSingletonPointer();
	//�O�̂���
	if(!pSceneInstans) _endthread();
	pSceneInstans->SetLoadingFlag(1);

	//�����擾
	SCENDDATA *Temp = (SCENDDATA*)lpThreadParameter;

	//�J�����ǂݍ��݊J�n
	Temp->TopScene->Uninit();

	if(Temp->ChangeScene->Init())//����������
		pSceneInstans->SetLoadSuccess(true);
	else//���������s
		pSceneInstans->SetLoadSuccess(false);

	delete Temp;

	//�ǂݍ��ݏI��
	pSceneInstans->SetLoadingFlag(2);
	_endthread();
}


//------------------------------------------
//�V�[���}�l�[�W���[�X���b�h
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread::SceneManagerThread(){
	m_LoadingFlag		= 0;
	m_spLoading		= NULL;
	m_Thread			= NULL;
	m_LoadSuccess		= true;
	m_LoadWaitTime	= 10;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread::~SceneManagerThread(){
	Release();
	//delete this->GetSingletonPointer();
}
	
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�V���O���g������
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
SceneManagerThread* SceneManagerThread::GetSingletonPointer(){
	static SceneManagerThread* pStaticSceneManager = new SceneManagerThread;

	return pStaticSceneManager;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	Release
//	�����T�v�F	�J���֐�
//	����	�F
//	�o��	�F
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
volatile void SceneManagerThread::Release(){
	//�q�X���b�h���J�������Ȃǂ��I��点���m_loadingflag��0�ɂȂ�B
	//�œK��������while(1)�ɂȂ肻���Ȃ̂�volatile�C���q�t��

	//�ǂݍ��ݏ������I���܂ő҂�
	while(m_LoadingFlag != 0){
		if(m_LoadingFlag == 2) break;
	}

	LoadingRelease();
	//�e�N���X�̃����[�X����
	SceneManagerBase::Release();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	LoadingRelease
//	�����T�v�F	�ǂݍ��݉�ʊJ���֐�
//	����	�F
//	�o��	�F
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void SceneManagerThread::LoadingRelease(){
	if(!m_LoadingFlag)return;

	m_spLoading->Uninit();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	GetScene
//	�����T�v�F	top�t���[�����擾����
//	����	�F
//	�o��	�F	top�t���[���̃|�C���^�B�Ȃ��ꍇ��NULL��Ԃ�
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
CSmartPointer<SceneBase> SceneManagerThread::GetScene(){
	if(m_LoadingFlag != 0){//�ǂݍ��ݒ�
		Sleep(m_LoadWaitTime);
		if(m_LoadingFlag == 2){//�ǂݍ��ݏI��
			if(!m_LoadSuccess){//�ǂݍ��ݎ��s
				m_Error="GetScene:�t���[���������Ɏ��s";
				return NULL;
			}
			//�t���O��ʏ펞�ɖ߂�
			m_LoadingFlag = 0;
		}
		m_Command = FMANAGERCOM_NO;
		return m_spLoading;
	}
	SCENEDATAMAP::iterator Iterator;

	switch( m_Command ){
		case FMANAGERCOM_NO://�C�x���g�Ȃ�
			return GetTopScene();

		case FMANAGERCOM_POP://pop
			ScenePopL();
			m_Command  =FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_CHANGE://�ύX
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//�Ȃ��ꍇ

			SceneChangeL(Iterator->second);
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_PUSH://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//�Ȃ��ꍇ
			ScenePushL(Iterator->second);
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_POPNORELEASE://Release�Ȃ�pop
			GetTopScene()->PreRelease();
			m_Stack.pop();
			break;

		case FMANAGERCOM_RESTART://���X�^�[�g
			GetTopScene()->PreRelease();
			GetTopScene()->ReStart();
			break;

		case FMANAGERCOM_HRESTART://�w�r�[���X�^�[�g
			SceneChangeL(GetTopScene());
			m_Command = FMANAGERCOM_NO;
			return m_spLoading;

		case FMANAGERCOM_POP_NOLOADING://pop
			ScenePop();
			break;

		case FMANAGERCOM_CHANGE_NOLOADING://�ύX
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//�Ȃ��ꍇ
			SceneChange(Iterator->second);
			break;

		case FMANAGERCOM_PUSH_NOLOADING://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end())
				break;//�Ȃ��ꍇ
			ScenePush(Iterator->second);
			break;

		case FMANAGERCOM_HRESTART_NOLOADING://�w�r�[���X�^�[�g
			SceneChange(GetTopScene());
			break;

		default://��O
			m_Error="GetScene:�s���ȃR�}���h�����͂���܂���";
			break;
	}

	m_Command = FMANAGERCOM_NO;
	return GetTopScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	RegistNowLoadingSceneData
//	�����T�v�F	NowLoading��ʗp�t���[���ǂݍ��݂��s��
//	����	�F	*pScene
//	�o��	�F	���������true���s�����false
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::RegistNowLoadingSceneData(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){//�t���[���I�u�W�F�N�g�Ȃ�
		m_Error = "RegistNowLoadingSceneData:�o�^�t���[���������ł�";
		return false;
	}

	//���ɓo�^����Ă�Ȃ�J������B
	if(m_spLoading.GetPointer()){
		m_spLoading->Uninit();
		//delete m_loading;
	}
	//����������
	m_spLoading = spScene;
	
	return m_spLoading->Init();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	ScenePopL
//	�����T�v�F	�t���[����pop����B�ǂݍ��݉�ʂ���
//	����	�F	
//	�o��	�F	
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void SceneManagerThread::ScenePopL(void){
	if(m_Stack.empty())return;
	CSmartPointer<SceneBase> spScene = m_Stack.top();
	m_spLoading->ReStart();
	//top�t���[���������[�X
	m_LoadingFlag = 1;
	m_Thread = _beginthread(ReleaseSceneFunc, 0, spScene.GetPointer());
	m_Stack.pop();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//	�֐���	�F	ScenePushL
//	�����T�v�F	�t���[����push����B�ǂݍ��݉�ʂ���
//	����	�F	pScene	push����t���[���̃|�C���^
//	�o��	�F	���������true���s�����false
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::ScenePushL(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "ScenePushL:Push�t���[�����ݒ肳��Ă��܂���";
		return false;
	}
	m_spLoading->ReStart();
	if(spScene->GetMakeFlag()){
		if(!spScene->ReStart()){
			m_Error = "ScenePushL:�ǂݍ��݃t���[���̍ċN���Ɏ��s";
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
//	�֐���	�F	SceneChangeL
//	�����T�v�F	top�t���[����ύX����B�ǂݍ��݉�ʂ���
//	����	�F	pScene	�ύX����t���[���̃|�C���^
//	�o��	�F	���������true���s�����false
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
bool SceneManagerThread::SceneChangeL(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "SceneChangeL:�ύX�t���[�����ݒ肳��Ă��܂���";
		return false;
	}
	//�ǂݍ��ݒ��t���O
	m_LoadingFlag = 1;
	//�ǂݍ��݉�ʃ��X�^�[�g
	m_spLoading->ReStart();

	if(spScene->GetMakeFlag() && spScene.GetPointer() != GetTopScene().GetPointer()){//�ύX�悪����Init�Ă΂�Ă���
		if(!m_Stack.empty()){
			m_Thread = _beginthread( ReleaseSceneFunc, 0, GetTopScene().GetPointer());
			m_Stack.pop();
			if(!spScene->ReStart())
				return false;
		}
		
		if(!spScene->ReStart())
			return false;
	}else{//�ύX��͂܂�Init�Ă΂�Ă��Ȃ�
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