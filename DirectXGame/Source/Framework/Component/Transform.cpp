//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Tramsform.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2016-05-04
//更新日:2016-05-04
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"Transform.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
Transform::Transform(){
	D3DXMatrixIdentity(&m_World);
	m_spParent.SetPointer(NULL);
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
Transform::~Transform(){
	m_spParent.SetPointer(NULL);
}

//------------------------------------------
//移動
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//座標の移動
void Transform::AddPosition(D3DXVECTOR3 AddPos){
	m_World._41 += AddPos.x;
	m_World._42 += AddPos.y;
	m_World._43 += AddPos.z;
}

//------------------------------------------
//回転
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//全軸回転
void Transform::Rotation(D3DXVECTOR3 vAngle){
	D3DXMATRIX rot_x, rot_y, rot_z;
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);

	m_World._41 = m_World._42 = m_World._43 = 0.0f;

	D3DXMatrixRotationAxis(&rot_z, &GetForward(),	D3DXToRadian(vAngle.z));
	D3DXMatrixRotationAxis(&rot_y, &GetUp(),		D3DXToRadian(vAngle.y));
	D3DXMatrixRotationAxis(&rot_x, &GetRight(),	D3DXToRadian(vAngle.x));
	
	m_World *= (rot_z * rot_y * rot_x);
	
	m_World._41 = Pos.x;
	m_World._42 = Pos.y;
	m_World._43 = Pos.z;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//X軸回転
void Transform::RotationX(float fAngleX){
	D3DXMATRIX Rot;
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);
	m_World._41 = m_World._42 = m_World._43 = 0.0f;

	D3DXMatrixRotationAxis(&Rot, &GetRight(), D3DXToRadian(fAngleX));
	
	m_World *= Rot;

	m_World._41 = Pos.x;
	m_World._42 = Pos.y;
	m_World._43 = Pos.z;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//Y軸回転
void Transform::RotationY(float fAngleY){
	D3DXMATRIX Rot;
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);
	m_World._41 = m_World._42 = m_World._43 = 0.0f;

	D3DXMatrixRotationAxis(&Rot, &GetUp(), D3DXToRadian(fAngleY));
	
	m_World *= Rot;

	m_World._41 = Pos.x;
	m_World._42 = Pos.y;
	m_World._43 = Pos.z;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//Z軸回転
void Transform::RotationZ(float fAngleZ){
	D3DXMATRIX Rot;
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);
	m_World._41 = m_World._42 = m_World._43 = 0.0f;

	D3DXMatrixRotationAxis(&Rot, &GetForward(), D3DXToRadian(fAngleZ));
	
	m_World *= Rot;

	m_World._41 = Pos.x;
	m_World._42 = Pos.y;
	m_World._43 = Pos.z;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//指定した軸で回転
void Transform::RotationAxis(D3DXVECTOR3 vAxis, float fAngle){
	D3DXMATRIX Rot;
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);
	m_World._41 = m_World._42 = m_World._43 = 0.0f;

	D3DXMatrixRotationAxis(&Rot, &vAxis, D3DXToRadian(fAngle));
	
	m_World *= Rot;

	m_World._41 = Pos.x;
	m_World._42 = Pos.y;
	m_World._43 = Pos.z;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//指定した座標の方向に向かせる
void Transform::LockAtTransform(D3DXVECTOR3& Pos){
	D3DXVECTOR3 VecX;
	D3DXVECTOR3 VecY = this->GetUp();
	D3DXVECTOR3 VecZ = Pos - this->GetPosition();
	
	//Z軸ベクトルの正規化
	D3DXVec3Normalize(&VecZ, &VecZ);

	//X軸の方向ベクトルを求める
	D3DXVec3Cross(&VecX, &VecY, &VecZ);
	//X軸ベクトルの正規化
	D3DXVec3Normalize(&VecX, &VecX);

	//Y軸の方向ベクトルを求める
	D3DXVec3Cross(&VecY, &VecZ, &VecX);
	//Y軸ベクトルの正規化
	D3DXVec3Normalize(&VecY, &VecY);
	
	//姿勢軸の代入
	this->m_World._11 = VecX.x; this->m_World._12 = VecX.y; this->m_World._13 = VecX.z;
	this->m_World._21 = VecY.x; this->m_World._22 = VecY.y; this->m_World._23 = VecY.z;
	this->m_World._31 = VecZ.x; this->m_World._32 = VecZ.y; this->m_World._33 = VecZ.z;
}

//------------------------------------------
//相対座標の取得
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ワールドマトリックス
D3DXMATRIX Transform::GetWorld(){
	if(m_spParent.GetPointer()) //親のトランスフォームの取得
		return m_World * m_spParent->GetWorld();
	else 
		return m_World;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//座標の取得
D3DXVECTOR3 Transform::GetPosition(){
	D3DXVECTOR3 Pos(m_World._41, m_World._42, m_World._43);
	if(m_spParent.GetPointer()){//親のトランスフォームが存在するか
		D3DXMATRIX World = m_World * m_spParent->GetWorld();
		Pos = D3DXVECTOR3(World._41, World._42, World._43);
		return Pos;
	}

	return Pos;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//前ベクトルの取得
D3DXVECTOR3 Transform::GetForward(){
	D3DXVECTOR3 Forward(m_World._31, m_World._32, m_World._33);
	if(m_spParent.GetPointer()){//親のトランスフォームが存在するか
		D3DXMATRIX World = m_World * m_spParent->GetWorld();
		Forward = D3DXVECTOR3(m_World._31, m_World._32, m_World._33);
		return Forward;
	}
	
	return Forward;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//右ベクトルの取得
D3DXVECTOR3 Transform::GetRight(){
	D3DXVECTOR3 Right(m_World._11, m_World._12, m_World._13);
	if(m_spParent.GetPointer()){//親のトランスフォームが存在するか
		D3DXMATRIX World = m_World * m_spParent->GetWorld();
		Right = D3DXVECTOR3(m_World._11, m_World._12, m_World._13);
		return Right;
	}else
		return Right;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//上ベクトルの取得
D3DXVECTOR3 Transform::GetUp(){
	D3DXVECTOR3 Up(m_World._21, m_World._22, m_World._23);
	if(m_spParent.GetPointer()){//親のトランスフォームが存在するか
		D3DXMATRIX World = m_World * m_spParent->GetWorld();
		Up = D3DXVECTOR3(m_World._21, m_World._22, m_World._23);
		return Up;
	}
	
	return Up;
}

//------------------------------------------
//ローカル座標の取得
//------------------------------------------
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ローカル座標の取得
D3DXVECTOR3 Transform::GetLocalPosition(){
	return D3DXVECTOR3(m_World._41, m_World._42, m_World._43);
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ローカル前ベクトルの取得
D3DXVECTOR3 Transform::GetLocalForward(){
	return D3DXVECTOR3(m_World._31, m_World._32, m_World._33);
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ローカル右ベクトルの取得
D3DXVECTOR3 Transform::GetLocalRight(){
	return D3DXVECTOR3(m_World._11, m_World._12, m_World._13);
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ローカル上ベクトルの取得
D3DXVECTOR3 Transform::GetLocalUp(){
	return D3DXVECTOR3(m_World._21, m_World._22, m_World._23);
}
