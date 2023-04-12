# Author: Maral Alyari
# Institution: Fermilab
# Analysing the flatness of the plates

import math
import csv

from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH2F
from ROOT import gROOT, gBenchmark, gRandom, gSystem, Double


dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/"

fileType = ".csv"



side = "top"
fileName = "CE-H7B-TOP-OUT-03102023_1"
feature = "flatness"

inputFile = dataToPlotFolder + fileName + "_" + feature + fileType


Label = []
X_meas = []
Y_meas = []
Z_meas = []
I = []
J = []
K = []
Diameter = [] 
Roundness = [] 


with open(inputFile) as csvDataFile:
	csvReader = csv.reader(csvDataFile)
	for row in csvReader:
		Label.append(row[0])
		X_meas.append(row[1])
		Y_meas.append(row[2])
		Z_meas.append(row[3])
		I.append(row[4])
		J.append(row[5])
		K.append(row[6])
		Diameter.append(row[7]) 
		Roundness.append(row[8]) 


title_com = side + " surface " + feature
plot_z = TH2F("measured z", title_com, 40, -4, 4, 40, -20, 20)
plot_z.Fill(X_meas, Y_meas)
plot_z.Draw("colz,text")









