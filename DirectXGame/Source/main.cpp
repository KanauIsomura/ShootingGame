#include<Windows.h>

#include"Framework\System\CSmartPointer.h"
#include"GameMain.h"

#include"Framework\System\MemoryLeakDebug.h"
#include"Framework\System\DebugWindow.h"

//--	WinMain
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int ShowCmd){
	//TO DO: VisualStudio2015���ƏI�����ɃG���[���ł�
	//�f�o�b�N�E�B���h�E�̕\��
	//Debug::DebugWindow DebugWindow;
	//DebugWindow.Init();
	//���������[�N���o�p�ݒ�
	SetMemoryLeakDebug();

	//�Q�[�����C���̐���
	CSmartPointer<CGameMain> spGameMain(new CGameMain);

	//--	����J�n
	return spGameMain->Run();
}