# Author: Maral Alyari
# Institution: Fermilab
# Analysing all the top surface CMM measurements made on cooling plates

import math
import os

# what side of the plate was measured
side = "top"

# nominal values fed to the CMM 
inputCMM = "allTop_7B_config1_sorted"
os.system("root -l 'scripts/separateFile.C(\"" + inputCMM + "\",\""+ side +"\")'")

# CMM measured values 
outputCMM = "CE-H7B-TOP-OUT-03102023_1";
os.system("root -l 'scripts/separateFile.C(\""+ outputCMM + "\",\""+ side +"\")'")

# check granite table points

# plot flatness
feature = "flatness"
os.system("root -l 'scripts/plot_flatness.C(\""+ outputCMM + "\",\""+ side +"\",\""+ feature +"\")'")

# For now you need to make the sorted flatness files in x and y by hand before running the next command
# Calculate and plot the curvature
os.system("root -l 'scripts/cal_plot_curvature.C(\""+ outputCMM + "\",\""+ side +"\",\""+ feature +"\")'")
