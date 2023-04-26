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

struct ReadingEdges {  
    double nEdge;
    double X_M;
    double Y_M;
    double X_N;
    double Y_N;
    string edgeType;
    ReadingEdges(double e, double x_m, double y_m, double x_n, double y_n, string l) :
                 nEdge(e), X_M(x_m), Y_M(y_m), X_N(x_n), Y_N(y_n), edgeType(l)  {}

};

void cal_plot_outline_holes(char const* inputFile_nom, char const* inputFile_meas, char const* side, char const* feature, char const* factor, char const* fitOption){ 
    ////////// read the input files for nominal and measured
    string inputDataFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataCMM/";
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/";
    string fileType = ".csv";
    
    string plateSide(side);
    double f = stod(factor);

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.cstr());

    string postFit = "postFit"; 
    string preFit = "preFit"; 

    string file_nom_name;
    string file_meas_name;
    file_nom_name = dataToPlotFolder + inputFile_nom + "_" + feature + fileType;

    if (fitOption == postFit){
        file_meas_name = dataToPlotFolder + inputFile_meas + "_" + feature + "_" + fitOption + fileType;
    }
    if (fitOption == preFit){
        file_meas_name = dataToPlotFolder + inputFile_meas + "_" + feature + fileType;
    }

    cout << file_nom_name << endl;
    cout << file_meas_name << endl;

    
    ifstream ifile_nom(file_nom_name );
    if (! ifile_nom.is_open()) {cout << "Couldn't open input file" << endl;};

    ifstream ifile_meas(file_meas_name );
    if (! ifile_meas.is_open()) {cout << "Couldn't open input file" << endl;};
    
    ifstream edgeFile_info(inputDataFolder + inputFile_nom + "_edgeInfo" + fileType );
    if (! edgeFile_info.is_open()) {cout << "Couldn't open input file" << endl;};

   
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
    vector<double> X_NOM;   vector<double> X_MEAS;
    vector<double> Y_NOM;   vector<double> Y_MEAS;
    double tanTheta; double X_plot_end; double Y_plot_end;
    ofstream ofile(dataToPlotFolder + inputFile_meas + "_" + feature + "_arrow" + "_" + fitOption + fileType);

    string type = "outline"; 
    // correcting for perpendicularity for outline measurements
    if (feature == type){
        vector<ReadingEdges> edge;
        vector<vector<ReadingEdges>> file_edge;
        // 
        double nEdge;
        double fromPoint; double toPoint; double num; string edgeType;
        line.clear(); item.clear();
        if(edgeFile_info.is_open()){
            while(getline(edgeFile_info, line)){
                row.clear();
                stringstream str(line);
                while(getline(str, item, ','))
                    row.push_back(item);
                    nEdge = stod(row[0]); fromPoint = stod(row[1]); toPoint = stod(row[2]); edgeType = row[3];
                    // cout << "nEdge: " << nEdge << " fromPoint: " << fromPoint << "  toPoint: " << toPoint << endl;
                    for (int i = 0; i < file_nom.size(); i++){
                        size_t pos = file_nom[i].Label.find(type);
                        num = stod(file_nom[i].Label.substr( pos+type.size()));
                        if ( fromPoint <= num and num <= toPoint ){
                            // cout << "nEdge: " << nEdge << "  fromPoint <= num <= toPoint: " << fromPoint << " <= "<< num << " <= " << toPoint << endl;
                            edge.push_back(ReadingEdges(nEdge, file_meas[i].X, file_meas[i].Y, file_nom[i].X, file_nom[i].Y, edgeType));
                            // cout << nEdge<< "   " << file_meas[i].X<< "   " << file_meas[i].Y<< "   " << file_nom[i].X << "   " << file_nom[i].Y << "   " << edgeType << endl;
                        }
                    }
                    // cout << " edge.size(): " << edge.size() << endl;
                    file_edge.push_back(edge);
                    edge.clear();

                
            }
        }  
        // cout << "******* file_edge.size(): " << file_edge.size() << endl;
        // cout << "******* file_edge[1].size(): " << file_edge[1].size() << endl;
        //file_edge[edge][points on that edge]    

        for (int i = 0 ; i < file_edge.size(); i++){
            // cout << file_edge[i][0].edgeType << endl;
            size_t point = file_edge[i][0].edgeType.find("corner");
            if (point == 0){
                // cout << "not calculating " << file_edge[i][0].nEdge << endl;
                for (int j = 0 ; j < file_edge[i].size() ; j++){
                    dX = file_edge[i][j].X_M - file_edge[i][j].X_N;
                    DX.push_back(dX);
                    dY = file_edge[i][0].Y_M - file_edge[i][0].Y_N;
                    DY.push_back(dY);

                    sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
                    SQRTROOT.push_back(sqrtroot);
                    X_NOM.push_back(file_edge[i][j].X_N);   X_MEAS.push_back(file_edge[i][j].X_M);
                    Y_NOM.push_back(file_edge[i][j].Y_N);   Y_MEAS.push_back(file_edge[i][j].Y_M);
                }
                
            }
            else{
                // cout << file_edge[i][0].nEdge << "  " << file_edge[i][0].edgeType << endl;
                if (file_edge[i].size() == 2){    
                    // cout << "number of points on edge " << file_edge[i][0].nEdge << " : " << file_edge[i].size() << endl;
                
                    dX = file_edge[i][0].X_M - file_edge[i][0].X_N - (file_edge[i][1].X_N - file_edge[i][0].X_N)*(( (file_edge[i][0].X_M - file_edge[i][0].X_N)*(file_edge[i][1].X_N - file_edge[i][0].X_N) + (file_edge[i][0].Y_M - file_edge[i][0].Y_N)*(file_edge[i][1].Y_N - file_edge[i][0].Y_N) )/( pow((file_edge[i][1].X_N - file_edge[i][0].X_N),2) + pow((file_edge[i][1].Y_N - file_edge[i][0].Y_N),2) ));
                    DX.push_back(dX);
                    dY = file_edge[i][0].Y_M - file_edge[i][0].Y_N - (file_edge[i][1].Y_N - file_edge[i][0].Y_N)*(( (file_edge[i][0].X_M - file_edge[i][0].X_N)*(file_edge[i][1].X_N - file_edge[i][0].X_N) + (file_edge[i][0].Y_M - file_edge[i][0].Y_N)*(file_edge[i][1].Y_N - file_edge[i][0].Y_N) )/( pow((file_edge[i][1].X_N - file_edge[i][0].X_N),2) + pow((file_edge[i][1].Y_N - file_edge[i][0].Y_N),2) ));
                    DY.push_back(dY);
                    // cout << file_edge[i][0].X_N << "    " << file_edge[i][0].Y_N << "   " << dX << "    " << dY << endl;

                    sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
                    SQRTROOT.push_back(sqrtroot);
                    X_NOM.push_back(file_edge[i][0].X_N);   X_MEAS.push_back(file_edge[i][0].X_M);
                    Y_NOM.push_back(file_edge[i][0].Y_N);   Y_MEAS.push_back(file_edge[i][0].Y_M);


                    dX = file_edge[i][1].X_M - file_edge[i][1].X_N - (file_edge[i][0].X_N - file_edge[i][1].X_N)*(( (file_edge[i][1].X_M - file_edge[i][1].X_N)*(file_edge[i][0].X_N - file_edge[i][1].X_N) + (file_edge[i][1].Y_M - file_edge[i][1].Y_N)*(file_edge[i][0].Y_N - file_edge[i][1].Y_N) )/( pow((file_edge[i][0].X_N - file_edge[i][1].X_N),2) + pow((file_edge[i][0].Y_N - file_edge[i][1].Y_N),2) ));
                    DX.push_back(dX);
                    dY = file_edge[i][1].Y_M - file_edge[i][1].Y_N - (file_edge[i][0].Y_N - file_edge[i][1].Y_N)*(( (file_edge[i][1].X_M - file_edge[i][1].X_N)*(file_edge[i][0].X_N - file_edge[i][1].X_N) + (file_edge[i][1].Y_M - file_edge[i][1].Y_N)*(file_edge[i][0].Y_N - file_edge[i][1].Y_N) )/( pow((file_edge[i][0].X_N - file_edge[i][1].X_N),2) + pow((file_edge[i][0].Y_N - file_edge[i][1].Y_N),2) ));
                    DY.push_back(dY);
                    // cout << file_edge[i][1].X_N << "    " << file_edge[i][1].Y_N << "   " << dX << "    " << dY << endl;

                    sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
                    SQRTROOT.push_back(sqrtroot);
                    X_NOM.push_back(file_edge[i][1].X_N);   X_MEAS.push_back(file_edge[i][1].X_M);
                    Y_NOM.push_back(file_edge[i][1].Y_N);   Y_MEAS.push_back(file_edge[i][1].Y_M);
                    
                } 
                else if (file_edge[i].size() > 2){
                    // cout << file_edge[i].size() << endl;
                    for (int j = 0 ; j < file_edge[i].size() - 1; j++){
                        dX = file_edge[i][j].X_M - file_edge[i][j].X_N - (file_edge[i][j+1].X_N - file_edge[i][j].X_N)*(( (file_edge[i][j].X_M - file_edge[i][j].X_N)*(file_edge[i][j+1].X_N - file_edge[i][j].X_N) + (file_edge[i][j].Y_M - file_edge[i][j].Y_N)*(file_edge[i][j+1].Y_N - file_edge[i][j].Y_N) )/( pow((file_edge[i][j+1].X_N - file_edge[i][j].X_N),2) + pow((file_edge[i][j+1].Y_N - file_edge[i][j].Y_N),2) ));
                        DX.push_back(dX);
                        dY = file_edge[i][j].Y_M - file_edge[i][j].Y_N - (file_edge[i][j+1].Y_N - file_edge[i][j].Y_N)*(( (file_edge[i][j].X_M - file_edge[i][j].X_N)*(file_edge[i][j+1].X_N - file_edge[i][j].X_N) + (file_edge[i][j].Y_M - file_edge[i][j].Y_N)*(file_edge[i][j+1].Y_N - file_edge[i][j].Y_N) )/( pow((file_edge[i][j+1].X_N - file_edge[i][j].X_N),2) + pow((file_edge[i][j+1].Y_N - file_edge[i][j].Y_N),2) ));
                        DY.push_back(dY);
                        // cout << file_edge[i][j].X_N << "    " << file_edge[i][j].Y_N << "   " << dX << "    " << dY << endl;

                        sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
                        SQRTROOT.push_back(sqrtroot);
                        X_NOM.push_back(file_edge[i][j].X_N);   X_MEAS.push_back(file_edge[i][j].X_M);
                        Y_NOM.push_back(file_edge[i][j].Y_N);   Y_MEAS.push_back(file_edge[i][j].Y_M);
                        
                    };
                    int j = file_edge[i].size()-1;
                    dX = file_edge[i][j].X_M - file_edge[i][j].X_N - (file_edge[i][j-1].X_N - file_edge[i][j].X_N)*(( (file_edge[i][j].X_M - file_edge[i][j].X_N)*(file_edge[i][j-1].X_N - file_edge[i][j].X_N) + (file_edge[i][j].Y_M - file_edge[i][j].Y_N)*(file_edge[i][j-1].Y_N - file_edge[i][j].Y_N) )/( pow((file_edge[i][j-1].X_N - file_edge[i][j].X_N),2) + pow((file_edge[i][j-1].Y_N - file_edge[i][j].Y_N),2) ));
                    DX.push_back(dX);
                    dY = file_edge[i][j].Y_M - file_edge[i][j].Y_N - (file_edge[i][j-1].Y_N - file_edge[i][j].Y_N)*(( (file_edge[i][j].X_M - file_edge[i][j].X_N)*(file_edge[i][j-1].X_N - file_edge[i][j].X_N) + (file_edge[i][j].Y_M - file_edge[i][j].Y_N)*(file_edge[i][j-1].Y_N - file_edge[i][j].Y_N) )/( pow((file_edge[i][j-1].X_N - file_edge[i][j].X_N),2) + pow((file_edge[i][j-1].Y_N - file_edge[i][j].Y_N),2) ));
                    DY.push_back(dY);
                    // cout << file_edge[i][j].X_N << "    " << file_edge[i][j].Y_N << "   " << dX << "    " << dY << endl;

                    sqrtroot = sqrt(pow(dX,2)+pow(dY,2));
                    SQRTROOT.push_back(sqrtroot);
                    X_NOM.push_back(file_edge[i][j].X_N);   X_MEAS.push_back(file_edge[i][j].X_M);
                    Y_NOM.push_back(file_edge[i][j].Y_N);   Y_MEAS.push_back(file_edge[i][j].Y_M);
                    
                };


            }
            
            // cout << file_edge[i].size() << "    " << DX.size() << " " << DY.size() << " " << SQRTROOT.size() << endl;
        }
            
        
        for (int i = 0; i < DX.size() ; i++){
            if (DX[i] > 0){
                tanTheta = DY[i]/DX[i];
                X_plot_end = SQRTROOT[i]*f/sqrt(1+pow(tanTheta,2)) + X_NOM[i];
                Y_plot_end = tanTheta*(X_plot_end - X_NOM[i]) + Y_NOM[i];
                // cout << X_NOM[i] << "    " << Y_NOM[i] << "   " << DX[i] << " " << X_plot_end << " " << Y_plot_end << endl;
            }

            else if (DX[i] == 0) {
                X_plot_end = X_MEAS[i] ; 
                Y_plot_end = Y_MEAS[i]; 
                // cout << X_NOM[i] << "    " << Y_NOM[i] << "   " << DX[i] << " " << X_plot_end << " " << Y_plot_end << endl;
            } 


            else if (DX[i] < 0) {
                tanTheta = DY[i]/DX[i];
                X_plot_end = (-1)*SQRTROOT[i]*f/sqrt(1+pow(tanTheta,2)) + X_NOM[i];
                Y_plot_end = tanTheta*(X_plot_end - X_NOM[i]) + Y_NOM[i];
                // cout << X_NOM[i] << "    " << Y_NOM[i] << "   " << DX[i] << " " << X_plot_end << " " << Y_plot_end << endl;
            }

            ofile << X_NOM[i] << "," << Y_NOM[i] << "," << X_plot_end << "," << Y_plot_end << "\n";


        }
        
    }


    // for all the holes
    else {
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

            ofile << file_nom[i].X << "," << file_nom[i].Y << "," << X_plot_end << "," << Y_plot_end << "\n";

        }

    };
    
       
    ofile.close();


    
    gSystem->Exit(0);
}
    

      

    

  
   
