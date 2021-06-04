Input and output format
=============================

Contact Input (essential)
-------------------------

-in\_bin
''''''''

define the chromosome, start position and end position of each bin.
Format is as the following:

``cbin    chr from.coord  to.coord     
  1   2L  6000        7000     
  2   2L  7000        8000     
  3   2L  8000        9000     
  4   2L  9000        10000     
  5   2L  12000       13000``

-in\_map 
''''''''

A contact map indexed by bins. Format is as the following:
\* expected\_count : the expected contact between those two chromosome
regions (bins) according to model 
\* observed\_count : the observed contact between those two chromosome regions (bins) by HiC data

``cbin1   cbin2   expected_count  observed_count     
  1   1   0.077080    50     
  1   2   0.389912    314     
  1   3   0.493750    163     
  1   4   0.560505    169     
  1   5   0.368884    79``
  
 its binary format transferred by ``bin/genBiMap``
  
 ::
  genBiMap [options] -in_ncontact input.n_contact -out_bimap out.bimap
  
