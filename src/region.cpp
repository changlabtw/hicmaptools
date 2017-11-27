#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>

#include "region.h"
#include "index.h"

REGION::REGION()
{
}

// load map from binary file
REGION::REGION(const char *file_name, INDEX &index)
{
	fstream input_f;
	string str;
	stringstream ss;	
	REGION_E tmp;

	input_f.open(file_name, ios_base::in);
	if(!input_f)
	{
		system("hostname");
		perror(file_name);
		exit(0);
	}
	else
	{
		cout << "\treading file =\t" << file_name << endl;
	}

	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
			if ( ss >> tmp.chrom >> tmp.start >> tmp.end)
			{			
				// find cbin for position		
				tmp.sbin = index.find_index(tmp.chrom, tmp.start, 1, 0);
				tmp.ebin = index.find_index(tmp.chrom, tmp.end, 0, 1);

				region_vec.push_back(tmp);	
			}
		}		
	}
	input_f.close();
	input_f.clear();
}

REGION::~REGION()
{
}

void REGION::output(const char *OutputfileName, BINMAP &binmap)
{
	int counter=1;
	for(vector< REGION_E >::iterator iter = region_vec.begin(); iter != region_vec.end(); iter++)
	{

		//generate output file	
		stringstream ss;
		string filename=(string)OutputfileName;
		int found = filename.find_last_of(".");
		filename = filename.substr(0,found);
		ss << filename << "_" << counter << ".txt";		

		binmap.out_subcontact(iter->sbin,iter->ebin,ss.str().c_str());
		counter++;
	}
}
