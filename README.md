hicmaptools
=================

hicmaptools is a collection of tools for downstream HiC contmap analysis.


Prerequisites
--------------
hicmaptools compilation requires the following tools installed on your system ``make``, ``gcc-c++`` and ``g77``. 


Compile/Installation 
--------------------

Clone the git repository on your computer with the following command: 

    git clone git@github.com:cbcrg/hicmaptools.git hicmaptools
    
    
Make sure you have installed the required dependencies listed above. 
When done, move in the project root folder named ``hicmaptools`` and enter the 
following commands:     
    
    $ cd src
    $ make
    

The binary will be automatically copied to the path ``hicmaptools/bin``.

    
    $ make install
    
 
The binary will be automatically copied to the path specified by the environment 
variable ``$USER_BIN`` (check that it exists before run the make command).

Usage 
--------------------

	hicmaptools -in_map in.binmap -in_bin in.bins SELECT_ONE_QUERY_MODE query.bed -output out_file.tsv  
	options:  
        	-in_map 	 text .n_contact or binary .bimap by genBiMap commend 
        	-in_bin 	 the bin file for contact map, .bins
		
	query modes: 
		-bat 	 	 a loci bat: chr	start	end
        	-output 	 ave neighboring contact of the bat
		
        	-couple 	 pair of sites: chr1	start1	end1	chr2	start2	end2
        	-output 	 contacts between all pairs

        	-local 	 	 a interval: chr	start	end
        	-output 	 all contacts inside interval

        	-loop 	 	 loci gene: chr	start	end
        	-output 	 contact between two ends, ie. 5' and 3' genes
        	
        	-TAD 	 	 loci interval: chr	start	end
        	-output 	 sum/ave contact of the TAD

        	-sites 	 	 interesting sites: chr	start	end
        	-output 	 contact between those sites        	        	

        	-submap 	 genome region to extract: chr	start	end
        	-output 	 sub contact map, ie. 3R:10~15MB
        	
For instance:
>hicmaptools -in_map nm_none_1000_reduced.bimap -in_bin nm_none_1000.bins -query_interval data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv


Contact Input (essential)
-------------------------

##### -in_bin
   define the chromosome, start position and end position of each bin. Format is as the following:
   ```
	cbin	chr	from.coord	to.coord
	1	2L	6000		7000
	2	2L	7000		8000
	3	2L	8000		9000
	4	2L	9000		10000
	5	2L	12000		13000
   ```
##### -in_map
   contact map indexed by bins. Format is as the following: 
   * expected_count : the expected contact between those two chromosome regions (bins) according to model
   * observed_count : the observed contact between those two chromosome regions (bins) by HiC data

   ```
	cbin1	cbin2	expected_count	observed_count
	1	1	0.077080	50
	1	2	0.389912	314
	1	3	0.493750	163
	1	4	0.560505	169
	1	5	0.368884	79
  ```
  
Query Modes  
--------------------------
[bed format](https://genome.ucsc.edu/FAQ/FAQformat.html#format1) : first three required columns are enough.

##### -bat

  * calculate the average contacts +- 5000bps of the interested position
  * PcG binding sites

##### -couple

  * Contact of the pair
  * a enhancer vs a promoter

##### -local

  * List all contacts inside the interval
  * Check intra-contacts of TAD

##### -loop

  * Contact between two ends
  * Check whether gene 5’ & 3’ form loop or not

##### -TAD

  * sum./ave. contacts among the interval
  * PcG/Active TAD different contact property

##### -sites

  * Contact between these sites
  * Calculate contact for PcG pair sites

##### -submap

  * Contact map of interesting region
  * Extract contacts of specific genome region

Illustration for different query options 
![](https://github.com/cbcrg/hicmaptools/blob/master/doc/queryExample.png)

There will generate two output files after excuting hicmaptools commands :
  * First	 : Includes the processed information you interested.
  * Second: Includes all the random data from the assigned random size.

Random Test
--------------------
