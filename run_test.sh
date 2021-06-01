mkdir test
./bin/hicmaptools -in_map examples/input_3k.n_contact -in_bin examples/input_3k.cbins -loop examples/loopTest1.bed -output test/3k_loopTest.tsv

./bin/hicmaptools -in_map examples/input_3k.n_contact -in_bin examples/input_3k.cbins -bat bat.bed -output test/batTest.tsv

Rscript tools/visualizeRandomTest.R test/batTest_random_1.txt test/batTest_random_1_plot