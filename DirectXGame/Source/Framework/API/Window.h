//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Window.h]
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

#ifndef ______WINDOW_CLASS_H_INCLUDE_____
#define ______WINDOW_CLASS_H_INCLUDE_____

//=== インクルード ===
#include<Windows.h>
#include<tchar.h>

#include"../System/CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ウィンドウクラス
class CWindow{
private:
	HWND			m_WindowHandle;		// ウィンドウハンドル
	MSG			m_Message;			// メッセージ情報
	WNDCLASSEX	m_Wcex;				// ウィンドウクラス
	LPTSTR		m_WindowName;			// ウィンドウの名前
	int			m_ClientWidth;		// クライアント領域の幅
	int			m_ClientHeight;		// クライアント領域の高さ
	int			m_WindowWidth;		// ウィンドウの幅
	int			m_WindowHeight;		// ウィンドウの高さ

private:
	//コンストラクタ
	CWindow();

public:
	//ウィンドウクラスの静的生成関数
	static CSmartPointer<CWindow> GetSingletonPointer();

	//デストラクタ
	~CWindow(){};
	
	//ウィンドウの作成
	HRESULT MakeWindow(	LPTSTR WindowName, HINSTANCE& hInstance,
		const int& ClientWidth = 800, const int& ClientHeight = 600);
	
	//ウィンドウの表示
	HRESULT Show();

	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc( HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
	
	//メッセージの処理関数
	bool HandleMessage();

	//ゲッター
	//終了コードの取得関数
	int GetExitValue()const{ return m_Message.wParam; }
	//ウィンドウハンドルの取得関数
	HWND	GetWindowHandle()const{ return m_WindowHandle; }
	//ウィンドウの名前取得関数
	LPTSTR GetName()const{ return m_WindowName; }
	//クライアント領域の幅取得関数
	int GetClientWidth()const{ return m_ClientWidth; }
	//クライアント領域の高さ取得関数
	int GetClientHeight()const{ return m_ClientHeight; }
	//ウィンドウの幅取得関数
	int GetWindowWidth()const{ return m_WindowWidth; }
	//ウィンドウの高さ取得関数
	int GetWindowHeight()const{ return m_WindowHeight; }
};

inline CSmartPointer<CWindow> GetWindowClass(){
	return CWindow::GetSingletonPointer();
}

#endif
