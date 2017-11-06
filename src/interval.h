#ifndef _INTERVAL_H
#define _INTERVAL_H

#include <vector>
#include "binmap.h"
#include "index.h"

using namespace std;

struct BININTERVAL
{
	string chrom;
	int index;
	int start;
	int end;
	int sbin;
	int ebin;
	float obs;
	float exp;
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

//===INTERVAL===
class INTERVAL
{
	public:
		INTERVAL();
		INTERVAL( const char *fileName, BINMAP &binmap, INDEX &index );
		~INTERVAL();
		void gen_internal_contact( BINMAP &binmap, INDEX &index , const int RANDOME_TEST_SIZE, const char *fileName );
		void output(const char *fileName, bool show_average);
		void output_internal(const char *fileName);
	protected:
		vector< BININTERVAL > BININTERVAL_vec;
		vector< BININTERVAL > internal_BININTERVAL_vec;
		vector< BININTERVAL > rightdown_BININTERVAL_vec;		
};

#endif /* _INTERVAL_H */
