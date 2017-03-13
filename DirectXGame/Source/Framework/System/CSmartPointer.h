//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[CSmartPointer.h]
//概要	:スマートポインタのテンプレートクラス
//		>登録されたオブジェクトポインタの使用者がいなくなったら自動的にdelete
//		>通常のポインタ操作演算子「*」「->」「[ ]」が使える。
//		>異なるオブジェクトポインタが登録された状態でアップキャストができる
//-----------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-08-01
//更新日:2015-08-01		更新内容:サンプルプログラムを見ながら作成
//		:2015-08-04				:ポインタの入れ替えのサポートを追加
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
	unsigned int		*m_pReferenceCounter;		//参照カウンタへのポインタ
	Template**			m_ppAnyPointer;				//T型のオブジェクトへのポインタ
	static Template*	m_NullPointer;				//NULLポインタ値

private:
	//参照カウンタ増加
	void AddReference(void){
		(*m_pReferenceCounter)++;
	}
	
	//参照カウンタ減少
	void Release(void){
		if(--(*m_pReferenceCounter) == 0){
			delete *m_ppAnyPointer;
			delete m_ppAnyPointer;
			delete m_pReferenceCounter;
		}
	}

public:
	//デフォルトコンストラクタ
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

	//コピーコンストラクタ(同型コピー)
	CSmartPointer(const CSmartPointer<Template> &Src){
		//相手のポインタをコピー
		m_pReferenceCounter = Src.m_pReferenceCounter;	//参照カウンタポインタ
		m_ppAnyPointer		= Src.m_ppAnyPointer;		//T型ダブルポインタ

		//自分の参照カウンタを増加
		AddReference();	
	}

	//コピーコンストラクタ（暗黙的アップキャスト）
	template<class Template2> CSmartPointer(CSmartPointer<Template2> &Src){
		//相手のダブルポインタをコピー
		m_pReferenceCounter = Src.GetReferencePointer();
		m_ppAnyPointer		= Src.GetDoublePointer();

		//型チェックコピー
		*m_ppAnyPointer = Src.GetPointer();

		//自分自身の参照カウンタを増やす
		AddReference();
	}

	//コピーコンストラクタ(NULL代入代用)
	CSmartPointer(const int Nullval){
		m_pReferenceCounter		= new unsigned int;
		*m_pReferenceCounter	= 0;
		m_ppAnyPointer			= new Template*;
		*m_ppAnyPointer			= m_NullPointer;
		AddReference();
	}
	
	//デストラクタ
	virtual ~CSmartPointer(void){
		Release();
	}

//_/_/_/演算子のオーバーロード_/_/_/
	
	// =代入演算子(明示的コピー)
	CSmartPointer<Template>& operator =(const CSmartPointer<Template> &Src){
		//自分自身への代入は不正なので行わない
		if(*Src.m_ppAnyPointer == *m_ppAnyPointer)
			return (*this);

		//他のアドレスに変わってしまうので参照カウンタを一つ減らす
		Release();

		//相手のポインタをコピー
		m_pReferenceCounter = Src.m_pReferenceCounter;
		m_ppAnyPointer		= Src.m_ppAnyPointer;

		//コピーしたポインタの参照カウンタを増やす
		AddReference();

		return (*this);
	}

	// =代入演算子(明示的アップキャスト)
	template<class Template2> CSmartPointer<Template>& operator =(CSmartPointer<Template2> &Src){
		//自分自身への代入は不正なので行わない
		if(Src.GetPointer() == *m_ppAnyPointer)
			return (*this);

		//他のアドレスに変わってしまうので参照カウンタを一つ減らす
		Release();

		//相手のポインタをコピー
		m_pReferenceCounter = Src.GetReferencePointer();
		m_ppAnyPointer		= Src.GetDoublePointer();

		//型チェックコピー
		*m_ppAnyPointer = Src.GetPointer();

		//コピーしたポインタの参照カウンタを増やす
		AddReference();

		return (*this);
	}

	// =代入演算子(NULL代入によるリセット)
	CSmartPointer<Template>& operator =(const int NullVal){
		Release();

		//新規に自分自身を作る
		m_pReferenceCounter = new unsigned int(1);
		m_ppAnyPointer		= new Template*;
		*m_ppAnyPointer		= m_NullPointer;

		return (*this);
	}

	// *間接演算子
	Template& operator *(void){
		return **m_ppAnyPointer;
	}

	//->メンバ選択演算子
	Template* operator ->(void){
		return *m_ppAnyPointer;
	}

	////[]配列参照演算子
	//Template& operator [](int Num){
	//	return m_ppAnyPointer[Num];
	//}

	//==比較演算子
	bool operator ==(Template *Val){
		if(*m_ppAnyPointer == Val)
			return true;
		return false;
	}

	//!=比較演算子
	bool operator !=(Template *pVal){
		if(*m_ppAnyPointer != pVal)
			return true;
		return false;
	}

//_/_/_/メンバ関数_/_/_/
public:
	//ポインタの明示的な登録
	void SetPointer(Template* Src = nullptr, int Add = 0){
		//参照カウンタを減らしたあとに再初期化
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

	//ポインタの貸し出し
	Template*  GetPointer(void){		return *m_ppAnyPointer;}
	Template** GetDoublePointer(void){	return m_ppAnyPointer;}

	//参照カウンタへのポインタを取得
	unsigned int* GetReferencePointer(void){
		return m_pReferenceCounter;
	}
	
	//ダウンキャストコピー
	template <class Template2> bool DownCast(CSmartPointer<Template2> &Src){
		//引数のスマートポインタの持つポインタが、
		//自分の登録しているポインタに
		//ダウンキャスト可能な場合はダウンキャストコピーを実行
		Template* castPointer = dynamic_cast<Template*>(Src.GetPointer());
		if(castPointer){// ダウンキャスト成功
			// 既存の参照カウンタを1つ減少
			Release();

			// 新しいポインタと参照カウンタを共有
			m_ppAnyPointer = (Template**)Src.GetDoublePointer();
			*m_ppAnyPointer = castPointer;
			m_pReferenceCounter = Src.GetReferencePointer();
			
			// 参照カウンタ増加
			AddReference();
			return true;
		}

		return false;
	}

	//ポインタの交換
	void SwapPointer(CSmartPointer<Template> &Src){
		Template* pWork		= Src.GetPointer();
		*Src.m_ppAnyPointer = *m_ppAnyPointer;	//ポインタの交換
		*m_ppAnyPointer		= pWork;
	}
};

template <class Template>
Template* CSmartPointer<Template>::m_NullPointer = nullptr;

#endif //_____CLASS_SMART_POINTER_H_INCLUDE_____