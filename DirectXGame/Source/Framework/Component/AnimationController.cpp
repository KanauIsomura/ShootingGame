//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[AnimationController.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-00
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"AnimationController.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
AnimationController::AnimationController(){
	m_pAnimationController	= NULL;
	m_pAnimationSet		= NULL;
	m_uNowAnimationID		= 0;
	m_uBeforeAnimationID	= 0;
	m_uMaxAnimationSet		= 0;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
AnimationController::~AnimationController(){
	if(m_pAnimationSet){
		delete[] m_pAnimationSet;
		m_pAnimationSet = NULL;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// ループ時間を設定
bool AnimationController::SetLoopTime(UINT AnimationID, FLOAT LoopTime){
   // 指定のアニメーションIDの存在をチェック
   if(m_uMaxAnimationSet <= AnimationID)
      return false;

	// トラックスピード調節値を算出
	FLOAT DefTime= (FLOAT)m_pAnimationSet[AnimationID].pAnimationSet->GetPeriod();
	m_pAnimationSet[AnimationID].fLoopTime		= LoopTime;
	m_pAnimationSet[AnimationID].fTrackSpeed		= DefTime / LoopTime;

   return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 動作開始にかかる時間を設定
bool AnimationController::SetShiftTime(UINT AnimationID, FLOAT Interval){
	// 指定のアニメーションIDの存在をチェック
	if( m_uMaxAnimationSet <= AnimationID )
		return false;

	// シフト時間を登録
	m_pAnimationSet[AnimationID].fShiftTime = Interval;

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// アニメーションを切り替え
bool AnimationController::ChangeAnimation(UINT AnimationID){
	// 指定のアニメーションIDの存在をチェック
	if(m_uMaxAnimationSet <= AnimationID)
	   return false;
	
	// 異なるアニメーションであるかをチェック
	if( m_uNowAnimationID == AnimationID )
	   return false;
	
	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	m_pAnimationController->GetTrackDesc( 0, &TD );
	
	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	m_pAnimationController->SetTrackAnimationSet( 1, m_pAnimationSet[m_uNowAnimationID].pAnimationSet);
	m_pAnimationController->SetTrackDesc( 1, &TD );
	
	// 新しいアニメーションセットをトラック0に設定
	m_pAnimationController->SetTrackAnimationSet( 0, m_pAnimationSet[AnimationID].pAnimationSet);
	
	// トラック0のスピードの設定
	m_pAnimationController->SetTrackSpeed( 0, m_pAnimationSet[AnimationID].fTrackSpeed );
	
	// トラックの合成を許可
	m_pAnimationController->SetTrackEnable( 0, true );
	m_pAnimationController->SetTrackEnable( 1, true );
	
	// ウェイト時間を初期化
	m_pAnimationSet[AnimationID].fNowWeightTime = 0.0f;
	
	// 現在のアニメーション番号を切り替え
	m_uBeforeAnimationID	= m_uNowAnimationID;
	m_uNowAnimationID		= AnimationID;

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// アニメーションを更新
bool AnimationController::UpdateTime(FLOAT Time){
	// 合成中か否かを判定
	m_pAnimationSet[m_uNowAnimationID].fNowWeightTime	+= Time;
	if(m_pAnimationSet[m_uNowAnimationID].fNowWeightTime <= m_pAnimationSet[m_uNowAnimationID].fShiftTime ){
		// 合成中。ウェイトを算出
		FLOAT Weight = m_pAnimationSet[m_uNowAnimationID].fNowWeightTime / m_pAnimationSet[m_uNowAnimationID].fShiftTime;
		// ウェイトを登録
		m_pAnimationController->SetTrackWeight( 0, Weight);		// 現在のアニメーション
		m_pAnimationController->SetTrackWeight( 1, 1.0f - Weight);	// 前のアニメーション
	}else{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		m_pAnimationController->SetTrackWeight( 0, 1.0f);			// 現在のアニメーション
		m_pAnimationController->SetTrackEnable( 1, false);		// 前のアニメーションを無効にする
	}
	
	// 時間を更新
	m_pAnimationController->AdvanceTime( Time, NULL );

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//アニメーション情報を設定
void AnimationController::SetAnimation(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXANIMATIONSET* ppAnimationSet, UINT uMaxAnimationSet){
	if(!pAnimationController || !ppAnimationSet) return;

	//アニメーションコントローラのコピー
	pAnimationController->CloneAnimationController(
		pAnimationController->GetMaxNumAnimationOutputs(),
		pAnimationController->GetMaxNumAnimationSets(),
		pAnimationController->GetMaxNumTracks(),
		pAnimationController->GetMaxNumEvents(),
		&m_pAnimationController);

	//アニメーション最大数をコピー
	m_uMaxAnimationSet	= uMaxAnimationSet;

	//アニメーション情報をコピー
	m_pAnimationSet = new Animation[uMaxAnimationSet];
	for(UINT nCountID = 0; nCountID < uMaxAnimationSet; ++nCountID){
		m_pAnimationSet[nCountID].pAnimationSet	= ppAnimationSet[nCountID];
		m_pAnimationSet[nCountID].uAnimationID	= nCountID;
		m_pAnimationSet[nCountID].fLoopTime		= 1.0f;
		m_pAnimationSet[nCountID].fTrackSpeed	= 1.0f;
		m_pAnimationSet[nCountID].fShiftTime		= 1.0f;
		m_pAnimationSet[nCountID].fNowWeightTime	= 0.0f;
	}
}
