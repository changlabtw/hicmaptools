#ifndef _PAR_H
#define _PAR_H

#include <algorithm>

////////////////////////////////////
// structure definiation
////////////////////////////////////
struct PARAMETER{
// initialization
	PARAMETER() : in_binmap_name(), in_bins_name(), query_name(), output_name(), query_mode("none"), ner_bin(10), random_size(500), sel_chr("NONE"), useNormal(false) {}

    char in_binmap_name[1024];
	char in_bins_name[1024];
	char query_name[1024];
	char output_name[1024];
	string query_mode;
	int ner_bin; // check bins in +- ner_bin
	int random_size;
	string sel_chr; // only output specified chromosome
	bool useNormal;
};

void parse_command_line(int argc, char **argv, PARAMETER &par);
void show_param(PARAMETER par);

#endif /* _PAR_H */
