// TODPO
// might remove check header part

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>
#include <limits>

#include "bat.h"
#include "index.h"
#include "random.h"

BAT::BAT()
{
}

// read BAT file from text
BAT::BAT(const char *file_name, INDEX &index, const int fordward_the, const int backward_the)
{
	fstream input_f;
	string str;
	stringstream ss;	
	BINBAT tmp;
	pair<int, int> r_tmp;
	int count = 1;

	// initialisation
	tmp.sum_obs = tmp.sum_exp = tmp.sum_nor = tmp.sum_bin = tmp.sum_rand_obs = tmp.sum_rand_exp = tmp.sum_rand_nor = 0;
	tmp.sbin = tmp.ebin = tmp.fordward_bin = tmp.backward_bin = -1;
	tmp.rank_obs = tmp.rank_exp = tmp.rank_nor = 0;

	input_f.open(file_name, ios_base::in);
	if(!input_f)
	{
		system("hostname");
		perror(file_name);
		exit(0);
	}
	else
	{
		cout << "\treading BAT file =\t" << file_name << endl;
	}

	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
			// handle for the first line which might contain header	
			if(	ss >> tmp.chrom >> tmp.start >> tmp.end )
			{
				// remove chr from chrom name
				size_t pos = tmp.chrom.find( "chr" );
				if ( pos != string::npos ) {
					tmp.chrom.replace( pos, 3, "" );   // 3 = length( chr )
				}

				tmp.index = count;
				// find cbin for position		
				tmp.sbin = index.find_index(tmp.chrom, tmp.start, 1, 0);
				tmp.ebin = index.find_index(tmp.chrom, tmp.end, 0, 1);				

				// inset only find corresponding bins
				if ((tmp.sbin != -1) && (tmp.ebin != -1))
				{
					// get the index range for the specified chrom: begin & end
					r_tmp = index.get_index_range(tmp.chrom);

					tmp.fordward_bin = (tmp.sbin-fordward_the > r_tmp.first)  ? tmp.sbin-fordward_the : r_tmp.first;
					tmp.backward_bin = (tmp.ebin+backward_the < r_tmp.second) ? tmp.ebin+backward_the : r_tmp.second;
					BINBAT_vec.push_back(tmp);
				}
				count++;
			}
		}		
	}

	input_f.close();
	input_f.clear();

	cout << "\t\t# of BATs =\t" << (int)BINBAT_vec.size() << endl << endl;
}

BAT::~BAT()
{
}

void BAT::cal_contact(BINMAP &binmap, INDEX &index, int fordward_the, int backward_the, int RANDOME_TEST_SIZE)
{
	// initialisation
	float obs, exp;

	//  loop for all bats	
	for(vector<BINBAT>::iterator iter = BINBAT_vec.begin(); iter != BINBAT_vec.end(); iter++)
	{
		// loop for all internal contacts inside each bat
		for(int i=iter->sbin; i<=iter->ebin; i++)
		{
			for(int j=iter->fordward_bin; j<=iter->backward_bin; j++)
			{
				// check pair contact for sbin
				obs = binmap.get_observe(i, j);
				exp = binmap.get_expect(i, j);

				if ((obs != -1) && (exp != -1))
				{
					iter->sum_obs += obs;
					iter->sum_exp += exp;
					iter->sum_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
					iter->sum_bin++;  					
				}
			}		
		}
		iter->ran.quer_obs=iter->sum_obs;
		iter->ran.quer_exp=iter->sum_exp;
		iter->ran.quer_nor=iter->sum_nor;
		RANDOM(binmap,index,iter->sbin,iter->ebin,fordward_the,backward_the,RANDOME_TEST_SIZE,iter->ran,1);
	}
}

// output function
void BAT::output(const char *fileName)
{
	ofstream output_f;
	output_f.open (fileName);

	if(!output_f)
	{
		system("hostname");
		perror(fileName);
		exit(0);
	}

	// print header 
	output_f << "index\tchrom\tstart\tend\tsbin\tebin\tsum_bin\t"
		<< "sum_obs\tsum_exp\tsum_nor\t" 
		<< "rand_obs\trand_exp\trand_nor\t"
		<< "divide_obs\tdivide_exp\tdivide_nor\t"
		<< "rank_obs\trank_exp\trank_nor\t"	                                                       
		<< endl;

	for(vector<BINBAT>::iterator iter = BINBAT_vec.begin(); iter != BINBAT_vec.end(); iter++)
	{
		output_f << iter->index << "\t" << iter->chrom << "\t" 
			<< iter->start << "\t" << iter->end << "\t" 
			<< iter->sbin << "\t" << iter->ebin << "\t"
			<< iter->sum_bin << "\t";

		output_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
		output_f.precision(3); // for fixed format, two decimal places    

		output_f << iter->sum_obs << "\t" << iter->sum_exp << "\t" << iter->sum_nor << "\t"
			<< iter->ran.rand_obs << "\t" << iter->ran.rand_exp << "\t" << iter->ran.rand_nor << "\t"
			<< iter->sum_obs/iter->ran.rand_obs << "\t" << iter->sum_exp/iter->ran.rand_exp << "\t" << iter->sum_nor/iter->ran.rand_nor << "\t" 
			<< iter->ran.rank_obs << "\t" << iter->ran.rank_exp << "\t" << iter->ran.rank_nor << "\t" 		         
			<< endl;
	}

	output_f.close();	
}

void BAT::output_pair(const char *fileName, BINMAP &binmap)
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

	// print header
	output_f << "index1\tchrom1\tstart1\tend1\tsbin1\tebin1\t" 
		<< "index2\tchrom2\tstart2\tend2\tsbin2\tebin2\t" 
		<< "sum_bin\tsum_obs\tsum_exp\tsum_nor" << endl;

	for(vector<BINBAT>::iterator i = BINBAT_vec.begin(); i != BINBAT_vec.end(); i++)
	{
		for(vector<BINBAT>::iterator j = i+1; j != BINBAT_vec.end(); j++)
		{
			// initial
			sum_bin = 0;
			sum_obs = sum_exp = sum_nor = 0;

			for(int m = i->sbin; m <= i->ebin; m++)
			{
				for(int n = j->sbin; n <= j->ebin; n++)
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
				output_f << i->index << "\t" << i->chrom << "\t" << i->start << "\t" << i->end << "\t" << i->sbin << "\t" << i->ebin << "\t" 
					<< j->index << "\t" << j->chrom << "\t" << j->start << "\t" << j->end << "\t" << j->sbin << "\t" << j->ebin << "\t" 
					<< sum_bin << "\t" << sum_obs << "\t" << sum_exp << "\t" << sum_nor << endl;					
			}
		}
	}	

	output_f.close();	
}
