//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Window.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-0
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______WINDOW_CLASS_H_INCLUDE_____
#define ______WINDOW_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<Windows.h>
#include<tchar.h>

#include"../System/CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�E�B���h�E�N���X
class CWindow{
private:
	HWND			m_WindowHandle;		// �E�B���h�E�n���h��
	MSG			m_Message;			// ���b�Z�[�W���
	WNDCLASSEX	m_Wcex;				// �E�B���h�E�N���X
	LPTSTR		m_WindowName;			// �E�B���h�E�̖��O
	int			m_ClientWidth;		// �N���C�A���g�̈�̕�
	int			m_ClientHeight;		// �N���C�A���g�̈�̍���
	int			m_WindowWidth;		// �E�B���h�E�̕�
	int			m_WindowHeight;		// �E�B���h�E�̍���

private:
	//�R���X�g���N�^
	CWindow();

public:
	//�E�B���h�E�N���X�̐ÓI�����֐�
	static CSmartPointer<CWindow> GetSingletonPointer();

	//�f�X�g���N�^
	~CWindow(){};
	
	//�E�B���h�E�̍쐬
	HRESULT MakeWindow(	LPTSTR WindowName, HINSTANCE& hInstance,
		const int& ClientWidth = 800, const int& ClientHeight = 600);
	
	//�E�B���h�E�̕\��
	HRESULT Show();

	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc( HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
	
	//���b�Z�[�W�̏����֐�
	bool HandleMessage();

	//�Q�b�^�[
	//�I���R�[�h�̎擾�֐�
	int GetExitValue()const{ return m_Message.wParam; }
	//�E�B���h�E�n���h���̎擾�֐�
	HWND	GetWindowHandle()const{ return m_WindowHandle; }
	//�E�B���h�E�̖��O�擾�֐�
	LPTSTR GetName()const{ return m_WindowName; }
	//�N���C�A���g�̈�̕��擾�֐�
	int GetClientWidth()const{ return m_ClientWidth; }
	//�N���C�A���g�̈�̍����擾�֐�
	int GetClientHeight()const{ return m_ClientHeight; }
	//�E�B���h�E�̕��擾�֐�
	int GetWindowWidth()const{ return m_WindowWidth; }
	//�E�B���h�E�̍����擾�֐�
	int GetWindowHeight()const{ return m_WindowHeight; }
};

inline CSmartPointer<CWindow> GetWindowClass(){
	return CWindow::GetSingletonPointer();
}

#endif
