//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[LightBase.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-09-12
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______LIGHT_BASE_CLASS_H_INCLUDE_____
#define ______LIGHT_BASE_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../3DObjectBase/3DObjectBase.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class LightBase : public C3DObjectBase{
private:
	LPD3DXMESH m_pSpherMesh;

public:
	LightBase();
	virtual ~LightBase();

	virtual void Initialize();
	virtual void Update();
	virtual void Finalize();
};

#endif
