PROJECT := 2mm

make:
	pgcc -I../utilities -o $(PROJECT) $(PROJECT).c ../utilities/polybench.c
	
acc:
	pgcc -acc -gpu=managed -I../utilities -o $(PROJECT)_acc $(PROJECT).c ../utilities/polybench.c 
