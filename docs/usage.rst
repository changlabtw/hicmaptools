How to use HiCmappTools
=============================

Commend
-----

::

    hicmaptools -in_map in.binmap -in_bin in.bins SELECT_ONE_QUERY_MODE query.bed -output out_file.tsv  

    options:  
            -in_map      text .n_contact or binary .bimap by genBiMap commend 
            -in_bin      the bin file for contact map, .bins
        
    query modes: 
        -bat         a loci bat: chr    start   end
            -output      ave neighboring contact of the bat
        
            -couple      pair of sites: chr1    start1  end1    chr2    start2  end2
            -output      contacts between all pairs

            -local       a interval: chr    start   end
            -output      all contacts inside interval

            -loop        loci gene: chr start   end
            -output      contact between two ends, ie. 5' and 3' genes
            
            -TAD         loci interval: chr start   end
            -output      sum/ave contact of the TAD

            -sites       interesting sites: chr start   end
            -output      contact between those sites                        

            -submap      genome region to extract: chr  start   end
            -output      sub contact map, ie. 3R:10~15MB
        
    other parameters:
            -ner_bin     check neighbouring bins for bat mode, d.f=10
            -random      assign random size, d,f=500
        
        
For instance: >hicmaptools -in\_map nm\_none\_1000\_reduced.bimap
-in\_bin nm\_none\_1000.bins -query\_interval
data/10000\_40000\_top5.epi\_domains -output
10000\_40000\_top5-contact.tsv

Query Modes
-----------

`bed format <https://genome.ucsc.edu/FAQ/FAQformat.html#format1>`__ :
first three required columns are enough.

-bat
''''

-  calculate the average contacts +- 5000bps of the interested position
-  PcG binding sites

-couple
'''''''

-  Contact of the pair
-  a enhancer vs a promoter

-local
''''''

-  List all contacts inside the interval
-  Check intra-contacts of TAD

-loop
'''''

-  Contact between two ends
-  Check whether gene 5’ & 3’ form loop or not

-TAD
''''

-  sum./ave. contacts among the interval
-  PcG/Active TAD different contact property

-sites
''''''

-  Contact between these sites
-  Calculate contact for PcG pair sites

-submap
'''''''

-  Contact map of interesting region
-  Extract contacts of specific genome region

-output
'''''''

There will generate two output files after excuting hicmaptools commands
: \* First : Includes the processed information you interested. \*
Second : Includes all the random data from the assigned random size.

Illustration for different query options |image0|

Example
-------

Suppose you have such files below: \* map file :
nm\_none\_30000.n\_contact \* bin file : 30000.cbins \* query input file
: BATtest.txt

And you want to use the query such as ``-bat``

use the command : >hicmaptools -in\_map nm\_none\_30000.n\_contact
-in\_bin 30000.cbins -bat BATtest.txt -output temp.txt

temp : output name you assign

You will get two output files : \* temp.txt : processed information \*
temp\_random.txt : random data

When you open the temp.txt, you may see:

::

    index   chrom   start   end ... rank_obs    rank_exp    rank_nor    
    1   3R  100000  200000  ...     0.880       0.990       0.760

You may concern whether the rank information are conviced, so you can
use the tool we support to examine it.
