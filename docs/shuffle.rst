Suffle test
=============================

The biological meaning of the extracted intensity is assessed by the probability that the intensity occurs by chance. We define a null hypothesis by shuffling a query position to estimate its probability distribution. That is, we randomly shuffle the query position many times (default=100, controlled by the -random option) and calculate the contact intensities. Finally, the query's contact intensity is evaluated by its ranking among the sampled intensities.

We provide tools/visualizeSampling.R, a script to visualize the query's output against the random samples' distribution.
::
    Rscript tools/visualizeRandomTest.R output_random.tsv shuffle_plot.pdf

An example is given as below:

image1|

image1| image:: https://github.com/cbcrg/hicmaptools/blob/master/docs/figs/randomExample.pdf
