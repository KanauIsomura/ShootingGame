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
#include"Device.h"
#include"../API/Window.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//コンストラクタ
CDevice::CDevice():
	m_pDirect3D(nullptr),
	m_pDirect3DDevice(nullptr),
	m_bWindowMode(false){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//デストラクタ
CDevice::~CDevice(){
	if( m_pDirect3D )
		m_pDirect3D->Release();
	if( m_pDirect3DDevice )
		m_pDirect3DDevice->Release();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//クリエイト関数
CSmartPointer<CDevice> CDevice::GetSingletonPointer(){
	static CSmartPointer<CDevice> pStaticDevice(new CDevice);
	return pStaticDevice;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化関数
HRESULT CDevice::InitDirect3D(bool WindowMode){
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE		d3ddm;
	m_bWindowMode = WindowMode;

	//Direct3Dオブジェクトの生成
	m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pDirect3D == NULL ){
		return E_FAIL;
	}

	//現在のディスプレイモードを習得
	if( FAILED( m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) ) ){
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ワークをゼロクリア
	d3dpp.BackBufferWidth			= GetWindowClass()->GetClientWidth();	// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= GetWindowClass()->GetClientHeight();	// ゲーム画面サイズ(高)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの設定
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする。
	d3dpp.Windowed				= m_bWindowMode;			// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;					// デプスバファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if( m_bWindowMode ){
		//ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}else{
		//フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	//デバイスオブジェクトの生成
	//[デバイス作成制御]<描画>と<頂点処理>をハードウェアで行う
	if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
										D3DDEVTYPE_HAL,						// デバイスタイプ
										GetWindowClass()->GetWindowHandle(),			// フォーカスするウィンドウへのハンドル
										D3DCREATE_HARDWARE_VERTEXPROCESSING |
										D3DCREATE_MULTITHREADED,				// デバイス作成制御の組み合わせ
										&d3dpp,								// デバイスのプレゼンテーションパラメータ
										&m_pDirect3DDevice)) )	{				// デバイスインターフェースへのポインタ

			//上記の設定が失敗したら
			//[デバイス作成制御]<描画>とハードウェアで行い、<頂点処理>はCPUで行う
			if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
												D3DDEVTYPE_HAL,						// デバイスタイプ
												GetWindowClass()->GetWindowHandle(),		// フォーカスするウィンドウへのハンドル
												D3DCREATE_SOFTWARE_VERTEXPROCESSING |
												D3DCREATE_MULTITHREADED,				// デバイス作成制御の組み合わせ
												&d3dpp,								// デバイスのプレゼンテーションパラメータ
												&m_pDirect3DDevice)) ){				// デバイスインターフェースへのポインタ

					//記の設定が失敗したら
					//デバイス作成制御]<描画>とハ<頂点処理>はCPUで行う
					if( FAILED(m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
														D3DDEVTYPE_REF,						// デバイスタイプ
														GetWindowClass()->GetWindowHandle(),		// フォーカスするウィンドウへのハンドル
														D3DCREATE_SOFTWARE_VERTEXPROCESSING |
														D3DCREATE_MULTITHREADED,				// デバイス作成制御の組み合わせ
														&d3dpp,								// デバイスのプレゼンテーションパラメータ
														&m_pDirect3DDevice)) ){				// デバイスインターフェースへのポインタ
							//初期化失敗
							return E_FAIL;
					}
			}
	}
	InitRender();

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//画面のクリア
void CDevice::Clear(){
	m_pDirect3DDevice->Clear( 
		0, NULL, 
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 
		D3DCOLOR_RGBA( 125, 125, 125, 255), 1.0f, 0 );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//画面の描画開始
HRESULT CDevice::BeginScene(){
	return m_pDirect3DDevice->BeginScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//画面の描画終わり
void CDevice::EndScene(){
	m_pDirect3DDevice->EndScene();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/	
//Present
void CDevice::Present(){
	m_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//レンダリングの初期化
void CDevice::InitRender(){	
	//テクスチャの設定
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//レンダーステートのパラメータの設定
	m_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// 裏面をカリング
	m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );				// アルファブレンドを行う
	m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );			// ソースの設定
	m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );		// ですとの設定

	//サンプラーステートパラメータの設定
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );		// テクスチャU値の繰り返し設定
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );		// テクスチャV値の繰り返し設定
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );		// テクスチャ拡大時の補間設定
	m_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );		// テクスチャ拡小時の補間設定
}