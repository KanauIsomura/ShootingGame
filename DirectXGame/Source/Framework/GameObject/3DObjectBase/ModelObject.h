//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ModelObject.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-05-31
//�X�V��:2016-05-31
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______MODEL_OBJECT_CLASS_H_INCLUDE_____
#define ______MODEL_OBJECT_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"3DObjectBase.h"
//#include"../../Scene/SceneGame/ColliderSphere.h"

#include<tchar.h>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���f��(���b�V��)�������I�u�W�F�N�g
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
