# Author: Maral Alyari
# Institution: Fermilab
# Analysing all the bottom surface CMM measurements made on cooling plates

import math
import os

# what side of the plate was measured
side = "bottom"

# nominal values fed to the CMM 
inputCMM = "allBottom_7B_config1_sorted"
os.system("root -l 'scripts/separateFile.C(\"" + inputCMM + "\",\""+ side +"\")'")

# CMM measured values 
outputCMM = "CE-H7B-BOTTOM-OUT-03152023_1";
os.system("root -l 'scripts/separateFile.C(\""+ outputCMM + "\",\""+ side +"\")'")

# check granite table points

# plot flatness
feature = "flatness"
os.system("root -l 'scripts/plot_flatness.C(\""+ outputCMM + "\",\""+ side +"\",\""+ feature +"\")'")

# For now you need to make the sorted flatness files in x and y by hand before running the next command
# Calculate and plot the curvature
os.system("root -l 'scripts/cal_plot_curvature.C(\""+ outputCMM + "\",\""+ side +"\",\""+ feature +"\")'")
