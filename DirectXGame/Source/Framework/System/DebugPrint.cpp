#include"DebugPrint.h"
#include"../API/Window.h"
#include"Device.h"


LPD3DXFONT   DebugPrint::m_pD3DXFont = NULL;
std::wstring DebugPrint::m_FontName  = _T("Terminl");
D3DXCOLOR    DebugPrint::m_FontColor = D3DXCOLOR(0,0,0,255);
int          DebugPrint::m_FontSize  = 20;

DebugPrint::DebugPrint(){
}

DebugPrint::~DebugPrint(){
}

void DebugPrint::SetFont(int FontSize, std::wstring FontName){
	
#if _DEBUG
	D3DXCreateFont( 
		GetDeviceClass()->GetDirect3DDevice(), 
		m_FontSize = FontSize, 
		0, 0, 0, 
		FALSE, 
		SHIFTJIS_CHARSET, 
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH, 
		FontName.c_str(), 
		&m_pD3DXFont);
#endif
}
void DebugPrint::Print(int PosX,int PosY, std::wstring Text){
#if _DEBUG
	RECT rect = { PosX, PosY, GetWindowClass()->GetClientWidth(), GetWindowClass()->GetClientHeight()};

	//テキストの描画
	m_pD3DXFont->DrawText( NULL, Text.c_str(), -1, &rect, DT_LEFT, m_FontColor );
#endif
}