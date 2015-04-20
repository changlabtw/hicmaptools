#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <stdio.h>
#include <string.h>

#include "binmap.h"
#include "index.h"
#include "query.h"
#include "interval.h"
#include "bat.h"
#include "region.h"
#include "couple.h"

using namespace std;

void parse_command_line(int argc, char **argv, char *in_binmap_name, char *in_bins_name, char *query_name, char *output_name, string &query_mode);
void show_param(char *in_binmap_name, char *in_bins_name, char *query_name, char *output_name);

void exit_with_help()
{
	printf(
		"Usage: hicmaptools [options] -in_binmap in.binmap -in_bins in.bins -loop|-TAD|-bat|-submap query.bed -output out_file.tsv\n"
		"options:\n"
		"\t-in_binmap \t binary .binmap generate from .n_contact 3CONET-build \n"
		"\t-in_bins \t the bin file for contact map, .bins\n"
		"\n"
		"\t-loop \t loci gene: chr\tstrat\tend\n"
		"\t-output \t contact between two ends, ie. gene 5' 3'\n"		
		"\n"
		"\t-TAD \t loci interval: chr\tstrat\tend\n"
		"\t-output \t sum/ave contact of the TAD, ie. TAD\n"		
		"\n"
		"\t-bat \t loci bat: chr\tstrat\tend\n"
		"\t-output \t neighboring ave contact of the bat, ie. PcG binding site\n"		
		"\n"
		"\t-submap \t genome region to extract: chr\tstrat\tend\n"
		"\t-output \t sub contact map, ie. 3R:10~15MB\n"		
		"\n"		
		"\t-site \t interesting sites: chr\tstrat\tend\n"
		"\t-output \t contact between those sites\n"
		"\n"
		"\t-local \t interval: chr\tstrat\tend\n"
		"\t-output \t all contacts inside interval\n"
		"\n"		
		"\t-couple \t pair sites: chr1\tstrat1\tend1\tchr2\tstrat2\tend2\n"
		"\t-output \t output pair contacts\n"
		"\n"
		"\nFor instance:\n"
		"\thicmaptools -in_binmap nm_none_1000_reduced.bimap -in_bins nm_none_1000.bins -query_interval data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv\n"
	);
	exit(0);
}

int main(int argc, char *argv[])
{
	char in_binmap_name[1024] = "";
	char in_bins_name[1024] = "";
	char query_name[1024] = "";
	char output_name[1024] = "";
	string query_mode;
	
	parse_command_line(argc, argv, in_binmap_name, in_bins_name, query_name, output_name, query_mode);
	show_param(in_binmap_name, in_bins_name, query_name, output_name);
		
	cout << endl << "[BEGIN]" << endl;
	BINMAP map(in_binmap_name);
	INDEX index(in_bins_name);
	
	if(query_mode == "none")
	{
		map.out_contIne(10000, index, output_name);
	}
	else if(query_mode == "loop")
	{
		QUERY query(query_name, map, index);
		query.output(output_name);
	}
	else if (query_mode == "TAD")
	{
		INTERVAL interval(query_name, map, index);
		interval.gen_internal_contact(map);		
		interval.output(output_name, 1);
	}
	else if (query_mode == "local")
	{
		INTERVAL interval(query_name, map, index);
		
		cout << " generating internal contact " << endl;
		interval.gen_internal_contact(map);
		interval.output_internal(output_name);		
	}	
	else if (query_mode == "bat")
	{
		int bin_the = 500;
		BAT bat(query_name, map, index, bin_the, bin_the);
		
		bat.cal_contact(map);
		bat.output(output_name);	
	}
	else if (query_mode == "site")
	{
		int bin_the = 500;
		BAT bat(query_name, map, index, bin_the, bin_the);
		
		bat.cal_contact(map);
		bat.output_pair(output_name, map);		
	}	
	else if (query_mode == "submap")
	{
		REGION region(query_name, index);		
		region.output(output_name, map);	
	}
	else if (query_mode == "couple")
	{
// need to implement
		COUPLE couple(query_name, index);		
		couple.output(output_name, map);	
	}
		
	cout << endl << "[END]" << endl;
	return 0;
}

//void parse_command_line(int argc, char **argv, char *in_binmap_name, char *in_bins_name, char *query_name, char *output_name, char *output2_name, string& query_mode)
void parse_command_line(int argc, char **argv, char *in_binmap_name, char *in_bins_name, char *query_name, char *output_name, string& query_mode)
{
	int i;
	// default values
	query_mode = "none";
	
	if (argc == 1){
	 exit_with_help();
	}
	
	// parse options
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();
		
		if( strncmp(argv[i-1],"-in_binmap", 20)==0 ){
			strcpy(in_binmap_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-in_bins", 20)==0 ){
			strcpy(in_bins_name, argv[i]);
		}
// query mode
		else if( strncmp(argv[i-1],"-bat", 20)==0 ){
			query_mode = "bat";
			strcpy(query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-TAD", 20)==0 ){
			query_mode = "TAD";
			strcpy(query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-local", 20)==0 ){
			query_mode = "local";
			strcpy(query_name, argv[i]);
		}		
		else if( strncmp(argv[i-1],"-couple", 20)==0 ){
			query_mode = "couple";
			strcpy(query_name, argv[i]);
		}				
		else if( strncmp(argv[i-1],"-submap", 20)==0 ){
			query_mode = "submap";
			strcpy(query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-loop", 20)==0 ){
			query_mode = "loop";
			strcpy(query_name, argv[i]);
		}
		else if( strncmp(argv[i-1],"-site", 20)==0 ){
			query_mode = "site";
			strcpy(query_name, argv[i]);
		}				
// output								
		else if( strncmp(argv[i-1],"-output", 20)==0 ){
			strcpy(output_name, argv[i]);
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

void show_param(char *in_binmap_name, char *in_bins_name, char *query_name, char *output_name)
{
	cout << "Input" << endl
	     << "\t binmap =\t" << in_binmap_name << endl
	     << "\t bins   =\t"   << in_bins_name << endl
	     << "\t query  =\t"  << query_name << endl;

	cout << "Output" << endl
	     << "\t output =\t" << output_name << endl;
}
