#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno> // for errno 
#include <string.h>
#include <cstdlib>
#include <limits>
#include <vector>
#include <math.h> 
#include <iomanip>

#include "binmap.h"

struct NCON_ELM
{
	int cbin1;
	int cbin2;
	float exp;
	float obs;
};

BINMAP::BINMAP()
{
}


// load map from binary file
BINMAP::BINMAP(const char *file_name)
{
	ifstream input_f;
	string str, name, seq, tmp;
	stringstream ss;
	int cbin1, cbin2;
	float exp, obs;
	std::string file_n(file_name);
	pair<int, int> tmp_pair;
	
// load map from binary or text	
	string binary_format = ".binmap";
	string text_format   = ".n_contact";
	
	std::size_t b_found = file_n.find(binary_format);
	std::size_t t_found = file_n.find(text_format);

// opening file
	if (b_found!=std::string::npos) {
		input_f.open(file_name, ios::binary);
	}
	else if (t_found!=std::string::npos) {
		input_f.open(file_name, ios_base::in);
	}
	else{
		cout << "\twarning : input format not in .binmap or .n_contact " << file_name << endl;	
		input_f.open(file_name, ios_base::in);
	}

	if(!input_f){
			system("hostname");
			perror(file_name);
			exit(0);
	}
	else{
		cout << "\treading contact file =\t" << file_name << endl;
	}
	//checking empty file
	if(input_f.peek() == std::ifstream::traits_type::eof()){
		cout << "empty query file" << endl;
		exit(-1);
	}

	
// load map from binary : .binmap	
	if (b_found!=std::string::npos)
	{		
		input_f.seekg(0,ifstream::end);
		long f_size = input_f.tellg();
		const size_t count = input_f.tellg()/sizeof(NCON_ELM);
		vector<NCON_ELM> tmp_vec(count);
	
		input_f.seekg(0,ifstream::beg);
		input_f.read(reinterpret_cast<char*>(&tmp_vec[0]), f_size);
			
		for(vector<NCON_ELM>::iterator iter = tmp_vec.begin(); iter != tmp_vec.end();iter++)
		{
			observe_map.insert(make_pair(make_pair(iter->cbin1,iter->cbin2),iter->obs));
			expect_map.insert(make_pair(make_pair(iter->cbin1,iter->cbin2),iter->exp));
		}
	}
// load map from text : .n_contact
	else if (t_found!=std::string::npos)
	{
		while(!input_f.eof())
		{
//			cout << "1";
			getline(input_f, str);
			if((int)str.length() > 0)
			{
				ss.clear(); ss.str(str);
	// handle for the first line which might contain header
				if(	ss >> cbin1 >> cbin2 >> exp >> obs )
				{
					tmp_pair = make_pair(cbin1, cbin2);
					observe_map.insert(make_pair(tmp_pair ,obs));
					expect_map.insert(make_pair(tmp_pair, exp));
				}
			}		
		}		
	}
// load map from other formats
	else
	{
		while(!input_f.eof())
		{
			getline(input_f, str);
			if((int)str.length() > 0)
			{
				ss.clear(); ss.str(str);
	// handle for the first line which might contain header
				if(	ss >> cbin1 >> cbin2 >> obs )
				{
					observe_map.insert(make_pair(make_pair(cbin1,cbin2),obs));
					expect_map.insert(make_pair(make_pair(cbin1,cbin2),obs));
				}
			}
		}		
	}

// output reading information
	cout << "\tmap size             =\t" << (int)observe_map.size() << endl << endl;
	
// close file	
	input_f.close();
	input_f.clear();

#ifdef DEBUG
	cout << "[DEBUG] show n_contact" << endl;
		int i = 0;
		for(map< pair<int, int>, float >::iterator iter = observe_map.begin(); iter != observe_map.end() && i < 5;iter++, i++)
		{
			cout << iter->first.first << "\t" << iter->first.second  << "\t" << iter->second << endl;
		}
		cout << endl;
		i = 0;
		for(map< pair<int, int>, float >::iterator iter = expect_map.begin(); iter != expect_map.end() && i < 5;iter++, i++)
		{
			cout << iter->first.first << "\t" << iter->first.second  << "\t" << iter->second << endl;
		}

    	cout << "the last n_contact" << endl;
		map< pair<int, int>, float >::iterator i1 = observe_map.end();
		--i1;
		cout << i1->first.first << "\t" << i1->first.second  << "\t" << i1->second << endl;

		map< pair<int, int>, float >::iterator i2 = expect_map.end();
		--i2;
		cout << i2->first.first << "\t" << i2->first.second  << "\t" << i2->second << endl;
#endif
	
}

// create map from n_contact file
BINMAP::BINMAP(const char *input_name, const char *output_name)
{
	fstream input_f;
	ofstream output_f;	
	string str;
	stringstream ss;
	vector<NCON_ELM> tmp_vec;
	NCON_ELM tmp_elm;

	output_f.open(output_name, ios::binary);
	input_f.open(input_name, ios_base::in);
	if((!input_f)||(!output_f))
	{
		system("hostname");
		perror(input_name);
		perror(output_name);
		exit(0);
	}
	else
	{
		cout << "reading file \t" << input_name << endl;
		cout << "output file \t" << output_name << endl;
	}
	
	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0)
		{
			ss.clear(); ss.str(str);
// handle for the first line which might contain header
			if(	ss >> tmp_elm.cbin1 >> tmp_elm.cbin2 >> tmp_elm.exp >> tmp_elm.obs )
			{
				tmp_vec.push_back(tmp_elm);
			}
		}		
	}
	cout << "writing n_contact size =\t" << (int)tmp_vec.size() << endl;
// close file
	input_f.close();
	input_f.clear();

#ifdef DEBUG	
	cout << "DEBUG: show n_contact" << endl;
	for(vector<NCON_ELM>::iterator iter = tmp_vec.begin(); iter != tmp_vec.end();iter++)
	{
		cout << iter->cbin1 << "\t" << iter->cbin2  << "\t" << iter->exp << "\t" << iter->obs << endl;
	}
#endif

	output_f.write(reinterpret_cast<const char*>(&tmp_vec[0]), tmp_vec.size()*sizeof(NCON_ELM));
	output_f.close();
	output_f.clear();	
}

BINMAP::~BINMAP()
{
}

// get observe contact from contact map
float BINMAP::get_observe( const int i, const int j )
{
	pair<int, int> index;

// contact map index : bin1 < bin2	
	if(i <= j)
		index = make_pair(i, j);
	else	
		index = make_pair(j, i);
		
	return ( observe_map.find(index) != observe_map.end() ) ? observe_map[index] : -1;		
};

// get expect contact from contact map
float BINMAP::get_expect( const int i, const int j )
{
	pair<int, int> index;

// contact map index : bin1 < bin2	
	if(i <= j)
		index = make_pair(i, j);
	else	
		index = make_pair(j, i);

	return ( expect_map.find(index) != expect_map.end() ) ? expect_map[index] : -1;
};

void BINMAP::out_subcontact( const int start_bin, const int end_bin, const char *fileName )
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
	output_f << "cbin1\tcbin2\tobserved_count\texpected_count" << endl;
	for(map< pair<int, int>, float >::iterator iter = expect_map.begin(); iter != expect_map.end(); iter++)
	{
		if ((iter->first.first >= start_bin) && (iter->first.first <= end_bin) && (iter->first.second >= start_bin) && (iter->first.second <= end_bin)) 
			output_f << iter->first.first << "\t" << iter->first.second  << "\t" << observe_map[iter->first] << "\t" << iter->second << endl;
	}	
	
	output_f.close();			
};

void BINMAP::out_contIne( const int cis_thre, INDEX &index, char *prefix )
{
	map< int, int > tran_m;
	map< int, int > cis_far_m; // cis >= 10k
	map< int, int > cis_col_m; // cis < 10k
	stringstream ss;
	INDEX_ELE index1;
	INDEX_ELE index2;

	for(map< pair<int, int>, float >::iterator iter = observe_map.begin(); iter != observe_map.end(); iter++)
	{
		index1 = index.get_index(iter->first.first);
		index2 = index.get_index(iter->first.second);
		
// add count to specify class
		if(index1.chr != index2.chr){
			tran_m[iter->first.first]  += iter->second;
			tran_m[iter->first.second] += iter->second;
		}
		else if (abs(index1.start-index2.start) >= cis_thre){
			cis_far_m[iter->first.first]  += iter->second;
			cis_far_m[iter->first.second] += iter->second;
		}
		else{
			cis_col_m[iter->first.first]  += iter->second;
			cis_col_m[iter->first.second] += iter->second;
		}		
	}	

// output tran	
	ofstream tra_f;
	tra_f.open (strcat(prefix,"_tran.bed"));
	if(!tra_f)
	{
		system("hostname");
		perror(strcat(prefix,"_tran.bed"));
		exit(0);
	}
	for(map< int, int >::iterator iter = tran_m.begin(); iter != tran_m.end(); iter++)
	{
		index1 = index.get_index(iter->first);
		tra_f << "chr" << index1.chr << "\t" << index1.start  << "\t" << index1.end << "\tcbin" << iter->first << "\t" << iter->second  << "\t+" << endl;
	}	
	tra_f.close();			

//output far-cis
	ofstream far_cis_f;
	far_cis_f.open (strcat(prefix,"_far-cis.bed"));
	if(!far_cis_f)
	{
		system("hostname");
		perror(strcat(prefix,"_far-cis.bed"));
		exit(0);
	}
	for(map< int, int >::iterator iter = cis_far_m.begin(); iter != cis_far_m.end(); iter++)
	{
		index1 = index.get_index(iter->first);
		far_cis_f << "chr" << index1.chr << "\t" << index1.start  << "\t" << index1.end << "\tcbin" << iter->first << "\t" << iter->second  << "\t+" << endl;
	}	
	far_cis_f.close();			

//output close-cis
	ofstream col_cis_f;
	col_cis_f.open (strcat(prefix,"_col-cis.bed"));
	if(!col_cis_f)
	{
		system("hostname");
		perror(strcat(prefix,"_col-cis.bed"));
		exit(0);
	}
	for(map< int, int >::iterator iter = cis_col_m.begin(); iter != cis_col_m.end(); iter++)
	{
		index1 = index.get_index(iter->first);
		col_cis_f << "chr" << index1.chr << "\t" << index1.start  << "\t" << index1.end << "\tcbin" << iter->first << "\t" << iter->second  << "\t+" << endl;
	}	
	col_cis_f.close();			

};

void BINMAP::out2list( INDEX &index, PARAMETER par )
{	
	INDEX_ELE index1;
	INDEX_ELE index2;
	
	char *f_name = par.output_name;
	const int bin_dis = par.ner_bin;
	
// open output list file
	ofstream o_f;
	o_f.open (f_name);
	if(!o_f)
	{
		system("hostname");
		perror(f_name);
		exit(0);
	}

	for(map< pair<int, int>, float >::iterator iter = observe_map.begin(); iter != observe_map.end(); iter++)
	{
// only output half part of matrix & two bins in certain distance
		if((iter->first.second > iter->first.first) && (iter->first.second - iter->first.first) > bin_dis)
		{
			index1 = index.get_index(iter->first.first);
			index2 = index.get_index(iter->first.second);	
			if(iter->second > 0){
				float output_cont=(par.useNormal) ? log((expect_map[iter->first]+std::numeric_limits<float>::min())/(iter->second+std::numeric_limits<float>::min())) : iter->second;
				
				o_f << "chr" << index1.chr << "\t" << index1.start  << "\t" << index1.end << "\t"
					<< "chr" << index2.chr << ":"  << index2.start  << "-" << index2.end  << "," << output_cont << "\t1\t." << endl
					<< "chr" << index2.chr << "\t" << index2.start  << "\t" << index2.end << "\t"
					<< "chr" << index1.chr << ":"  << index1.start  << "-" << index1.end  << "," << output_cont << "\t2\t." << endl;
			}
		}	
	}
	o_f.close();
};

void BINMAP::out2matrix( INDEX &index, PARAMETER par )
{
	char *f_name = par.output_name;
	const string sel_chr = par.sel_chr; 
	INDEX_ELE index1;
	INDEX_ELE index2;
	pair<int, int> index_range = index.get_index_range(sel_chr);
	INDEX_ELE tmp_index;
	float obs, exp;
		
	ofstream o_f;
	o_f.open (f_name);
	if(!o_f)
	{
		system("hostname");
		perror(f_name);
		exit(0);
	}
	
	if ((index_range.first == -1)||(index_range.second == -1)){
		cout<< "[ERROR] no bins for select chromosome " << sel_chr << endl;
		exit(0);
	}
	
	for(int i = index_range.first; i <= index_range.second; i++)
	{
		tmp_index = index.get_index(i);
		o_f << tmp_index.chr << "\t" << tmp_index.start << "\t" << tmp_index.end << "\t";

		for(int j = index_range.first; j <= index_range.second; j++)
		{
			obs = get_observe(i, j);
			exp = get_expect(i, j);
			
			if((obs != -1) && (exp != -1)){
				if(par.useNormal){
					o_f << fixed << setprecision(2) << log((obs+std::numeric_limits<float>::min())/(exp+std::numeric_limits<float>::min()));					
				}else{
					o_f << obs;
				}
			}else
				o_f << 0;
				
			o_f << "\t";
		}
		o_f << endl;
	}	

	o_f.close();
};

// load map from .hic file
void BINMAP::insert_from_hic(vector<contactRecord> & records, int binsize, long cbin_number)
{
	for(vector<contactRecord>::iterator iter = records.begin(); iter != records.end(); iter ++)
	{
		observe_map.insert(make_pair(make_pair((iter->binX / binsize) + cbin_number, (iter->binY / binsize) + cbin_number), iter->counts));
		expect_map.insert(make_pair(make_pair((iter->binX / binsize) + cbin_number, (iter->binY / binsize)+ cbin_number), 1));
	}

	//output reading information
	//cout << "\tmap size                 =\t" << (int)observe_map.size() << endl << endl;
}

