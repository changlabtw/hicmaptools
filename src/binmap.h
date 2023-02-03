#ifndef _BINMAP_H
#define _BINMAP_H

#include <map>
#include "index.h"
#include "par.h"

#include "straw.h"

using namespace std;

//===BINMAP===
class BINMAP
{
public:
	BINMAP();
	BINMAP(const char *input_name, const char *output_name);
	BINMAP(const char *fileName);

	~BINMAP();
	float get_observe( const int i, const int j );
	float get_expect( const int i, const int j );
	
	int get_obs_size(){ return  (int)observe_map.size(); };
	int get_exp_size(){ return  (int)expect_map.size(); };
			
	void insert_obs(const int i, const int j, const float obs){ observe_map.insert(make_pair(make_pair(i,j),obs)); };
	void insert_exp(const int i, const int j, const float expect){ expect_map.insert(make_pair(make_pair(i,j),expect)); };
	
	void out_subcontact( const int start_bin, const int end_bin, const char *fileName );
	void out_contIne( const int cis_thre, INDEX &index, char *fileName ); //output close/far-cis/tran contact information into bed format
	void out2list( INDEX &index, PARAMETER par); //output contact in pair list format for Wash U
	void out2matrix( INDEX &index, PARAMETER par); //output contact in 2D matrix format for Hommer
	
	// load .hic data
	void insert_from_hic(vector<contactRecord> & records, int binsize, long cbin_number); // for .hic format

protected:
	map< pair<int, int>, float > observe_map;
	map< pair<int, int>, float > expect_map;
};

#endif /* _BINMAP_H */
