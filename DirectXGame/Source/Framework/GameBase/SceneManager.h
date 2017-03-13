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

#ifndef ______SCENE_MANAGER_CALSS_H_INCLUDE_____
#define ______SCENE_MANAGER_CALSS_H_INCLUDE_____

#include<stack>
#include<map>
#include<string>

#include"../System/CSmartPointer.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//SceneBase�N���X
//�t���[���p���ۃN���X
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneBase{
private:
	bool m_MakeFlag;							//Init���Ăяo���ꂽ��

protected:
	//�������z�֐�
	virtual bool InitMain() = 0;				//������
	virtual void ReleaseMain() = 0;				//�J��

public:
	SceneBase() { m_MakeFlag = false;}			//�R���X�g���N�^
	virtual ~SceneBase(){};

	virtual bool Init() { m_MakeFlag = true; return InitMain();}
	virtual void Uninit() { m_MakeFlag = false; ReleaseMain();}
	
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual bool ReStart() { return true;}
	virtual void PreRelease() { return;}

	bool GetMakeFlag() { return m_MakeFlag;}		//SceneManager�N���X�Ŏg�p�B

};

/////////////////////////////////////////////////////////////////////////////////////////////
//SceneManager�N���X
//�t���[������N���X
/////////////////////////////////////////////////////////////////////////////////////////////
class SceneManagerBase{
protected:
	//��`
	enum{
		FMANAGERCOM_NO,				//�ύX�C�x���g�Ȃ�
		FMANAGERCOM_POP,				//pop����
		FMANAGERCOM_CHANGE,			//�ύX����
		FMANAGERCOM_PUSH,				//push����
		FMANAGERCOM_RESTART,			//�t���[���ċN��
		FMANAGERCOM_HRESTART,			//�t���[���ċN��(�f�[�^�Ăяo���܂�)
		FMANAGERCOM_POPNORELEASE,		//pop���邪Release�ł͂Ȃ�PreRelease
		FMANAGERCOM_CHANGE_NOLOADING,	//�ύX����B�ǂݍ��݉�ʂȂ�
		FMANAGERCOM_PUSH_NOLOADING,		//push����B�ǂݍ��݉�ʂȂ�
		FMANAGERCOM_HRESTART_NOLOADING,	//�t���[���ċN��(�f�[�^�Ăяo���܂�)�B�ǂݍ��݉�ʂȂ�
		FMANAGERCOM_POP_NOLOADING		//pop����B�ǂݍ��݉�ʂȂ�
	};
	
	typedef std::map< std::string, CSmartPointer<SceneBase>>	SCENEDATAMAP;
	typedef std::pair< std::string, CSmartPointer<SceneBase>>	SCENEDATAPAIR;
	typedef std::stack< CSmartPointer<SceneBase>>				SCENESTACK;
	typedef std::map< std::string, void *>					MSGMAP;
	typedef std::map< std::string, void *>::iterator			MSGITE;

	//�����o�ϐ�
	SCENEDATAMAP	m_SceneDataMap;		//�V�[���f�[�^�z��
	SCENESTACK	m_Stack;				//�V�[���̃X�^�b�N
	int			m_Command;			//�V�[������R�}���h
	std::string	m_TmpSceneName;		//�ړ���V�[���ԍ�
	std::string	m_Error;				//�G���[���
	MSGMAP		m_MassageMap;			//���b�Z�[�W�}�b�v
	
	//�����o�֐�
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//����Scene�̎擾
	CSmartPointer<SceneBase> GetTopScene() { return ( ( m_Stack.empty() ) ? NULL : m_Stack.top() );}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//����Scene������
	void ScenePop();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�w�肵���V�[�������݃V�[����
	bool ScenePush(CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���݂̃V�[�����J�����Ďw�肵���V�[���ɕύX
	bool SceneChange(CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�o�^���ꂽ�V�[���̑S�폜
	void RegistedAllSceneDelete();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�w�肵���V�[������Ԍ��ɓ����
	std::string RegistSceneData(std::string SceneName, CSmartPointer<SceneBase> spScene);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�w�肵���V�[�������Ԗڂɓ����Ă��邩
	std::string GetSceneNum(CSmartPointer<SceneBase> spScene){
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���b�Z�[�W�̊m�F
	void *FindMsgData(char *key){
		std::string MessageKey = key;
		MSGITE MassageIte = m_MassageMap.find(MessageKey);
		return ( MassageIte == m_MassageMap.end() ) ? NULL : MassageIte->second;
	}

public:
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�R���X�g���N�^
	SceneManagerBase();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�f�X�g���N�^
	virtual ~SceneManagerBase(){Release();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�J��
	void Release();
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�V�[���̓o�^
	template <typename T> std::string RegistSceneDataMakeInstance(std::string SceneName){
		CSmartPointer<SceneBase> spDataTemplate(new T); return RegistSceneData( SceneName, spDataTemplate);}

	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���݃V�[���̎擾
	CSmartPointer<SceneBase> GetScene();
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//ID�Ŏw�肵���V�[���̊J��
	void SceneRelease(std::string SceneName) { m_SceneDataMap.find(SceneName)->second->Uninit();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���ݓo�^����Ă�V�[���̐�
	int GetRegistedSceneNum() { return m_SceneDataMap.size();}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//ID�Ŏw�肵���V�[���̍폜
	void DeleteRegistedScene(std::string SceneName);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���݂̃V�[����ID
	int GetTopSceneNum() { return -1;}//GetSceneNum(GetTopScene());}

	//���b�Z�[�W
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���b�Z�[�W�̓o�^
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
	//�w�肵�����b�Z�[�W�̍폜
	void DeleteMsg(char *key){
		std::string ss = key;
		MSGITE pp = m_MassageMap.find(ss);
		if(pp == m_MassageMap.end())
			return;
		delete pp->second;
		m_MassageMap.erase(pp);
	}
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�S�Ẵ��b�Z�[�W�̍폜
	void ClearMsg();
	
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//���b�Z�[�W�̍X�V
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
	//���b�Z�[�W�̎擾
	template <typename T> bool GetMsg(char *key,T *out){
		T *Massage = (T *)FindMsgData(key);
		if(!Massage)
			return FALSE;
		*out = *Massage;
		return TRUE;
	}

	//�C�x���g�B���ۂ̏�����GetScene���Ă΂ꂽ���ɍs����
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

//��y�Ɏg����悤��
//-------------------------------------------------------
//inline SceneManager *GetSceneManager(void){
//	return SceneManager::GetSingletonPointer();
//}
//-------------------------------------------------------

#endif //__SceneMANAGER_H__