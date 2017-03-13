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

#ifndef ______DEVICE_CALSS_H_INCLUDE_____
#define ______DEVICE_CALSS_H_INCLUDE_____

//=== �C���N���[�h ===
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include<d3dx9.h>
#include<d3d9.h>

#include"CSmartPointer.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�f�o�C�X�N���X
class CDevice{
private:
	LPDIRECT3D9		m_pDirect3D;
	LPDIRECT3DDEVICE9	m_pDirect3DDevice;
	bool				m_bWindowMode;

public:
	//�f�X�g���N�^
	~CDevice();

	//�N���G�C�g�֐�
	static CSmartPointer<CDevice> GetSingletonPointer();

	//�������֐�
	HRESULT InitDirect3D(bool WindowMode = true);

	//��ʂ̃N���A
	void Clear();
	//��ʂ̕`��J�n
	HRESULT BeginScene();
	//��ʂ̕`��I���
	void EndScene();
	//Present
	void Present();

	//�f�o�C�X�̃Q�b�^�[
	LPDIRECT3DDEVICE9 GetDirect3DDevice()const{ return m_pDirect3DDevice; }
	
private:
	//�R���X�g���N�^
	CDevice();

	void InitRender();
};

//�f�o�C�X�N���X�̃V���O���g���̎擾
inline CSmartPointer<CDevice> GetDeviceClass(){
	return CDevice::GetSingletonPointer();
}

#endif
