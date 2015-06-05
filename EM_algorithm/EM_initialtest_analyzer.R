# dataset.org <- read.csv("square_initialtest.csv")
# KL情報量の積分領域
r <- 150

# 初期分布内のKL情報量
mu <- matrix(c(1,1),2,1)
sigma <- matrix(c(1,0,0,1),2,2)
sigmainv <- solve(sigma)
d <- det(sigma)
d2norm <- function(x,y) {
  v <- matrix(c(x,y),2,1)
  return(1/(2*pi*sqrt(d))*exp(-1/2*(t(v-mu)) %*% sigmainv %*% (v-mu)))
}
x <- seq(-r,r,0.1)
y <- seq(-r,r,0.1)
p <- outer(x,y,d2norm)

# 初期分布と真の分布の
