library(ggplot2)

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0 || length(args)==1) {
        stop("USAGE: Rscript normality_test.R InputFile OutputName", call.=FALSE)
}else if (length(args)==2) {
    #cat("Enter output filename: ")
    #filename <- scan("stdin", character(), n=1)
    mydata = read.table(args[1],header = TRUE,sep = ",")
    temp <- args[2]
    g <- gregexpr("/", temp, fixed=TRUE)
    loc <- g[[1]]
    pos <- loc[length(loc)]
    filename <- substr(temp, pos+1, nchar(temp))
    path <- substr(temp, 0, pos)
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

pdf(paste(path,filename,".pdf"))

qplot(x=mydata$random_obs,
data = mydata,
col = "red",
geom="density",
xlab="obs",
)+theme(legend.position="none")+geom_vline(xintercept = mydata[1,1])+ggtitle(paste0("Densityplot of random obs,\nquery obs =",mydata[1,1])) +theme(plot.title = element_text(hjust = 0.5))


qplot(x=mydata$random_exp,
data = mydata,
col = "red",
geom="density",
xlab="exp",
)+theme(legend.position="none")+geom_vline(xintercept = mydata[1,2])+ggtitle(paste0("Densityplot of random exp,\nquery exp =",mydata[1,2])) +theme(plot.title = element_text(hjust = 0.5))


qplot(x=mydata$random_nor,
data = mydata,
col = "red",
geom="density",
xlab="exp",
)+theme(legend.position="none")+geom_vline(xintercept = mydata[1,3])+ggtitle(paste0("Densityplot of random nor,\nquery nor =",mydata[1,3])) +theme(plot.title = element_text(hjust = 0.5))

a <- dev.off()
