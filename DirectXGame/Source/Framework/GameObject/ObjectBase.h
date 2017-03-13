//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ObjectBase.h]
//概要	:すべてのゲームオブジェクトのベース。
//		>基本的にゲームオブジェクトはこれを継承して使っていく。
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-05-26
//更新日:2015-05-26
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______OBJECT_BASE_CLASS_H_INCLUDE_____
#define ______OBJECT_BASE_CLASS_H_INCLUDE_____

//=== インクルード ===
#include<string>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//すべてのゲームオブジェクトのベース
class CObjectBase{
private:
	std::string m_ObjectName;

private:
	bool m_bDelete;

public:
	CObjectBase(std::string ObjectName);
	virtual ~CObjectBase();
	
	virtual void Initialize();
	virtual void Update();
	virtual void DrawNoAlpha();		//不透明部
	virtual void DrawAlpha();		//透明部
	virtual void Finalize();

	std::string GetName(){return m_ObjectName;}
	void Delete(){m_bDelete = true;}
	bool GetDeleteFlag(){return m_bDelete;}

};

#endif
