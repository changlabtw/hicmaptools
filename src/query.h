#ifndef _QUERY_H
#define _QUERY_H

#include <vector>
#include "binmap.h"
#include "index.h"

using namespace std;

struct BINCONT
{
	string chrom;
	int pos1;
	int pos2;
	int cbin1;
	int cbin2;
	float obs;
	float exp;
<<<<<<< HEAD
    float nor;
    float rank_obs;
    float rank_exp;
    float rank_nor;
=======
	float nor;
	float rank_obs;
	float rank_exp;
	float rank_nor;
>>>>>>> 532c712dd8a398b4ba4a71506605874e6fcfd805
};

//===QUERY===
class QUERY
{
<<<<<<< HEAD
public:
	QUERY();
	QUERY(const char *fileName, BINMAP &binmap, INDEX &index , const int RANDOME_TEST_SIZE, const char *OutputName);
	~QUERY();
	void gen_random();
	void gen_rightdown();
	void output(const char *fileName);
protected:
	vector< BINCONT > bincont_vec;
	vector< BINCONT > random_bincont_vec;
	vector< BINCONT > rightdown_bincont_vec;		
=======
	public:
		QUERY();
		QUERY(const char *fileName, BINMAP &binmap, INDEX &index);
		~QUERY();
		void gen_random(BINMAP &binmap, INDEX &index, const int RANDOME_TEST_SIZE, const char *OutputfileName);
		void gen_rightdown();
		void output(const char *fileName);
	protected:
		vector< BINCONT > bincont_vec;
		vector< BINCONT > random_bincont_vec;
		vector< BINCONT > rightdown_bincont_vec;		
>>>>>>> 532c712dd8a398b4ba4a71506605874e6fcfd805
};

#endif /* _QUERY_H */
