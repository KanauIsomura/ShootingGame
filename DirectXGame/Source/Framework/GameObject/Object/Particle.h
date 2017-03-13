//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Particle.h]
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

#ifndef ______PARTICLE_CLASS_H_INCLUDE_____
#define ______PARTICLE_CLASS_H_INCLUDE_____

//=== インクルード ===
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
//パーティクルの発生設定
enum EParticleCoord {
	eCartesian = 0,			// 直交座標系(x,y,z)
	eSpherical,				// 球座標系(r,φ,θ)
	eCylindrical,				// 円筒座標系(r,h,θ)

	eMaxParticleCoord
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//描画方法
enum ESynthetic {
	eNone = 0,	// 通常描画
	eAdditive,	// 加算合成
	eAlphaBlend,	// 半透明合成
	eSubtractive,	// 減算合成

	eMaxSynthetic
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクル構造体
struct TParticleParam{
	ESynthetic		synthetic;			// 描画方法
	EParticleCoord	coord;				// 座標系
	float			fDuration;			// 生成周期
	UINT				uMaxParticles;		// 最大パーティクル数
	bool				bLooping;				// 繰り返し
	float			fGravity;				// 重力加速度
	float			fStartSpeed;			// 初速
	float			fStartSpeed2;			// 初速2
	float			fStartSize;			// 初期サイズ
	float			fStartSize2;			// 初期サイズ2
	float			fStartRotation;		// 初期角度
	float			fStartRotation2;		// 初期角度2
	D3DCOLOR			cStartColor;			// 初期カラー
	D3DCOLOR			cStartColor2;			// 初期カラー2
	float			fStartLifetime;		// 寿命
	float			fStartLifetime2;		// 寿命2
	UINT				uRate;				// 1秒あたりのパーティクル生成数
	float			fCornAngle;			// コーン角度
	float			fRadius;				// 半径(180:半球、360:球)
	bool				bColorOverLifeTime;	// 生存時間で色を変更(初期カラーが無効になる)
	D3DCOLOR			cColorOverLifeTime[2];	// [0]:開始色、[1]:終了色
	bool				bSizeOverLifeTime;		// 生存時間でサイズを変更(初期サイズが無効になる)
	float			fSizeOverLifeTime[2];	// [0]:開始サイズ、[1]:終了サイズ
	TCHAR			szTexFName[_MAX_PATH];	// テクスチャファイル名
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクルクラス
class CParticle : public C3DObjectBase{
private:
	CCameraBase*		m_pCamera;				//カメラオブジェクト
	TCHAR			m_szTexFName[_MAX_PATH];	//テクスチャパス

	TParticleParam	m_ParticleParam;			//パーティクルの設定
	bool				m_bStart;					//始まってるか
	float			m_fTime;					// パーティクル生成時間
	float			m_fRate;					// パーティクル生成数ワーク
	UINT				m_uParticles;				// 残りパーティクル数

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

	void SetParam(TParticleParam& pp);			//パーティクル情報のセット
	void GetDefaultParam(TParticleParam& pp);	//デフォルトの初期パラメータ取得
	void GetDebugString(LPTSTR pszText);			//デバック情報文字列取得

private:
	void UpdateTexture(void);
};

#endif