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
#include "index.h"
#include "ginter.h"

// for map<CHR_ELE, int>
bool operator<(const CHR_ELE &l_CHR_ELE, const CHR_ELE &r_CHR_ELE) {
    if (l_CHR_ELE.chr == r_CHR_ELE.chr) {
    	if (l_CHR_ELE.start == r_CHR_ELE.start) {
    	        return l_CHR_ELE.end < r_CHR_ELE.end;
    	}
    	return l_CHR_ELE.start < r_CHR_ELE.start;
    }
    return l_CHR_ELE.chr < r_CHR_ELE.chr;
}

void parse_ginteraction(const char *input_name, BINMAP &binmap, INDEX &index)
{
	ifstream input_f;
	string str;
	stringstream ss;
	float obs; 
	CHR_ELE tmp1, tmp2;	
	vector<CHR_ELE> tmpVec1, tmpVec2;
	vector<float> obsVec;
	map<CHR_ELE, int> CHR2int;
	int binIndex = 1;
	
	input_f.open(input_name, ios_base::in);
	while(!input_f.eof())
	{
		getline(input_f, str);
		if((int)str.length() > 0){
			ss.clear(); ss.str(str);
			// handle for the first line which might contain header
			// parse chr2L	5000	5100	chr2L	5500	5600	3
			if(	ss >> tmp1.chr >> tmp1.start >> tmp1.end 
				   >> tmp2.chr >> tmp2.start >> tmp2.end
				   >> obs ){
				tmpVec1.push_back(tmp1);
				tmpVec2.push_back(tmp2);
				obsVec.push_back(obs);
				
				if(CHR2int.find(tmp1) == CHR2int.end()){
					CHR2int.insert(make_pair(tmp1, binIndex));
					binIndex++;
				}					
			}
		}
	}
	
	cout << "CHR2int size " << (int)CHR2int.size() << endl;
	
	// check vector size
	if( (int)tmpVec1.size() != (int)tmpVec2.size() ){
		cout << "ERROR tmpVec1 size !=  tmpVec2 size " 
			 << (int)tmpVec1.size()  << " " << (int)tmpVec2.size() << endl;
		exit(-1);
	}
	
	if( (int)tmpVec1.size() != (int)obsVec.size() ){
		cout << "ERROR tmpVec1 size !=  obsVec size " 
			 << (int)tmpVec1.size()  << " " << (int)obsVec.size() << endl;
		exit(-1);
	}
		
	//insert data into map
	for (int i = 0; i < tmpVec1.size(); i++) {
		tmp1 = tmpVec1[i];
		tmp2 = tmpVec2[i];
		obs = obsVec[i];
		
		if( (CHR2int.find(tmp1) != CHR2int.end()) && (CHR2int.find(tmp2) != CHR2int.end()) ){
			binmap.insert_obs(CHR2int[tmp1], CHR2int[tmp2], obs);
			binmap.insert_exp(CHR2int[tmp1], CHR2int[tmp2], 1);
		}
// skip to reduce IO
//		else{
//			if (CHR2int.find(tmp1) == CHR2int.end()){
//				cout << "WARNNING: key is not inside hash, " << tmp1.chr << " " << tmp1.start << " " << tmp1.end << endl;
//			}
//			
//			if (CHR2int.find(tmp2) == CHR2int.end()){
//				cout << "WARNNING: key is not inside hash, " << tmp2.chr << " " << tmp2.start << " " << tmp2.end << endl;
//			}
//		}
    }
	cout << binmap.get_obs_size() << endl;

	//insert data into index 
	index.insert4ginteraction(CHR2int);
}
