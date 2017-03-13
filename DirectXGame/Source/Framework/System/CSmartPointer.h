//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CSmartPointer.h]
//�T�v	:�X�}�[�g�|�C���^�̃e���v���[�g�N���X
//		>�o�^���ꂽ�I�u�W�F�N�g�|�C���^�̎g�p�҂����Ȃ��Ȃ����玩���I��delete
//		>�ʏ�̃|�C���^���쉉�Z�q�u*�v�u->�v�u[ ]�v���g����B
//		>�قȂ�I�u�W�F�N�g�|�C���^���o�^���ꂽ��ԂŃA�b�v�L���X�g���ł���
//-----------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-08-01
//�X�V��:2015-08-01		�X�V���e:�T���v���v���O���������Ȃ���쐬
//		:2015-08-04				:�|�C���^�̓���ւ��̃T�|�[�g��ǉ�
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef _____CLASS_SMART_POINTER_H_INCLUDE_____
#define _____CLASS_SMART_POINTER_H_INCLUDE_____

#include<Windows.h>

template <class Template>
class CSmartPointer
{
private:
	unsigned int		*m_pReferenceCounter;		//�Q�ƃJ�E���^�ւ̃|�C���^
	Template**			m_ppAnyPointer;				//T�^�̃I�u�W�F�N�g�ւ̃|�C���^
	static Template*	m_NullPointer;				//NULL�|�C���^�l

private:
	//�Q�ƃJ�E���^����
	void AddReference(void){
		(*m_pReferenceCounter)++;
	}
	
	//�Q�ƃJ�E���^����
	void Release(void){
		if(--(*m_pReferenceCounter) == 0){
			delete *m_ppAnyPointer;
			delete m_ppAnyPointer;
			delete m_pReferenceCounter;
		}
	}

public:
	//�f�t�H���g�R���X�g���N�^
	explicit CSmartPointer(Template* Src = nullptr, int Add = 0){
		m_pReferenceCounter	= new unsigned int;
		*m_pReferenceCounter	= Add;
		m_ppAnyPointer		= new Template*;
		m_NullPointer			= nullptr;
		if(Src)
			*m_ppAnyPointer = Src;
		else
			*m_ppAnyPointer = m_NullPointer;

		AddReference();
	}

	//�R�s�[�R���X�g���N�^(���^�R�s�[)
	CSmartPointer(const CSmartPointer<Template> &Src){
		//����̃|�C���^���R�s�[
		m_pReferenceCounter = Src.m_pReferenceCounter;	//�Q�ƃJ�E���^�|�C���^
		m_ppAnyPointer		= Src.m_ppAnyPointer;		//T�^�_�u���|�C���^

		//�����̎Q�ƃJ�E���^�𑝉�
		AddReference();	
	}

	//�R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�j
	template<class Template2> CSmartPointer(CSmartPointer<Template2> &Src){
		//����̃_�u���|�C���^���R�s�[
		m_pReferenceCounter = Src.GetReferencePointer();
		m_ppAnyPointer		= Src.GetDoublePointer();

		//�^�`�F�b�N�R�s�[
		*m_ppAnyPointer = Src.GetPointer();

		//�������g�̎Q�ƃJ�E���^�𑝂₷
		AddReference();
	}

	//�R�s�[�R���X�g���N�^(NULL�����p)
	CSmartPointer(const int Nullval){
		m_pReferenceCounter		= new unsigned int;
		*m_pReferenceCounter	= 0;
		m_ppAnyPointer			= new Template*;
		*m_ppAnyPointer			= m_NullPointer;
		AddReference();
	}
	
	//�f�X�g���N�^
	virtual ~CSmartPointer(void){
		Release();
	}

//_/_/_/���Z�q�̃I�[�o�[���[�h_/_/_/
	
	// =������Z�q(�����I�R�s�[)
	CSmartPointer<Template>& operator =(const CSmartPointer<Template> &Src){
		//�������g�ւ̑���͕s���Ȃ̂ōs��Ȃ�
		if(*Src.m_ppAnyPointer == *m_ppAnyPointer)
			return (*this);

		//���̃A�h���X�ɕς���Ă��܂��̂ŎQ�ƃJ�E���^������炷
		Release();

		//����̃|�C���^���R�s�[
		m_pReferenceCounter = Src.m_pReferenceCounter;
		m_ppAnyPointer		= Src.m_ppAnyPointer;

		//�R�s�[�����|�C���^�̎Q�ƃJ�E���^�𑝂₷
		AddReference();

		return (*this);
	}

	// =������Z�q(�����I�A�b�v�L���X�g)
	template<class Template2> CSmartPointer<Template>& operator =(CSmartPointer<Template2> &Src){
		//�������g�ւ̑���͕s���Ȃ̂ōs��Ȃ�
		if(Src.GetPointer() == *m_ppAnyPointer)
			return (*this);

		//���̃A�h���X�ɕς���Ă��܂��̂ŎQ�ƃJ�E���^������炷
		Release();

		//����̃|�C���^���R�s�[
		m_pReferenceCounter = Src.GetReferencePointer();
		m_ppAnyPointer		= Src.GetDoublePointer();

		//�^�`�F�b�N�R�s�[
		*m_ppAnyPointer = Src.GetPointer();

		//�R�s�[�����|�C���^�̎Q�ƃJ�E���^�𑝂₷
		AddReference();

		return (*this);
	}

	// =������Z�q(NULL����ɂ�郊�Z�b�g)
	CSmartPointer<Template>& operator =(const int NullVal){
		Release();

		//�V�K�Ɏ������g�����
		m_pReferenceCounter = new unsigned int(1);
		m_ppAnyPointer		= new Template*;
		*m_ppAnyPointer		= m_NullPointer;

		return (*this);
	}

	// *�Ԑډ��Z�q
	Template& operator *(void){
		return **m_ppAnyPointer;
	}

	//->�����o�I�����Z�q
	Template* operator ->(void){
		return *m_ppAnyPointer;
	}

	////[]�z��Q�Ɖ��Z�q
	//Template& operator [](int Num){
	//	return m_ppAnyPointer[Num];
	//}

	//==��r���Z�q
	bool operator ==(Template *Val){
		if(*m_ppAnyPointer == Val)
			return true;
		return false;
	}

	//!=��r���Z�q
	bool operator !=(Template *pVal){
		if(*m_ppAnyPointer != pVal)
			return true;
		return false;
	}

//_/_/_/�����o�֐�_/_/_/
public:
	//�|�C���^�̖����I�ȓo�^
	void SetPointer(Template* Src = nullptr, int Add = 0){
		//�Q�ƃJ�E���^�����炵�����Ƃɍď�����
		Release();
		m_pReferenceCounter		= new unsigned int;
		*m_pReferenceCounter	= Add;
		m_ppAnyPointer			= new Template*;
		if(Src)
			*m_ppAnyPointer	= Src;
		else
			*m_ppAnyPointer = m_NullPointer;
		AddReference();
	}

	//�|�C���^�݂̑��o��
	Template*  GetPointer(void){		return *m_ppAnyPointer;}
	Template** GetDoublePointer(void){	return m_ppAnyPointer;}

	//�Q�ƃJ�E���^�ւ̃|�C���^���擾
	unsigned int* GetReferencePointer(void){
		return m_pReferenceCounter;
	}
	
	//�_�E���L���X�g�R�s�[
	template <class Template2> bool DownCast(CSmartPointer<Template2> &Src){
		//�����̃X�}�[�g�|�C���^�̎��|�C���^���A
		//�����̓o�^���Ă���|�C���^��
		//�_�E���L���X�g�\�ȏꍇ�̓_�E���L���X�g�R�s�[�����s
		Template* castPointer = dynamic_cast<Template*>(Src.GetPointer());
		if(castPointer){// �_�E���L���X�g����
			// �����̎Q�ƃJ�E���^��1����
			Release();

			// �V�����|�C���^�ƎQ�ƃJ�E���^�����L
			m_ppAnyPointer = (Template**)Src.GetDoublePointer();
			*m_ppAnyPointer = castPointer;
			m_pReferenceCounter = Src.GetReferencePointer();
			
			// �Q�ƃJ�E���^����
			AddReference();
			return true;
		}

		return false;
	}

	//�|�C���^�̌���
	void SwapPointer(CSmartPointer<Template> &Src){
		Template* pWork		= Src.GetPointer();
		*Src.m_ppAnyPointer = *m_ppAnyPointer;	//�|�C���^�̌���
		*m_ppAnyPointer		= pWork;
	}
};

template <class Template>
Template* CSmartPointer<Template>::m_NullPointer = nullptr;

#endif //_____CLASS_SMART_POINTER_H_INCLUDE_____