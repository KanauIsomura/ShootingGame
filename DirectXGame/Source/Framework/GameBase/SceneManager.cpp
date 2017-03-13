#include "SceneManager.h"

#include <windows.h>

////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^
////////////////////////////////////////////////////////////////////////////
SceneManagerBase::SceneManagerBase(){
	//�X�^�b�N�����
	while(!m_Stack.empty()){
		m_Stack.pop();
	}
	m_Command			= FMANAGERCOM_NO;
	m_Error			= "�G���[�Ȃ�";
	m_TmpSceneName.clear();

	m_SceneDataMap.clear();
	m_MassageMap.clear();
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	RegistSceneData
//	�����T�v	�F	�t���[���I�u�W�F�N�g��o�^����
//	����		�F	pScene	�o�^����t���[���I�u�W�F�N�g
//	�o��		�F	�o�^���ꂽ�t���[���ԍ��B���s����-1��Ԃ�
////////////////////////////////////////////////////////////////////////////
std::string SceneManagerBase::RegistSceneData(std::string SceneName, CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error ="RegistSceneData:�t���[�����ݒ肳��Ă��܂���";
		return "";
	}
	SCENEDATAMAP::iterator Iterator;

	Iterator = m_SceneDataMap.find(SceneName);

	if(Iterator != m_SceneDataMap.end())
		return SceneName;	//���łɓo�^����Ă���
	//�o�^����
	SCENEDATAPAIR NewScene(SceneName, spScene);
	m_SceneDataMap.insert(NewScene);

	return SceneName;
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	DeleteRegistedScene
//	�����T�v�F	�t���[���I�u�W�F�N�g���폜����
//	����	�F	SceneNum	�t���[���ԍ�
//	�o��	�F
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::DeleteRegistedScene(std::string SceneName){
	SCENEDATAMAP::iterator Iterator;

	Iterator = m_SceneDataMap.find(SceneName);

	if(Iterator == m_SceneDataMap.end())
		return;	//���łɃV�[�����Ȃ�
	//delete m_SceneData.at(SceneNum);

	//Scene�̍폜
	m_SceneDataMap.erase(SceneName);
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�FRegistedAllSceneDelete
//	�����T�v	�F�S�Ă̓o�^���ꂽ�t���[���I�u�W�F�N�g���폜����
//	����		�F	
//	�o��		�F	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::RegistedAllSceneDelete(){
	SCENEDATAMAP::iterator Iterator = m_SceneDataMap.begin();

	//���g��S�ď�����
	for( ; Iterator != m_SceneDataMap.end() ; Iterator ++){
		Iterator->second->Uninit();
		//delete m_SceneData.at(i);
	}

	m_SceneDataMap.clear();
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	ResetMsg
//	�����T�v�F	���b�Z�[�W�f�[�^��S����
//	����	�F	
//	�o��	�F	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::ClearMsg(){
	for(MSGITE pp = m_MassageMap.begin(); pp != m_MassageMap.end(); ++ pp){
		delete pp->second;
	}

	m_MassageMap.clear();
}



////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	ScenePop
//	�����T�v	�F	�t���[����pop����
//	����		�F	
//	�o��		�F	
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::ScenePop(){
	if( m_Stack.empty())
		return;
	//top�t���[���������[�X
	m_Stack.top()->Uninit();
	m_Stack.pop();
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�FScenePush
//	�����T�v	�F�t���[����push����
//	����		�FpScene	push����t���[���̃|�C���^
//	�o��		�F���������true���s�����false
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::ScenePush(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "ScenePush:Push�t���[�����ݒ肳��Ă��܂���";
		return false;
	}

	if(spScene->GetMakeFlag()){
		if(!spScene->ReStart()){
			m_Error = "�t���[���̏������Ɏ��s";
			spScene->Uninit();
			return false;
		}
	}else{
		if(!spScene->Init()){
			m_Error = "�t���[���̏������Ɏ��s";
			spScene->Uninit();
			return false;
		}
	}

	m_Stack.push(spScene);
	
	return true;
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�FSceneChange
//	�����T�v	�Ftop�t���[����ύX����
//	����		�FpScene	�ύX����t���[���̃|�C���^
//	�o��		�F���������true���s�����false
////////////////////////////////////////////////////////////////////////////
bool SceneManagerBase::SceneChange(CSmartPointer<SceneBase> spScene){
	if(!spScene.GetPointer()){
		m_Error = "SceneChange:�ύX�t���[�����ݒ肳��Ă��܂���";
		return false;
	}

	ScenePop();

	if(!ScenePush(spScene))
		return false;
	return true;
}


////////////////////////////////////////////////////////////////////////////
//	�֐���	�FGetScene
//	�����T�v	�Ftop�t���[�����擾����
//	����		�F
//	�o��		�Ftop�t���[���̃|�C���^�B�Ȃ��ꍇ��NULL��Ԃ�
////////////////////////////////////////////////////////////////////////////
CSmartPointer<SceneBase> SceneManagerBase::GetScene(){
	
	bool bSearch = true;
	SCENEDATAMAP::iterator Iterator;

	switch(m_Command){
		case FMANAGERCOM_NO://�C�x���g�Ȃ�
			return GetTopScene();

		case FMANAGERCOM_POP://pop
			ScenePop();
			break;

		case FMANAGERCOM_CHANGE://�ύX
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end()){
				bSearch = false;
				break;//�Ȃ��ꍇ
			}
			bSearch = SceneChange(Iterator->second);
			break;

		case FMANAGERCOM_PUSH://push
			Iterator = m_SceneDataMap.find(m_TmpSceneName);
			if(Iterator == m_SceneDataMap.end()){
				bSearch = false;
				break;//�Ȃ��ꍇ
			}
			bSearch = ScenePush(Iterator->second);
			break;

		case FMANAGERCOM_POPNORELEASE://Release�Ȃ�pop
			GetTopScene()->PreRelease();
			m_Stack.pop();
			break;

		case FMANAGERCOM_RESTART://���X�^�[�g
			GetTopScene()->PreRelease();
			GetTopScene()->ReStart();
			break;

		case FMANAGERCOM_HRESTART://�w�r�[���X�^�[�g
			bSearch = SceneChange(GetTopScene());
			break;
		default://��O
			m_Error="GetScene:�s���ȃR�}���h�����͂���܂���";
			break;
	}

	m_Command = FMANAGERCOM_NO;
	if(bSearch == false){
		return NULL;
	}

	return GetTopScene();
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	Release
//	�����T�v	�F	�J���֐�
//	����		�F
//	�o��		�F
////////////////////////////////////////////////////////////////////////////
void SceneManagerBase::Release(){
	//�X�^�b�N�̊J������
	if(!m_Stack.empty()){
		m_Stack.pop();
	}

	//�o�^���̍폜
	RegistedAllSceneDelete();
	m_Command	 = FMANAGERCOM_NO;
	m_TmpSceneName.clear();
	ClearMsg();
}