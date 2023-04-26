#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <time.h>
#include <algorithm> 

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TLatex.h"
#include "Riostream.h"
#include <cstdlib>


using namespace std;


struct Reading {
    string Label;
    double X_meas;
    double Y_meas;
    double Z_meas;
    double I;
    double J;
    double K;
    double Diameter; 
    double Roundness; 
    Reading(string label, double x_meas,  double y_meas,  double z_meas,  int i, int j, int k, double d,    double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}


};

void applyCorr_fit(){
    string measuredDataFolder = "data/dataToPlot/"; //change for each round of measurements
    string nominalDataFolder = "data/nominals/";
    string outputDataFolder = "data/dataToPlot/dataWithDeltas_postfit/round1/";   //change for each round of measurements
    string plots = "plots/postfit/"; //change for each round of measurements
    string dataRound = "postfit_round1_"; //change for each round of measurements

    vector<string> measuredFile = {"F10161394-TOPBUTPOCKETS-02-MAR-22_Run1_outline",   // change for each round of measurements
                                   // "F10161394-TOPBUTPOCKETS-02-MAR-22_Run1_pinhole",
                                   // "F10161394-TOPBUTPOCKETS-02-MAR-22_Run1_M4",
                                   // "F10161394-TOPBUTPOCKETS-02-MAR-22_Run1_M3",
                                   // "F10161394-TOPBUTPOCKETS-02-MAR-22_Run1_M1p6",
                                   // "F10161394-TOP-OUT-14Dec21-Run1_topPocket_outline"
                               };
    string fileType = ".csv";
    string outputFile = "_withDeltas";

    string recordFile = "recordFile.txt";

    vector<string> nominalFile = {"outline_CMM",
                                  // "pinHoles_CMM",
                                  // "M4Holes_CMM",
                                  // "M3Holes_CMM",
                                  // "M1p6Holes_CMM",
                                  // "pockets_top_CMM_outline"
                              };

    vector<string> plotsFolders = {"outline",
                                   // "pinHoles",
                                   // "M4Holes",
                                   // "M3Holes",
                                   // "M1p6Holes",
                                   // "pockets_top"
                               };

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.c_str());

    

    

    ofstream ofile( plots + dataRound + recordFile);

    
    for (int j = 0; j<measuredFile.size(); j++){
        vector<Reading> mfile;
        vector<Reading> nfile;
        string Label;
        double X_meas;
        double Y_meas;
        double Z_meas;
        double I;
        double J;
        double K;
        double Diameter; 
        double Roundness; 

        double dX; vector<double> DX;
        double dY; vector<double> DY;
        double sqrtroot; vector<double> SQRTROOT;

        double tanTheta; double X_plot_end; double Y_plot_end;

        ifstream imfile(measuredDataFolder + measuredFile[j] + fileType );
        if (! imfile.is_open()) {cout << "Couldn't open input file" << endl;};

        ifstream infile(nominalDataFolder + nominalFile[j] + fileType );
        if (! infile.is_open()) {cout << "Couldn't open input file" << endl;};


        while (imfile >> Label >> X_meas >> Y_meas >> Z_meas >> I >> J >> K >> Diameter >> Roundness) {
            mfile.push_back(Reading(Label, X_meas, Y_meas, Z_meas, I, J, K, Diameter, Roundness));
        }

        while (infile >> Label >> X_meas >> Y_meas >> Z_meas >> I >> J >> K >> Diameter >> Roundness) {
            nfile.push_back(Reading(Label, X_meas, Y_meas, Z_meas, I, J, K, Diameter, Roundness));
        }

        ofile << "****************************************************" << "\n";
        ofile << "calculating " << plotsFolders[j] << "\n";
        ofile << "Number of measured points: " << mfile.size() << "\n";
        ofile << "Number of nominal points: " << nfile.size() << "\n";
        
        ofstream outfile;
        outfile.open(outputDataFolder + measuredFile[j] + outputFile + fileType);

        double theta = -1.69729e-05; 
        double x_t = 0.00895315 ;   
        double y_t = 0.0227088;    

        // vector<double> X_corr; vector<double> Y_corr;
        double x_corr = 0.0; double y_corr = 0.0;

        for (int n = 0; n<mfile.size(); n++){  
            // calculate the corrections:
            x_corr = mfile[n].X_meas*cos(theta) - mfile[n].Y_meas*sin(theta) + x_t;
            y_corr = mfile[n].X_meas*sin(theta) + mfile[n].Y_meas*cos(theta) + y_t;

            // needed for outline:
            // cout << "x_corr: " << x_corr << "\t" << "y_corr: " << y_corr << endl;

            // calculate the deltas
            dX = x_corr - nfile[n].X_meas;
            dX = round(dX*1000)/1000;
            // ofile << "dx = " << dX << "\n";
            DX.push_back(dX);

            dY = y_corr - nfile[n].Y_meas;
            dY = round(dY*1000)/1000;
            // ofile << "dy = " << dY << "\n";
            DY.push_back(dY);

            sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
            sqrtroot = round(sqrtroot*1000)/1000;
            SQRTROOT.push_back(sqrtroot);

            if (dX > 0){
                tanTheta = dY/dX;
                X_plot_end = sqrtroot*1000/sqrt(1+pow(tanTheta,2)) + nfile[n].X_meas;
                Y_plot_end = tanTheta*(X_plot_end - nfile[n].X_meas) + nfile[n].Y_meas;
            }

            else if (dX == 0) {
                X_plot_end = nfile[n].X_meas ;
                Y_plot_end = y_corr;
            } 

            else if (dX < 0) {
                tanTheta = dY/dX;
                X_plot_end = (-1)*sqrtroot*1000/sqrt(1+pow(tanTheta,2)) + nfile[n].X_meas;
                Y_plot_end = tanTheta*(X_plot_end - nfile[n].X_meas) + nfile[n].Y_meas;
            }

            outfile << mfile[n].Label << "," << nfile[n].X_meas << "," << nfile[n].Y_meas << "," << nfile[n].Z_meas   << "," << 
                       nfile[n].I     << "," << nfile[n].J      << "," << nfile[n].K      << "," << mfile[n].Diameter << "," << mfile[n].Roundness << "," <<
                       dX             << "," << dY              << "," << sqrtroot        << "," << X_plot_end        << "," << Y_plot_end         << "," << "\n";


        }
        outfile.close();
        imfile.close();
        infile.close();
        

        ofile << "Max DX = " << *max_element(DX.begin(), DX.end()) << "\n";
        ofile << "Min DX = " << *min_element(DX.begin(), DX.end()) << "\n";
        ofile << "Max DY = " << *max_element(DY.begin(), DY.end()) << "\n";
        ofile << "Min DY = " << *min_element(DY.begin(), DY.end()) << "\n";

        ofile << "****************************************************" << "\n";
        ofile << "\n";

    }
    ofile.close();
    
    
}
    

      

    

  
   
