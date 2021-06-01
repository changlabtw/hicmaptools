library(ggplot2)
library(nortest)

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0 || length(args)==1) {
        stop("USAGE: Rscript normality_test.R InputFile OutputName", call.=FALSE)
}else if (length(args)==2) {
    mydata = read.table(args[1],header = TRUE,sep = ",")
    temp <- args[2]
    g <- gregexpr("/", temp, fixed=TRUE)
    loc <- g[[1]]
    pos <- loc[length(loc)]
    filename <- substr(temp, pos+1, nchar(temp))
    path <- substr(temp, 0, pos)
}

message("For obs:");
    result = pearson.test(mydata$random_obs)
        result
if(result$p.value<0.05){
    message("According to Pearson chi-square TEST, we reject the null hypothesis that the sample is normal distribution\n");
}else{
    message("According to Pearson chi-square TEST, we do not have evidence to reject the null hypothesis that the sample is normal distribution\n");
}

message("For exp:");
result = pearson.test(mydata$random_exp)
result
if(result$p.value<0.05){
    message("According to Pearson chi-square TEST, we reject the null hypothesis that the sample is normal distribution\n");
}else{
    message("According to Pearson chi-square TEST, we do not have evidence to reject the null hypothesis that the sample is normal distribution\n");
}

message("For nor:");
result = pearson.test(mydata$random_nor)
result
if(result$p.value<0.05){
    message("According to Pearson chi-square TEST, we reject the null hypothesis that the sample is normal distribution\n");
}else{
    message("According to Pearson chi-square TEST, we do not have evidence to reject the null hypothesis that the sample is normal distribution\n");
}

pdf(paste(path,filename,".pdf"))

qplot(random_nor,
      data = mydata,
      col = "red",
      geom="density",
      xlab="score",)+
  theme(legend.position="none")+
  geom_vline(xintercept = mydata[1,3])+
  ggtitle(paste0("Densityplot of random nor,\n query nor =",mydata[1,3]))+
  theme(plot.title = element_text(hjust = 0.5))

a <- dev.off()
