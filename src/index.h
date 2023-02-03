#ifndef _INDEX_H
#define _INDEX_H

#include <vector>
#include <map>
#include <string>

#include "straw.h"

using namespace std;

struct INDEX_ELE
{
	string chr;
	int start;
	int end;
	int count;
};

struct CHR_ELE
{
	string chr;
	int start;
	int end;
};

//===INDEX===
class INDEX
{
public:
	INDEX();
	INDEX(const char *fileName);
	~INDEX();
	
	void insert4ginteraction(map<CHR_ELE, int> &CHR2int);
	
	int find_index( const string q_chr, const int q_pos, bool forStart, bool forEnd );
	pair<int, int> get_index_range(const string chr){if(cbin_map.find(chr) != cbin_map.end()) return make_pair(cbin_map[chr].front(), cbin_map[chr].back()); else return make_pair(-1,-1);};
	INDEX_ELE get_index(const int q_bin){return index_map[q_bin];};
	void gen_random_index(const int i, const int j, vector< pair<int, int> > &random_vec);
	
	// load hic data
	void insert_from_hic(vector<contactRecord> & records, int binsize, string chrName);
	long get_cbin_number();

protected:
	bool FIXED_BIN;
// save by chromosome
	map< string, vector<int> > cbin_map;
	map< string, vector<int> > from_map;
	map< string, vector<int> > to_map;		
	map< int, INDEX_ELE> index_map;

	// record the cbin number for loading data from .hic file
	long cbin_record;
};

#endif /* _INDEX_H */
