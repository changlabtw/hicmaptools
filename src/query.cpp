#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <cstdlib>

#include "query.h"
#include "index.h"

QUERY::QUERY()
{
}

// load map from binary file
QUERY::QUERY(const char *file_name, BINMAP &binmap, INDEX &index)
{
	fstream input_f;
	string str;
	stringstream ss;	
	BINCONT tmp;
	
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
	
// handle for the first line which might contain header
	getline(input_f, str);
	if (((int)str.length() > 0) && std::isdigit(str[0])) 
	{
		ss.clear(); ss.str(str);
		ss >> tmp.chrom >> tmp.pos1 >> tmp.pos2;

// find cbin for position		
		tmp.cbin1 = index.find_index(tmp.chrom, tmp.pos1, 1, 0);
  		tmp.cbin2 = index.find_index(tmp.chrom, tmp.pos2, 0, 1);

// get contact from bin
			if ((tmp.cbin1 != -1) && (tmp.cbin2 != -1)){ 
				tmp.obs = binmap.get_observe(tmp.cbin1, tmp.cbin2);
  				tmp.exp = binmap.get_expect(tmp.cbin1, tmp.cbin2);
  			}
  			else{
  				tmp.obs = tmp.exp = -1;
  			}		
		  				
		bincont_vec.push_back(tmp);		
	}		
	
	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
			ss >> tmp.chrom >> tmp.pos1 >> tmp.pos2;
			
// find cbin for position		
			tmp.cbin1 = index.find_index(tmp.chrom, tmp.pos1, 1, 0);
  			tmp.cbin2 = index.find_index(tmp.chrom, tmp.pos2, 0, 1);

// get contact from bin
			if ((tmp.cbin1 != -1) && (tmp.cbin2 != -1)){ 
				tmp.obs = binmap.get_observe(tmp.cbin1, tmp.cbin2);
  				tmp.exp = binmap.get_expect(tmp.cbin1, tmp.cbin2);
  			}
  			else{
  				tmp.obs = tmp.exp = -1;
  			}	
  				
			bincont_vec.push_back(tmp);	
		}		
	}
		
	input_f.close();
	input_f.clear();
}

QUERY::~QUERY()
{
}

void QUERY::output(const char *fileName)
{
	ofstream output_f;
	output_f.open (fileName);
	
	if(!output_f)
	{
		system("hostname");
		perror(fileName);
		exit(0);
	}
	
// print header 
	output_f << "chrom\tpos1\tpos2\tcbin1\tcbin2\tobser_contact\texpect_contact" << endl;
		
	for(vector<BINCONT>::iterator iter = bincont_vec.begin(); iter != bincont_vec.end(); iter++)
	{
		output_f << iter->chrom << "\t" << iter->pos1 << "\t" << iter->pos2
			<< "\t" << iter->cbin1 << "\t" << iter->cbin2 
			<< "\t" << iter->obs << "\t" << iter->exp << endl;
	}
	output_f.close();
}
