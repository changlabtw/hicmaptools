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

	hicmaptools -in_binmap in.binmap -in_bins in.bins -loop|-TAD|-bat|-submap query.bed -output out_file.tsv  
	options:  
        	-in_binmap 	 binary .binmap generate from .n_contact 3CONET-build 
        	-in_bins 	 the bin file for contact map, .bins
        	
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
>hicmaptools -in_binmap nm_none_1000_reduced.bimap -in_bins nm_none_1000.bins -query_interval data/10000_40000_top5.epi_domains -output 10000_40000_top5-contact.tsv


Contact Input (essential)
-------------------------

#####-in_ncontact

  * .n_contact (hicpipeline)
  * Contact map

#####-in_bins

  * .bins (hicpipeline)
  * Bins index
  * 
  
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
