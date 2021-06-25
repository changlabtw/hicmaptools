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

// straw .hic
#include "straw.h"
#include <map>

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
		"inputs:\n"
		"** Usage for either .binmap and .bin files or .hic file **\n"
		"\n"
		"  Usage for binmap and bin format: hicmaptools [options] -in_map in.binmap -in_bin in.bins [query]\n"
		"\t-in_map \t binary .binmap generate from .n_contact 3CONET-build \n"
		"\t-in_bin \t the bin file for contact map, .bins\n"
		"\n"
		"  Usage for .hic format: hicmaptools [options] -in_hic_name in.hic -in_hic_norm NONE|VC|VC_SORT|KR|OE -in_hic_res 10000 [query]\n"
		"\t-in_hic \t binary .hic generate from juicer \n"
		"\t-in_hic_norm \t the normalization method\n"
		"\t-in_hic_rsol \t specify the resolution in .hic format\n"
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
		"\t-random N \t random sampling corresponding N contacts. No random sampling if N = 0 [default: 100 contacts]\n"
		"\t-ner_bin N\t consider within +- N bins [default: 10 bins]\n"
		"\nFor instance:\n"
		"\thicmaptools -in_map nm_none_1000_reduced.bimap -in_bin nm_none_1000.bins -TAD data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv\n"
		"\tor\n"
		"\thicmaptools -in_hic_name file.hic -in_hic_norm NONE -in_hic_chr 20 -in_hic_res 100000 -TAD data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv\n"
	);
	exit(0);
}

int main(int argc, char *argv[])
{
	
	string matrix = "observed";
	string unit = "BP";
	
// parse data
	PARAMETER par;
	
	parse_command_line(argc, argv, par);
	show_param(par);

//	for random
	srand((unsigned)time(NULL)); 

	cout << endl << "[BEGIN]" << endl;
	
	BINMAP map_;
	INDEX index;
	
	if(strncmp(par.in_hic, "NONE", 10) != 0){
		map <string, chromosome> chromosomeMap = getAllChr(par.in_hic);
		vector<contactRecord> records;
		for(map <string, chromosome>::iterator it = chromosomeMap.begin(); it != chromosomeMap.end(); it++){
			//		cout << it->second.index << "\t" << it->first << endl;
			if(it->first == "ALL")
				continue;
			vector<contactRecord> temp = straw(matrix, par.in_hic_norm, par.in_hic, it->first, it->first, unit, par.in_hic_rsol);
			sortContactRecord(temp);
			map_.insert_from_hic(temp, par.in_hic_rsol, index.get_cbin_number());
			index.insert_from_hic(temp, par.in_hic_rsol, it->first);
		}
		//	records = straw(matrix, par.in_hic_norm, par.in_hic_name, par.in_hic_chr, par.in_hic_chr, unit, par.in_hic_res);
		//	sortContactRecord(records);
		//	map = BINMAP(records, par.in_hic_res);
		//	index = INDEX(records, par.in_hic_res, par.in_hic_chr);
	}
	else{
		map_ = BINMAP(par.in_binmap_name);
		index = INDEX(par.in_bins_name);
	}
	
/*
	for(int i=1; i <= 20;i++){
		INDEX_ELE test = index.get_index(i);
		cout << i << "\t" << test.chr << "\t" << test.start << "\t" << test.end <<endl;
	}
	cout << endl;

	cout << "bin1\tbin2\tobserved" << endl;
	for(int i=1; i<= 20;i++){
		cout << "1" << "\t" << i << "\t" << map_.get_observe(1,i) << endl;
	}
*/
	if(par.query_mode == "none")
	{
		map_.out_contIne(10000, index, par.output_name);
	}
	else if(par.query_mode == "loop")
	{
		QUERY query(par.query_name, map_, index, par.random_size, par.output_name);
		query.output(par.output_name);
	}
	else if (par.query_mode == "TAD")
	{
		INTERVAL interval(par.query_name, map_, index);
		interval.gen_internal_contact(map_, index, par.random_size, par.output_name);
		interval.output(par.output_name, 1);
	}
	else if (par.query_mode == "local")
	{
		INTERVAL interval(par.query_name, map_, index);
		
		cout << " generating internal contact " << endl;
		interval.gen_internal_contact(map_, index, par.random_size, par.output_name);
		interval.output_internal(par.output_name);		
	}	
	else if (par.query_mode == "bat")
	{
        BAT bat(par.query_name, index, par.ner_bin, par.ner_bin);
        
        bat.cal_contact(map_, index, par.ner_bin, par.ner_bin, par.random_size, par.output_name);
        bat.output(par.output_name);
	}
	else if (par.query_mode == "site")
	{
		BAT bat(par.query_name, index, par.ner_bin, par.ner_bin);
		
		bat.output_pair(par.output_name, map_, index, par.random_size, par.output_name);    
	}	
	else if (par.query_mode == "submap")
	{
		REGION region(par.query_name, index);		
		region.output(par.output_name, map_);	
	}
	else if (par.query_mode == "couple")
	{
		COUPLE couple(par.query_name, index);	
		couple.cal_contact(map_, index, par.random_size, par.output_name);
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
// hic format
		else if( strncmp(argv[i-1],"-in_hic", 20)== 0){
			strcpy(par.in_hic, argv[i]);
		}
		else if( strncmp(argv[i-1],"-in_hic_norm", 20)== 0){
			strcpy(par.in_hic_norm, argv[i]);
		}
		else if( strncmp(argv[i-1],"-in_hic_rsol", 20)== 0){
			par.in_hic_rsol = stoi(argv[i]);
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
	// show bin and map input
	if (strncmp(par.in_hic, "NONE", 10) == 0){
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
	else{ // show hic input
		cout << "Input" << endl
			<< "\t hic          =\t" << par.in_hic << endl
			<< "\t normalization=\t" << par.in_hic_norm << endl
			<< "\t resolution   =\t" << par.in_hic_rsol << endl
			<< "\t query =\t" << par.query_name << endl;

		cout << "Parameters" << endl
			<< "\t ner_bin     =\t"  << par.ner_bin << endl
			<< "\t random size =\t"  << par.random_size << endl;

		cout << "Output" << endl
			<< "\t output =\t" << par.output_name << endl;  

	}
}
