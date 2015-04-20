#ifndef _INDEX_H
#define _INDEX_H

#include <vector>
#include <map>
#include <string>

using namespace std;

struct INDEX_ELE
{
	string chr;
	int start;
	int end;
	int count;
};

//===INDEX===
class INDEX
{
public:
	INDEX();
	INDEX(const char *fileName);
	~INDEX();
	int find_index( const string q_chr, const int q_pos, bool forStart, bool forEnd );
	pair<int, int> get_index_range(const string chr){return make_pair(cbin_map[chr].front(), cbin_map[chr].back());};
	INDEX_ELE get_index(const int q_bin){return index_map[q_bin];};
protected:
	map< string, vector<int> > cbin_map;
	map< string, vector<int> > from_map;
	map< string, vector<int> > to_map;		
	map< int, INDEX_ELE> index_map;
};

#endif /* _INDEX_H */
