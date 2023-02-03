#ifndef _PAR_H
#define _PAR_H

#include <algorithm>

////////////////////////////////////
// structure definiation
////////////////////////////////////
struct PARAMETER{
// initialization
	PARAMETER() : in_binmap_name("NONE"), in_bins_name(), in_ginter_name("NONE"),  query_name(), output_name(), query_mode("none"), ner_bin(10), random_size(100), sel_chr("NONE"), useNormal(false), in_hic("NONE"), in_hic_norm("NONE"),  in_hic_resol(10000) {}

    char in_binmap_name[1024];
	char in_bins_name[1024];
	char in_ginter_name[1024]; // ginteraction format
	char query_name[1024];
	char output_name[1024];
	string query_mode;
	int ner_bin; // check bins in +- ner_bin
	int random_size;
	string sel_chr; // only output specified chromosome
	bool useNormal;

	/* data for straw function*/
	char in_hic[1024];
	char in_hic_norm[1024];
	int in_hic_resol;
};

void parse_command_line(int argc, char **argv, PARAMETER &par);
void show_param(PARAMETER par);

#endif /* _PAR_H */
