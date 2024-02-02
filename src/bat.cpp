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

// make folder
#include <iostream>
#include <string>
#include <sys/stat.h>

int mkpath(std::string s,mode_t mode)
{
    size_t pos=0;
    std::string dir;
    int mdret;

    if(s[s.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        s+='/';
    }

    while((pos=s.find_first_of('/',pos))!=std::string::npos){
        dir=s.substr(0,pos++);
        if(dir.size()==0) continue; // if leading / first time is 0 length
        if((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST){
            return mdret;
        }
    }
    return mdret;
}

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

void BAT::cal_contact(BINMAP &binmap, INDEX &index, const int fordward_the, const int backward_the, const int RANDOME_TEST_SIZE, const char *outputfileName)
{
	// initialisation
	float obs, exp;
	float run_obs, run_exp, run_nor;
	//vector< pair<int, int> > random_bins (RANDOME_TEST_SIZE, make_pair(0,0));
	vector< pair<int, int> > random_bins;
	pair<int, int> r_tmp;
	int tmp_s, tmp_e;
	int fordward_bin, backward_bin;
	// random test
	// float test[RANDOME_TEST_SIZE][3];
	float** test;
	int outputcount=1;
	
	//arg -random != 0
	if(RANDOME_TEST_SIZE != 0){
		random_bins = vector< pair<int, int> >(RANDOME_TEST_SIZE, make_pair(0,0));
		test = new float*[RANDOME_TEST_SIZE];
		for(int i=0; i<RANDOME_TEST_SIZE ;i++){
			test[i] = new float[3];
		}
	}

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
		
		if(RANDOME_TEST_SIZE != 0){
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

					fordward_bin = (tmp_s-fordward_the > r_tmp.first)  ? tmp_s-fordward_the : r_tmp.first;
					backward_bin = (tmp_e+backward_the < r_tmp.second) ? tmp_e+backward_the : r_tmp.second;

					for(int i=tmp_s; i<=tmp_e; i++)
					{
						for(int j=fordward_bin; j<=backward_bin; j++)
						{
							obs = binmap.get_observe(i, j);
							exp = binmap.get_expect(i, j);

							if ((obs != -1) && (exp != -1))
							{
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

#ifdef DEBUG						 
			cout << "normal end" << endl;
#endif

			iter->sum_rand_obs /= RANDOME_TEST_SIZE;
			iter->sum_rand_exp /= RANDOME_TEST_SIZE;
			iter->sum_rand_nor /= RANDOME_TEST_SIZE;

			iter->rank_obs /= RANDOME_TEST_SIZE;
			iter->rank_exp /= RANDOME_TEST_SIZE;
			iter->rank_nor /= RANDOME_TEST_SIZE;		 		 

			//ranom test
			string filename = (string)outputfileName;
			int found = filename.find_last_of(".");
			filename = filename.substr(0,found) + "_random_" + to_string(outputcount) + ".txt";
			ofstream myfile(filename);
			if (myfile.is_open())
			{
				myfile << "random_obs,";
				myfile << "random_exp,";
				myfile << "random_nor\n";
				myfile << iter->sum_obs << "," << iter->sum_exp << "," << iter->sum_nor << endl;
				for(int i = 0; i < RANDOME_TEST_SIZE; i ++){
					myfile << test[i][0] << ","<<test[i][1]<<","<<test[i][2]<<endl ;
				}
				myfile.close();
				outputcount++;
			}
			else cout << "Unable to open file";
		}
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
	output_f << "index\tchrom\tstart\tend\t"
		<< "sum_obs\tsum_exp\tsum_nor\t" 
		<< "rand_obs\trand_exp\trand_nor\t"
		<< "divide_obs\tdivide_exp\tdivide_nor\t"
		<< "rank_obs\trank_exp\trank_nor\t"	                                                       
		<< endl;

	for(vector<BINBAT>::iterator iter = BINBAT_vec.begin(); iter != BINBAT_vec.end(); iter++)
	{
		output_f << iter->index << "\t" << iter->chrom << "\t" 
			<< iter->start << "\t" << iter->end << "\t" ;

		output_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
		output_f.precision(3); // for fixed format, two decimal places    

		output_f << iter->sum_obs << "\t" << iter->sum_exp << "\t" << iter->sum_nor << "\t"
			<< iter->sum_rand_obs << "\t" << iter->sum_rand_exp << "\t" << iter->sum_rand_nor << "\t"
			<< iter->sum_obs/iter->sum_rand_obs << "\t" << iter->sum_exp/iter->sum_rand_exp << "\t" << iter->sum_nor/iter->sum_rand_nor << "\t" 
			<< iter->rank_obs << "\t" << iter->rank_exp << "\t" << iter->rank_nor << "\t" 		         
			<< endl;
	}

	output_f.close();	
}

void BAT::output_pair(const char *fileName, BINMAP &binmap, INDEX &index, const int RANDOME_TEST_SIZE, const char *OutputfileName)
{
	int sum_bin;
	float sum_obs, sum_exp, sum_nor, obs, exp;
	float sum_rand_obs, sum_rand_exp, sum_rand_nor;
	float rank_obs, rank_exp, rank_nor;
	float run_obs, run_exp, run_nor;
	vector< pair<int, int> > random_bins (RANDOME_TEST_SIZE, make_pair(0,0));
	int tmp_s1, tmp_e1, tmp_s2, tmp_e2;
	// random test
	float test[RANDOME_TEST_SIZE][3];
	int outputcount=1;
	stringstream ss;
	string outputcount_str;
	// output the header of random test when random size > 0
	string random_str = (RANDOME_TEST_SIZE > 0)? "rand_obs\trand_exp\trand_nor\tdivide_obs\tdivide_exp\tdivide_nor\trank_obs\trank_exp\trank_nor\t":"";

	ofstream output_f;
	output_f.open (fileName);

	if(!output_f)
	{
		system("hostname");
		perror(fileName);
		exit(0);
	}

	// print header
	output_f << "index1\tchrom1\tstart1\tend1\t"
		<< "index2\tchrom2\tstart2\tend2\t"
		<< "sum_obs\tsum_exp\tsum_nor\t"
		<< random_str
		<< endl;

	for(vector<BINBAT>::iterator i = BINBAT_vec.begin(); i != BINBAT_vec.end(); i++)
	{
		for(vector<BINBAT>::iterator j = i+1; j != BINBAT_vec.end(); j++)
		{
			// initial
			sum_bin = 0;
			sum_obs = sum_exp = sum_nor = 0;
			sum_rand_obs = sum_rand_exp = sum_rand_nor =0;
			rank_obs = rank_exp = rank_nor=0;

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
				output_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
				output_f.precision(3); // for fixed format, two decimal places
				output_f << i->index << "\t" << i->chrom << "\t" << i->start << "\t" << i->end << "\t"
					<< j->index << "\t" << j->chrom << "\t" << j->start << "\t" << j->end << "\t"
					<< sum_obs << "\t" << sum_exp << "\t" << sum_nor<<"\t";

				// 
				// generate random bin pair for randomisation test
				//
				// only perform random test if user specify
				if (RANDOME_TEST_SIZE > 0)
				{
					index.gen_random_index(i->ebin, j->sbin, random_bins);

					for(int r = 0; r < RANDOME_TEST_SIZE; r++){
						run_obs = run_exp = run_nor = 0;
						// get the index range for the specified chrom: begin & end
						tmp_s1 = random_bins[r].first-(i->ebin-i->sbin);
						tmp_e1 = random_bins[r].first;
						tmp_s2 = random_bins[r].second;
						tmp_e2 = random_bins[r].second+(j->ebin-j->sbin);
						// if generate random index
						if ((tmp_s1 != 0) && (tmp_e1 != 0)&&(tmp_s2 != 0)&&(tmp_e2 != 0)){
							for(int m=tmp_s1; m<=tmp_e1; m++)
							{
								for(int n=tmp_s2; n<=tmp_e2; n++)
								{
									obs = binmap.get_observe(m, n);
									exp = binmap.get_expect(m, n);

									if ((obs != -1) && (exp != -1))
									{
										sum_rand_obs += obs;
										sum_rand_exp += exp;
										sum_rand_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
										run_obs += obs;
										run_exp += exp;
										run_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
									}
								}
							}

							test[r][0] = run_obs;
							test[r][1] = run_exp;
							test[r][2] = run_nor;
							if (run_obs > sum_obs) rank_obs++;
							if (run_exp > sum_exp) rank_exp++;
							if (run_nor > sum_nor) rank_nor++;
						}
#ifdef DEBUG
						cout << " normal " << r << "\t" << run_obs << "\t" << run_exp << "\t" << run_nor << endl;
#endif
					}
					sum_rand_obs /= RANDOME_TEST_SIZE;
					sum_rand_exp /= RANDOME_TEST_SIZE;
					sum_rand_nor /= RANDOME_TEST_SIZE;

					rank_obs /= RANDOME_TEST_SIZE;
					rank_exp /= RANDOME_TEST_SIZE;
					rank_nor /= RANDOME_TEST_SIZE;

					//output random test
					//	construct output file name
					string filename = (string)OutputfileName;
					int found = filename.find_last_of(".");

					// make folder for keeping all random files
					int mkdirretval;
					mkdirretval=mkpath(filename.substr(0,found), 0755);

					ss.clear();
					ss << outputcount;
					ss >> outputcount_str;

					// keep random files into the folder
					filename = filename.substr(0,found) + "/random_" + outputcount_str + ".txt";
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

					output_f << sum_rand_obs << "\t" << sum_rand_exp << "\t" << sum_rand_nor << "\t"
						<< sum_obs/sum_rand_obs << "\t" << sum_exp/sum_rand_exp << "\t" << sum_nor/sum_rand_nor << "\t"
						<< rank_obs << "\t" << rank_exp << "\t" << rank_nor << "\t";
				}
				output_f << endl;
			}
			// output zero for all metrics if there is no contact information for the query
			else
			{
				output_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
				output_f.precision(3); // for fixed format, two decimal places
				output_f << i->index << "\t" << i->chrom << "\t" << i->start << "\t" << i->end << "\t"
					<< j->index << "\t" << j->chrom << "\t" << j->start << "\t" << j->end << "\t"
					<< 0 << "\t" << 0 << "\t" << 0 <<"\t";
					
				if (RANDOME_TEST_SIZE > 0)
				{
					output_f << 0 << "\t" << 0 << "\t" << 0 << "\t"
						<< 0 << "\t" << 0 << "\t" << 0 << "\t"
						<< 0 << "\t" << 0 << "\t" << 0 << "\t";				
				}
				output_f << endl;				
			}
		}
	}	

	output_f.close();	
}
