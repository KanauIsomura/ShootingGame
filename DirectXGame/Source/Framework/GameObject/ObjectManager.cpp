//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ObjectManager.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-05-26
//更新日:2015-05-26
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//=== インクルード ===
#include"ObjectManager.h"
#include"ObjectBase.h"

#include"../System/Device.h"
#include"Object\CameraBase.h"
#include"Object\SkyDome.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CObjectManager::CObjectManager(){
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
CObjectManager::~CObjectManager(){
	ObjAllRelese();
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//マネージャー側の初期化
void CObjectManager::Initialize(){
	ObjAllRelese();
	m_ObjectMap.clear();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//マネージャー側の更新
void CObjectManager::Update(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//マネージャーの終了処理
void CObjectManager::Finalize(){
	ObjAllRelese();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//オブジェクトの登録
void CObjectManager::SetObject(OBJECT_NAME ObjName, CObjectBase* pObject){
	//この名前のオブジェクトが存在するか
	auto MapIterator = m_ObjectMap.find(ObjName);

	if(MapIterator != m_ObjectMap.end()){
		//名前が登録されている
		MapIterator->second.push_back(pObject);
	}else{
		//名前が登録されていない
		OBJECT_LIST NewObjectList;
		NewObjectList.push_back(pObject);

		OBJECT_PAIR NewObjectPair(ObjName, NewObjectList);
		m_ObjectMap.insert(NewObjectPair);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//登録された全てのオブジェクトの初期化
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
//登録された全てのオブジェクトの更新
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
//登録された全てのオブジェクトの描画
void CObjectManager::ObjDraw(){
	if(CObjectManager::FindObject("Camera"))
		((CCameraBase*)CObjectManager::FindObject("Camera"))->PreDraw();
	if(CObjectManager::FindObject("SkyDome"))
		((SkyDome*)CObjectManager::FindObject("SkyDome"))->PreDraw();

	
	
	//不透明部描画
	for(auto MapIterator = m_ObjectMap.begin();MapIterator != m_ObjectMap.end(); ++MapIterator){
		for(auto ListIterator = MapIterator->second.begin();
			ListIterator != MapIterator->second.end(); ++ListIterator){
				(*ListIterator)->DrawNoAlpha();
		}
	}

	//半透明部
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
//登録された全ての全てのオブジェクトの解放
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
//オブジェクトを名前と要素番号を指定して検索
CObjectBase* CObjectManager::FindObject(OBJECT_NAME ObjName, int nElement){
	auto MapIterator = m_ObjectMap.find(ObjName);

	//オブジェクトが登録されているか
	if(MapIterator == m_ObjectMap.end())
		return NULL;

	//要素数がオブジェクトの数を超えていないか
	if(MapIterator->second.size() == nElement)
		return NULL;

	return *(MapIterator->second.begin() + nElement);
}