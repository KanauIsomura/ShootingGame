//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Particle.h]
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

#ifndef ______PARTICLE_CLASS_H_INCLUDE_____
#define ______PARTICLE_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include"../3DObjectBase/3DObjectBase.h"
#include"CameraBase.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)			if(x){ delete x; x=NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){ delete[] x; x=NULL; }
#endif

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�p�[�e�B�N���̔����ݒ�
enum EParticleCoord {
	eCartesian = 0,			// �������W�n(x,y,z)
	eSpherical,				// �����W�n(r,��,��)
	eCylindrical,				// �~�����W�n(r,h,��)

	eMaxParticleCoord
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�`����@
enum ESynthetic {
	eNone = 0,	// �ʏ�`��
	eAdditive,	// ���Z����
	eAlphaBlend,	// ����������
	eSubtractive,	// ���Z����

	eMaxSynthetic
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�p�[�e�B�N���\����
struct TParticleParam{
	ESynthetic		synthetic;			// �`����@
	EParticleCoord	coord;				// ���W�n
	float			fDuration;			// ��������
	UINT				uMaxParticles;		// �ő�p�[�e�B�N����
	bool				bLooping;				// �J��Ԃ�
	float			fGravity;				// �d�͉����x
	float			fStartSpeed;			// ����
	float			fStartSpeed2;			// ����2
	float			fStartSize;			// �����T�C�Y
	float			fStartSize2;			// �����T�C�Y2
	float			fStartRotation;		// �����p�x
	float			fStartRotation2;		// �����p�x2
	D3DCOLOR			cStartColor;			// �����J���[
	D3DCOLOR			cStartColor2;			// �����J���[2
	float			fStartLifetime;		// ����
	float			fStartLifetime2;		// ����2
	UINT				uRate;				// 1�b������̃p�[�e�B�N��������
	float			fCornAngle;			// �R�[���p�x
	float			fRadius;				// ���a(180:�����A360:��)
	bool				bColorOverLifeTime;	// �������ԂŐF��ύX(�����J���[�������ɂȂ�)
	D3DCOLOR			cColorOverLifeTime[2];	// [0]:�J�n�F�A[1]:�I���F
	bool				bSizeOverLifeTime;		// �������ԂŃT�C�Y��ύX(�����T�C�Y�������ɂȂ�)
	float			fSizeOverLifeTime[2];	// [0]:�J�n�T�C�Y�A[1]:�I���T�C�Y
	TCHAR			szTexFName[_MAX_PATH];	// �e�N�X�`���t�@�C����
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�p�[�e�B�N���N���X
class CParticle : public C3DObjectBase{
private:
	CCameraBase*		m_pCamera;				//�J�����I�u�W�F�N�g
	TCHAR			m_szTexFName[_MAX_PATH];	//�e�N�X�`���p�X

	TParticleParam	m_ParticleParam;			//�p�[�e�B�N���̐ݒ�
	bool				m_bStart;					//�n�܂��Ă邩
	float			m_fTime;					// �p�[�e�B�N����������
	float			m_fRate;					// �p�[�e�B�N�����������[�N
	UINT				m_uParticles;				// �c��p�[�e�B�N����

	struct TGrain {
		TGrain*		m_pBack;
		TGrain*		m_pNext;
		D3DXVECTOR3	m_pos;
		D3DXVECTOR3	m_startPos;
		D3DXVECTOR3	m_velocity;
		D3DXVECTOR3	m_accel;
		float		m_size;
		float		m_startSize;
		float		m_angle;
		D3DCOLOR		m_color;
		D3DCOLOR		m_startColor;
		float		m_lifetime;
		float		m_startLifetime;
	};
	TGrain*			m_pGrain;
	TGrain*			m_pUse;
	TGrain*			m_pUnused;
	UINT				m_uUse;
	struct TVertex {
		D3DVECTOR		m_pos;
		D3DVECTOR		m_normal;
		D3DCOLOR		m_diffuse;
		float		m_u, m_v;
	};
	TVertex*		m_pVertex;
	WORD*		m_pIndex;

public:
	CParticle();
	CParticle(TParticleParam* ppp);
	virtual ~CParticle();
	
	TGrain* New();
	void Delete(TGrain* p);

	virtual void Initialize();
	virtual void Update();
	virtual void DrawAlpha();
	virtual void Finalize();
	
	void Start();
	void Stop();

	void SetParam(TParticleParam& pp);			//�p�[�e�B�N�����̃Z�b�g
	void GetDefaultParam(TParticleParam& pp);	//�f�t�H���g�̏����p�����[�^�擾
	void GetDebugString(LPTSTR pszText);			//�f�o�b�N��񕶎���擾

private:
	void UpdateTexture(void);
};

#endif