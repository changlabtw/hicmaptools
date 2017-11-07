#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>
#include <limits>

#include "interval.h"
#include "index.h"

INTERVAL::INTERVAL()
{
}

// load map from binary file
INTERVAL::INTERVAL(const char *file_name, BINMAP &binmap, INDEX &index)
{
	fstream input_f;
	string str;
	stringstream ss;	
	BININTERVAL tmp;

	// initialisation
	tmp.sum_obs = tmp.sum_exp = tmp.sum_bin = 0;	
	int count = 1;

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

	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			tmp.index = count;
			ss.clear(); ss.str(str);
			// handle for the first line which might contain header	
			if(	ss >> tmp.chrom >> tmp.start >> tmp.end )
			{
				// remove chr from chrom name
				size_t pos = tmp.chrom.find( "chr" );
				if ( pos != string::npos ) {
					tmp.chrom.replace( pos, 3, "" );   // 3 = length( chr )
				}

				// find cbin for position		
				tmp.sbin = index.find_index(tmp.chrom, tmp.start, 1, 0);
				tmp.ebin = index.find_index(tmp.chrom, tmp.end, 0, 1);

				// get contact from bin
				if ((tmp.sbin != -1) && (tmp.ebin != -1)){
					tmp.obs = binmap.get_observe(tmp.sbin, tmp.ebin);
					tmp.exp = binmap.get_expect(tmp.sbin, tmp.ebin);
				}
				else{
					tmp.obs = tmp.exp = -1;
				}	

				BININTERVAL_vec.push_back(tmp);
				count++;
			}
		}		
	}

	input_f.close();
	input_f.clear();
}

INTERVAL::~INTERVAL()
{
}

struct INTER_ELE
{
	int count;
	float sum_obs;
	float sum_exp;
	float sum_nor;
};

void INTERVAL::gen_internal_contact(BINMAP &binmap, INDEX &index, const int RANDOME_TEST_SIZE, const char *OutputfileName)
{
	// initialization
	BININTERVAL tmp;
	tmp.start = tmp.end = -1;
	map <int, INTER_ELE > ave_map;
	INTER_ELE tmp_ele;
	tmp_ele.count = tmp_ele.sum_obs = tmp_ele.sum_exp = 0;
	// random
	float obs, exp;
	vector< pair<int, int> > random_bins (RANDOME_TEST_SIZE, make_pair(0,0));
	float run_obs, run_exp, run_nor;
	pair<int, int> r_tmp;
	int tmp_s, tmp_e;
	// random test
	float test[RANDOME_TEST_SIZE][3];
	int outputcount=1;
	stringstream ss;
	string outputcount_str;


	//  loop for all intervals	
	for(vector<BININTERVAL>::iterator iter = BININTERVAL_vec.begin(); iter != BININTERVAL_vec.end(); iter++)
	{
		// check bin & contact exists	
		if((iter->sbin != -1) & (iter->ebin != -1) & (iter->obs != -1) & (iter->exp != -1))
		{
			// loop for all internal contacts inside each interval
			for(int i=iter->sbin; i<=iter->ebin; i++)
			{
				for(int j=i; j<=iter->ebin; j++)
				{
					tmp.index = iter->index;
					tmp.chrom = iter->chrom;
					tmp.sbin = i;
					tmp.ebin = j;
					tmp.obs = binmap.get_observe(tmp.sbin, tmp.ebin);
					tmp.exp = binmap.get_expect(tmp.sbin, tmp.ebin);

					if ((tmp.obs != -1) & (tmp.exp != -1))
					{
						// add internal contact inf to interval  					
						internal_BININTERVAL_vec.push_back(tmp);
						iter->sum_obs += tmp.obs;
						iter->sum_exp += tmp.exp;
						iter->sum_nor += tmp.obs/(tmp.exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
						iter->sum_bin++;

						// add internal contact inf to its bin_size  						
						if(ave_map.find(tmp.ebin-tmp.sbin) == ave_map.end()){
							ave_map.insert(make_pair(tmp.ebin-tmp.sbin, tmp_ele));
						}
						ave_map[tmp.ebin-tmp.sbin].count++;
						ave_map[tmp.ebin-tmp.sbin].sum_obs+=tmp.obs;
						ave_map[tmp.ebin-tmp.sbin].sum_exp+=tmp.exp;
						ave_map[tmp.ebin-tmp.sbin].sum_nor+=tmp.obs/(tmp.exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
					}
				}
			}
		}
		else
		{
			clog << "[WARNING] skip generate internal for "
				<< iter->chrom << "\t" 
				<< iter->start << "\t" << iter->end << "\t" 
				<< iter->sbin << "\t" << iter->ebin << "\t" 
				<< iter->obs << "\t" << iter->exp << endl;
		}
		// generate random bin pair for randomisation test
		index.gen_random_index(iter->sbin, iter->ebin, random_bins);

		for(int r = 0; r < RANDOME_TEST_SIZE; r++){
			run_obs = run_exp = run_nor = 0;
			// get the index range for the specified chrom: begin & end
			tmp_s = random_bins[r].first;
			tmp_e = random_bins[r].second;
			// if generate random index
			if ((tmp_s != 0) && (tmp_e != 0)){
				r_tmp = index.get_index_range(index.get_index(tmp_s).chr);

				for(int i=tmp_s; i<=tmp_e; i++)
				{
					for(int j=i; j<=tmp_e; j++)
					{
						obs = binmap.get_observe(i, j);
						exp = binmap.get_expect(i, j);

						if ((obs != -1) && (exp != -1))
						{
							// add internal contact inf to interval
							iter->sum_rand_obs += obs;
							iter->sum_rand_exp += exp;
							iter->sum_rand_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
							run_obs += obs;
							run_exp += exp;
							run_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
						}
					}
				}

				test[r][0] = run_obs;
				test[r][1] = run_exp;
				test[r][2] = run_nor;

				if (run_obs > iter->sum_obs) iter->rank_obs++;
				if (run_exp > iter->sum_exp) iter->rank_exp++;
				if (run_nor > iter->sum_nor) iter->rank_nor++;
#ifdef DEBUG
				cout << " normal " << r << "\t" << run_obs << "\t" << run_exp << "\t" << run_nor << endl;
#endif
			}
		}
		iter->rank_obs /= RANDOME_TEST_SIZE;
		iter->rank_exp /= RANDOME_TEST_SIZE;
		iter->rank_nor /= RANDOME_TEST_SIZE;

		cout << "[SUMMARY] average internal contact" << endl;	
		cout << "bin_size\tcounts\tsum_obs\tsum_exp\tsum_nor" << endl;
		for(map <int, INTER_ELE >::iterator iter = ave_map.begin(); iter != ave_map.end(); iter++)
		{
			cout << iter->first << "\t" << iter->second.count << "\t" << iter->second.sum_obs << "\t" << iter->second.sum_exp << "\t" <<iter->second.sum_nor << endl;
		}

#ifdef DEBUG
		cout << "[DEBUG] total internal size = " << (int)internal_BININTERVAL_vec.size()  << endl;
		for (vector<BININTERVAL>::size_type i = 0; i < 20; i++)
		{
			cout << internal_BININTERVAL_vec[i].chrom << "\t" 
				<< internal_BININTERVAL_vec[i].start << "\t" << internal_BININTERVAL_vec[i].end << "\t" 
				<< internal_BININTERVAL_vec[i].sbin << "\t" << internal_BININTERVAL_vec[i].ebin << "\t" 
				<< internal_BININTERVAL_vec[i].obs << "\t" << internal_BININTERVAL_vec[i].exp << endl;
		}   	
#endif

		//random test
		string filename = (string)OutputfileName;
		int found = filename.find_last_of(".");
		ss.clear();
		ss << outputcount;
		ss >> outputcount_str;
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
		}
		else cout << "Unable to open file";
		outputcount++;
	}

}

// output function
void INTERVAL::output(const char *fileName, bool show_average)
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
	if (show_average)
		output_f <<"index\tchrom\tstart\tend\tobser_contact\texpect_contact\tsum_obser\tsum_expect\t"
			<< "rank_obs\trank_exp\trank_nor"
			<<endl;
	else
		output_f << "index\tchrom\tstart\tend\tobser_contact\texpect_contact"
			<< "rank_obs\trank_exp\t\rank_nor"
			<< endl;

	for(vector<BININTERVAL>::iterator iter = BININTERVAL_vec.begin(); iter != BININTERVAL_vec.end(); iter++)
	{
		output_f << iter->index << "\t" << iter->chrom << "\t" 
			<< iter->start << "\t" << iter->end << "\t" 
			<< iter->obs << "\t" << iter->exp;

		if(show_average)
		{		 
			output_f << "\t" << iter->sum_obs << "\t" << iter->sum_exp << "\t"
				<< iter->rank_obs << "\t" << iter->rank_exp << "\t" << iter->rank_nor
				<<endl;
		}
		else{
			output_f << "\t" <<iter->rank_obs << "\t" << iter->rank_exp << "\t" << iter->rank_nor << endl;
		}
	}

	output_f.close();	
}

void INTERVAL::output_internal(const char *fileName)
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
	output_f << "index\tchrom\tstart\tend\tobser_contact\texpect_contact" << endl;

	if ((int)internal_BININTERVAL_vec.size() > 0)
	{
		cout << "[SUMMARY] output internal " << (int)internal_BININTERVAL_vec.size() << " elements to " << fileName << endl;
		for(vector<BININTERVAL>::iterator iter = internal_BININTERVAL_vec.begin(); iter != internal_BININTERVAL_vec.end(); iter++)
		{
			output_f << iter->index << "\t" << iter->chrom << "\t" 
				<< iter->start << "\t" << iter->end << "\t" 
				<< iter->obs << "\t" << iter->exp << endl;
		}	
	}
	else
	{
		cout << "[WARNING] there is no internal elements = " << (int)internal_BININTERVAL_vec.size() << " elements" << endl;
	}

	output_f.close();	
}
