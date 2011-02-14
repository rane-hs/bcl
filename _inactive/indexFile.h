#include "bcl.h"
#include <string>

M__BEGIN_NAMESPACE_BCL
	
//local structure
struct index_T{
	std::string idxPath;
	short idx;
	index_T(const char *fpath = NULL){if(fpath != NULL)idxPath = fpath;};
	operator++(int);
	int GetIdx(){return idx;};
//	operator=(const short new_idx);
	operator int(){return (int)idx;};
	int ReadIdx();
	int WriteIdx();
	bool Check0();
	bool Check1();
};

M__END_NAMESPACE_BCL