prime_strict <- read.csv("fermat_work4.csv", header=FALSE)
prime_prob <- read.csv("fermat_work3.csv", header=FALSE)
prime_test <- cbind(prime_strict, prime_prob)
colnames(prime_test) <- c("is_prime", "prime_prob")
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