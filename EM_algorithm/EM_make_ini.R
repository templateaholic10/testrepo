# definition
initial_num <- 1000

source("EM_make_initial.R")
data <- record

for (i in 2:initial_num) {
  source("EM_make_initial.R")
  data <- rbind(data,record)
}

write.csv(data, "data/square_initial.csv", quote=FALSE, row.names=FALSE, col.names=NA)