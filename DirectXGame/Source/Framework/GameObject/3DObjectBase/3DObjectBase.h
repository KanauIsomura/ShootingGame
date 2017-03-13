//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-05-25
//�X�V��:2015-05-25
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______3D_OBJECT_BASE_CLASS_H_INCLUDE_____
#define ______3D_OBJECT_BASE_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../ObjectBase.h"
#include"../../Component/Transform.h"

#include"../../System/CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class C3DObjectBase : public CObjectBase{
protected:
	CSmartPointer<Transform> m_spTransform;

public:
	C3DObjectBase(std::string ObjName);
	virtual ~C3DObjectBase();

	virtual void Initialize();
	virtual void Update();
	virtual void DrawAlpha();
	virtual void DrawNoAlpha();
	virtual void Finalize();

	CSmartPointer<Transform> GetTransform(){return m_spTransform;}
};

#endif
