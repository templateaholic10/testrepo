locals <-subset(dataset.kaiki,local,c(KLmax))
nlocals <-subset(dataset.kaiki,!local,c(KLmax)) 
locals <- locals[[1]]
nlocals <- nlocals[[1]]
ks.test(locals,nlocals)