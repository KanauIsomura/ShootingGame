
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[DebugPrint.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2016-08-10
//�X�V��:2017-03-05
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______DEBUG_PRINT_CLASS_H_INCLUDE_____
#define ______DEBUG_PRINT_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<string>
#include<tchar.h>

#include"d3dx9.h"

class DebugPrint{
private:
	static LPD3DXFONT   m_pD3DXFont;
	static std::wstring m_FontName;
	static int          m_FontSize;
	static D3DXCOLOR    m_FontColor;

public:
	DebugPrint();
	~DebugPrint();

	static void SetFont(int FontSize = m_FontSize, std::wstring FontName = m_FontName);
	static void Print(int PosX,int PosY, std::wstring Text);
	static void SetfontColor(D3DXCOLOR Color){m_FontColor = Color;}
};

#endif
