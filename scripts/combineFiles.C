// Author: Maral Alyari
// Institution: Fermilab
// Combining the files that will be used for doing a fit into one .csv file

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <time.h>
#include <algorithm> 
#include <iomanip>

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
    Reading(string label, double x_meas, double y_meas, double z_meas, double i, double j, double k, double d, double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}

};

struct Reading2 {
    double X_nom;
    double Y_nom;
    double X_plot_end;
    double Y_plot_end;
    Reading2(double x_nom, double y_nom, double x_plot_end, double y_plot_end) :
            X_nom(x_nom), Y_nom(y_nom), X_plot_end(x_plot_end), Y_plot_end(y_plot_end) {}

};

void combineFiles(char const* inputFile, char const* featuresToCombine, char const* useOption, char const* fitOption){ 
    ////////// read the input file
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string fileType = ".csv";

    vector<string> type;
    string feature;

    string fit(fitOption);
    string use(useOption);

    // cout << features << endl;
    stringstream str_feat(featuresToCombine);
    while(getline(str_feat, feature, ','))
        type.push_back(feature);

    ofstream ofile(dataToPlotFolder + inputFile + "_" + use + "_" + fit + fileType);

    if (strcmp(use.c_str(), "holes_arrow") == 0){
        for (int i = 0; i<type.size(); i++){
        cout << type[i] << endl;

        ifstream ifile(dataToPlotFolder + inputFile + "_" + type[i] + "_arrow_" + fit +fileType );
        if (! ifile.is_open()) {cout << "Couldn't open input file" << endl;};

        vector<Reading2> file;
        double X_nom; double Y_nom;
        double X_plot_end; double Y_plot_end;

        vector<string> row;
        string line, item;
     
        if(ifile.is_open()){
            while(getline(ifile, line)){
                row.clear();
                stringstream str(line);
                while(getline(str, item, ','))
                    row.push_back(item);
                    X_nom = stod(row[0]); Y_nom = stod(row[1]);  X_plot_end = stod(row[2]);  Y_plot_end = stod(row[3]);
                    file.push_back(Reading2(X_nom, Y_nom, X_plot_end, Y_plot_end));
            }
        }
        else
            cout<<"Could not open the ifile\n";
        ifile.close();
     
        cout << "Number of points measured: " << file.size() << endl; 

        for (int j = 0; j<file.size(); j++){
            ofile << file[j].X_nom << "," << file[j].Y_nom << "," << file[j].X_plot_end << "," << file[j].Y_plot_end << "\n";                       
            };  
        }
    }


    if (strcmp(use.c_str(), "holes_arrow") != 0){
        for (int i = 0; i<type.size(); i++){
        cout << type[i] << endl;

        ifstream ifile(dataToPlotFolder + inputFile + "_" + type[i] + fileType );
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
     
        cout << "Number of points measured: " << file.size() << endl; 

        for (int j = 0; j<file.size(); j++){
            ofile << file[j].Label << "," << file[j].X_meas << "," << file[j].Y_meas << "," << file[j].Z_meas << "," << 
                     file[j].I     << "," << file[j].J      << "," << file[j].K      << "," << file[j].Diameter << "," << file[j].Roundness << "\n";                       
            };  
        }
    }
  
    
    ofile.close(); 
    gSystem->Exit(0);
}
    

      

    

  
   
