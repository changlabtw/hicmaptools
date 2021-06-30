#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <stdio.h>
#include <string.h>

#include "binmap.h"
#include "index.h"

using namespace std;

void parse_command_line(int argc, char **argv, char *in_ncontact_name, char *out_binmap_name);
void show_param(char *in_ncontact_name, char *out_binmap_name);

void exit_with_help()
{
	printf(
		"Turn .n_contact into binary format, .binmap\n"	
		"Usage: genBinMap [options] -in_ncontact input.n_contact -out_binmap out.binmap\n"
		"options:\n"
		"\t-in_ncontact \t HiC contact map in .n_contact format\n"
		"\t-out_binmap \t HiC contact in binary format .binmap\n"
		"\nFor instance:\n"
		"genBinMap -in_ncontact example/cm_uniform_1000.n_contact -out_binmap example/1000.binmap\n"
	);
	exit(0);
}

int main(int argc, char *argv[])
{
	char in_ncontact_name[1024];
	char out_binmap_name[1024];
	
	parse_command_line(argc, argv, in_ncontact_name, out_binmap_name);
	show_param(in_ncontact_name, out_binmap_name);

	cout << endl << "[BEGIN]" << endl;
	BINMAP map(in_ncontact_name, out_binmap_name);

#ifdef DEBUG	
	BINMAP debug_map(out_binmap_name);
#endif
	
	cout << endl << "[END]" << endl;
	return 0;
}

void parse_command_line(int argc, char **argv, char *in_ncontact_name, char *out_binmap_name)
{
	int i;
	// default values
	
	if (argc == 1){
	 exit_with_help();
	}
	
	// parse options
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();
		
		if( strncmp(argv[i-1],"-in_ncontact", 20)==0 ){
			strcpy(in_ncontact_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-out_binmap", 20)==0 ){
			strcpy(out_binmap_name, argv[i]);
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

void show_param(char *in_ncontact_name, char *out_binmap_name)
{
	cout << "Input" << endl
	 << "\t n_contact =\t"  << in_ncontact_name << endl
     << "Output binary" << endl
	 << "\t binmap =\t"  << out_binmap_name << endl;
}
