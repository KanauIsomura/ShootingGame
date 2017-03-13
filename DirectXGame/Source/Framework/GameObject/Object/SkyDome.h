//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-0
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______SKY_DOME_CLASS_H_INCLUDE_____
#define ______SKY_DOME_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"../3DObjectBase/ModelObject.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//スカイドーム
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
