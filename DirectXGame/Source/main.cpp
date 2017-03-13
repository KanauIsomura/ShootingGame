#include<Windows.h>

#include"Framework\System\CSmartPointer.h"
#include"GameMain.h"

#include"Framework\System\MemoryLeakDebug.h"
#include"Framework\System\DebugWindow.h"

//--	WinMain
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int ShowCmd){
	//TO DO: VisualStudio2015だと終了時にエラーがでる
	//デバックウィンドウの表示
	//Debug::DebugWindow DebugWindow;
	//DebugWindow.Init();
	//メモリリーク検出用設定
	SetMemoryLeakDebug();

	//ゲームメインの生成
	CSmartPointer<CGameMain> spGameMain(new CGameMain);

	//--	動作開始
	return spGameMain->Run();
}