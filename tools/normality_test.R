library(ggplot2)

args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
        stop("USAGE: Rscript normality_test.R input", call.=FALSE)
}else if (length(args)==1) {
    cat("Enter output filename: ")
        filename <- scan("stdin", character(), n=1)
            mydata = read.table(args[1],header = TRUE,sep = ",")
}

message("For obs:");
    result = shapiro.test(mydata$random_obs)
        result
        if(result$p.value<0.05){
            message("According to SHAPIRO TEST, we do not have evidence to reject that the sample is normal distribution");
        }else{
            message("According to SHAPIRO TEST, we reject that the  sample is normal distribution");
        }

message("For exp:");
result = shapiro.test(mydata$random_exp)
result
if(result$p.value<0.05){
    message("According to SHAPIRO TEST, we do not have evidence to reject that the sample is normal distribution");
}else{
    message("According to SHAPIRO TEST, we reject that the  sample is normal distribution");
}

message("For nor:");
result = shapiro.test(mydata$random_nor)
result
if(result$p.value<0.05){
    message("According to SHAPIRO TEST, we do not have evidence to reject that the sample is normal distribution");
}else{
    message("According to SHAPIRO TEST, we reject that the  sample is normal distribution");
}

qplot(x=mydata$random_obs,
data = mydata,
col = "red",
geom="density",
main = "Densityplot of obs",
xlab="obs",
)+theme(legend.position="none")

ggsave(paste("Distribution_of_",filename,"_obs.pdf"), width = 4, height = 3)

qplot(x=mydata$random_exp,
data = mydata,
col = "red",
geom="density",
main = "Densityplot of exp",
xlab="exp",
)+theme(legend.position="none")

ggsave(paste("Distribution_of_",filename,"_exp.pdf"), width = 4, height = 3)

qplot(x=mydata$random_nor,
data = mydata,
col = "red",
geom="density",
main = "Densityplot of nor",
xlab="exp",
)+theme(legend.position="none")

ggsave(paste("Distribution_of_",filename,"_nor.pdf"), width = 4, height = 3)
