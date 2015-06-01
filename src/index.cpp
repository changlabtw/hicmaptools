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
	cout << "\t\treading # of bins = " <<  (int)index_map.size()  << endl;
	
#ifdef DEBUG
		cout << "cbin" << endl;
		for(map< string, vector<int> >::iterator iter = cbin_map.begin(); iter != cbin_map.end();iter++)
		{
			cout << "\t" << iter->first << endl;
			for(vector<int>::iterator iter_v = iter->second.begin(); iter_v != iter->second.end();iter_v++)
			{
				cout << "\t\t" << *iter_v << endl;
			}
		}
		
		cout << "from" << endl;
		for(map< string, vector<int> >::iterator iter = from_map.begin(); iter != from_map.end();iter++)
		{
			cout << "\t" << iter->first << endl;
			for(vector<int>::iterator iter_v = iter->second.begin(); iter_v != iter->second.end();iter_v++)
			{
				cout << "\t\t" << *iter_v << endl;
			}
		}

		cout << "to" << endl;
		for(map< string, vector<int> >::iterator iter = to_map.begin(); iter != to_map.end();iter++)
		{
			cout << "\t" << iter->first << endl;
			for(vector<int>::iterator iter_v = iter->second.begin(); iter_v != iter->second.end();iter_v++)
			{
				cout << "\t\t" << *iter_v << endl;
			}
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
// for case : 3R	10000000 	lower bin range 9995000	10000000 			uper bin range 10000000	10005000   			
   			else if (forStart && from_map[q_chr][up_i] == q_pos) {
   				return cbin_map[q_chr][up_i];
   			}
// for case : 3R	15000000 			lower bin range 14995000	15000000 			uper bin range 15000000	15005000   			
   			else if (forEnd && to_map[q_chr][low_i] == q_pos) {
   				return cbin_map[q_chr][low_i];
   			}   			
   			else {
   				cout << "[WARNING] no bin for " << q_chr << "\t" << q_pos
   					 << " 			lower bin range " << from_map[q_chr][low_i] << "\t" << to_map[q_chr][low_i]
   				     << " 			uper bin range " << from_map[q_chr][up_i] << "\t" << to_map[q_chr][up_i] << endl;   				
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
	