//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[LightBase.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-09-12
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______LIGHT_BASE_CLASS_H_INCLUDE_____
#define ______LIGHT_BASE_CLASS_H_INCLUDE_____

//=== インクルード ===
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
