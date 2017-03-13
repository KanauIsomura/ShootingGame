//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Particle.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-00
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

//=== インクルード ===
#include "Particle.h"

#include"../ObjectManager.h"
#include"../../System/Device.h"
#include"../../DXGraphic/TextureManager.h"

#define PATH_TEXTUREFNAME	TEXT("../Data/Texture/Particle/flare07.bmp")

#define FVF_PVERTEX			( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define frand()				(rand()/(float)RAND_MAX)

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// コンストラクタ
CParticle::CParticle() : C3DObjectBase("Particle"){
	m_szTexFName[0] = _T('\0');
	GetDefaultParam(m_ParticleParam);
	
	m_fTime = 0.0f;
	m_fRate = 0.0f;
	m_uParticles = m_ParticleParam.uMaxParticles;

	m_pGrain = NULL;
	m_pUse = NULL;
	m_uUse = 0;
	m_pUnused = NULL;

	m_pVertex = NULL;
	m_pIndex = NULL;
}
CParticle::CParticle(TParticleParam* ppp) : C3DObjectBase("Particle"){
	m_szTexFName[0] = _T('\0');

	GetDefaultParam(m_ParticleParam);
	
	m_fTime = 0.0f;
	m_fRate = 0.0f;
	m_uParticles = m_ParticleParam.uMaxParticles;

	m_pGrain = NULL;
	m_pUse = NULL;
	m_uUse = 0;
	m_pUnused = NULL;

	m_pVertex = NULL;
	m_pIndex = NULL;

	if (ppp) {
		SetParam(*ppp);
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// デストラクタ
CParticle::~CParticle(){
	Finalize();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクルの生成
CParticle::TGrain* CParticle::New(){
	if (m_pUnused) {
		TGrain* p = m_pUnused;
		m_pUnused = m_pUnused->m_pNext;
		if (m_pUnused) {
			m_pUnused->m_pBack = NULL;
		}
		p->m_pNext = m_pUse;
		if (m_pUse) {
			m_pUse->m_pBack = p;
		}
		m_pUse = p;

		// 初期値設定
		p->m_pos = m_spTransform->GetPosition();
		p->m_pos.y += m_ParticleParam.fStartSize2 * 0.5f;
		if (m_ParticleParam.coord == eSpherical) {
			p->m_accel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		} else {
			p->m_accel = D3DXVECTOR3(0.0f, -m_ParticleParam.fGravity, 0.0f);
		}
		float a = 0.0f;									// 開始角
		float b = D3DXToRadian(m_ParticleParam.fCornAngle / 2.0f);	// 終了角
		float theta = acosf(cosf(a) - frand() * (cosf(a) - cosf(b)));
		float r = sinf(theta);
		float eta = frand() * 2.0f * D3DX_PI;
		p->m_velocity.x = r * cosf(eta);
		p->m_velocity.y = cosf(theta);
		p->m_velocity.z = r * sinf(eta);
		D3DXVec3Normalize(&p->m_velocity, &p->m_velocity);
		p->m_pos += p->m_velocity * m_ParticleParam.fRadius;
		p->m_startPos = p->m_pos;
		switch (m_ParticleParam.coord) {
		case eSpherical:
			p->m_velocity.x = r;
			p->m_velocity.y = D3DXToDegree(eta);
			p->m_velocity.z = D3DXToDegree(theta);
			break;
		case eCylindrical:
			p->m_velocity.x = r;
			p->m_velocity.y = cosf(theta);
			p->m_velocity.z = D3DXToDegree(theta);
			break;
		case eCartesian:
		default:
			break;
		}
		p->m_velocity *= m_ParticleParam.fStartSpeed + (m_ParticleParam.fStartSpeed2 - m_ParticleParam.fStartSpeed) * frand();
		p->m_size = m_ParticleParam.fStartSize + (m_ParticleParam.fStartSize2 - m_ParticleParam.fStartSize) * frand();
		p->m_startSize = p->m_size;
		p->m_angle = m_ParticleParam.fStartRotation + (m_ParticleParam.fStartRotation2 - m_ParticleParam.fStartRotation) * frand();
		UINT uA = HIBYTE(HIWORD(m_ParticleParam.cStartColor));
		UINT uR = LOBYTE(HIWORD(m_ParticleParam.cStartColor));
		UINT uG = HIBYTE(LOWORD(m_ParticleParam.cStartColor));
		UINT uB = LOBYTE(m_ParticleParam.cStartColor);
		UINT uA2 = HIBYTE(HIWORD(m_ParticleParam.cStartColor2));
		UINT uR2 = LOBYTE(HIWORD(m_ParticleParam.cStartColor2));
		UINT uG2 = HIBYTE(LOWORD(m_ParticleParam.cStartColor2));
		UINT uB2 = LOBYTE(m_ParticleParam.cStartColor2);
		uA += (uA2 - uA) * rand() / RAND_MAX;
		uR += (uR2 - uR) * rand() / RAND_MAX;
		uG += (uG2 - uG) * rand() / RAND_MAX;
		uB += (uB2 - uB) * rand() / RAND_MAX;
		p->m_color = D3DCOLOR_ARGB(uA, uR, uG, uB);
		p->m_startColor = p->m_color;
		p->m_lifetime = m_ParticleParam.fStartLifetime + (m_ParticleParam.fStartLifetime2 - m_ParticleParam.fStartLifetime) * frand();
		p->m_startLifetime = p->m_lifetime;

		++m_uUse;
		return p;
	}
	return NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクルの削除
void CParticle::Delete(TGrain* p){
	if(p->m_pBack){
		p->m_pBack->m_pNext = p->m_pNext;
	}else{
		m_pUse = p->m_pNext;
	}
	if(p->m_pNext){
		p->m_pNext->m_pBack = p->m_pBack;
	}

	p->m_pBack = NULL;
	p->m_pNext = m_pUnused;
	if(m_pUnused){
		m_pUnused->m_pBack = p;
	}
	m_pUnused = p;

	--m_uUse;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// テクスチャ更新
void CParticle::UpdateTexture(){
	D3DXIMAGE_INFO info;
	ZeroMemory(&info, sizeof(info));
	if (m_ParticleParam.szTexFName[0])
		lstrcpy(m_szTexFName, m_ParticleParam.szTexFName);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化
void CParticle::Initialize(){
	m_pCamera = (CCameraBase*)GetObjectManager()->FindObject("Camera");

	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE_ARRAY(m_pGrain);
	m_pUse = m_pUnused = NULL;

	UpdateTexture();

	m_pGrain = new TGrain[m_ParticleParam.uMaxParticles];
	m_pGrain[0].m_pBack = NULL;
	m_pGrain[0].m_pNext = &m_pGrain[1];
	for (UINT i = 1; i < m_ParticleParam.uMaxParticles - 1; ++i) {
		m_pGrain[i].m_pNext = &m_pGrain[i + 1];
		m_pGrain[i].m_pBack = &m_pGrain[i - 1];
	}
	m_pGrain[m_ParticleParam.uMaxParticles - 1].m_pBack = &m_pGrain[m_ParticleParam.uMaxParticles - 2];
	m_pGrain[m_ParticleParam.uMaxParticles - 1].m_pNext = NULL;
	m_pUse		= NULL;
	m_uUse		= 0;
	m_pUnused		= &m_pGrain[0];

	m_pVertex		= new TVertex[m_ParticleParam.uMaxParticles * 4];
	m_pIndex		= new WORD[m_ParticleParam.uMaxParticles * 6];
	
	m_fTime		= m_ParticleParam.fDuration;
	m_uParticles	= 0;
	m_bStart		= false;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 更新
void CParticle::Update(){
	// 経過時間 (実時間に変更した方が良い)
	float fDeltaTime = 1.0f / 60.0f;

	// 生成
	if (m_uParticles) {
		m_fRate += m_ParticleParam.uRate * fDeltaTime;
		while (m_fRate >= 1.0f) {
			m_fRate -= 1.0f;
			--m_uParticles;
			New();
		}
	}

	if(!m_pCamera){
		m_pCamera = (CCameraBase*)GetObjectManager()->FindObject("Camera");
		if(!m_pCamera)
			return;
	}
	D3DXMATRIX& mView = m_pCamera->GetViewMatrix();
	D3DXMATRIX mBB(
		mView._11, mView._21, mView._31, 0.0f,
		mView._12, mView._22, mView._32, 0.0f,
		mView._13, mView._23, mView._33, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mRZ, mtx;
	TGrain* pNext = NULL;
	TVertex* pV = m_pVertex;
	WORD* pI = m_pIndex;
	WORD wIndex = 0;
	for (TGrain* p = m_pUse; p; p = pNext) {
		pNext = p->m_pNext;
		// 更新
		p->m_lifetime -= fDeltaTime;
		if (p->m_lifetime <= 0.0f) {
			Delete(p);
			continue;
		}
		p->m_velocity += p->m_accel * fDeltaTime;
		p->m_pos += p->m_velocity * fDeltaTime;
		if (p->m_pos.y <= -p->m_size * 2.0f) {
			Delete(p);
			continue;
		}
		float delta;
		if (m_ParticleParam.bColorOverLifeTime || m_ParticleParam.bSizeOverLifeTime)
			delta = p->m_lifetime / p->m_startLifetime;

		if (m_ParticleParam.bColorOverLifeTime) {
			int nA	= HIBYTE(HIWORD(m_ParticleParam.cColorOverLifeTime[0]));
			int nR	= LOBYTE(HIWORD(m_ParticleParam.cColorOverLifeTime[0]));
			int nG	= HIBYTE(LOWORD(m_ParticleParam.cColorOverLifeTime[0]));
			int nB	= LOBYTE(m_ParticleParam.cColorOverLifeTime[0]);
			int nA2	= HIBYTE(HIWORD(m_ParticleParam.cColorOverLifeTime[1]));
			int nR2	= LOBYTE(HIWORD(m_ParticleParam.cColorOverLifeTime[1]));
			int nG2	= HIBYTE(LOWORD(m_ParticleParam.cColorOverLifeTime[1]));
			int nB2	= LOBYTE(m_ParticleParam.cColorOverLifeTime[1]);
			nA = (int)(nA2 + (nA - nA2) * delta);
			nR = (int)(nR2 + (nR - nR2) * delta);
			nG = (int)(nG2 + (nG - nG2) * delta);
			nB = (int)(nB2 + (nB - nB2) * delta);
			p->m_color = D3DCOLOR_ARGB(nA, nR, nG, nB);
		}
		if (m_ParticleParam.bSizeOverLifeTime) {
			p->m_size = m_ParticleParam.fSizeOverLifeTime[1] + (m_ParticleParam.fSizeOverLifeTime[0] - m_ParticleParam.fSizeOverLifeTime[1]) * delta;
		}
		// 頂点構築
		D3DXMatrixRotationZ(&mRZ, D3DXToRadian(p->m_angle));
		switch (m_ParticleParam.coord) {
		case eCartesian:
		default:
			mBB._41 = p->m_pos.x;
			mBB._42 = p->m_pos.y;
			mBB._43 = p->m_pos.z;
			break;
		case eSpherical:
			mBB._41 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y));
			mBB._42 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.y));
			mBB._43 = p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z)) * cosf(D3DXToRadian(p->m_pos.y));
			break;
		case eCylindrical:
			mBB._41 = p->m_pos.x * cosf(D3DXToRadian(p->m_pos.z));
			mBB._42 = p->m_pos.y;
			mBB._43 = p->m_pos.x * sinf(D3DXToRadian(p->m_pos.z));
			break;
		}
		mtx = mRZ * mBB;
		D3DXVECTOR3 pos;
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(p->m_size * -0.5f, p->m_size * 0.5f, 0.0f), &mtx);
		pV[0].m_pos = pos;
		D3DXVec3TransformNormal(&pos, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &mtx);
		pV[0].m_normal = pos;
		pV[0].m_diffuse = p->m_color;
		pV[0].m_u = 0.0f, pV[0].m_v = 0.0f;
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(p->m_size * 0.5f, p->m_size * 0.5f, 0.0f), &mtx);
		pV[1].m_pos = pos;
		pV[1].m_normal = pV[0].m_normal;
		pV[1].m_diffuse = p->m_color;
		pV[1].m_u = 1.0f, pV[1].m_v = 0.0f;
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(p->m_size * -0.5f, p->m_size * -0.5f, 0.0f), &mtx);
		pV[2].m_pos = pos;
		pV[2].m_normal = pV[0].m_normal;
		pV[2].m_diffuse = p->m_color;
		pV[2].m_u = 0.0f, pV[2].m_v = 1.0f;
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(p->m_size * 0.5f, p->m_size * -0.5f, 0.0f), &mtx);
		pV[3].m_pos = pos;
		pV[3].m_normal = pV[0].m_normal;
		pV[3].m_diffuse = p->m_color;
		pV[3].m_u = 1.0f, pV[3].m_v = 1.0f;
		pV += 4;
		// インデックス構築
		pI[0] = wIndex + 0;
		pI[1] = wIndex + 1;
		pI[2] = wIndex + 2;
		pI[3] = wIndex + 2;
		pI[4] = wIndex + 1;
		pI[5] = wIndex + 3;
		pI += 6;
		wIndex += 4;
	}

	// 時間を進める
	m_fTime += fDeltaTime;
	if (m_fTime >= m_ParticleParam.fDuration) {
		m_fRate = 0.0f;
		if (m_ParticleParam.bLooping && m_bStart) {
			m_fTime = 0.0f;
			m_uParticles = m_ParticleParam.uMaxParticles;
		} else {
			m_fTime = m_ParticleParam.fDuration;
			m_uParticles = 0;
		}
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//半透明の描画
void CParticle::DrawAlpha(){
	if (!m_uUse && !m_pCamera)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);

	switch (m_ParticleParam.synthetic) {
	case eSubtractive:		// 減算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		// THRU
	case eAdditive:		// 加算合成
	case eAlphaBlend:		// 半透明合成
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンド可
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		switch (m_ParticleParam.synthetic) {
		case eAdditive:		// 加算合成
		case eSubtractive:	// 減算合成
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		default:
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		}
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// ライティングしない
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;
	default:
		break;
	}

	pDevice->SetFVF(FVF_PVERTEX);
	const D3DMATERIAL9 Material = {
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{0.1f, 0.1f, 0.1f, 1.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		0.0f,
	};
	pDevice->SetMaterial(&Material);
	pDevice->SetTexture(0, GetTextureManager()->LoadTexture(m_szTexFName));									// テクスチャを登録

	pDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST, 0, m_uUse * 6, m_uUse * 2, m_pIndex,D3DFMT_INDEX16, m_pVertex, sizeof(*m_pVertex));

	switch (m_ParticleParam.synthetic) {
	case eSubtractive:		// 減算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//THRU
	case eAdditive:		// 加算合成
	case eAlphaBlend:		// 半透明合成
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライティングする
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			// αブレンド 不可
		break;
	}
	
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//開放
void CParticle::Finalize(){
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE_ARRAY(m_pGrain);
	m_pUse = m_pUnused = NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクル発生
void CParticle::Start(){
	m_bStart		= true;
	m_fTime		= 0.0f;
	m_uParticles	= m_ParticleParam.uMaxParticles;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//パーティクルストップ
void CParticle::Stop(){
	m_bStart		= false;
	m_fTime		= m_ParticleParam.fDuration;
	m_uParticles	= 0;
}

//--------------------------------------------------------------
//セッター
//--------------------------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// パラメータ更新
void CParticle::SetParam(TParticleParam& pp){
	m_ParticleParam = pp;
	if (m_ParticleParam.fStartSpeed > m_ParticleParam.fStartSpeed2) {
		float fSpeed = m_ParticleParam.fStartSpeed;
		m_ParticleParam.fStartSpeed = m_ParticleParam.fStartSpeed2;
		m_ParticleParam.fStartSpeed2 = fSpeed;
	}
	if (m_ParticleParam.fStartSize > m_ParticleParam.fStartSize2) {
		float fSize = m_ParticleParam.fStartSize;
		m_ParticleParam.fStartSize = m_ParticleParam.fStartSize2;
		m_ParticleParam.fStartSize2 = fSize;
	}
	if (m_ParticleParam.fStartRotation > m_ParticleParam.fStartRotation2) {
		float fRotation = m_ParticleParam.fStartRotation;
		m_ParticleParam.fStartRotation = m_ParticleParam.fStartRotation2;
		m_ParticleParam.fStartRotation2 = fRotation;
	}
	if (m_ParticleParam.fStartLifetime > m_ParticleParam.fStartLifetime2) {
		float fLifetime = m_ParticleParam.fStartLifetime;
		m_ParticleParam.fStartLifetime = m_ParticleParam.fStartLifetime2;
		m_ParticleParam.fStartLifetime2 = fLifetime;
	}
	UINT uA = HIBYTE(HIWORD(m_ParticleParam.cStartColor));
	UINT uR = LOBYTE(HIWORD(m_ParticleParam.cStartColor));
	UINT uG = HIBYTE(LOWORD(m_ParticleParam.cStartColor));
	UINT uB = LOBYTE(m_ParticleParam.cStartColor);
	UINT uA2 = HIBYTE(HIWORD(m_ParticleParam.cStartColor2));
	UINT uR2 = LOBYTE(HIWORD(m_ParticleParam.cStartColor2));
	UINT uG2 = HIBYTE(LOWORD(m_ParticleParam.cStartColor2));
	UINT uB2 = LOBYTE(m_ParticleParam.cStartColor2);
	bool bChange = false;
	if(uA > uA2) {UINT u = uA; uA = uA2; uA2 = u;bChange = true;}
	if(uR > uR2) {UINT u = uR; uR = uR2; uR2 = u;bChange = true;}
	if(uG > uG2) {UINT u = uG; uG = uG2; uG2 = u;bChange = true;}
	if(uB > uB2) {UINT u = uB; uB = uB2; uB2 = u;bChange = true;}
	if(bChange) {
		m_ParticleParam.cStartColor = D3DCOLOR_ARGB(uA, uR, uG, uB);
		m_ParticleParam.cStartColor2 = D3DCOLOR_ARGB(uA2, uR2, uG2, uB2);
	}
	m_fTime = 0.0f;
	m_fRate = 0.0f;
	m_uParticles = m_ParticleParam.uMaxParticles;

	UpdateTexture();
}

//--------------------------------------------------------------
//ゲッター
//--------------------------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デフォルトのパラメータ取得
void CParticle::GetDefaultParam(TParticleParam& pp){
	pp.fDuration = 10.0f;
	pp.uMaxParticles = 10000;
	pp.coord = eCartesian;
	pp.synthetic = eAlphaBlend;
	pp.bLooping = true;
	pp.fGravity = 9.8f;
	pp.fStartSpeed = 25.0f;
	pp.fStartSpeed2 = 35.0f;
	pp.fStartSize = 1.0f;
	pp.fStartSize2 = 1.0f;
	pp.fStartRotation = 0.0f;
	pp.fStartRotation2 = pp.fStartRotation;
	pp.cStartColor = D3DCOLOR_ARGB(127, 191, 255, 255);
	pp.cStartColor2 = pp.cStartColor;
	pp.fStartLifetime = 5.0f;
	pp.fStartLifetime2 = pp.fStartLifetime;
	pp.uRate = 600;
	pp.fCornAngle = 20.0f;
	pp.fRadius = 0.0f;
	pp.bColorOverLifeTime = true;
	pp.cColorOverLifeTime[0] = D3DCOLOR_ARGB(127, 191, 255, 255);
	pp.cColorOverLifeTime[1] = D3DCOLOR_ARGB(0, 255, 255, 255);
	pp.bSizeOverLifeTime = true;
	pp.fSizeOverLifeTime[0] = 1.0f;
	pp.fSizeOverLifeTime[1] = 10.0f;
	lstrcpy(pp.szTexFName, PATH_TEXTUREFNAME);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デバック文字の取得
void CParticle::GetDebugString(LPTSTR pszText){
	TCHAR sz[1024];
	_stprintf(pszText, _T("m_uUse=%u\n"), m_uUse);
	for (UINT i = 0; i < m_uUse * 3 && i < 24; ++i) {
		_stprintf(sz, _T("%u(%f,%f,%f)\n"), i, m_pVertex[i].m_pos.x, m_pVertex[i].m_pos.y, m_pVertex[i].m_pos.z);
		lstrcat(pszText, sz);
	}
}