converges <-subset(dataset.notinf,dataset.notinf$converge,c(KLmean))
diverges <-subset(dataset.notinf,!dataset.notinf$converge,c(KLmean)) 
converges <- converges[[1]]
diverges <- diverges[[1]]
ks.test(converges,diverges)