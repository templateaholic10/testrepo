pdf.org <- read.csv("square.dataset", header = FALSE)
library(MASS)
x <- pdf.org$V1
y <- pdf.org$V2
d=kde2d(x,y,c(bandwidth.nrd(x),bandwidth.nrd(y)),n=80)
image(d)
contour(d)
hist(pdf.org$V1)
hist(pdf.org$V2)