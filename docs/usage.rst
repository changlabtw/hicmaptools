How to use HiCmappTools?
=============================

Usage
-----

::

    hicmaptools -in_map in.binmap -in_bin in.bins QUERY_MODE query.bed -output out_file.tsv  

    input:  
            -in_map      text .n_contact or binary .bimap by genBiMap commend 
            -in_bin      the bin file for contact map, .bins
        
    QUERY_MODE: 
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


Query Modes
-----------

`bed format <https://genome.ucsc.edu/FAQ/FAQformat.html#format1>`__ :
first three required columns are enough.

-bat
''''

-  calculate the average contacts +- 5000bps of the interested position
-  i.e., PcG binding sites

-couple
'''''''

-  Contact of the pair
-  i.e., a enhancer vs a promoter

-local
''''''

-  List all contacts inside the interval
-  i.e., Check intra-contacts of TAD

-loop
'''''

-  Contact between two ends
-  i.e., Check whether gene 5’ & 3’ form loop or not

-TAD
''''

-  sum./ave. contacts among the interval
-  i.e., PcG/Active TAD different contact property

-sites
''''''

-  Contact between these sites
-  i.e., Calculate contact for PcG pair sites

-submap
'''''''

-  Contact map of interesting region
-  i.e., Extract contacts of specific genome region

-output
'''''''

There are two output files

- specificOutput.tsv: the contact intensity of you interested
- specificOutput **_random** .tsv: the contact intensities of the null hypothesis

Illustration of query modes |image0|



Command example
-------

Suppose you have below files and want to query ``-bat``:

- map file : nm\_none\_30000.n\_contact
- bin file : 30000.cbins
- query input file : BATtest.txt

Then use the command
::

>hicmaptools -in_map nm_none_30000.n_contact -in_bin 30000.cbins -bat BATtest.txt -output temp.tsv
