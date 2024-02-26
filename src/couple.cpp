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

	//checking empty file
	if(input_f.peek() == std::ifstream::traits_type::eof()){
		cout << "empty query file" << endl;
		exit(-1);
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

void COUPLE::cal_contact(BINMAP &binmap, INDEX &index, const int RANDOME_TEST_SIZE, const char *outputfileName)
{
	vector< pair<int, int> > random_bins;
	float run_obs, run_exp, run_nor, obs, exp;
	int tmp_s1, tmp_e1, tmp_s2, tmp_e2, dif1, dif2;
	pair<int, int> r_tmp;
		
// random test
    float** test;

 	//arg -random > 0
	if(RANDOME_TEST_SIZE > 0){
		random_bins = vector< pair<int, int> >(RANDOME_TEST_SIZE, make_pair(0,0));
		test = new float*[RANDOME_TEST_SIZE];
		for(int i=0; i<RANDOME_TEST_SIZE ;i++){
			test[i] = new float[3];
		}
	}
   int outputcount=1;
    
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
  						iter->cont.quer_obs += obs;
  						iter->cont.quer_exp += exp;
  						iter->cont.quer_nor += obs/(exp+std::numeric_limits<float>::epsilon());	
  						iter->cont.num_bins++;
					}
				}
			}
			
			/////////////////////////
			// perform ranom test
			////////////////////////
			if(RANDOME_TEST_SIZE > 0){
				// generate random bin pair for randomisation test
				index.gen_random_index(iter->sbin1, iter->ebin2, random_bins);

				for(int r = 0; r < RANDOME_TEST_SIZE; r++){
					run_obs = run_exp = run_nor = 0;
					tmp_s1 = random_bins[r].first;
					tmp_e2 = random_bins[r].second;

					// if generate random index				
					if ((tmp_s1 != 0) && (tmp_e2 != 0)){
						// get the index range for the specified chrom: begin & end
						r_tmp = index.get_index_range(index.get_index(tmp_s1).chr);

						tmp_e1 = (tmp_s1+dif1 < r_tmp.second) ? tmp_s1+dif1  : r_tmp.second;
						tmp_s2 = (tmp_e2-dif2 > r_tmp.first)  ? tmp_e2-dif2  : r_tmp.first;

						for(int i=tmp_s1; i<=tmp_e1; i++)
						{
							for(int j=tmp_s2; j<=tmp_e2; j++)
							{
								obs = binmap.get_observe(i, j);
								exp = binmap.get_expect(i, j);

								if ((obs != -1) && (exp != -1))
								{
									iter->cont.rand_obs += obs;
									iter->cont.rand_exp += exp;
									iter->cont.rand_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
									run_obs += obs;
									run_exp += exp; 
									run_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
								}
							}
						}

						test[r][0] = run_obs;
						test[r][1] = run_exp;
						test[r][2] = run_nor;

						if (run_obs > iter->cont.quer_obs) iter->cont.rank_obs++;
						if (run_exp > iter->cont.quer_exp) iter->cont.rank_exp++;
						if (run_nor > iter->cont.quer_nor) iter->cont.rank_nor++;
#ifdef DEBUG				
						cout << " normal " << r << "\t" << run_obs << "\t" << run_exp << "\t" << run_nor << endl;
#endif
					}
				}

				iter->cont.rand_obs /= RANDOME_TEST_SIZE;
				iter->cont.rand_exp /= RANDOME_TEST_SIZE;
				iter->cont.rand_nor /= RANDOME_TEST_SIZE;

				string filename = (string)outputfileName;
				int found = filename.find_last_of(".");
				filename = filename.substr(0,found) + "_random_" + to_string(outputcount) + ".txt";
				ofstream myfile(filename);
				if (myfile.is_open())
				{
					myfile << "random_obs," << "random_exp," << "random_nor\n";
					myfile << iter->cont.quer_obs << "," << iter->cont.quer_exp << "," << iter->cont.quer_nor << endl;
					for(int i = 0; i < RANDOME_TEST_SIZE; i ++){
						myfile << test[i][0] << ","<<test[i][1]<<","<<test[i][2]<<endl ;
					}
					myfile.close();
					outputcount++;
				}
				else cout << "Unable to open file";
				
#ifdef DEBUG						 
				cout << "random end" << endl;
#endif
			}
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
	output_f << "chrom1\tstart1\tend1\t"
			 << "chrom2\tstart2\tend2\t";
	COUPLE_vec.front().cont.out_header(output_f);		 
	output_f << endl;					

	for(vector< COUPLE_E >::iterator iter = COUPLE_vec.begin(); iter != COUPLE_vec.end(); iter++)
	{		
		output_f << iter->chrom1 << "\t" << iter->start1 << "\t" << iter->end1 << "\t"
				 << iter->chrom2 << "\t" << iter->start2 << "\t" << iter->end2 << "\t";
		iter->cont.output(output_f);		 
		output_f << endl;					
	}
	
	output_f.close();
}
