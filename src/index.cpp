#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort

#include "index.h"

INDEX::INDEX()
{
}

// load map from binary file
INDEX::INDEX(const char *file_name)
{
	FIXED_BIN = true;
	fstream input_f;
	string str, chr;
	stringstream ss1,ss2;
	int cbin;	
	vector<int> tmp_vec;
	INDEX_ELE tmp_index;

// initialization	
	tmp_index.count = 0;
	
	input_f.open(file_name, ios_base::in);
	if(!input_f)
	{
		system("hostname");
		perror(file_name);
		exit(0);
	}
	else
	{
		cout << "\treading index file =\t" << file_name << endl;
	}

	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss1.clear(); ss1.str(str);
			ss2.clear(); ss2.str(str);			
// handle for the first line which might contain header
// parse by two formats : "cbin	chr	from.coord	to.coord	count" | "cbin	chr	start	end"
			if ((ss1 >> cbin >> tmp_index.chr >> tmp_index.start >> tmp_index.end >> tmp_index.count) || 
			    (ss2 >> cbin >> tmp_index.chr >> tmp_index.start >> tmp_index.end))
			{
				if(cbin_map.find(tmp_index.chr) == cbin_map.end())
				{
					cbin_map.insert(make_pair(tmp_index.chr, tmp_vec));
					from_map.insert(make_pair(tmp_index.chr, tmp_vec));
					to_map.insert(make_pair(tmp_index.chr, tmp_vec));
				}
				
				cbin_map[tmp_index.chr].push_back(cbin);
				from_map[tmp_index.chr].push_back(tmp_index.start);
				to_map[tmp_index.chr].push_back(tmp_index.end);
				index_map.insert(make_pair(cbin,tmp_index));
			}
		}		
	}
		
	input_f.close();
	input_f.clear();

	cout << "\t\treading # of chrs = " <<  (int)cbin_map.size()  << endl;
	cout << "\t\treading # of bins = " <<  (int)index_map.size()  << endl << endl;
	
#ifdef DEBUG
		cout << "cbins" << endl;
		for(map< string, vector<int> >::iterator iter = cbin_map.begin(); iter != cbin_map.end();iter++)
		{
			cout << "\tchr\t" << iter->first << "\t first = " << iter->second.front() << " end = " << iter->second.back() << endl;			
		}
		
		cout << "start position" << endl;
		for(map< string, vector<int> >::iterator iter = from_map.begin(); iter != from_map.end();iter++)
		{
			cout << "\tchr\t" << iter->first << "\t first = " << iter->second.front() << " end = " << iter->second.back() << endl;			
		}

		cout << "end position" << endl;
		for(map< string, vector<int> >::iterator iter = to_map.begin(); iter != to_map.end();iter++)
		{
			cout << "\tchr\t" << iter->first << "\t first = " << iter->second.front() << " end = " << iter->second.back() << endl;			
		}		
#endif

}

INDEX::~INDEX()
{
}

int INDEX::find_index( const string q_chr, const int q_pos, bool forStart, bool forEnd )
{
	vector<int>::iterator low,up;
	int NOT_FOUND = -1;
	
	if(cbin_map.find(q_chr) != cbin_map.end())
	{
//	lower	upper	
//	from	to*
//	from*	to
  		low=lower_bound(from_map[q_chr].begin(), from_map[q_chr].end(), q_pos);
  		up=upper_bound(to_map[q_chr].begin(), to_map[q_chr].end(), q_pos);
  		
  		if(low == from_map[q_chr].end() && (*low != q_pos)){
   			//element not found
   			cout << "[WARNING] " << q_chr << "\t" << q_pos << " out the range of bin files"  << "\t return the last bin: "  << cbin_map[q_chr].back() << endl;
   			return cbin_map[q_chr].back();
 		} 
		else{
   			//element found
   			int low_i = (low - from_map[q_chr].begin())-1;
   			int up_i  = (up - to_map[q_chr].begin());
   			
   			if (low_i == up_i){   			
#ifdef DEBUG   			
	cout << q_pos << " in bin index = " << cbin_map[q_chr][up_i] << "\t" << from_map[q_chr][low_i] << "\t" << to_map[q_chr][low_i] << endl;	
#endif
				return cbin_map[q_chr][up_i];			
   			}
// for case : 3R	10000000	lower bin range 9995000	10000000 			uper bin range 10000000	10005000   			
   			else if (forStart && from_map[q_chr][up_i] == q_pos) {
   				return cbin_map[q_chr][up_i];
   			}
// for case : 3R	15000000 	lower bin range 14995000	15000000 			uper bin range 15000000	15005000		
   			else if (forEnd && to_map[q_chr][low_i] == q_pos) {
   				return cbin_map[q_chr][low_i];
   			}
// for case : 1  45453905	lower bin range 45451483    45454915	upper bin range 45451483 45454915:two same bins => pick up the first one, special for fragment input		    			
   			else if (forStart && from_map[q_chr][up_i] == from_map[q_chr][low_i]) {
   				return cbin_map[q_chr][up_i];
   			}
// for case : 1  45453905	lower bin range 45451483    45454915	upper bin range 45451483 45454915:two same bins => pick up the second one, special for fragment input	   			
   			else if (forEnd && to_map[q_chr][low_i] == to_map[q_chr][up_i]) {
   				return cbin_map[q_chr][low_i];
   			}  			   			
   			else {
   				cout << "[WARNING] no bin for " << q_chr << "\t" << q_pos
   					 << "\tLOWER bin range " << from_map[q_chr][low_i] << "\t" << to_map[q_chr][low_i]
   				     << "\tUPPER bin range " << from_map[q_chr][up_i] << "\t" << to_map[q_chr][up_i] << endl;   				
  				return NOT_FOUND;   			
   			}
		}
	}
	else
	{
		cout << "[WARNING] " << q_chr << " not in bins file" << endl;
		return NOT_FOUND;
	}	
}

void INDEX::gen_random_index(const int binx, const int biny, vector< pair<int, int> > &random_index)
{
	INDEX_ELE ele1, ele2;
	int bin_dis = abs(biny-binx);
//select bins from the chromosome
	vector<int> pool_bins = cbin_map[index_map[binx].chr];

//	for random
	int random_size = (int)random_index.size();
	int random_x, random_y;
	int count = 0;
	int rand_x_limit;
		
// check inter-chromosome or intra-chromosome
	if (index_map.find(binx) != index_map.end() && index_map.find(biny) != index_map.end()){
		ele1 = index_map[binx];
		ele2 = index_map[biny];
		cout << "\tgenerate random for " << binx << " " << biny << "\trandom size = " << random_size << " ... ";		
	}	
	else{
		cout << "ERROR: can not find index for bins " << binx << " or " << biny << endl;
		return; 
	}
	
// inter-chromosome	
	if(ele1.chr == ele2.chr){
		if(FIXED_BIN){
// fixed bins : random use bin difference
			rand_x_limit = (int)pool_bins.size()-2-bin_dis;
			do{ 
				random_x = (int)(((double)rand()/RAND_MAX)*rand_x_limit);
        		random_y = random_x + bin_dis;
        		if ((pool_bins[random_y] - pool_bins[random_x]) == bin_dis)	// avoiding the vector of the bins is not continuos
        		{
        			random_index[count] = make_pair(pool_bins[random_x],pool_bins[random_y]);
        			count++;
//        			cout << "\tpair " << random_x << " " << random_y << endl; 
        		}
    		} while (count < random_size); 
		}else{
// non-fixed bins : random use distance => take longer computing time
			cout << " WARNING: random index for non-fxied bin is not implemented, time consuming" << endl;		
		}	
	}
// intra-chromosome
	else{
		cout << " WARNING: random index for intra-chromosome is not implemented" << endl;
	}
	cout << "[DONE]" << endl;		
}
