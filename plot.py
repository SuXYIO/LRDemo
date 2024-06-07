'''
LinearRegressionPlot
Simple example of using the output CSV file of LR to plot.
'''
import os
import matplotlib.pyplot as plt
import pandas as pd

PATTERN = ".csv"
csvfiles = []
for f in os.listdir("."):
    if f.endswith(PATTERN):
        csvfiles.append(f)
if len(csvfiles) > 1:
    # muti csv, ask for one
    csvfilename = input(f'Found mutiple CSV files: \n{print(csvfiles)}\nChoose one CSV file\n> ')
elif len(csvfiles) == 1:
    csvfilename = csvfiles[0]
else:
    # no csv
    csvfilename = input('Enter filepath\n> ')

df = pd.read_csv(csvfilename)
ln = df.index

plt.plot(ln, df['f_w'], label = 'f_w')
plt.plot(ln, df['f_b'], label = 'f_b')
plt.plot(ln, df['g_w'], label = 'g_w')
plt.plot(ln, df['g_b'], label = 'g_b')
plt.plot(ln, df['l'], label = 'l')
plt.plot(ln, df['grad_w'], label = 'grad_w')
plt.plot(ln, df['grad_b'], label = 'grad_b')

plt.title('LinearRegressionPlot')
plt.xlabel('iter')
plt.ylabel('value')
plt.legend()

plt.show()
