//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ObjectManager.h]
//�T�v	:�I�u�W�F�N�g�̊Ǘ��ҁB
//		>�I�u�W�F�N�g�ւ̃A�N�Z�X�̌�����X�V�Ȃǂ��s��
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-05-26
//�X�V��:2016-05-26
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______OBJECT_MANAGER_CLASS_H_INCLUDE_____
#define ______OBJECT_MANAGER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<map>
#include<list>
#include<vector>
#include<string>

#include"../System/CSmartPointer.h"

class CObjectBase;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CObjectManager{
private:
	typedef std::string							OBJECT_NAME;
	typedef std::vector<CObjectBase*>			OBJECT_LIST;
	typedef std::map<OBJECT_NAME, OBJECT_LIST>	OBJECT_MAP;
	typedef std::pair<OBJECT_NAME, OBJECT_LIST>	OBJECT_PAIR;

	OBJECT_MAP m_ObjectMap;

private:
	CObjectManager();

public:
	virtual ~CObjectManager();

	static CSmartPointer<CObjectManager> GetSingletonPointer(){
		static CSmartPointer<CObjectManager> StaticObjManager(new CObjectManager);
		return StaticObjManager;
	}
	
	void Initialize();
	void Update();
	void Finalize();

	void ObjInitialize();
	void ObjUpdate();
	void ObjDraw();
	void ObjAllRelese();
	
	void SetObject(OBJECT_NAME ObjName, CObjectBase* pObject);
	CObjectBase* FindObject(OBJECT_NAME ObjName, int nElement = 0);
};

inline CSmartPointer<CObjectManager> GetObjectManager(){
	return CObjectManager::GetSingletonPointer();
}

#endif
