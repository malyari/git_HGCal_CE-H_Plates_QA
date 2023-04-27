# Author: Maral Alyari
# Institution: Fermilab
# Analysing all the top surface CMM measurements made on cooling plates

import math
import os
import subprocess

################### Need to make adjustments here for every measurement ###################
# What side of the plate was measured:
side = "top"

# Thickness of the measured plate:
thickness = "6.35"

# File names:
inputCMM = "allTop_7B_config1_sorted"
outputCMM = "CE-H7B-TOP-OUT-03102023_1"

# What features where measured:
features = "flatness,outline,6mmPinHole,6p5mmPinHole,10mmPinHole,M3Holes,M6Holes"

# What features to use for doing a fit:
featuresForFit = "outline"

############################################################################################


# Separating the file with nominal values fed to the CMM:
os.system("root -l 'scripts/separateFile.C(\"" + inputCMM + "\",\""+ features +"\")'")

# Separating the file with CMM measured values:
os.system("root -l 'scripts/separateFile.C(\""+ outputCMM + "\",\""+ features +"\")'")

# Sort flatness measurements for calculating the curvature. 
os.system("/usr/local/bin/python3 -i scripts/sort_flatness.py " + outputCMM + " flatness x")
os.system("/usr/local/bin/python3 -i scripts/sort_flatness.py " + outputCMM + " flatness y")

# Check granite table flatness


# Plot flatness:
os.system("root -l 'scripts/plot_flatness.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\",\""+ thickness +"\")'")

# Calculate and plot the curvature:
os.system("root -l 'scripts/cal_plot_curvature.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\")'")

# Combine the files that will be used for the fit
os.system("root -l 'scripts/combineFiles.C(\""+ outputCMM + "\",\""+ featuresForFit +"\")'")
os.system("root -l 'scripts/combineFiles.C(\""+ inputCMM + "\",\""+ featuresForFit +"\")'")

# Do the fit and save theta, x_t and y_t in a text file
os.system("root -l 'scripts/NumericalMinimization.C(\"" + inputCMM + "\",\""+ outputCMM + "\")'")

# Correct the measurements based on the fit
os.system("root -l 'scripts/applyCorr_fit.C(\"" + outputCMM + "\",\""+ features +"\")'")

# Plot outline plots
os.system("root -l 'scripts/cal_plot_outline_holes.C(\""+ inputCMM + "\",\""+ outputCMM + "\",\""+ side +"\",\"outline\",\"1000\",\"preFit\")'")
os.system("root -l 'scripts/cal_plot_outline_holes.C(\""+ inputCMM + "\",\""+ outputCMM + "\",\""+ side +"\",\"outline\",\"1000\",\"postFit\")'")

# Plot outline arrow plots
os.system("/usr/local/bin/python3 -i scripts/2Dplotter.py " + outputCMM + " outline" + " preFit")
os.system("/usr/local/bin/python3 -i scripts/2Dplotter.py " + outputCMM + " outline" + " postFit")


allFeatures = features.split(',')
for i in range(len(allFeatures)):
    if "Hole" in allFeatures[i]:
        # print(allFeatures[i])

        # Plot holes plots
        os.system("root -l 'scripts/cal_plot_outline_holes.C(\""+ inputCMM + "\",\""+ outputCMM + "\",\""+ side +"\",\""+ allFeatures[i] +"\",\"1000\",\"preFit\")'")
        os.system("root -l 'scripts/cal_plot_outline_holes.C(\""+ inputCMM + "\",\""+ outputCMM + "\",\""+ side +"\",\""+ allFeatures[i] +"\",\"1000\",\"postFit\")'")

        # Plot the holes arrow plots
        os.system("/usr/local/bin/python3 -i scripts/2Dplotter.py " + outputCMM + " "+allFeatures[i] + " preFit")
        os.system("/usr/local/bin/python3 -i scripts/2Dplotter.py " + outputCMM + " "+allFeatures[i] + " postFit")

os._exit(1)





