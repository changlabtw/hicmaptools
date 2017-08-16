#include <iostream>
#include <limits>
#include "random.h"
#include "binmap.h"
#include "index.h"

using namespace std;

void RANDOM(BINMAP &binmap, INDEX &index, int sbin, int ebin, int X, int Y, int RANDOME_TEST_SIZE, RANDOM_E &ran, int flag) {

	vector< pair<int, int> > random_bins (RANDOME_TEST_SIZE, make_pair(0,0));
	float run_obs, run_exp, run_nor, obs, exp;
	pair<int, int> r_tmp;
	int tmp_s, tmp_e,S,E;

	index.gen_random_index(sbin, ebin, random_bins);

	for(int r = 0; r < RANDOME_TEST_SIZE; r++){
		run_obs = run_exp = run_nor = 0;
		tmp_s = random_bins[r].first;
		tmp_e = random_bins[r].second;

		if ((tmp_s != 0) && (tmp_e != 0)){
			r_tmp = index.get_index_range(index.get_index(tmp_s).chr);

			if(flag==1){

				S = (tmp_s-X > r_tmp.first)  ? tmp_s-X : r_tmp.first;
				E = (tmp_e+Y < r_tmp.second) ? tmp_e+Y : r_tmp.second;

				for(int i=tmp_s; i<=tmp_e; i++)
				{
					for(int j=S; j<=E; j++)
					{
						obs = binmap.get_observe(i, j);
						exp = binmap.get_expect(i, j);

						if ((obs != -1) && (exp != -1))
						{
							ran.rand_obs += obs;
							ran.rand_exp += exp;
							ran.rand_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
							run_obs += obs;
							run_exp += exp; 
							run_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
						}
					}
				}
			}
			else if(flag==2){

				S = (tmp_e-X > r_tmp.first)  ? tmp_e-X : r_tmp.first;
				E = (tmp_s+Y < r_tmp.second) ? tmp_s+Y : r_tmp.second;
				for(int i=tmp_s; i<=E; i++)
				{
					for(int j=S; j<=tmp_e; j++)
					{
						obs = binmap.get_observe(i, j);
						exp = binmap.get_expect(i, j);

						if ((obs != -1) && (exp != -1))
						{
							ran.rand_obs += obs;
							ran.rand_exp += exp;
							ran.rand_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
							run_obs += obs;
							run_exp += exp; 
							run_nor += obs/(exp+std::numeric_limits<float>::epsilon()); // avoid x/0 => nan
						}
					}
				}
			}
			if (run_obs > ran.quer_obs) ran.rank_obs++;
			if (run_exp > ran.quer_exp) ran.rank_exp++;
			if (run_nor > ran.quer_nor) ran.rank_nor++;
#ifdef DEBUG				
			cout << " normal " << r << "\t" << run_obs << "\t" << run_exp << "\t" << run_nor << endl;
#endif
		}
	}

#ifdef DEBUG						 
	cout << "normal end" << endl;
#endif

	ran.rand_obs /= RANDOME_TEST_SIZE;
	ran.rand_exp /= RANDOME_TEST_SIZE;
	ran.rand_nor /= RANDOME_TEST_SIZE;

	ran.rank_obs /= RANDOME_TEST_SIZE;
	ran.rank_exp /= RANDOME_TEST_SIZE;
	ran.rank_nor /= RANDOME_TEST_SIZE;
}
