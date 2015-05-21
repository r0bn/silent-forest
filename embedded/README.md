# Arduino Embedded 

## Extern Librarys

* https://github.com/sudar/Arduino-Makefile
* https://github.com/stanleyseow
* Symbolic Linkfrom (SF) in Arduino Library folder (~/Arduino/libraries/)

### Path dependencies

* Path to Arduino-Makefile in makefile's

## Silent Forest Library (SF)

Silent Forest Logic

### Running Tests

uncomment `test.cpp`

```
cd embedded/SF
g++ test.cpp SF.cpp -o main
./main
```


## Components



## LED's

* Player
  * Status LED - last ping successful received by hill

* Hill
  * Status LED - last hill status succesful received by king (should glow all the time)

* King
  * 10 LED's - to show the global game state

