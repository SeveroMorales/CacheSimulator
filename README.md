# CacheSimulator
A cache simulator that let you compare and undertand the difference between Direct Mapped, Fully Associative and N-Way Associative Cahes
========================================================================================================================================
How to make your own the simulations
    1.- First make a vector of the class named instructions
    2.- Use the "LoadTraceFile" function to load the trace file
        .- The parameters of this function are 1.- String with the name of the trace file 2.- a vector of the instructions class
    3.-There are the classes on this simulation 
        .-CacheDirectMapped
        .-CacheSetAssociative
        .-CacheFullyAssociative
    4.- Make an object of your desired Cache and the parameters to construct the object are 
        1.- number of Sets
        2.- number of bytes per line (or block)
        3.- number of lines (or block) per set
    The CacheSetAssociative and CacheFullyAssociative class have 2 functions
        .-GetRatioLRU   The parameter of this function is a vector of instructions 
        .-GetRatoFIFO   The parameter of this function is a vector of instructions
    CacheDirectMapped has one function
        .- GetHitRatio  he parameter of this function is a vector of instructions
    5.- After you create your object and called a function the terminal will output a hit rate of your desired cache design
    
    6.- you then will need to compile your program run in the terminal g++ o- *name of your program* main.cpp
    7.- Now run your program with ./*name of your program*

    8.- Congratulations! you just made a cache simulation, Your friends and family are proud of you!
    
How To compile your simulation:
    1.- Run in terminal g++ -o *name of your program* main.cpp
    2.- Run your program by clicking the .exe file and hit enter once
    3.- or run the program in the terminal

    MAKE FILE: 
        you can also just run the program by typing "make" on the folder where the src code is
         
