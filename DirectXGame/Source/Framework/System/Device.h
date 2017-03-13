//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
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

#ifndef ______DEVICE_CALSS_H_INCLUDE_____
#define ______DEVICE_CALSS_H_INCLUDE_____

//=== インクルード ===
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include<d3dx9.h>
#include<d3d9.h>

#include"CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デバイスクラス
class CDevice{
private:
	LPDIRECT3D9		m_pDirect3D;
	LPDIRECT3DDEVICE9	m_pDirect3DDevice;
	bool				m_bWindowMode;

public:
	//デストラクタ
	~CDevice();

	//クリエイト関数
	static CSmartPointer<CDevice> GetSingletonPointer();

	//初期化関数
	HRESULT InitDirect3D(bool WindowMode = true);

	//画面のクリア
	void Clear();
	//画面の描画開始
	HRESULT BeginScene();
	//画面の描画終わり
	void EndScene();
	//Present
	void Present();

	//デバイスのゲッター
	LPDIRECT3DDEVICE9 GetDirect3DDevice()const{ return m_pDirect3DDevice; }
	
private:
	//コンストラクタ
	CDevice();

	void InitRender();
};

//デバイスクラスのシングルトンの取得
inline CSmartPointer<CDevice> GetDeviceClass(){
	return CDevice::GetSingletonPointer();
}

#endif
