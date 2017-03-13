//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ObjectManager.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-05-26
//�X�V��:2015-05-26
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//=== �C���N���[�h ===
#include"ObjectManager.h"
#include"ObjectBase.h"

#include"../System/Device.h"
#include"Object\CameraBase.h"
#include"Object\SkyDome.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CObjectManager::CObjectManager(){
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
CObjectManager::~CObjectManager(){
	ObjAllRelese();
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�}�l�[�W���[���̏�����
void CObjectManager::Initialize(){
	ObjAllRelese();
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�}�l�[�W���[���̍X�V
void CObjectManager::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�}�l�[�W���[�̏I������
void CObjectManager::Finalize(){
	ObjAllRelese();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�I�u�W�F�N�g�̓o�^
void CObjectManager::SetObject(OBJECT_NAME ObjName, CObjectBase* pObject){
	//���̖��O�̃I�u�W�F�N�g�����݂��邩
	auto MapIterator = m_ObjectMap.find(ObjName);

	if(MapIterator != m_ObjectMap.end()){
		//���O���o�^����Ă���
		MapIterator->second.push_back(pObject);
	}else{
		//���O���o�^����Ă��Ȃ�
		OBJECT_LIST NewObjectList;
		NewObjectList.push_back(pObject);

		OBJECT_PAIR NewObjectPair(ObjName, NewObjectList);
		m_ObjectMap.insert(NewObjectPair);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�o�^���ꂽ�S�ẴI�u�W�F�N�g�̏�����
void CObjectManager::ObjInitialize(){
	auto MapIterator = m_ObjectMap.begin();

	for(;MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end(); ++ListIterator){
				(*ListIterator)->Initialize();
		}
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�o�^���ꂽ�S�ẴI�u�W�F�N�g�̍X�V
void CObjectManager::ObjUpdate(){
	auto MapIterator = m_ObjectMap.begin();

	for(;MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end(); ){
				(*ListIterator)->Update();
				if((*ListIterator)->GetDeleteFlag()){
					(*ListIterator)->Finalize();
					delete (*ListIterator);
					ListIterator = MapIterator->second.erase(ListIterator);
					continue;
				}

				++ListIterator;
		}
	}
	
	if(CObjectManager::FindObject("Camera"))
		((CCameraBase*)CObjectManager::FindObject("Camera"))->PostUpdate();
	if(CObjectManager::FindObject("SkyDome"))
		((SkyDome*)CObjectManager::FindObject("SkyDome"))->PostUpdate();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�o�^���ꂽ�S�ẴI�u�W�F�N�g�̕`��
void CObjectManager::ObjDraw(){
	if(CObjectManager::FindObject("Camera"))
		((CCameraBase*)CObjectManager::FindObject("Camera"))->PreDraw();
	if(CObjectManager::FindObject("SkyDome"))
		((SkyDome*)CObjectManager::FindObject("SkyDome"))->PreDraw();

	
	
	//�s�������`��
	for(auto MapIterator = m_ObjectMap.begin();MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end(); ++ListIterator){
				(*ListIterator)->DrawNoAlpha();
		}
	}

	//��������
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	for(auto MapIterator = m_ObjectMap.begin();MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end(); ++ListIterator){
				pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				(*ListIterator)->DrawAlpha();
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�o�^���ꂽ�S�Ă̑S�ẴI�u�W�F�N�g�̉��
void CObjectManager::ObjAllRelese(){
	auto MapIterator = m_ObjectMap.begin();

	for(;MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end();){
				(*ListIterator)->Finalize();
				delete (*ListIterator);
				ListIterator = MapIterator->second.erase(ListIterator);
		}
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�I�u�W�F�N�g�𖼑O�Ɨv�f�ԍ����w�肵�Č���
CObjectBase* CObjectManager::FindObject(OBJECT_NAME ObjName, int nElement){
	auto MapIterator = m_ObjectMap.find(ObjName);

	//�I�u�W�F�N�g���o�^����Ă��邩
	if(MapIterator == m_ObjectMap.end())
		return NULL;

	//�v�f�����I�u�W�F�N�g�̐��𒴂��Ă��Ȃ���
	if(MapIterator->second.size() == nElement)
		return NULL;

	return *(MapIterator->second.begin() + nElement);
}