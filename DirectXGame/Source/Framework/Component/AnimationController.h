//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[AnimationController.h]
//概要	:スキンメッシュアニメーション用コントローラーを使いやすく
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

#ifndef ______ANIMATION_CONTROLLER_CLASS_H_INCLUDE_____
#define ______ANIMATION_CONTROLLER_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"d3dx9.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーションコントローラ構造体
typedef struct{
	LPD3DXANIMATIONSET pAnimationSet;  //アニメーションセット
	UINT               uAnimationID;   //アニメーションID
	FLOAT              fLoopTime;      //1ループの時間
	FLOAT              fTrackSpeed;    //トラックスピード調節値
	FLOAT              fShiftTime;     //シフトするのにかかる時間
	FLOAT              fNowWeightTime; //現在のウェイト時間
}Animation;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//自作アニメーションコントローラ
class AnimationController{
private:
	LPD3DXANIMATIONCONTROLLER m_pAnimationController; //アニメーションコントローラ
	Animation*                m_pAnimationSet;        //アニメーション情報の配列
	UINT                      m_uMaxAnimationSet;     //アニメーションの最大数
	UINT                      m_uNowAnimationID;      //現在のアニメーションID
	UINT                      m_uBeforeAnimationID;   //前アニメーションID

public:
	AnimationController();
	virtual ~AnimationController();
	
	// ループ時間を設定
	bool SetLoopTime(UINT AnimationID, FLOAT LoopTime);
	// 動作開始にかかる時間を設定
	bool SetShiftTime(UINT AnimationID, FLOAT Interval);
	// アニメーションを切り替え
	bool ChangeAnimation(UINT AnimationID);
	// アニメーションを更新
	bool UpdateTime(FLOAT Time);

	//アニメーション情報を設定
	void SetAnimation(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXANIMATIONSET* ppAnimationSet, UINT uMaxAnimationSet);
	//アニメーションセットのマックス数を取得
	UINT GetMaxAnimationID(){return m_uMaxAnimationSet;}
	//現在のアニメーションIDを取得
	UINT GetNowAnimationID(){return m_uNowAnimationID;}
	//現在のアニメーションの名前を取得
	LPCSTR GetNowAnimationName(){return m_pAnimationSet[m_uNowAnimationID].pAnimationSet->GetName();}
};

#endif
