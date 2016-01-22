#ifndef NALATTRIB_H_
#define NALATTRIB_H_

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <vector>
#include "types.hpp"

namespace H264Parser {

#define getInt32(X, Y) boost::get<int32_t>((*(*X)[Y]))

class NalAttrib
	: boost::noncopyable{

public:
	NalAttrib(std::string sName);
	virtual ~NalAttrib();

	typedef boost::shared_ptr<NalAttrib> ptr;

	std::string m_sName;
	NalVarPtr m_Var;

	void add(std::string str, int32_t numb);

	std::vector<NalAttrib::ptr> m_Leaves;

	NalAttrib& operator+(NalAttrib::ptr op);
	NalAttrib& operator=(NalVarPtr);

	NalAttrib& operator=(int32_t value);

	NalVarPtr  operator[](std::string str);
};

}

#endif /* NALATTRIB_H_ */
