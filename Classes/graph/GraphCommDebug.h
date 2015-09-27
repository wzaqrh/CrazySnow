#pragma once
#include "GraphCommDef.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

inline void debug_point2i(std::stringstream& ss, const Point2i& pt) {
	ss<<"("<<pt.x<<","<<pt.y<<")";
}
inline void debug_point2i(std::stringstream& ss, const Move2i& mv) {
	debug_point2i(ss, mv.src);
	ss<<"->";
	debug_point2i(ss, mv.dst);
}
template<class __cls__name>
void debug_point2i_seq(const std::string& name, const std::vector<__cls__name>& pt_seq)
{
	std::stringstream ss;  
	ss<<name<<std::endl;
	for (size_t i=0; i<pt_seq.size(); ++i)
	{
		debug_point2i(ss, pt_seq[i]);
		ss<<" ";
	}
	ss<<std::endl;
#ifdef WIN32
	OutputDebugStringA(ss.str().c_str());
#else
	std::cout<<ss.str()<<std::endl;
#endif
}