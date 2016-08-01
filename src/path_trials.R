arr <- array(dim = c(2,2,3))
arr1 <- array(1:12,dim = c(2,2,3))
es <- matrix(1:12,nrow = 2,byrow = T)
for(k in 1:dim(es)[1]){
for (i in 1:dim(es)[2]){
    for(j in 0:2){
        arr[k,i,j] <- es[k,i + j]
    }
}
}
dim(arr)

arr1[1,,]
