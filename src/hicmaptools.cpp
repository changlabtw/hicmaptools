#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <ctime> // for random generator

#include <stdio.h>
#include <string.h>

#include "binmap.h"
#include "index.h"
#include "query.h"
#include "interval.h"
#include "bat.h"
#include "region.h"
#include "couple.h"
#include "par.h"

using namespace std;

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

void exit_with_help()
{
	printf(
			"Usage: hicmaptools [options] -in_map in.binmap -in_bin in.bins -loop|-TAD|-bat|-submap query.bed -output out_file.tsv\n"
			"inputs:\n"
			"\t-in_map \t binary .binmap generate from .n_contact 3CONET-build \n"
			"\t-in_bin \t the bin file for contact map, .bins\n"
			"\n"
			"queries:\n"
			"\t-loop   \t loci gene: chr\tstrat\tend\n"
			"\t-output \t contact between two ends, ie. gene 5' 3'\n"		
			"\n"
			"\t-TAD    \t loci interval: chr\tstrat\tend\n"
			"\t-output \t sum/ave contact of the TAD, ie. TAD\n"		
			"\n"
			"\t-bat    \t loci bat: chr\tstrat\tend\n"
			"\t-output \t neighboring ave contact of the bat, ie. PcG binding site\n"		
			"\n"
			"\t-submap \t genome region to extract: chr\tstrat\tend\n"
			"\t-output \t sub contact map, ie. 3R:10~15MB\n"		
			"\n"		
			"\t-site   \t interesting sites: chr\tstrat\tend\n"
			"\t-output \t contact between those sites\n"
			"\n"
			"\t-local  \t interval: chr\tstrat\tend\n"
			"\t-output \t all contacts inside interval\n"
			"\n"		
			"\t-couple \t pair sites: chr1\tstrat1\tend1\tchr2\tstrat2\tend2\n"
			"\t-output \t output pair contacts\n"
			"\n"
			"query parameters:\n"
			"\t-random N \t random sampling corresponding N contacts [default: 100 contacts]\n"
			"\t-ner_bin N\t consider within +- N bins [default: 10 bins]\n"
			"\nFor instance:\n"
			"\thicmaptools -in_map nm_none_1000_reduced.bimap -in_bin nm_none_1000.bins -TAD data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv\n"
			);
	exit(0);
}

int main(int argc, char *argv[])
{
	// parse data
	PARAMETER par;

	parse_command_line(argc, argv, par);
	show_param(par);

	//	for random
	srand((unsigned)time(NULL)); 

	cout << endl << "[BEGIN]" << endl;
	BINMAP map(par.in_binmap_name);
	INDEX index(par.in_bins_name);

	if(par.query_mode == "none")
	{
		map.out_contIne(10000, index, par.output_name);
	}
	else if(par.query_mode == "loop")
	{
		QUERY query(par.query_name, map, index, par.random_size, par.output_name);
		query.output(par.output_name);
	}
	else if (par.query_mode == "TAD")
	{
		INTERVAL interval(par.query_name, map, index);
		interval.gen_internal_contact(map, index, par.random_size, par.output_name,1);
		interval.output(par.output_name, 1);
	}
	else if (par.query_mode == "local")
	{
		INTERVAL interval(par.query_name, map, index);

		cout << " generating internal contact " << endl;
		interval.gen_internal_contact(map, index, par.random_size, par.output_name,0);
		interval.output_internal(par.output_name);		
	}	
	else if (par.query_mode == "bat")
	{
		BAT bat(par.query_name, index, par.ner_bin, par.ner_bin);

		bat.cal_contact(map, index, par.ner_bin, par.ner_bin, par.random_size, par.output_name);
		bat.output(par.output_name);
	}
	else if (par.query_mode == "site")
	{
		BAT bat(par.query_name, index, par.ner_bin, par.ner_bin);

		bat.output_pair(par.output_name, map, index, par.random_size, par.output_name);		
	}	
	else if (par.query_mode == "submap")
	{
		REGION region(par.query_name, index);		
		region.output(par.output_name, map);	
	}
	else if (par.query_mode == "couple")
	{
		COUPLE couple(par.query_name, index);	
		couple.cal_contact(map, index, par.random_size, par.output_name);
		couple.output(par.output_name);	
	}

	cout << endl << "[END]" << endl;
	return 0;
}

void parse_command_line(int argc, char **argv, PARAMETER &par)
{
	int i;

	if (argc == 1 || cmdOptionExists(argv, argv+argc, "-h")){
		exit_with_help();
	}

	// parse options
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();

		if( strncmp(argv[i-1],"-in_map", 20)==0 ){
			strcpy(par.in_binmap_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-in_bin", 20)==0 ){
			strcpy(par.in_bins_name, argv[i]);
		}
		// query mode
		else if( strncmp(argv[i-1],"-bat", 20)==0 ){
			par.query_mode = "bat";
			strcpy(par.query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-TAD", 20)==0 ){
			par.query_mode = "TAD";
			strcpy(par.query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-local", 20)==0 ){
			par.query_mode = "local";
			strcpy(par.query_name, argv[i]);
		}		
		else if( strncmp(argv[i-1],"-couple", 20)==0 ){
			par.query_mode = "couple";
			strcpy(par.query_name, argv[i]);
		}				
		else if( strncmp(argv[i-1],"-submap", 20)==0 ){
			par.query_mode = "submap";
			strcpy(par.query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-loop", 20)==0 ){
			par.query_mode = "loop";
			strcpy(par.query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-site", 20)==0 ){
			par.query_mode = "site";
			strcpy(par.query_name, argv[i]);
		}		
		// other parameters		
		// check neighbouring bins for bat mode
		else if( strncmp(argv[i-1],"-ner_bin", 20)==0 ){
			par.ner_bin = atoi(argv[i]);
		}						
		// random size
		else if( strncmp(argv[i-1],"-random", 20)==0 ){
			par.random_size = atoi(argv[i]);
		}								
		// output								
		else if( strncmp(argv[i-1],"-output", 20)==0 ){
			strcpy(par.output_name, argv[i]);
		}				
		else{
			fprintf(stderr,"unknown option:%s\n",argv[i-1]);
			exit_with_help();	
		}
	}

	// determine filenames
	if(i != argc)
		exit_with_help();
}

void show_param(PARAMETER par)
{
	cout << "Input" << endl
		<< "\t map   =\t" << par.in_binmap_name << endl
		<< "\t bin   =\t" << par.in_bins_name << endl
		<< "\t query =\t" << par.query_name << endl;

	cout << "Parameters" << endl
		<< "\t ner_bin     =\t"  << par.ner_bin << endl
		<< "\t random size =\t"  << par.random_size << endl;

	cout << "Output" << endl
		<< "\t output =\t" << par.output_name << endl;  
}
