//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[ObjectBase.h]
//�T�v	:���ׂẴQ�[���I�u�W�F�N�g�̃x�[�X�B
//		>��{�I�ɃQ�[���I�u�W�F�N�g�͂�����p�����Ďg���Ă����B
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-05-26
//�X�V��:2015-05-26
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______OBJECT_BASE_CLASS_H_INCLUDE_____
#define ______OBJECT_BASE_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#include<string>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//���ׂẴQ�[���I�u�W�F�N�g�̃x�[�X
class CObjectBase{
private:
	std::string m_ObjectName;

private:
	bool m_bDelete;

public:
	CObjectBase(std::string ObjectName);
	virtual ~CObjectBase();
	
	virtual void Initialize();
	virtual void Update();
	virtual void DrawNoAlpha();		//�s������
	virtual void DrawAlpha();		//������
	virtual void Finalize();

	std::string GetName(){return m_ObjectName;}
	void Delete(){m_bDelete = true;}
	bool GetDeleteFlag(){return m_bDelete;}

};

#endif
