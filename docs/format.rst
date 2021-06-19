File formats
=============================

Input
-------------------------

HiC map
............

a bin-contact pair format
^^^^^^^^^^^

-in\_bin
''''''''

define the chromosome, start and end positions of each bin.
Format is as the following:

::

 cbin   chr from.coord  to.coord
  1     2L  6000        7000
  2     2L  7000        8000     
  3     2L  8000        9000     
  4     2L  9000        10000     
  5     2L  12000       13000


-in\_map 
''''''''

A contact map indexed by bins. Format is as the following:

::

 cbin1   cbin2   expected_count  observed_count     
  1   1   0.077080    50     
  1   2   0.389912    314     
  1   3   0.493750    163     
  1   4   0.560505    169     
  1   5   0.368884    79

- expected\_count : the expected contact between those two chromosome regions (bins) according to model 
- observed\_count : the observed contact between those two chromosome regions (bins) by Hi-C data


its binary format transferred by ``bin/genBiMap``
  
::
 
 genBiMap [options] -in_ncontact input.n_contact -out_bimap out.bimap
  

.hic format
^^^^^^^^^^^

Output
-------------------------

There are two output files. You can use the tool ``tools/visualizeRandomTest.R`` to examine query's contact intensity aganist the null hypothesis (`Shuffle test <random.html>`_).

- output.tsv : the contact intensity of you interested

::

    index   chrom   start   end ... rank_obs    rank_exp    rank_nor    
    1   3R  100000  200000  ...     0.880       0.990       0.760

- output **_random** .tsv : the contact intensities of the null hypothesis
