//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelObject.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-05-31
//更新日:2016-05-31
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______MODEL_OBJECT_CLASS_H_INCLUDE_____
#define ______MODEL_OBJECT_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"3DObjectBase.h"
//#include"../../Scene/SceneGame/ColliderSphere.h"

#include<tchar.h>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//モデル(メッシュ)をつかうオブジェクト
class CModelObject : public C3DObjectBase{
protected:
	std::wstring					m_MeshFilePath;
	//CSmartPointer<ColiderSphere>	m_spSphereColider;

public:
	CModelObject(std::string ObjName, std::wstring FilePath);
	virtual ~CModelObject();

	virtual void Initialize();
	virtual void Update();
	virtual void DrawAlpha();
	virtual void DrawNoAlpha();
	virtual void Finalize();
	
	//CSmartPointer<ColiderSphere> GetColiderSphere(){return m_spSphereColider;}

};

#endif
