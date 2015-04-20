#ifndef _COUPLE_H
#define _COUPLE_H

#include <vector>
#include "binmap.h"
#include "index.h"

using namespace std;

struct COUPLE_E
{
	string chrom1;
	int start1;
	int end1;
	int sbin1;
	int ebin1;
	string chrom2;
	int start2;
	int end2;
	int sbin2;
	int ebin2;	
};

//===COUPLE===
class COUPLE
{
public:
	COUPLE();
	COUPLE(const char *fileName, INDEX &index);
	~COUPLE();
	void output(const char *fileName, BINMAP &binmap);
protected:
	vector< COUPLE_E > COUPLE_vec;	
};

#endif /* _COUPLE_H */
