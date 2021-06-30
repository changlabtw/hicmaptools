# make res folder
mkdir -p res logs

# test bat query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -bat bat.bed -output res/batTest.tsv -random 1 &> logs/batTest.log

# test local query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -local local.bed -output res/localTest.tsv -random 1 &> logs/localTest.log

# test loop query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -loop loop.bed -output res/loopTest.tsv -random 1 &> logs/loopTest.log

# test pair query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -pair pair.bed -output res/pairTest.tsv -random 1 &> logs/pairTest.log

# test sites query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -sites sites.bed -output res/sitesTest.tsv -random 1 &> logs/sitesTest.log

# test submap query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -submap submap.bed -output res/submapTest.tsv &> logs/submapTest.log

# test submap query
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -TAD TAD.bed -output res/TADTest.tsv -random 1 &> logs/TADTest.log
