#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>
#include <limits>

#include "couple.h"
#include "index.h"
#include "random.h"

COUPLE::COUPLE()
{
}

// load map from binary file
COUPLE::COUPLE(const char *file_name, INDEX &index)
{
	fstream input_f;
	string str;
	stringstream ss;	
	COUPLE_E tmp;
	int tmp_i;
	
	input_f.open(file_name, ios_base::in);
	if(!input_f){
		system("hostname");
		perror(file_name);
		exit(0);
	}
	else{
		cout << "\treading file =\t" << file_name << endl;
	}
	
	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
			if ( ss >> tmp.chrom1 >> tmp.start1 >> tmp.end1 >> tmp.chrom2 >> tmp.start2 >> tmp.end2)
			{	
			
// for chrom1 == chrom2, keep start1 & end1 be smaller position 					
				if((tmp.chrom1 == tmp.chrom2)&&(tmp.start1 > tmp.start2))
				{
					tmp_i = tmp.start1;
					tmp.start1 = tmp.start2;
					tmp.start2 = tmp_i;

					tmp_i = tmp.end1;
					tmp.end1 = tmp.end2;
					tmp.end2 = tmp_i;					
				}
				
// find cbin for position		
				tmp.sbin1 = index.find_index(tmp.chrom1, tmp.start1, 1, 0);
				tmp.ebin1 = index.find_index(tmp.chrom1, tmp.end1, 0, 1);

				tmp.sbin2 = index.find_index(tmp.chrom2, tmp.start2, 1, 0);
				tmp.ebin2 = index.find_index(tmp.chrom2, tmp.end2, 0, 1);
			
				COUPLE_vec.push_back(tmp);	
			}
		}		
	}
		
	input_f.close();
	input_f.clear();
}

COUPLE::~COUPLE()
{
}

void COUPLE::cal_contact(BINMAP &binmap, INDEX &index, const int RANDOME_TEST_SIZE)
{
	float obs, exp;
	int dif1, dif2;
		
	for(vector< COUPLE_E >::iterator iter = COUPLE_vec.begin(); iter != COUPLE_vec.end(); iter++)
	{
		if ((iter->sbin1 != -1) && (iter->ebin1 != -1) && (iter->sbin2 != -1) && (iter->ebin2 != -1))
		{	
			dif1 = iter->ebin1 - iter->sbin1;
			dif2 = iter->ebin2 - iter->sbin2;
								
// loop for all pair contacts between two sites		
			for(int m = iter->sbin1; m <= iter->ebin1; m++)
			{
				for(int n = iter->sbin2; n <= iter->ebin2; n++)
				{
					obs = binmap.get_observe(m, n);
					exp = binmap.get_expect(m, n);
					if ((obs != -1) && (exp != -1))
  					{
  						iter->ran.quer_obs += obs;
  						iter->ran.quer_exp += exp;
  						iter->ran.quer_nor += obs/(exp+std::numeric_limits<float>::epsilon());	
  						iter->num_bins++;
					}
				}
			}
			RANDOM(binmap,index,iter->sbin1,iter->ebin2,dif2,dif1,RANDOME_TEST_SIZE,iter->ran,2);
		}				
	}		
}

void COUPLE::output(const char *fileName)
{	
	ofstream output_f;
	output_f.open (fileName);
	
	if(!output_f)
	{
		system("hostname");
		perror(fileName);
		exit(0);
	}
	
//output header	
	output_f << "chrom1\tstart1\tend1\tsbin1\tebin1\t"
			 << "chrom2\tstart2\tend2\tsbin2\tebin2\t";
	COUPLE_vec.front().ran.out_header(output_f);		 
	output_f << endl;					

	for(vector< COUPLE_E >::iterator iter = COUPLE_vec.begin(); iter != COUPLE_vec.end(); iter++)
	{		
		output_f << iter->chrom1 << "\t" << iter->start1 << "\t" << iter->end1 << "\t" << iter->sbin1 << "\t" << iter->ebin1 << "\t"
				 << iter->chrom2 << "\t" << iter->start2 << "\t" << iter->end2 << "\t" << iter->sbin2 << "\t" << iter->ebin2 << "\t"<<iter->num_bins<<"\t";
		iter->ran.output(output_f);		 
		output_f << endl;					
	}
	
	output_f.close();
}
