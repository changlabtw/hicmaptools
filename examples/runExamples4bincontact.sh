# make res logs folder
mkdir -p res logs

# test bat query
echo "[QUERY] bat mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -bat bat.bed -output res/bat.tsv -random 1 &> logs/bat.log

# test local query
echo "[QUERY] local mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -local local.bed -output res/local.tsv -random 1 &> logs/local.log

# test loop query
echo "[QUERY] loop mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -loop loop.bed -output res/loop.tsv -random 1 &> logs/loop.log

# test pair query
echo "[QUERY] pair mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -pair pair.bed -output res/pair.tsv -random 1 &> logs/pair.log

# test sites query
echo "[QUERY] sites mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -sites sites.bed -output res/sites.tsv -random 1 &> logs/sites.log

# test submap query
echo "[QUERY] submap mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -submap submap.bed -output res/submap.tsv &> logs/submap.log

# test TAD query
echo "[QUERY] TAD mode"
../bin/hicmaptools -in_map fly_30k.binmap -in_bin fly_30k.cbins -TAD TAD.bed -output res/TAD.tsv -random 1 &> logs/TAD.log
