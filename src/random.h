#ifndef _RANDOM_H
#define _RANDOM_H

#include <iostream>
#include <fstream>
#include "binmap.h"
#include "index.h"

using namespace std;

struct RANDOM_E{
	float quer_obs=0;
	float quer_exp=0;
	float quer_nor=0;

	float rand_obs=0;
	float rand_exp=0;
	float rand_nor=0;

	float rank_obs=0;
	float rank_exp=0;
	float rank_nor=0;

	void out_header(ofstream &out_f){
		out_f	<< "pairs\t"
			<< "quer_obs\tquer_exp\tquer_nor\t" 
			<< "rand_obs\trand_exp\trand_nor\t"
			<< "divi_obs\tdivi_exp\tdivi_nor\t"
			<< "rank_obs\trank_exp\trank_nor";    	
	}

	void output(ofstream &out_f){
		// specify format for printing float point   
		out_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
		out_f.precision(3); // for fixed format, two decimal places    

		out_f	<< quer_obs << "\t" << quer_exp << "\t" << quer_nor << "\t"
			<< rand_obs << "\t" << rand_exp << "\t" << rand_nor << "\t"
			<< quer_obs/rand_obs << "\t" << quer_exp/rand_exp << "\t" << quer_nor/rand_nor << "\t" 
			<< rank_obs << "\t" << rank_exp << "\t" << rank_nor;
	}
};

void RANDOM(BINMAP &binmap, INDEX &index, int sbin, int ebin, int X, int Y, int RANDOME_TEST_SIZE, RANDOM_E &ran, int flag);

#endif
