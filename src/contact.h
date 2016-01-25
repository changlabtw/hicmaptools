#ifndef _CONTACT_H
#define _CONTACT_H

struct CONTACT_E
{
// observe, expect, normalization of query 	
	int num_bins;
	float quer_obs;
	float quer_exp;
	float quer_nor;
	
// observe, expect, normalization for random sample	
	float rand_obs;
	float rand_exp;
	float rand_nor;
	
// the rank of observe, expect, normalization among random sampling, in top sense
	float rank_obs;
	float rank_exp;
	float rank_nor;
	
	CONTACT_E()
    {
        num_bins=0;
        quer_obs=0;
        quer_exp=0;
        quer_nor=0;
        
        rand_obs=0;
        rand_exp=0;
        rand_nor=0;
        
        rank_obs=0;
        rank_exp=0;
        rank_nor=0;
    }		
    
    void out_header(ofstream &out_f){
    	out_f	<< "pairs\t"
    			<< "quer_obs\tquer_exp\tquer_nor\t" 
	         	<< "rand_obs\trand_exp\trand_nor\t"
	         	<< "divi_obs\tdivi_exp\tdivi_nor\t"
	         	<< "rank_obs\trank_exp\trank_nor";    	
    }
 
    void output(ofstream &out_f){
 // specify format for printing float point   
		out_f.setf(ios::fixed, ios::floatfield); // set fixed floating format
		out_f.precision(3); // for fixed format, two decimal places    
		         
		out_f << num_bins << "\t" 
			  << quer_obs << "\t" << quer_exp << "\t" << quer_nor << "\t"
		      << rand_obs << "\t" << rand_exp << "\t" << rand_nor << "\t"
		      << quer_obs/rand_obs << "\t" << quer_exp/rand_exp << "\t" << quer_nor/rand_nor << "\t" 
		      << rank_obs << "\t" << rank_exp << "\t" << rank_nor;
    }
};

#endif /* _COUPLE_H */
