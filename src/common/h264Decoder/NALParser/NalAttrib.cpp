#include "NalAttrib.h"
#include <boost/foreach.hpp>

namespace H264Parser {

NalAttrib::NalAttrib(std::string sName) : m_sName(sName)
{
}

NalAttrib::~NalAttrib() {
}

void NalAttrib::add(std::string str, int32_t numb)
{
	NalAttrib::ptr ptr(new NalAttrib(str));

	*ptr = numb;

	*this + ptr;
}

NalAttrib& NalAttrib::operator+(NalAttrib::ptr op)
{
	m_Leaves.push_back(op);

	return *this;
}

NalAttrib& NalAttrib::operator=(NalVarPtr var)
{
	m_Var = var;

	return *this;
}

NalAttrib& NalAttrib::operator=(int32_t value)
{
	m_Var = NalVarPtr(new NalVar(value));

	return *this;
}

NalVarPtr NalAttrib::operator[](std::string str)
{
	if (m_sName == str) {

		return m_Var;

	} else {

		NalVarPtr ptr;

		BOOST_FOREACH(NalAttrib::ptr leave, m_Leaves) {

			ptr = (*leave)[str];

			if (ptr != NULL)
				return ptr;
		}

		return ptr;
	}
}

};
