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
    double I;
    double J;
    double K;
    double Diameter; 
    double Roundness; 
    Reading(string label, double x_meas, double y_meas, double z_meas, double i, double j, double k, double d, double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}

};

void plot_flatness(char const* inputFile, char const* side, char const* feature, char const* plateThickness){ 
    ////////// read the input file
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/";
    string fileType = ".csv";
    
    double thickness = stod(plateThickness);
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
    ifile.close();

    ////////// plot the z values
    TCanvas *c1 = new TCanvas("c1","c1",1500,1000);
    string title = "Z " + plateSide + " surface (mm)";
    TH2F *plot_z = new TH2F("", title.c_str(),  
                        48,0,2400,    // X axis
                        56,-1400,1400); // Y axis


    vector<double> X_values; vector<double> Y_values; vector<double> Z_values; 
    for (int i = 0; i<file.size(); i++){
        plot_z->Fill(file[i].Y_meas, file[i].X_meas, file[i].Z_meas - thickness);
        X_values.push_back(file[i].X_meas);
        Y_values.push_back(file[i].Y_meas);
        Z_values.push_back(file[i].Z_meas - thickness);
    }

    double x_max; double x_min; double y_max; double y_min; double z_max; double z_min;
    x_max = *max_element(X_values.begin(), X_values.end());
    x_min = *min_element(X_values.begin(), X_values.end());
    y_max = *max_element(Y_values.begin(), Y_values.end());
    y_min = *min_element(Y_values.begin(), Y_values.end());
    z_max = *max_element(Z_values.begin(), Z_values.end());
    z_min = *min_element(Z_values.begin(), Z_values.end());

    // cout << "x_max: " << round(x_max/50)*50;
    // cout << "y_max: " << round(y_max/50)*50;
    cout << "z_max: " << z_max << endl; 

    double x_axis_max; double y_axis_max;
    x_axis_max = (round(y_max/50)*50*1.05);
    if (strcmp(side, "top") == 0) {
        // cout << "hi  top" << endl;
        y_axis_max = (round(x_max/50)*50);
        plot_z->GetYaxis()->SetRangeUser(0,y_axis_max);
    }
    if (strcmp(side, "bottom") == 0){
        // cout << "hi   bottom" << endl;
        y_axis_max = (round(x_min/50)*50);
        plot_z->GetYaxis()->SetRangeUser(y_axis_max,0);   
    }
    plot_z->GetXaxis()->SetRangeUser(0,x_axis_max);
    
    gStyle->SetPalette(kBird);
    plot_z->SetStats(0);
    plot_z->GetXaxis()->SetTitle("Y (mm)");
    plot_z->GetYaxis()->SetTitle("X (mm)");
    plot_z->SetMarkerSize(0.8);
    plot_z->Draw("COLZ, text");

    stringstream z_max_stream;  z_max_stream << fixed << setprecision(3) << z_max;
    string z_max_s = z_max_stream.str();
    
    stringstream z_min_stream;  z_min_stream << fixed << setprecision(3) << z_min;
    string z_min_s = z_min_stream.str();
    
    string max = "Max = " + z_max_s + " (mm)"; 
    string min = "Min = " + z_min_s + " (mm)"; 

    TText *tmax = new TText(1400,900,max.c_str());
    if (strcmp(side, "bottom") == 0 ) {tmax->SetX(x_axis_max - 200); tmax->SetY(-820);}
    tmax->SetTextAlign(31);
    tmax->SetTextColor(kBlack);
    tmax->SetTextFont(42);
    tmax->SetTextSize(0.05);
    tmax->SetTextAngle(0);
    tmax->Draw();

    TText *tmin = new TText(1400,820,min.c_str());
    if (strcmp(side, "bottom") == 0 ) {tmin->SetX(x_axis_max - 200); tmin->SetY(-900);}
    tmin->SetTextAlign(31);
    tmin->SetTextColor(kBlack);
    tmin->SetTextFont(42);
    tmin->SetTextSize(0.05);
    tmin->SetTextAngle(0);
    tmin->Draw();

    stringstream thickness_stream;  thickness_stream << fixed << setprecision(3) << thickness;
    string thickness_s = thickness_stream.str();
    string note = "Z_plotted = Z_measured - " + thickness_s;
    TText *tnote = new TText(320,-80,note.c_str());
    if (strcmp(side, "bottom")==0) {tnote->SetX(320); tnote->SetY(-1000);}
    tnote->SetTextAlign(22);
    tnote->SetTextColor(kBlack);
    tnote->SetTextFont(42);
    tnote->SetTextSize(0.04);
    tnote->SetTextAngle(0);
    tnote->Draw();
    

    string saveFile = plotsFolder + inputFile + "_measured_z" + ".pdf";
    c1->SaveAs(saveFile.c_str());
    cout << "saved " << saveFile << endl;
    c1->Close();

    gSystem->Exit(0);
}
    

      

    

  
   
