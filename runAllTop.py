import math
# import numpy as np 
# import csv
# import pandas as pd
# from openpyxl import workbook
# from openpyxl import load_workbook
import os

# what side of the plate was measured
side = "top"

# nominal values fed to the CMM 
inputCMM = "allTop_7B_config1_sorted"
os.system("root -l 'scripts/separateFile.C(\"" + inputCMM + "\",\""+ side +"\")'")

# CMM measured values 
outputCMM = "CE-H7B-TOP-OUT-03102023_1";
os.system("root -l 'scripts/separateFile.C(\""+ outputCMM + "\",\""+ side +"\")'")

