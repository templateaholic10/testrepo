local.mod <- dataset.kaiki.mod$logL.1 < -3988
localf.mod <- factor(local.mod)
boxplot(dataset.kaiki.mod$KLmax~localf.mod)
logKLmax <- log(dataset.kaiki.mod$KLmax)
boxplot(logKLmax~localf.mod)