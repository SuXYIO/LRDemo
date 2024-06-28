# LRDemo
## Intro
Linear regression demo made with C standard & POSIX libs.  

## Install
### Linux/Unix:
Clone:
```bash
git clone https://github.com/SuXYIO/LRDemo
```
Compile:
```
cd LRDemo
make
```
### Windows:
Not supported yet.  
The program uses POSIX libs, which vanilla Windows doesn't support. The versions before might have a chance of working on Windows.  
Might work if using methods like MinGW to use POSIX standards on Windows.  

## Usage
```bash
./LR.out [options]
```
### Args
See manual:
```bash
./LR.out -h
```
### Other
1. **./stat/plot.py**  
A simple plot python program for plotting the CSV formatted file that can be outputted by LR using `-f` option (see Args).  
Note that it needs `matplotlib` and `pandas` lib to work.  
More info in plot.py's first mutiline comment.  
2. **./stat/mutiplot.py**  
A version of plot.py, but shows two set of CSV formatted file merged into one plot, doesn't show gradient information.  
More info in mutiplot.py's first mutiline comment.  

## Technical Details
### Algorithms
Loss function: `MSE`  
Regression algorithm: `Gradient decent`  
Random algorithm: `Box-muller normal distro`  
### Functions
**f(x)**: Training function, for training  
**g(x)**: Data function, for generating training data  

