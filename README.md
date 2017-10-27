# Arduino RTTTL parser
A parser for RTTTL (Nokia) melodies for Arduino (uses protothreading).
Useful for game making, robotics with melody background ...

## Usage
Install this library to your sketchbook/libraries directory.
First make sure that you have installed [LedControl library](https://github.com/wayoda/LedControl) before installing this one (it is needed for some examples).

### Linux :
```
sudo apt-get install arduino
sudo apt-get install python-pip
sudo pip install confduino
sudo python -m confduino.libinstall https://github.com/netmonster/arduino_rtttl_parser/archive/master.zip
```
### Windows
```
Download this repository as a zip file
Open arduino application.
Go to menue Sketch > Include library > Add Zip Library
choose the zip file, and that's it !!
Restart the arduino application
```


Instanciate  the RTTTL object  by giving the buzzer pinID as well as a pointer to the melody to be played in the form of a null terminated string.
In  your loop function, call   <the instance of RTTTL>.tick(); This will trigger the parsing of the notes and the  playing .

## Status
Still  under developement.
