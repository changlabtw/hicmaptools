#ifndef _PAR_H
#define _PAR_H

#include <algorithm>

////////////////////////////////////
// structure definiation
////////////////////////////////////
struct PARAMETER{
// initialization
	PARAMETER() : in_binmap_name(), in_bins_name(), in_ginter_name(),  query_name(), output_name(), ner_bin(10), random_size(100), useNormal(false), query_mode("none"), sel_chr("NONE"),  in_hic(), in_hic_norm(),  in_hic_resol(10000) {}
	
    char in_binmap_name[1024];
	char in_bins_name[1024];
	char in_ginter_name[1024]; // ginteraction format
	char query_name[1024];
	char output_name[1024];
	int ner_bin; // check bins in +- ner_bin
	int random_size;
	bool useNormal; // if TURE = obs/exp for bin/binmap input
	string query_mode;

	// data for straw function
	string sel_chr; // only output specified chromosome
	char in_hic[1024];
	char in_hic_norm[1024]; // 4 modes: NONE/VC/VC_SQRT/KR
	int in_hic_resol;
};

void parse_command_line(int argc, char **argv, PARAMETER &par);
void show_param(PARAMETER par);

#endif /* _PAR_H */
