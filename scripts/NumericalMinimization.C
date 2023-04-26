// Author: Maral Alyari
// Institution: Fermilab
// Doing a min best fit

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
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
    Reading(string label, double x_meas, double y_meas, double z_meas, double i, double j, double k, double d, double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}

};


double Distance(char const* inputFile_nom, char const* inputFile_meas, const double *xxx){

    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string fileType = ".csv";


    ifstream imfile(dataToPlotFolder + inputFile_meas + "_forFit" +  fileType );
    if (! imfile.is_open()) {cout << "Couldn't open input file" << endl;};

    ifstream infile(dataToPlotFolder + inputFile_nom + "_forFit" + fileType);
    if (! infile.is_open()) {cout << "Couldn't open input file" << endl;};

    vector<Reading> nfile;
    vector<Reading> mfile;
    string Label;
    double X_meas;
    double Y_meas;
    double Z_meas;
    double I;
    double J;
    double K;
    double Diameter; 
    double Roundness; 
    

    vector<string> row;
    string line, item;

    double dX; vector<double> DX;
    double dY; vector<double> DY;
    double sqrtroot; vector<double> SQRTROOT;

    double tanTheta; double X_plot_end; double Y_plot_end;

    
    if(imfile.is_open()){
        while(getline(imfile, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, item, ','))
                row.push_back(item);
                Label = row[0]; X_meas = stod(row[1]);  Y_meas = stod(row[2]);  Z_meas = stod(row[3]);
                I = stod(row[4]);   J = stod(row[5]);   K = stod(row[6]);
                Diameter = stod(row[7]);    Roundness = stod(row[8]); 
                mfile.push_back(Reading(Label, X_meas,  Y_meas,  Z_meas,  I,  J,  K,  Diameter,  Roundness));
        }
    }
    else
        cout<<"Could not open the imfile\n";
    imfile.close();
    // cout << "Number of points measured: " << mfile.size() << endl; 


    if(infile.is_open()){
        while(getline(infile, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, item, ','))
                row.push_back(item);
                Label = row[0]; X_meas = stod(row[1]);  Y_meas = stod(row[2]);  Z_meas = stod(row[3]);
                I = stod(row[4]);   J = stod(row[5]);   K = stod(row[6]);
                Diameter = stod(row[7]);    Roundness = stod(row[8]); 
                nfile.push_back(Reading(Label, X_meas,  Y_meas,  Z_meas,  I,  J,  K,  Diameter,  Roundness));
        }
    }
    else
        cout<<"Could not open the infile\n";
    infile.close();
    // cout << "Number of points measured: " << nfile.size() << endl; 



    vector<double> X_corr; vector<double> Y_corr;
    double x_corr = 0.0; double y_corr = 0.0;


    double theta = xxx[0];
    double x_t = xxx[1];
    double y_t = xxx[2];

    for (int i=0; i<mfile.size() ; i++){
        double x_corr = mfile[i].X_meas*cos(theta) - mfile[i].Y_meas*sin(theta) + x_t;
        double y_corr = mfile[i].X_meas*sin(theta) + mfile[i].Y_meas*cos(theta) + y_t;
        // cout << "x_corr: " << x_corr << endl;
        // cout << "y_corr: " << y_corr << endl;
        X_corr.push_back(x_corr);
        Y_corr.push_back(y_corr);

    }

    // double sum_x = 0; double sum_y = 0;
    double sum = 0; 

    for (int i=0; i<mfile.size() ; i++){
        sum = sum + pow((X_corr[i]-nfile[i].X_meas),2)+ pow((Y_corr[i]-nfile[i].Y_meas),2);
        // cout << "sum: " << sum << endl;

    }

    return sqrt(sum);    

}

    

int NumericalMinimization(char const* inputFile_nom, char const* inputFile_meas, 
                          const char * minName = "Minuit2", const char *algoName = "" , int randomSeed = -1) {
    ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer(minName, algoName);

    // set tolerance , etc...
    min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2 
    min->SetMaxIterations(10000);  // for GSL 
    min->SetTolerance(0.001);
    min->SetPrintLevel(1);

    // create funciton wrapper for minmizer
    // a IMultiGenFunction type 
    // ROOT::Math::Functor f(&Distance,3); 
    ROOT::Math::Functor f([&](const double *xxx){return Distance(inputFile_nom,inputFile_meas,xxx);},3); 

    double step[3] = {0.001,0.001,0.001};

    // starting point
    double variable[3] = {0,0,0};

    if (randomSeed >= 0) { 
        TRandom2 r(randomSeed);
        // variable[0] = r.Uniform(-0.01,0.01);
        // variable[1] = r.Uniform(-0.3,0.3);
        // variable[2] = r.Uniform(-0.3,0.3);
        variable[0] = r.Uniform();
        variable[1] = r.Uniform();
        variable[2] = r.Uniform();
    }

    min->SetFunction(f);

    // Set the free variables to be minimized!
    min->SetVariable(0,"theta",variable[0], step[0]);
    min->SetVariable(1,"x_t",variable[1], step[1]);
    min->SetVariable(2,"y_t",variable[2], step[2]);

    // do the minimization
     min->Minimize(); 

    const double *xs = min->X();
    std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "," << xs[2] << "): " << min->MinValue()  << std::endl;

    // // expected minimum is 0
    // if ( min->MinValue()  < 1.E-4  && f(xs) < 1.E-4) 
    //     std::cout << "Minimizer " << minName << " - " << algoName << "   converged to the right minimum" << std::endl;
    // else {
    //     std::cout << "Minimizer " << minName << " - " << algoName << "   failed to converge !!!" << std::endl;
    //     Error("NumericalMinimization","fail to converge");
    // }

    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string fileType = ".csv";
    ofstream ofile(dataToPlotFolder + "fitValues" + fileType);
    ofile << "theta" << "," << xs[0] << "\n";
    ofile << "x_t" << "," << xs[1] << "\n";
    ofile << "y_t" << "," << xs[2] << "\n";
    ofile.close();
    gSystem->Exit(0);

    return 0;
    
}





    