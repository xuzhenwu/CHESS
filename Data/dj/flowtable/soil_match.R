soil_class<-read.csv("I:/NSEC/Data/CHESS/dj/flowtable/soil_class.csv",header=TRUE)

soil_class_used<-read.csv("I:/NSEC/Data/CHESS/dj/flowtable/soil_class_used.csv",header=TRUE)

len<-nrow(soil_class)


len_used<-nrow(soil_class_used)

for(i in 1:len_used){
	for(j in 1:len){
	if(soil_class_used$ID[i]==soil_class$ID[j])
	soil_class_used$type[i]=soil_class$type[j]
	}
}

write.csv(soil_class_used,"I:/NSEC/Data/CHESS/dj/flowtable/soil_class_used.csv",row.names=FALSE)