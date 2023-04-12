// Author: Maral Alyari
// Institution: Fermilab
// Analysing the flatness of the plates

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
#include "TH2F.h"
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
    int I;
    int J;
    int K;
    double Diameter; 
    double Roundness; 
    Reading(string label, double x_meas, double y_meas, double z_meas, int i, int j, int k, double d, double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}

};

void plot_flatness(char const* inputFile, char const* side, char const* feature){ 
// void separateFile(){
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/";
    string fileType = ".csv";
    
    double thickness = 6.35;
    string plateSide(side);

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.cstr());

    ifstream ifile(dataToPlotFolder + inputFile + "_" + feature + fileType );
    if (! ifile.is_open()) {cout << "Couldn't open input file" << endl;};

   
    vector<Reading> file;
    string Label;
    double X_meas;
    double Y_meas;
    double Z_meas;
    double I;
    double J;
    double K;
    double Diameter; 
    double Roundness; 
    if (ifile.is_open()) {cout << "input file is open" << endl;};

    vector<string> row;
    string line, item;
 
    if(ifile.is_open()){
        while(getline(ifile, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, item, ','))
                row.push_back(item);
                Label = row[0]; X_meas = stod(row[1]);  Y_meas = stod(row[2]);  Z_meas = stod(row[3]);
                I = stod(row[4]);   J = stod(row[5]);   K = stod(row[6]);
                Diameter = stod(row[7]);    Roundness = stod(row[8]); 
                file.push_back(Reading(Label, X_meas,  Y_meas,  Z_meas,  I,  J,  K,  Diameter,  Roundness));
        }
    }
    else
        cout<<"Could not open the ifile\n";

    vector<double> Z_values;
    TCanvas *c1 = new TCanvas("c1","c1",1500,1000);
    TH2F *plot_z = new TH2F("", "Measured Z (mm)", 
                             34,0,1700,    // X axis
                             20,0,1000); // Y axis
    for (int i = 0; i<file.size(); i++){
        plot_z->Fill(file[i].Y_meas, file[i].X_meas, file[i].Z_meas - thickness);
        Z_values.push_back(file[i].Z_meas - thickness);
    }
    
    gStyle->SetPalette(kBird);
    plot_z->SetStats(0);
    plot_z->GetXaxis()->SetTitle("Y (mm)");
    plot_z->GetYaxis()->SetTitle("X (mm)");
    plot_z->SetMarkerSize(0.8);
    plot_z->Draw("COLZ, text");

    double z_min; double z_max;
    for (double i: Z_values){
        if (i < z_min) {z_min = i;
        }
        if (i > z_max) {z_max = i;
        }
    }


    stringstream z_max_stream;  z_max_stream << fixed << setprecision(3) << z_max;
    string z_max_s = z_max_stream.str();
    
    stringstream z_min_stream;  z_min_stream << fixed << setprecision(3) << z_min;
    string z_min_s = z_min_stream.str();
    
    string max = "Max = " + z_max_s + " (mm)"; 
    string min = "Min = " + z_min_s + " (mm)"; 

    TText *tmax = new TText(1400,800,max.c_str());
    tmax->SetTextAlign(22);
    tmax->SetTextColor(kBlack);
    tmax->SetTextFont(42);
    tmax->SetTextSize(0.05);
    tmax->SetTextAngle(0);
    tmax->Draw();

    TText *tmin = new TText(1400,720,min.c_str());
    tmin->SetTextAlign(22);
    tmin->SetTextColor(kBlack);
    tmin->SetTextFont(42);
    tmin->SetTextSize(0.05);
    tmin->SetTextAngle(0);
    tmin->Draw();
    

    string saveFile = plotsFolder + inputFile + "measured_z_" + plateSide + ".pdf";
    c1->SaveAs(saveFile.c_str());
    cout << "saved " << saveFile << endl;
    c1->Close();
    
    gSystem->Exit(0);
}
    

      

    

  
   
