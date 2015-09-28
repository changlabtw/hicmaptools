#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <stdio.h>
#include <string.h>

#include "binmap.h"
#include "index.h"
#include "par.h"

using namespace std;

void show_param(char *in_ncontact_name, char *out_bimap_name);

void exit_with_help()
{
	printf(
		"Turn .n_contact into list (for Wash U) or 2D matrix (for hommer)\n"	
		"Usage: formatmap [options] -in_map input.n_contact|s0.mat -in_bin input.cbins -out_list|-out_2dmatrix out.tsv\n"
		"options:\n"
		"\t-out_list \t list of pair-contact according to the format @ http://wiki.wubrowse.org/Long-range\n"
		"\t-out_2dmatrix \t 2d matrix format @ \n"
		"\nFor instance:\n"
		"formatmap -in_map example/s0.mat -in_bin example/DpnII_hg19.cbins -out_list example/1000.bimap\n"
	);
	exit(0);
}

int main(int argc, char *argv[])
{
// parse data
	PARAMETER par;
	
	parse_command_line(argc, argv, par);
	show_param(par);

	cout << endl << "[BEGIN]" << endl;
	BINMAP map(par.in_binmap_name);
	INDEX index(par.in_bins_name);

	if((par.query_mode == "none")||(par.query_mode == "list"))
	{
		map.out2list( index, par.output_name, par.ner_bin);
	}
	else if(par.query_mode == "2dmatrix")
	{
		map.out2matrix( index, par.output_name, par.sel_chr);
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
// other parameters		
// check neighbouring bins for bat mode
		else if( strncmp(argv[i-1],"-ner_bin", 20)==0 ){
			par.ner_bin = atoi(argv[i]);
		}
// select chromosome for output matrix
		else if( strncmp(argv[i-1],"-sel_chr", 20)==0 ){
			par.sel_chr = argv[i];
		}								
// output								
		else if( strncmp(argv[i-1],"-out_list", 20)==0 ){
			par.query_mode = "list";
			strcpy(par.output_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-out_2dmatrix", 20)==0 ){
			par.query_mode = "2dmatrix";
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
	     << "\t bin   =\t" << par.in_bins_name << endl;	     
	cout << "Parameters" << endl
	     << "\t ner_bin     =\t"  << par.ner_bin << endl	
	     << "\t sel_chr     =\t"  << par.sel_chr << endl;	     
	cout << "Output" << endl	
	     << "\t output format =\t" << par.query_mode << endl	
	     << "\t output        =\t" << par.output_name << endl;  
}
