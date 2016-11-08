# Raspberry Pi Module

###The idea

This part is about the *brain* of the project. The Raspberry Pi has the task of deciding whenever the load should be turned off or on and, as side effect of the simulation, give back information to the PC module with the aim of making visualize the right information to the, PC-based, monitoring interface.

###How it works

Using `cron`, every minute, three Python script will be executed:

- `saveData.py` (it gets pulses counted number from Arduino and save it in the DB)
- `dataHandler.py` (it performs the calculations for the estimate of the power of the load)
- `powerHandler.py` (it communicates with the Arduino board to turn on or off the load and with PC to give info to the interface)

> The other files are part of the module utilities, and are needed for the execution of the previously listed files.
