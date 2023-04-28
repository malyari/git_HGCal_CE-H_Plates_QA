// Author: Maral Alyari
// Institution: Fermilab
// Separating the CMM input and output files based on measured features

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

void separateFile(char const* inputFile, char const* features){ 
    ////////// read the input file
    string inputDataFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataCMM/";
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string fileType = ".csv";

    vector<string> type;
    string feature;

    // cout << features << endl;
    string toSearch = features;
    stringstream str_feat(features);
    while(getline(str_feat, feature, ','))
        type.push_back(feature);

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.cstr());

    ifstream ifile(inputDataFolder + inputFile + fileType );
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
    
    ////////// save the input values in different files based on label
    string num;
    for (int i = 0; i<type.size(); i++){
        ofstream ofile(dataToPlotFolder + inputFile + "_" + type[i] + fileType);
        ofstream ofile_else(dataToPlotFolder + inputFile + "_else" + fileType);
        for (int j = 1; j<file.size(); j++){
            if ( file[j].Label.substr(0, type[i].size()) == type[i] ){

                size_t pos = file[j].Label.find(type[i]); 
                num = file[j].Label.substr( pos+type[i].size());   // num can be used for sorting the file if need be.
                // cout << "number string: " << num << endl;
                // cout << "file[j].Label: " << file[j].Label << endl;
                // cout  << setprecision(10) << "file[j].I: "  << file[j].I << endl;
                ofile << file[j].Label << "," << file[j].X_meas << "," << file[j].Y_meas << "," << file[j].Z_meas << "," << 
                         file[j].I     << "," << file[j].J      << "," << file[j].K      << "," << file[j].Diameter << "," << file[j].Roundness << "\n";                
            }; 
            bool notFound = toSearch.find(file[j].Label.substr(0, type[i].size())) == string::npos;
            if (notFound){
                ofile_else << file[j].Label << "," << file[j].X_meas << "," << file[j].Y_meas << "," << file[j].Z_meas << "," << 
                         file[j].I     << "," << file[j].J      << "," << file[j].K      << "," << file[j].Diameter << "," << file[j].Roundness << "\n";                
            }     
        };  
        ofile.close();
    };




    gSystem->Exit(0);
}
    

      

    

  
   
