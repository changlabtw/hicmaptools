#ifndef _PAR_H
#define _PAR_H

#include <algorithm>

////////////////////////////////////
// structure definiation
////////////////////////////////////
struct PARAMETER{
// initialization
	PARAMETER() : in_binmap_name(), in_bins_name(), query_name(), output_name(), query_mode("none"), ner_bin(10), random_size(100) {}

    char in_binmap_name[1024];
	char in_bins_name[1024];
	char query_name[1024];
	char output_name[1024];
	string query_mode;
	int ner_bin; // check bins in +- ner_bin
	int random_size;
	string sel_chr; // only output specified chromosome
};

void parse_command_line(int argc, char **argv, PARAMETER &par);
void show_param(PARAMETER par);

////////////////////////////////////
// parse function
////////////////////////////////////
char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

#endif /* _PAR_H */