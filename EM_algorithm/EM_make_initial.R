# make initial record
delim <- ','
dim <- 2
mixture_num <- 4
epsilon <- 1.0e-2

# pi vector
pi <- runif(mixture_num)
tmpsum <- sum(pi)
pi <- pi / tmpsum

# mus vector
musmin <- -100
musmax <- 100
mus <- runif(dim * mixture_num, min=musmin, max=musmax)

# sigmas vector
# 対角成分はN(25, 100)
# 交差成分はN(10, 100)
sigmas <- 1:(dim * dim * mixture_num)
for (i in 1:mixture_num) {
  while (TRUE) {
    diag <- rnorm(dim, mean=25, sd=10)
    cros <- rnorm(dim, mean=10, sd=10)
    if (abs(diag[1]*diag[2]-cros[1]*cros[2]) > epsilon) {
      break;
    }
  }
  A <- matrix(c(diag[1],cros[1],cros[2],diag[2]),2,2)
  sigma <- A * t(A)
  sigmas[(4*i-3):(4*i)] <- c(sigma[1,],sigma[2,])
}

# record
record <- c(pi, mus, sigmas)