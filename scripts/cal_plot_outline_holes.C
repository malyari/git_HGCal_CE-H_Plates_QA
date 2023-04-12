// Author: Maral Alyari
// Institution: Fermilab
// Analysing the locations of features (holes and outline)

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
    double X;
    double Y;
    double Z;
    double I;
    double J;
    double K;
    double Diameter; 
    double Roundness; 
    Reading(string label, double x, double y, double z, double i, double j, double k, double d, double r) :
            Label(label), X(x), Y(y), Z(z), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}

};

void cal_plot_outline_holes(char const* inputFile_nom, char const* inputFile_meas, char const* side, char const* feature, char const* factor){ 
    ////////// read the input files for nominal and measured
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/";
    string fileType = ".csv";
    
    string plateSide(side);
    double f = stod(factor);

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.cstr());

    ifstream ifile_nom(dataToPlotFolder + inputFile_nom + "_" + feature + fileType );
    if (! ifile_nom.is_open()) {cout << "Couldn't open input file" << endl;};

    ifstream ifile_meas(dataToPlotFolder + inputFile_meas + "_" + feature + fileType );
    if (! ifile_meas.is_open()) {cout << "Couldn't open input file" << endl;};

   
    vector<Reading> file_nom;   vector<Reading> file_meas;
    string Label;         
    double X;               
    double Y;               
    double Z;               
    double I;               
    double J;               
    double K;               
    double Diameter;        
    double Roundness;       
    if (ifile_nom.is_open()) {cout << "input nominal file is open" << endl;};
    if (ifile_meas.is_open()) {cout << "input measured file is open" << endl;};

    vector<string> row;
    string line, item;
 
    if(ifile_nom.is_open()){
        while(getline(ifile_nom, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, item, ','))
                row.push_back(item);
                Label = row[0];     X = stod(row[1]);   Y = stod(row[2]);  Z = stod(row[3]);
                I = stod(row[4]);   J = stod(row[5]);   K = stod(row[6]);
                Diameter = stod(row[7]);    Roundness = stod(row[8]); 
                file_nom.push_back(Reading(Label, X,  Y,  Z,  I,  J,  K,  Diameter,  Roundness));
        }
    }
    else
        cout<<"Could not open the ifile nominal\n";
    ifile_nom.close();
    line.clear(); item.clear();
    if(ifile_meas.is_open()){
        while(getline(ifile_meas, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, item, ','))
                row.push_back(item);
                Label = row[0];     X = stod(row[1]);   Y = stod(row[2]);  Z = stod(row[3]);
                I = stod(row[4]);   J = stod(row[5]);   K = stod(row[6]);
                Diameter = stod(row[7]);    Roundness = stod(row[8]); 
                file_meas.push_back(Reading(Label, X,  Y,  Z,  I,  J,  K,  Diameter,  Roundness));
        }
    }
    else
        cout<<"Could not open the ifile nominal\n";
    ifile_meas.close();


    // for (int i = 0; i<file_nom.size(); i++){
    //     cout << "file_nom[i].X: " << file_nom[i].X << " ," << "file_meas[i].X: " << file_meas[i].X << endl;
    // }

    
    ////////// calculate the deltas and save points for arrow plots
    double dX; vector<double> DX;
    double dY; vector<double> DY;
    double sqrtroot; vector<double> SQRTROOT;

    double tanTheta; double X_plot_end; double Y_plot_end;



    ofstream ofile(dataToPlotFolder + inputFile_meas + "_" + feature + "_arrow" + fileType);
    for (int i = 0; i < file_nom.size(); i++){  
        dX = file_meas[i].X - file_nom[i].X;
        // ofile << "dx = " << dX << "\n";
        DX.push_back(dX);

        dY = file_meas[i].Y - file_nom[i].Y;
        // ofile << "dy = " << dY << "\n";
        DY.push_back(dY);

        sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
        SQRTROOT.push_back(sqrtroot);

        if (dX > 0){
            tanTheta = dY/dX;
            X_plot_end = sqrtroot*f/sqrt(1+pow(tanTheta,2)) + file_nom[i].X;
            Y_plot_end = tanTheta*(X_plot_end - file_nom[i].X) + file_nom[i].Y;
        }

        else if (dX == 0) {
            X_plot_end = file_meas[i].X ;
            Y_plot_end = file_meas[i].Y;
        } 

        else if (dX < 0) {
            tanTheta = dY/dX;
            X_plot_end = (-1)*sqrtroot*f/sqrt(1+pow(tanTheta,2)) + file_nom[i].X;
            Y_plot_end = tanTheta*(X_plot_end - file_nom[i].X) + file_nom[i].Y;
        }

        ofile << file_nom[i].X << "," << file_nom[i].Y << "," << X_plot_end << "," << Y_plot_end << "," << "\n";

    }
    ofile.close();

    


    
    
    
    gSystem->Exit(0);
}
    

      

    

  
   
