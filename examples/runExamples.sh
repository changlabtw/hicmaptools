# make res folder
mkdir res

# test bat query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -bat bat.bed -output res/batTest.tsv -random 1

exit 

# test local query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -local local.bed -output res/localTest.tsv -random 1

# test loop query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -loop loop.bed -output res/loopTest.tsv -random 1

# test pair query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -pair pair.bed -output res/pairTest.tsv -random 1

# test sites query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -sites sites.bed -output res/sitesTest.tsv -random 1

# test submap query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -submap submap.bed -output res/submapTest.tsv

# test submap query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -TAD TAD.bed -output res/TADTest.tsv -random 1

#Rscript tools/visualizeRandomTest.R test/batTest_random_1.txt test/batTest_random_1_plot