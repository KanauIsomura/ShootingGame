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
#include"Window.h"

//=== 定数定義 ===

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CWindow::CWindow()
	: m_WindowHandle(nullptr)
	, m_Message()
	, m_WindowName(_T("Window"))
	, m_WindowWidth(0)
	, m_WindowHeight(0)
	, m_ClientWidth(0)	
	, m_ClientHeight(0){
	ZeroMemory(&m_Message, sizeof(m_Message));
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ウィンドウクラスの生成
CSmartPointer<CWindow> CWindow::GetSingletonPointer(){
	static CSmartPointer<CWindow> pStaticWindow(new CWindow);
	return pStaticWindow;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ウィンドウの作成
HRESULT CWindow::MakeWindow( LPTSTR WindowName, HINSTANCE& hInstance,
	const int& ClientWidth, const int& ClientHeight){
	
	//ウィンドウの名前
	m_WindowName	= WindowName;
	//ウィンドウのサイズ
	m_ClientWidth	= ClientWidth;
	m_ClientHeight	= ClientHeight;

	ZeroMemory(static_cast<LPVOID>(&m_Wcex), sizeof(WNDCLASSEX));

	//WNDCLASSEXの定義
	m_Wcex.cbSize			= sizeof(WNDCLASSEX);					//WNDCLASSEXのメモリサイズを指定
	m_Wcex.style			= CS_CLASSDC;							//表示するウィンドウのスタイルを指定
	m_Wcex.lpfnWndProc		= CWindow::WindowProc;					//ウィンドウプロシージャのアドレス
	m_Wcex.cbClsExtra		= 0L;								//普通は使用しない（"0"を指定）
	m_Wcex.cbWndExtra		= 0L;								//普通は使用しない（"0"を指定）
	m_Wcex.hInstance		= GetModuleHandle(nullptr);				//WinMainのインスタンスハンドルを設定
	m_Wcex.hIcon			= nullptr;							//アイコン指定(デフォならNULL)
	m_Wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//マウスカーソルを指定
	m_Wcex.hbrBackground	= nullptr;							//クライアント領域背景をデフォ色に
	m_Wcex.lpszMenuName	= nullptr;							//Windowにつけるメニューを設定
	m_Wcex.lpszClassName	= _T("Window");						//ウィンドウクラスの名前
	m_Wcex.hIconSm		= nullptr;							//小アイコン設定の情報を記述

	RegisterClassEx(&m_Wcex);

	RECT rect = { 0, 0, ClientWidth, ClientHeight };
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	//ウィンドウHandleを作成
	m_WindowHandle = CreateWindow(	_T("Window"),
								m_WindowName,
								WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,				// ウィンドウタイプ
								CW_USEDEFAULT,										// ウィンドウx座標
								CW_USEDEFAULT,										// ウィンドウy座標
								m_WindowWidth = (m_ClientWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2),	// ウィンドウ幅
								m_WindowHeight = (m_ClientHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),		// ウィンドウ高さ
								GetDesktopWindow(),
								nullptr,
								m_Wcex.hInstance,
								nullptr);

	if(m_WindowHandle != nullptr)
		return S_OK;
	else
		return E_FAIL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ウィンドウの表示
HRESULT CWindow::Show(){
	if(m_WindowHandle != nullptr){
		ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
		UpdateWindow(m_WindowHandle);
		return S_OK;
	}else
		return E_FAIL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ウィンドウプロシージャ
LRESULT CALLBACK CWindow::WindowProc( 
	HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam){	
	switch( Message ){
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			::DestroyWindow(hWindow);
		break;
	case WM_CLOSE:
		::DestroyWindow(hWindow);
		PostMessage(hWindow, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWindow, Message, wParam, lParam));
	}
	return 0;

}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//メッセージの処理関数
bool CWindow::HandleMessage(){
	//ポスト済みメッセージの処理
	while(PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE)){
		//WM_QUITメッセージの処理
		if(m_Message.message == WM_QUIT)
			return false;
		
		//WM_QUIT以外のメッセージの処理
		TranslateMessage(&m_Message);
		DispatchMessage(&m_Message);
	}
	return true;

}