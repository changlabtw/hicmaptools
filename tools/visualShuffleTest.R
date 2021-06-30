library(ggplot2)

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0 || length(args)==1) {
        stop("USAGE: Rscript visualShuffleTest.R InputFile OutputName", call.=FALSE)
}else if (length(args)==2) {
    mydata = read.table(args[1],header = TRUE,sep = ",")
    temp <- args[2]
    g <- gregexpr("/", temp, fixed=TRUE)
    loc <- g[[1]]
    pos <- loc[length(loc)]
    filename <- substr(temp, pos+1, nchar(temp))
    path <- substr(temp, 0, pos)
}

pdf(paste(path,filename,sep=""))

qplot(random_nor,
      data = mydata,
      geom="density",
      xlab="intensity") +
  theme(legend.position="none") +
  geom_vline(xintercept = mydata[1,3], col="red") +
  ggtitle(paste0("Densityplot of shuffle sampling intensity,\n query intensity= ",mydata[1,3])) +
  theme(plot.title = element_text(hjust = 0.5))

a <- dev.off()
