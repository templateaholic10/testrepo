# definition
initial_num <- 1000
filepath <- "~/testrepo/EM_algorithm/"

source(paste(filepath,"EM_make_initial.R",sep=""))
mydata <- record

for (i in 2:initial_num) {
  source(paste(filepath,"EM_make_initial.R",sep=""))
  mydata <- rbind(mydata,record)
}

mydata.frame <- data.frame(mydata)
write.table( mydata,paste(filepath,"data03/","square_initial01.csv",sep=""), sep=",",row=F,col=F,qme="double")