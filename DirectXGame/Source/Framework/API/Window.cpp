//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-00
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"Window.h"

//=== �萔��` ===

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
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
//�E�B���h�E�N���X�̐���
CSmartPointer<CWindow> CWindow::GetSingletonPointer(){
	static CSmartPointer<CWindow> pStaticWindow(new CWindow);
	return pStaticWindow;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�E�B���h�E�̍쐬
HRESULT CWindow::MakeWindow( LPTSTR WindowName, HINSTANCE& hInstance,
	const int& ClientWidth, const int& ClientHeight){
	
	//�E�B���h�E�̖��O
	m_WindowName	= WindowName;
	//�E�B���h�E�̃T�C�Y
	m_ClientWidth	= ClientWidth;
	m_ClientHeight	= ClientHeight;

	ZeroMemory(static_cast<LPVOID>(&m_Wcex), sizeof(WNDCLASSEX));

	//WNDCLASSEX�̒�`
	m_Wcex.cbSize			= sizeof(WNDCLASSEX);					//WNDCLASSEX�̃������T�C�Y���w��
	m_Wcex.style			= CS_CLASSDC;							//�\������E�B���h�E�̃X�^�C�����w��
	m_Wcex.lpfnWndProc		= CWindow::WindowProc;					//�E�B���h�E�v���V�[�W���̃A�h���X
	m_Wcex.cbClsExtra		= 0L;								//���ʂ͎g�p���Ȃ��i"0"���w��j
	m_Wcex.cbWndExtra		= 0L;								//���ʂ͎g�p���Ȃ��i"0"���w��j
	m_Wcex.hInstance		= GetModuleHandle(nullptr);				//WinMain�̃C���X�^���X�n���h����ݒ�
	m_Wcex.hIcon			= nullptr;							//�A�C�R���w��(�f�t�H�Ȃ�NULL)
	m_Wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//�}�E�X�J�[�\�����w��
	m_Wcex.hbrBackground	= nullptr;							//�N���C�A���g�̈�w�i���f�t�H�F��
	m_Wcex.lpszMenuName	= nullptr;							//Window�ɂ��郁�j���[��ݒ�
	m_Wcex.lpszClassName	= _T("Window");						//�E�B���h�E�N���X�̖��O
	m_Wcex.hIconSm		= nullptr;							//���A�C�R���ݒ�̏����L�q

	RegisterClassEx(&m_Wcex);

	RECT rect = { 0, 0, ClientWidth, ClientHeight };
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	//�E�B���h�EHandle���쐬
	m_WindowHandle = CreateWindow(	_T("Window"),
								m_WindowName,
								WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,				// �E�B���h�E�^�C�v
								CW_USEDEFAULT,										// �E�B���h�Ex���W
								CW_USEDEFAULT,										// �E�B���h�Ey���W
								m_WindowWidth = (m_ClientWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2),	// �E�B���h�E��
								m_WindowHeight = (m_ClientHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),		// �E�B���h�E����
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
//�E�B���h�E�̕\��
HRESULT CWindow::Show(){
	if(m_WindowHandle != nullptr){
		ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
		UpdateWindow(m_WindowHandle);
		return S_OK;
	}else
		return E_FAIL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�E�B���h�E�v���V�[�W��
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
//���b�Z�[�W�̏����֐�
bool CWindow::HandleMessage(){
	//�|�X�g�ς݃��b�Z�[�W�̏���
	while(PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE)){
		//WM_QUIT���b�Z�[�W�̏���
		if(m_Message.message == WM_QUIT)
			return false;
		
		//WM_QUIT�ȊO�̃��b�Z�[�W�̏���
		TranslateMessage(&m_Message);
		DispatchMessage(&m_Message);
	}
	return true;

}