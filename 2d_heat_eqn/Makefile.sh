all:
    	gcc HeatEquation2DSerial.c -o HeatEquation2DSerial
        mpicc   HeatEquation2D.c -o  HeatEquation2D
