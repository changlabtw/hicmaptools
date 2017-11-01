#ifndef _BAT_H
#define _BAT_H

#include <vector>
#include "binmap.h"
#include "index.h"

using namespace std;

struct BINBAT
{
	int index;
	string chrom;
	int start;
	int end;
	int sbin;
	int ebin;
	int fordward_bin;
	int backward_bin;
// observe, expect, normalization for query BAT	
	int sum_bin;
	float sum_obs;
	float sum_exp;
	float sum_nor;
// observe, expect, normalization for random sample	
	float sum_rand_obs;
	float sum_rand_exp;
	float sum_rand_nor;
// the rank of observe, expect, normalization among random sampling, in top sense
	float rank_obs;
	float rank_exp;
	float rank_nor;			
};

//===BAT===
class BAT
{
public:
	BAT();
	BAT( const char *fileName, INDEX &index , int fordward_the, int backward_the);
	~BAT();
	void cal_contact(BINMAP &binmap, INDEX &index, int fordward_the, int backward_the, int random_size, const char *fileName);
	void output( const char *fileName );
	void output_pair(const char *fileName, BINMAP &binmap);
protected:
	vector< BINBAT > BINBAT_vec;
};

#endif /* _BAT_H */
