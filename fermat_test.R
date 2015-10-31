prime_strict <- read.csv("fermat_work4.csv", header=FALSE)
prime_prob <- read.csv("fermat_work3.csv", header=FALSE)
prime_test <- read.csv("fermat_work5.csv", header=FALSE)
prime_test <- cbind(prime_strict, prime_prob, prime_test)
colnames(prime_test) <- c("is_prime", "prime_prob", "test")
prime_test$p <- 1:10000
prime_test$is_prime <- factor(prime_test$is_prime)
prime_test$kind <- ifelse(prime_test$prime_prob == 0., "not_prime", ifelse(prime_test$prime_prob == 1., "prime", "pseudoprime"))
prime_test$kind <- factor(prime_test$kind)
write.csv(prime_test, file="fermat_tmp.csv")
all.df <- prime_test[c("p", "prime_prob")]
plot(all.df)
composite.df <- prime_test[prime_test$kind != "prime", ]
compo.df <- composite.df[c("p", "prime_prob")]
plot(compo.df)
pseudo.df <- prime_test[prime_test$kind == "pseudoprime", ]
pseu.df <- pseudo.df[c("p", "prime_prob")]
plot(pseu.df)
summary(pseu.df)
tab <- table(prime_test$is_prime, prime_test$kind)
tab
test_result <- prime_test[c("p", "is_prime", "test")]
test_result$test <- factor(test_result$test)
tab2 <- table(test_result$is_prime, test_result$test)
tab2
pseu.class <- cut(pseu.df$prime_prob, breaks=seq(0, 1, 0.001), right=FALSE, ordered_result=TRUE)
pseu.tab <- table(pseu.class)
pseu.len <- length(pseu.tab)
cum <- numeric(length=pseu.len)
pseu.sum <- 0
for ( i in 1:pseu.len ) {
  pseu.sum <- pseu.sum + pseu.tab[i]
  cum[i] <- pseu.sum
}
plot(seq(0.001, 1, 0.001), cum, type="l", xlab="P(unit(p))", ylab="cumulative frequency")