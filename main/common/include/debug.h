#ifndef __DEBUG__H__
#define __DEBUG__H__
//#define DEBUG


#ifdef DEBUG
#define  TRACE_ENTER std::cout<<"ENTER: "<<__FUNCTION__<<"--"<<__LINE__<<std::endl
#define TRACE_EXIT std::cout<<"EXIT: "<<__FUNCTION__<<"-"<<__LINE__<<std::endl
#else
#define TRACE_ENTER
#define TRACE_EXIT
#endif



#endif
