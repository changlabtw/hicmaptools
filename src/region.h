#ifndef _REGION_H
#define _REGION_H

#include <vector>
#include "binmap.h"
#include "index.h"

using namespace std;

struct REGION_E
{
	string chrom;
	string name;
	int start;
	int end;
	int sbin;
	int ebin;
};

//===REGION===
class REGION
{
public:
	REGION();
	REGION(const char *fileName, INDEX &index);
	~REGION();
	void output(const char *fileName, BINMAP &binmap);
protected:
	vector< REGION_E > region_vec;	
};

#endif /* _REGION_H */
