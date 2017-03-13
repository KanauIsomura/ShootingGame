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

#ifndef ______SKY_DOME_CLASS_H_INCLUDE_____
#define ______SKY_DOME_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../3DObjectBase/ModelObject.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�X�J�C�h�[��
class SkyDome : public CModelObject{
private:
	C3DObjectBase* m_pCameraObj;

public:
	SkyDome();
	virtual ~SkyDome();

	virtual void Initialize();
	virtual void Update();
	virtual void DrawAlpha(){};
	virtual void DrawNoAlpha(){};
	virtual void Finalize();
	
	void PostUpdate();
	void PreDraw();
};

#endif
