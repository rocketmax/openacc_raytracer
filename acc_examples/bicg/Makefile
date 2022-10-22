PROJECT := bicg

make:
	pgcc -I../utilities -i -o $(PROJECT) $(PROJECT).c ../utilities/polybench.c
	
acc:
	pgcc -acc -I../utilities -gpu=managed -o $(PROJECT)_acc $(PROJECT).c ../utilities/polybench.c 
