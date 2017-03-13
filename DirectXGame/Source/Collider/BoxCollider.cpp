//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
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
#include"BoxCollider.h"
#include"../Framework/System/Device.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
BoxCollider::BoxCollider(){
	m_spTransform		= NULL;
	m_Center			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BoundingBoxSize	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pBoundingBoxMesh	= NULL;
	m_MeshColor.r		= 0.0f;
	m_MeshColor.g		= 0.0f;
	m_MeshColor.b		= 1.0f;
	m_MeshColor.a		= 0.5f;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
BoxCollider::~BoxCollider(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ボックスコライダーの生成
void BoxCollider::Create(CSmartPointer<Transform> spTransform, D3DXVECTOR3& Center, D3DXVECTOR3& Size){
	m_spTransform		= spTransform;
	m_Center			= Center;
	m_BoundingBoxSize	= Size;

	//境界ボックスの表示用メッシュの作成
	D3DXCreateBox(
		GetDeviceClass()->GetDirect3DDevice(),
		m_BoundingBoxSize.x * 2.0f,
		m_BoundingBoxSize.y * 2.0f,
		m_BoundingBoxSize.z * 2.0f,
		&m_pBoundingBoxMesh, NULL);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//OBBでのあたり判定
bool BoxCollider::CollisionOBB(CSmartPointer<BoxCollider> spOtherCollider){
	// ワールドマトリックス取得
	D3DXMATRIX& mW1 = m_spTransform->GetWorld();
	D3DXMATRIX& mW2 = spOtherCollider->GetTransform()->GetWorld();
	// ワールド空間上の境界ボックス中心座標を求める
	D3DXVECTOR3 vPos1, vPos2;
	D3DXVec3TransformCoord(&vPos1, &GetCenter(), &mW1);
	D3DXVec3TransformCoord(&vPos2, &(spOtherCollider->GetCenter()), &mW2);
	// 中心座標間のベクトルを求める
	D3DXVECTOR3 vD = vPos2 - vPos1;

	// モデル座標軸を求める
	D3DXVECTOR3 vN[6];
	vN[0] = D3DXVECTOR3(mW1._11, mW1._12, mW1._13);
	vN[1] = D3DXVECTOR3(mW1._21, mW1._22, mW1._23);
	vN[2] = D3DXVECTOR3(mW1._31, mW1._32, mW1._33);
	vN[3] = D3DXVECTOR3(mW2._11, mW2._12, mW2._13);
	vN[4] = D3DXVECTOR3(mW2._21, mW2._22, mW2._23);
	vN[5] = D3DXVECTOR3(mW2._31, mW2._32, mW2._33);
	// OBBの大きさ(半分)の長さを掛けたベクトルを求める
	D3DXVECTOR3& vBBox1 = GetBoundingBox();
	D3DXVECTOR3& vBBox2 = spOtherCollider->GetBoundingBox();
	D3DXVECTOR3 vL[6];
	vL[0] = vN[0] * vBBox1.x;
	vL[1] = vN[1] * vBBox1.y;
	vL[2] = vN[2] * vBBox1.z;
	vL[3] = vN[3] * vBBox2.x;
	vL[4] = vN[4] * vBBox2.y;
	vL[5] = vN[5] * vBBox2.z;

	// 分離軸候補はモデル座標軸
	float fL, fD;
	for (int i = 0; i < 6; ++i) {
		D3DXVECTOR3& vS = vN[i];// 分離軸候補
		// OBBの影(半分)の合計
		fL = 0.0f;
		for (int j = 0; j < 6; ++j) {
			fL += fabsf(D3DXVec3Dot(&vS, &vL[j]));
		}
		// 影と中心間の距離の比較
		fD = fabsf(D3DXVec3Dot(&vS, &vD));
		if (fL < fD) {
			return false;	// 当たっていない
		}
	}
	// 分離軸候補は2辺から生成
	D3DXVECTOR3 vS;	// 分離軸候補
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; ++j) {
			// 分離軸候補を計算
			D3DXVec3Cross(&vS, &vN[i], &vN[j]);
			D3DXVec3Normalize(&vS, &vS);
			// OBBの影(半分)の合計
			fL = 0.0f;
			for (int k = 0; k < 6; ++k) {
				fL += fabsf(D3DXVec3Dot(&vS, &vL[k]));
			}
			// 影と中心間の距離の比較
			fD = fabsf(D3DXVec3Dot(&vS, &vD));
			if (fL < fD) {
				return false;	// 当たっていない
			}
		}
	}
	//あたった時の色に
	m_bHit = true;
	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ヒットボックスの描画
void BoxCollider::Draw(){
	if(!m_pBoundingBoxMesh)
		return;

	// 中心座標へ移動
	D3DXMATRIX mTrans;
	D3DXMatrixTranslation(&mTrans, m_Center.x, m_Center.y, m_Center.z);

	// ワールドマトリックス設定
	LPDIRECT3DDEVICE9 pDevice = GetDeviceClass()->GetDirect3DDevice();
	pDevice->SetTransform(D3DTS_WORLD, &(mTrans * m_spTransform->GetWorld()));
	
	// マテリアル設定
	if(m_bHit){
		m_MeshColor.r	= 1.0f; m_MeshColor.g	= 0.0f;
		m_MeshColor.b	= 0.0f; m_MeshColor.a	= 0.5f;
	}else{
		m_MeshColor.r	= 0.0f; m_MeshColor.g	= 0.0f;
		m_MeshColor.b	= 1.0f; m_MeshColor.a	= 0.5f;
	}
	m_bHit = false;
	pDevice->SetTexture(0, NULL);
	D3DMATERIAL9 Material = {{0.0f}};
	Material.Diffuse = m_MeshColor;
	pDevice->SetMaterial(&Material);
	
	// 描画
	m_pBoundingBoxMesh->DrawSubset(0);
}
