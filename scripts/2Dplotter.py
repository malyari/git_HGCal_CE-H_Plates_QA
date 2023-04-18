import matplotlib.pyplot as plt
import csv
import sys

# def 2Dplotter(inputFile_meas, feature):

inputFile_meas = sys.argv[1]
feature = sys.argv[2]
# reading the csv file
dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/"
plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/"
fileType = ".csv"

plotFileType = ".pdf"

inputName = dataToPlotFolder + inputFile_meas + "_" + feature + "_arrow" + fileType
outputName = plotsFolder + inputFile_meas + "_" + feature + "_arrow" + plotFileType
  
X_nom = []
Y_nom = []
X_plot_end = []
Y_plot_end = []
  
  
with open(inputName, mode='r') as csv_file:
  csv_reader = csv.reader(csv_file, delimiter=',')
  for row in csv_reader:
    # print (row )
    X_nom.append(row[0])
    Y_nom.append(row[1])
    X_plot_end.append(row[2])
    Y_plot_end.append(row[3])

    
for i in range(len(X_nom)):
 
  #add arrow to plot
  # #arrow(x_begin, y_begin, Dx, Dy, ...)
  deltaX_plot = float(X_plot_end[i])-float(X_nom[i])
  deltaY_plot = float(Y_plot_end[i])-float(Y_nom[i])
  X_nom_plot = float(X_nom[i])
  Y_nom_plot = float(Y_nom[i])
  plt.subplot(111, aspect=1.0)

  plt.arrow(x=X_nom_plot, y=Y_nom_plot, dx=deltaX_plot, dy=deltaY_plot,  head_width=8, color='b')  #0.08 8 width=.01,
  plt.xlabel("X_nom")
  plt.ylabel("Y_nom")
  # head_width=0.02, head_length=0.02, linewidth=4, color='g'
  # plt.arrow(x=1, y=2, dx=2, dy=5, width=.08 , facecolor='red') 
   
#display plot
fig = plt.gcf()

# plt.show()
plt.draw()
# print(outputName)
fig.savefig(outputName)






