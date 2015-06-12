numberings <- c("01","02","03","04","05","06")

dataset.org <- read.csv(paste("for_linear", numberings[1], ".csv", sep=""))
for (numbering in numberings[2:6]) {
  tmp <- read.csv(paste("for_linear", numbering, ".csv", sep=""))
  dataset.org <- rbind(dataset.org, tmp)
}
