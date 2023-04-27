# Author: Maral Alyari
# Institution: Fermilab
# Analysing all the bottom surface CMM measurements made on cooling plates

import math
import os

################### Need to make adjustments here for every measurement ###################
# What side of the plate was measured:
side = "bottom"

# Thickness of the measured plate:
thickness = "6.35"

# File names:
inputCMM = "allBottom_7B_config1_sorted"
outputCMM = "CE-H7B-BOTTOM-OUT-03152023_1";

# What features where measured:
features = "flatness"

############################################################################################


# Separating the file with nominal values fed to the CMM:
os.system("root -l 'scripts/separateFile.C(\"" + inputCMM + "\",\""+ features +"\")'")

# Separating the file with CMM measured values:
os.system("root -l 'scripts/separateFile.C(\""+ outputCMM + "\",\""+ features +"\")'")

# Sort flatness measurements for calculating the curvature. 
os.system("/usr/local/bin/python3 -i scripts/sort_flatness.py " + outputCMM + " flatness x")
os.system("/usr/local/bin/python3 -i scripts/sort_flatness.py " + outputCMM + " flatness y")

# check granite table points

# Plot flatness:
os.system("root -l 'scripts/plot_flatness.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\",\""+ thickness +"\")'")

# Calculate and plot the curvature:
os.system("root -l 'scripts/cal_plot_curvature.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\")'")








