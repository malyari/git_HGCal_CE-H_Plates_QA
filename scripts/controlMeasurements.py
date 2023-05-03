# Author: Maral Alyari
# Institution: Fermilab
# Quick check on the control measurements


from tabulate import tabulate
import csv
import sys
import os

# def 2Dplotter(inputFile_meas, feature):

inputFile_meas = sys.argv[1]
# reading the csv file
dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/"
plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/"
fileType = ".csv"

outputFileType = ".txt"

inputName = dataToPlotFolder + inputFile_meas + "_controlPoints" + fileType
outputName = plotsFolder + inputFile_meas + "_controlPoints" + outputFileType
  
Label = []
X_nom = []
Y_nom = []
Z_nom = []
I = []
J = []
K = []
Diameter = []
Roundness = []
  
  
with open(inputName, mode='r') as csv_file:
  csv_reader = csv.reader(csv_file, delimiter=',')
  for row in csv_reader:
    # print (row )
    Label.append(row[0])
    X_nom.append(row[1])
    Y_nom.append(row[2])
    Z_nom.append(row[3])
    I.append(row[4])
    J.append(row[5])
    K.append(row[6])
    Diameter.append(row[7])
    Roundness.append(row[8])


Dx_cls = []
Dy_cls = []
Dz_cls = []
Dz_grnt = []

for i in range(0,3):
  Dx_cls.append(abs(float(X_nom[i]) - float(X_nom[i+11])))
  Dy_cls.append(abs(float(Y_nom[i]) - float(Y_nom[i+11])))
  Dz_cls.append(abs(float(Z_nom[i]) - float(Z_nom[i+11])))
Dx_cls.append(abs(float(X_nom[3]) - float(X_nom[10])))
Dy_cls.append(abs(float(Y_nom[3]) - float(Y_nom[10])))
Dz_cls.append(abs(float(Z_nom[3]) - float(Z_nom[10])))

for i in range(3, 10):
  Dz_grnt.append(float(Z_nom[i]))



table1 = tabulate({'Label':               [Label[0],           Label[1],           Label[2],           Label[3]], 
                 'abs(ΔX)': [round(Dx_cls[0],3), round(Dx_cls[1],3), round(Dx_cls[2],3), round(Dx_cls[3],3)], 
                 'abs(ΔY)': [round(Dy_cls[0],3), round(Dy_cls[1],3), round(Dy_cls[2],3), round(Dy_cls[3],3)], 
                 'abs(ΔZ)': [round(Dz_cls[0],3), round(Dz_cls[1],3), round(Dz_cls[2],3), round(Dz_cls[3],3)], 
                 }, 
                 headers='keys', 
                 tablefmt='fancy_grid', 
                 missingval='N/A')

table2 = tabulate({'Label':            [Label[3],            Label[4],            Label[5],            Label[6],            Label[7],            Label[8],            Label[9]],  
                 'Measured Z': [round(Dz_grnt[0],3), round(Dz_grnt[1],3), round(Dz_grnt[2],3), round(Dz_grnt[3],3), round(Dz_grnt[4],3), round(Dz_grnt[5],3), round(Dz_grnt[6],3)], 
                 }, 
                 headers='keys', 
                 tablefmt='fancy_grid', 
                 missingval='N/A')

with open(outputName, 'w') as f:
  f.write(table1)
  f.write('\n')
  f.write(table2)




f.close()

os._exit(0)
# sys.exit(0)






