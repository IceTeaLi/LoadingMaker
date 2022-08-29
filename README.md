## LoadingMaker:A simple tool for making cpu usage 
### About
This is a simple tool for making cpu usage 
+ Specify the cpu usage value
+ Specify the number of cores
+ Cross-platform

### How to
```
usage: LoadingMaker.exe [options] ...
options:
  -u, --usage    cpu usage (int [=45])
  -c, --cores    cores (int [=1])
  -m, --mask     specify cpu cores by mask (unsigned int [=1])
  -?, --help     print this message
```
#### Example:
+ Make 4 cores 60% loading
```
LoadingMaker.exe -u 60 -c 4
```
+ Make 0,2,3 cores 45% loading
```
LoadingMaker.exe -u 45 -m 1101
```
#### Close: kill or Ctrl+c

### Details
+ Use an infinite loop to occupy cpu time, and control the cpu usage by adjusting the occupancy ratio within 100ms

### FAQ
There may be about +/- 10% error on different CPU platforms

