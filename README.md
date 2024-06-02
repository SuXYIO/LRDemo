# LR
## Intro
Linear regression demo made with C standard & POSIX libs.  
## Install
### Linux/Unix-like systems:  
Clone:
```bash
git clone https://github.com/SuXYIO/LR
```
Compile:
```
cd LR
make
```
### Windows
Not supported yet.  
With versions including and after commit `7bbfc08c43ca8c24be9d7da5e21884a420c4fa7a`, the program uses POSIX libs, which vanilla Windows doesn't support. The versions before might have a chance of working on Windows.  
## Usage
```bash
./LR.out [options]
```
### Args
See manual (./LR.out -h).  
## Technical Details
### Algorithms
Loss function: MSE  
Regression algorithm: Gradient decent  
Random algorithm: Box-muller normal distro  
### Functions
f(x): Training function, for training  
g(x): Data function, for generating training data  
## Purpose
A project to help improve C programming skills & understand MLPs better.  
