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

# check granite table points

# Plot flatness:
os.system("root -l 'scripts/plot_flatness.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\",\""+ thickness +"\")'")

# For now you need to make the sorted flatness files in x and y by hand before running the next command

# Calculate and plot the curvature:
os.system("root -l 'scripts/cal_plot_curvature.C(\""+ outputCMM + "\",\""+ side +"\",\"flatness\")'")
