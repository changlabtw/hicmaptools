#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>

#include "couple.h"
#include "index.h"

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
	// find cbin for position		
				tmp.sbin1 = index.find_index(tmp.chrom1, tmp.start1, 1, 0);
				tmp.ebin1 = index.find_index(tmp.chrom1, tmp.end1, 0, 1);

				tmp.sbin2 = index.find_index(tmp.chrom2, tmp.start2, 1, 0);
				tmp.ebin2 = index.find_index(tmp.chrom2, tmp.end2, 0, 1);
			
  				if ((tmp.sbin1 != -1) && (tmp.ebin1 != -1) && (tmp.sbin2 != -1) && (tmp.ebin2 != -1)){		
					COUPLE_vec.push_back(tmp);	
				}
				else{
					cerr << "no bins for " << tmp.chrom1 << "\t" << tmp.start1 << "\t" << tmp.end1 << "\t" 
										   << tmp.chrom2 << "\t" << tmp.start2 << "\t" << tmp.end2 << endl;
				}
			}
		}		
	}
		
	input_f.close();
	input_f.clear();
}

COUPLE::~COUPLE()
{
}

void COUPLE::output(const char *fileName, BINMAP &binmap)
{	
	int sum_bin;
	float sum_obs, sum_exp, sum_nor, obs, exp;

	ofstream output_f;
	output_f.open (fileName);
	
	if(!output_f)
	{
		system("hostname");
		perror(fileName);
		exit(0);
	}

	for(vector< COUPLE_E >::iterator iter = COUPLE_vec.begin(); iter != COUPLE_vec.end(); iter++)
	{
		sum_bin = 0;
		sum_obs = sum_exp = sum_nor = 0;

//generate output file	
		for(int m = iter->sbin1; m <= iter->ebin1; m++)
		{
			for(int n = iter->sbin2; n <= iter->ebin2; n++)
			{
				obs = binmap.get_observe(m, n);
				exp = binmap.get_expect(m, n);
				if ((obs != -1) && (exp != -1))
  				{
  					sum_obs += obs;
  					sum_exp += exp;
  					sum_nor += obs/exp;	
  					sum_bin++;
				}
			}
		}

// output result only there is bin count
		if(sum_bin > 0)
		{
			output_f << iter->chrom1 << "\t" << iter->start1 << "\t" << iter->end1 << "\t" << iter->sbin1 << "\t" << iter->ebin1 << "\t"
					 << iter->chrom2 << "\t" << iter->start2 << "\t" << iter->end2 << "\t" << iter->sbin2 << "\t" << iter->ebin2 << "\t"
					 << sum_bin << "\t" << sum_obs << "\t" << sum_exp << "\t" << sum_nor << endl;					
		}
	}
	
	output_f.close();
}
