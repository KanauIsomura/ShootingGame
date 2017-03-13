//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[AnimationController.h]
//�T�v	:�X�L�����b�V���A�j���[�V�����p�R���g���[���[���g���₷��
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

#ifndef ______ANIMATION_CONTROLLER_CLASS_H_INCLUDE_____
#define ______ANIMATION_CONTROLLER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"d3dx9.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�A�j���[�V�����R���g���[���\����
typedef struct{
	LPD3DXANIMATIONSET pAnimationSet;  //�A�j���[�V�����Z�b�g
	UINT               uAnimationID;   //�A�j���[�V����ID
	FLOAT              fLoopTime;      //1���[�v�̎���
	FLOAT              fTrackSpeed;    //�g���b�N�X�s�[�h���ߒl
	FLOAT              fShiftTime;     //�V�t�g����̂ɂ����鎞��
	FLOAT              fNowWeightTime; //���݂̃E�F�C�g����
}Animation;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//����A�j���[�V�����R���g���[��
class AnimationController{
private:
	LPD3DXANIMATIONCONTROLLER m_pAnimationController; //�A�j���[�V�����R���g���[��
	Animation*                m_pAnimationSet;        //�A�j���[�V�������̔z��
	UINT                      m_uMaxAnimationSet;     //�A�j���[�V�����̍ő吔
	UINT                      m_uNowAnimationID;      //���݂̃A�j���[�V����ID
	UINT                      m_uBeforeAnimationID;   //�O�A�j���[�V����ID

public:
	AnimationController();
	virtual ~AnimationController();
	
	// ���[�v���Ԃ�ݒ�
	bool SetLoopTime(UINT AnimationID, FLOAT LoopTime);
	// ����J�n�ɂ����鎞�Ԃ�ݒ�
	bool SetShiftTime(UINT AnimationID, FLOAT Interval);
	// �A�j���[�V������؂�ւ�
	bool ChangeAnimation(UINT AnimationID);
	// �A�j���[�V�������X�V
	bool UpdateTime(FLOAT Time);

	//�A�j���[�V��������ݒ�
	void SetAnimation(LPD3DXANIMATIONCONTROLLER pAnimationController, LPD3DXANIMATIONSET* ppAnimationSet, UINT uMaxAnimationSet);
	//�A�j���[�V�����Z�b�g�̃}�b�N�X�����擾
	UINT GetMaxAnimationID(){return m_uMaxAnimationSet;}
	//���݂̃A�j���[�V����ID���擾
	UINT GetNowAnimationID(){return m_uNowAnimationID;}
	//���݂̃A�j���[�V�����̖��O���擾
	LPCSTR GetNowAnimationName(){return m_pAnimationSet[m_uNowAnimationID].pAnimationSet->GetName();}
};

#endif
