#pragma once

template<class _Class>
class single_base
{
    typedef _Class class_name;
protected:
	single_base(){}
	~single_base(){};
public:
	static class_name* Inst() {
		static class_name* m_pInst = NULL;
		if (m_pInst == nullptr) {
			m_pInst = new class_name;
		}
		return m_pInst;
	}
};


template<class _Class, class _BaseClass>
class singleton
    :public _BaseClass
{
    typedef _Class class_name;
protected:
	singleton(){}
	~singleton(){};
public:
	static class_name* Inst() {
		static class_name* m_pInst = NULL;
		if (m_pInst == nullptr) {
			m_pInst = new class_name;
		}
		return m_pInst;
	}
};

