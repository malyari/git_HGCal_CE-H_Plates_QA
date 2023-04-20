# Author: Maral Alyari
# Institution: Fermilab
# Sorting the flatness data before calculating the curvature

import os
import csv
import sys
import pandas as pd

inputFile_meas = sys.argv[1]
feature = sys.argv[2]
direction = sys.argv[3]
print("sorting flatness data in ", direction)

# reading the csv file
dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/"
fileType = ".csv"


inputName =  dataToPlotFolder + inputFile_meas + "_" + feature + fileType
outputName = dataToPlotFolder + inputFile_meas + "_" + feature + "_sorted_" + direction + fileType


df = pd.read_csv(inputName, sep=',', header=None, engine='python')
# print(df)
dataGroup = []

if direction == "x": 
  iCol_first = 2
  iCol_second = 1
if direction == "y": 
  iCol_first = 1
  iCol_second = 2

df = df.sort_values(df.columns[iCol_first])
df = df.reset_index(drop=True)

# print("len(df): ", len(df))

with open(outputName, 'w') as outFile:
  for i in range(len(df)-1):
    value_row = df.loc[i, df.columns[iCol_first]] # values of column[iCol_first] in every row
    # a = df.index.get_loc(df.iloc[i].name) # index of every row
    iNextRow = i+1
    value_nextRow = df.loc[iNextRow, df.columns[iCol_first]]
    if abs(value_nextRow - value_row) < 1:
      dataGroup.append(df.iloc[i].values)
    if (iNextRow != len(df)-1):
      if abs(value_nextRow - value_row) > 1:
        dataGroup.append(df.iloc[i].values)
        df2 = pd.DataFrame(dataGroup)
        df2 = df2.sort_values(df.columns[iCol_second])
        df2.to_csv(outFile, index=False, header=False)
        dataGroup.clear()
    if (iNextRow == len(df)-1):
      dataGroup.append(df.iloc[iNextRow].values)
      df2 = pd.DataFrame(dataGroup)
      df2 = df2.sort_values(df.columns[iCol_second])
      df2.to_csv(outFile, index=False, header=False)

outFile.close()
os._exit(0)



  
