#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>
#include <limits>

#include "query.h"
#include "index.h"

QUERY::QUERY()
{
}

// load map from binary file
QUERY::QUERY(const char *file_name, BINMAP &binmap, INDEX &index , const int RANDOME_TEST_SIZE, const char *OutputfileName)
{
	fstream input_f;
	string str;
	stringstream ss;	
	BINCONT tmp;

	// random
	float obs, exp;
	vector< pair<int, int> > random_bins (RANDOME_TEST_SIZE, make_pair(0,0));
	BINCONT run;
	int tmp_s, tmp_e;
	pair<int, int> r_tmp;

	// random test
	float test[RANDOME_TEST_SIZE][3];
	int outputcount=1;
	stringstream tt;
	string outputcount_str;

	input_f.open(file_name, ios_base::in);
	if(!input_f)
	{
		system("hostname");
		perror(file_name);
		exit(0);
	}
	else
	{
		cout << "\treading file =\t" << file_name << endl;
	}

	/*
	// handle for the first line which might contain header
	getline(input_f, str);
	if (((int)str.length() > 0) && std::isdigit(str[0])) 
	{
	ss.clear(); ss.str(str);
	ss >> tmp.chrom >> tmp.pos1 >> tmp.pos2;

	// find cbin for position		
	tmp.cbin1 = index.find_index(tmp.chrom, tmp.pos1, 1, 0);
	tmp.cbin2 = index.find_index(tmp.chrom, tmp.pos2, 0, 1);

	// get contact from bin
	if ((tmp.cbin1 != -1) && (tmp.cbin2 != -1)){ 
	tmp.obs = binmap.get_observe(tmp.cbin1, tmp.cbin2);
	tmp.exp = binmap.get_expect(tmp.cbin1, tmp.cbin2);
	}
	else{
	tmp.obs = tmp.exp = -1;
	}		

	bincont_vec.push_back(tmp);		
	}		
	 */
	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
			ss >> tmp.chrom >> tmp.pos1 >> tmp.pos2;

			// find cbin for position		
			tmp.cbin1 = index.find_index(tmp.chrom, tmp.pos1, 1, 0);
			tmp.cbin2 = index.find_index(tmp.chrom, tmp.pos2, 0, 1);

			// get contact from bin
			if ((tmp.cbin1 != -1) && (tmp.cbin2 != -1)){
				r_tmp = index.get_index_range(index.get_index(tmp_s).chr);

				tmp.obs = binmap.get_observe(tmp.cbin1, tmp.cbin2);
				tmp.exp = binmap.get_expect(tmp.cbin1, tmp.cbin2);
				tmp.nor = tmp.obs/(tmp.exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
			}
			else{
				tmp.obs = tmp.exp = -1;
			}	

			//bincont_vec.push_back(tmp);
		}

		// generate random bin pair for randomisation test
		index.gen_random_index(tmp.cbin1, tmp.cbin2, random_bins);

		for(int r = 0; r < RANDOME_TEST_SIZE; r++){
			run.obs = run.exp = run.nor = 0;
			// get the index range for the specified chrom: begin & end
			tmp_s = random_bins[r].first;
			tmp_e = random_bins[r].second;
			// if generate random index
			if ((tmp_s != 0) && (tmp_e != 0)){
				obs = binmap.get_observe(tmp_s, tmp_e);
				exp = binmap.get_expect(tmp_s, tmp_e);

				if ((obs != -1) && (exp != -1))
				{
					// add internal contact inf to interval
					run.obs = obs;
					run.exp = exp;
					run.nor = obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
				}
				random_bincont_vec.push_back(run);

			}

			test[r][0] = run.obs;
			test[r][1] = run.exp;
			test[r][2] = run.nor;

			if (run.obs > tmp.obs) tmp.rank_obs++;
			if (run.exp > tmp.exp) tmp.rank_exp++;
			if (run.nor > tmp.nor) tmp.rank_nor++;

#ifdef DEBUG
			cout << " normal " << r << "\t" << run.obs << "\t" << run.exp << "\t" << run.nor << endl;
#endif
		}

		tmp.rank_obs/=RANDOME_TEST_SIZE;
		tmp.rank_exp/=RANDOME_TEST_SIZE;
		tmp.rank_nor/=RANDOME_TEST_SIZE;

		bincont_vec.push_back(tmp);
		tmp.rank_obs = tmp.rank_exp = tmp.rank_nor =0;

		//random test
		string filename = (string)OutputfileName;
		int found = filename.find_last_of(".");
		tt << outputcount;
		tt >> outputcount_str;
		filename = filename.substr(0,found) + "_random_" + outputcount_str + ".txt";
		const char *filename_chr = filename.c_str();
		ofstream myfile(filename_chr);
		if (myfile.is_open())
		{
			myfile << "random_obs,";
			myfile << "random_exp,";
			myfile << "random_nor\n";
			for(int i = 0; i < RANDOME_TEST_SIZE; i ++){
				myfile << test[i][0] << ","<<test[i][1]<<","<<test[i][2]<<endl ;
			}
			myfile.close();
			outputcount++;
		}
		else cout << "Unable to open file";

	}

	input_f.close();
	input_f.clear();
}

QUERY::~QUERY()
{
}

void QUERY::output(const char *fileName)
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
	output_f << "chrom\tpos1\tpos2\tcbin1\tcbin2\tobser_contact\texpect_contact\t"
		<< "rank_obs\trank_exp\trank_nor"<<endl;

	for(vector<BINCONT>::iterator iter = bincont_vec.begin(); iter != bincont_vec.end(); iter++)
	{
		output_f << iter->chrom << "\t" << iter->pos1 << "\t" << iter->pos2
			<< "\t" << iter->cbin1 << "\t" << iter->cbin2 
			<< "\t" << iter->obs << "\t" << iter->exp 
			<< "\t" << iter->rank_obs << "\t" <<iter->rank_exp << "\t" << iter->rank_nor <<endl;
	}
	output_f.close();
}
