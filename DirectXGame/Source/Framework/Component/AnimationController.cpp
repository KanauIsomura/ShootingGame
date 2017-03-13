//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[AnimationController.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-00
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"AnimationController.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
AnimationController::AnimationController(){
	m_pAnimationController	= NULL;
	m_pAnimationSet		= NULL;
	m_uNowAnimationID		= 0;
	m_uBeforeAnimationID	= 0;
	m_uMaxAnimationSet		= 0;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�X�g���N�^
AnimationController::~AnimationController(){
	if(m_pAnimationSet){
		delete[] m_pAnimationSet;
		m_pAnimationSet = NULL;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// ���[�v���Ԃ�ݒ�
bool AnimationController::SetLoopTime(UINT AnimationID, FLOAT LoopTime){
   // �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
   if(m_uMaxAnimationSet <= AnimationID)
      return false;

	// �g���b�N�X�s�[�h���ߒl���Z�o
	FLOAT DefTime= (FLOAT)m_pAnimationSet[AnimationID].pAnimationSet->GetPeriod();
	m_pAnimationSet[AnimationID].fLoopTime		= LoopTime;
	m_pAnimationSet[AnimationID].fTrackSpeed		= DefTime / LoopTime;

   return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// ����J�n�ɂ����鎞�Ԃ�ݒ�
bool AnimationController::SetShiftTime(UINT AnimationID, FLOAT Interval){
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if( m_uMaxAnimationSet <= AnimationID )
		return false;

	// �V�t�g���Ԃ�o�^
	m_pAnimationSet[AnimationID].fShiftTime = Interval;

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �A�j���[�V������؂�ւ�
bool AnimationController::ChangeAnimation(UINT AnimationID){
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if(m_uMaxAnimationSet <= AnimationID)
	   return false;
	
	// �قȂ�A�j���[�V�����ł��邩���`�F�b�N
	if( m_uNowAnimationID == AnimationID )
	   return false;
	
	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	m_pAnimationController->GetTrackDesc( 0, &TD );
	
	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	m_pAnimationController->SetTrackAnimationSet( 1, m_pAnimationSet[m_uNowAnimationID].pAnimationSet);
	m_pAnimationController->SetTrackDesc( 1, &TD );
	
	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_pAnimationController->SetTrackAnimationSet( 0, m_pAnimationSet[AnimationID].pAnimationSet);
	
	// �g���b�N0�̃X�s�[�h�̐ݒ�
	m_pAnimationController->SetTrackSpeed( 0, m_pAnimationSet[AnimationID].fTrackSpeed );
	
	// �g���b�N�̍���������
	m_pAnimationController->SetTrackEnable( 0, true );
	m_pAnimationController->SetTrackEnable( 1, true );
	
	// �E�F�C�g���Ԃ�������
	m_pAnimationSet[AnimationID].fNowWeightTime = 0.0f;
	
	// ���݂̃A�j���[�V�����ԍ���؂�ւ�
	m_uBeforeAnimationID	= m_uNowAnimationID;
	m_uNowAnimationID		= AnimationID;

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �A�j���[�V�������X�V
bool AnimationController::UpdateTime(FLOAT Time){
	// ���������ۂ��𔻒�
	m_pAnimationSet[m_uNowAnimationID].fNowWeightTime	+= Time;
	if(m_pAnimationSet[m_uNowAnimationID].fNowWeightTime <= m_pAnimationSet[m_uNowAnimationID].fShiftTime ){
		// �������B�E�F�C�g���Z�o
		FLOAT Weight = m_pAnimationSet[m_uNowAnimationID].fNowWeightTime / m_pAnimationSet[m_uNowAnimationID].fShiftTime;
		// �E�F�C�g��o�^
		m_pAnimationController->SetTrackWeight( 0, Weight);		// ���݂̃A�j���[�V����
		m_pAnimationController->SetTrackWeight( 1, 1.0f - Weight);	// �O�̃A�j���[�V����
	}else{
		// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
		m_pAnimationController->SetTrackWeight( 0, 1.0f);			// ���݂̃A�j���[�V����
		m_pAnimationController->SetTrackEnable( 1, false);		// �O�̃A�j���[�V�����𖳌��ɂ���
	}
	
	// ���Ԃ��X�V
	m_pAnimationController->AdvanceTime( Time, NULL );

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�A�j���[�V��������ݒ�
void AnimationController::SetAnimation(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXANIMATIONSET* ppAnimationSet, UINT uMaxAnimationSet){
	if(!pAnimationController || !ppAnimationSet) return;

	//�A�j���[�V�����R���g���[���̃R�s�[
	pAnimationController->CloneAnimationController(
		pAnimationController->GetMaxNumAnimationOutputs(),
		pAnimationController->GetMaxNumAnimationSets(),
		pAnimationController->GetMaxNumTracks(),
		pAnimationController->GetMaxNumEvents(),
		&m_pAnimationController);

	//�A�j���[�V�����ő吔���R�s�[
	m_uMaxAnimationSet	= uMaxAnimationSet;

	//�A�j���[�V���������R�s�[
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
