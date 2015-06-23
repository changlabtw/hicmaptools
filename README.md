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

	hicmaptools -in_map in.binmap -in_bin in.bins -loop|-TAD|-bat|-submap query.bed -output out_file.tsv  
	options:  
        	-in_map 	 binary .binmap generate from .n_contact 3CONET-build 
        	-in_bin 	 the bin file for contact map, .bins
        	
        	-loop 	 loci gene: chr	strat	end
        	-output 	 contact between two ends, ie. gene 5' 3'
        	
        	-TAD 	 loci interval: chr	strat	end
        	-output 	 sum/ave contact of the TAD, ie. TAD
        	
        	-bat 	 loci bat: chr	strat	end
        	-output 	 neighboring ave contact of the bat, ie. PcG binding site
        	
        	-submap 	 genome region to extract: chr	strat	end
        	-output 	 sub contact map, ie. 3R:10~15MB
        	
        	-site 	 interesting sites: chr	strat	end
        	-output 	 contact between those sites
        	
        	-local 	 interval: chr	strat	end
        	-output 	 all contacts inside interval
        	
        	-couple 	 pair sites: chr1	strat1	end1	chr2	strat2	end2
        	-output 	 output pair contacts


For instance:
>hicmaptools -in_map nm_none_1000_reduced.bimap -in_bin nm_none_1000.bins -query_interval data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv


Contact Input (essential)
-------------------------

#####-in_bin
   define the chromosome, start position and end position of each bin. Format is as the following:
   ```
	cbin	chr	from.coord	to.coord
	1		2L	6000	7000
	2		2L	7000	8000
	3		2L	8000	9000
	4		2L	9000	10000
	5		2L	12000	13000
   ```
#####-in_map
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
  
Query Input  
--------------------------
[bed format](https://genome.ucsc.edu/FAQ/FAQformat.html#format1) : first three required columns are enough.

#####-TAD

  * sum./ave. contacts among the interval
  * PcG/Active TAD different contact property

#####-local

  * List all contacts inside the interval
  * Check intra-contacts of TAD

#####-pair

  * Contact of the pair
  * Contact of given enhancer vs promoter

#####-submap

  * Contact map of interesting region
  * Extract contacts of specific genome region

#####-loop

  * Contact between two ends
  * Check whether gene 5’ & 3’ form loop or not

#####-site

  * Contact between these sites
  * Calculate contact for PcG pair sites

Illustration for different query options 
![](https://github.com/cbcrg/hicmaptools/blob/master/doc/outline.jpg)
