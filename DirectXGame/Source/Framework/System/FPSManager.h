//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
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

#ifndef ______FPS_MANAGER_CLASS_H_INCLUDE_____
#define ______FPS_MANAGER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#pragma comment(lib,"winmm.lib")
#include<Windows.h>
#include<MMSystem.h>

#include<string>
#include<tchar.h>

#include"DebugPrint.h"

//---�萔��`---
const int DISP_INTERVAL = 1000;

class FPSManager{
private:
	DWORD	m_ExecTime;
	DWORD	m_DispFPSTime;
	DWORD	m_CurrentTime;
	DWORD	m_FreameCount;
	DWORD	m_Freame;
	float	m_RATE;

public:
	FPSManager(float RATE = 60.0f):
		m_ExecTime(0),
		m_DispFPSTime(0),
		m_CurrentTime(0),
		m_FreameCount(0),
		m_Freame(0){
		m_RATE = 1000.0f / RATE;
		timeBeginPeriod(1);
		m_ExecTime = m_DispFPSTime = timeGetTime();
	}
		
	~FPSManager(){
		timeEndPeriod(1);
	}

	bool CheckExec(){
		m_CurrentTime = timeGetTime();
		if(m_CurrentTime - m_DispFPSTime >= DISP_INTERVAL){
			m_DispFPSTime	= m_CurrentTime;
			m_Freame = m_FreameCount;
			m_FreameCount	= 0;
		}

		if(m_CurrentTime - m_ExecTime >= m_RATE){
			m_ExecTime = m_CurrentTime;
			m_FreameCount ++;
			return true;
		}
		else
			return false;
	}

	void Draw(){
		wchar_t wStr[256];
		std::swprintf(wStr, 256, _T("FPS:%d"), m_Freame);

		DebugPrint::Print( 0, 0, wStr);
	}
};

#endif
