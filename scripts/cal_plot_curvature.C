// Author: Maral Alyari
// Institution: Fermilab
// Plotting the curvature on the plate surface in x and y directions


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
    Reading(string label, double x_meas,  double y_meas,  double z_meas,  double i, double j, double k, double d,    double r) :
            Label(label), X_meas(x_meas), Y_meas(y_meas), Z_meas(z_meas), I(i),  J(j),  K(k),  Diameter(d), Roundness(r) {}


};

void cal_plot_curvature(char const* inputFile, char const* side, char const* feature){
    ////////// read the input file
    string dataToPlotFolder = "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/data/dataToPlot/";
    string plotsFolder =      "/Users/maral87-local/Desktop/Maral/Projects/Workflow-Presentations/HGCal/Daily/git_HGCal_CE-H_Plates_QA/plots/";

    vector<string> sortType = {"_sorted_x", "_sorted_y"};
    vector<string> direction = {"x", "y"};
    string outputFile = "_plot_curvature";
    string fileType = ".csv";

    string plateSide(side);

    // string dir = "mkdir " + folder;
    // gSystem->Exec(dir.c_str());

    for (int nFile = 0 ; nFile < sortType.size(); nFile++){
        ifstream ifile(dataToPlotFolder + inputFile + "_" + feature + sortType[nFile] + fileType );  
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

        ////////// calculate the curvature
        cout << "Number of points measured: " << file.size() << endl;
        
        vector<double> X_values; vector<double> Y_values;
        vector<double> Curvature; 
        double c = 0.0;
        double d_1 = 0.0; double d_2 = 0.0; 

        for (int n = 1; n<file.size(); n++){
            if (nFile == 0){
                d_1 = abs(file[n].X_meas - file[n-1].X_meas); // x_curvature
                d_2 = abs(file[n+1].X_meas - file[n].X_meas); // x_curvature
                c = (-1)*(file[n+1].Z_meas - 2* file[n].Z_meas + file[n-1].Z_meas)/pow(file[n].X_meas - file[n-1].X_meas,2); // x_curvature
            }
            else if (nFile == 1){
                d_1 = abs(file[n].Y_meas - file[n-1].Y_meas); // y_curvature
                d_2 = abs(file[n+1].Y_meas - file[n].Y_meas); // y_curvature
                c = (-1)*(file[n+1].Z_meas - 2* file[n].Z_meas + file[n-1].Z_meas)/pow(file[n].Y_meas - file[n-1].Y_meas,2); // y_curvature
            }

            
            if ( 49 < d_1 && d_1 < 51){
                if (49 < d_2 && d_2 < 51) {
                    // cout << "D1: " << d_1 << endl;
                    // cout << "D2: " << d_2 << endl;
                    // cout << "c: " << c << endl;
                    c = round(c*pow(10,6)*100)/100;
                    X_values.push_back(file[n].X_meas);
                    Y_values.push_back(file[n].Y_meas);
                    Curvature.push_back(c);
            
                };           
            };      
        };

        ////////// plot the curvature
        TCanvas *c1 = new TCanvas("c1","c1",1500,1000);
        string title = plateSide + " surface curvature in " + direction[nFile];
        TH2F *plot_z = new TH2F("", title.c_str(), 
                                48,0,2400,    // X axis
                                56,-1400,1400); // Y axis
        

        for (int i = 0; i<Curvature.size(); i++){
            plot_z->Fill(Y_values[i], X_values[i], Curvature[i]);
        }

        double x_max; double x_min; double y_max; double y_min; double c_max; double c_min;
        x_max = *max_element(X_values.begin(), X_values.end());
        x_min = *min_element(X_values.begin(), X_values.end());
        y_max = *max_element(Y_values.begin(), Y_values.end());
        y_min = *min_element(Y_values.begin(), Y_values.end());
        c_max = *max_element(Curvature.begin(), Curvature.end());
        c_min = *min_element(Curvature.begin(), Curvature.end());

        cout << "x_max: " << round(x_max/50)*50;
        cout << "y_max: " << round(y_max/50)*50;

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


        stringstream c_max_stream;  c_max_stream << fixed << setprecision(1) << c_max;
        string c_max_s = c_max_stream.str();
        
        stringstream c_min_stream;  c_min_stream << fixed << setprecision(1) << c_min;
        string c_min_s = c_min_stream.str();
        
        string max = "Max = " + c_max_s + " e-6 mm-1"; 
        string min = "Min = " + c_min_s + " e-6 mm-1"; 

        TText *tmax = new TText(1400,800,max.c_str());
        if (strcmp(side, "bottom") == 0 ) {tmax->SetX(x_axis_max - 200); tmax->SetY(-720);}
        tmax->SetTextAlign(31);
        tmax->SetTextColor(kBlack);
        tmax->SetTextFont(42);
        tmax->SetTextSize(0.05);
        tmax->SetTextAngle(0);
        tmax->Draw();

        TText *tmin = new TText(1400,720,min.c_str());
        if (strcmp(side, "bottom") == 0 ) {tmin->SetX(x_axis_max - 200); tmin->SetY(-800);}
        tmin->SetTextAlign(31);
        tmin->SetTextColor(kBlack);
        tmin->SetTextFont(42);
        tmin->SetTextSize(0.05);
        tmin->SetTextAngle(0);
        tmin->Draw();

        string note = "Acceptable curvature = = +/-20 e-6 mm-1" ;
        TText *tnote = new TText(320,-80,note.c_str());
        if (strcmp(side, "bottom") == 0 ) {tnote->SetX(320); tnote->SetY(y_axis_max-60);}
        tnote->SetTextAlign(22);
        tnote->SetTextColor(kBlack);
        tnote->SetTextFont(42);
        tnote->SetTextSize(0.04);
        tnote->SetTextAngle(0);
        tnote->Draw();

        string saveFile = plotsFolder + inputFile + "_curvature" + "_" + direction[nFile] + ".pdf";
        c1->SaveAs(saveFile.c_str());
        cout << "saved " << saveFile << endl;
        c1->Close();

        ////////// plot the curvature histogram 
        TCanvas *c2 = new TCanvas("c2","c2",1500,1000);
        string title2 = plateSide + " surface curvature in " + direction[nFile] + " #sqrt{(#Delta x^{2} + #Delta y^{2})} ";
        TH1* h1 = new TH1I("", title2.c_str(), 100, -60, 60);
        for (int i = 0; i < Curvature.size() ; i++){
            h1->Fill(Curvature[i]);
        }
        
        int binmax = h1->GetMaximumBin();
        double hist_max = h1->GetBinContent(binmax);

        // h1->GetXaxis()->SetRangeUser(0,x_max*1.1);
        h1->GetYaxis()->SetRangeUser(0,hist_max*1.1);

        h1->Draw("hist, text");

        TBox *b = new TBox(-20,0,20,hist_max*1.1); 
        b->SetFillColor(851); b->SetFillStyle(3002);
        b->Draw();

        TText *tnote2 = new TText(-35,-2,note.c_str());
        if (strcmp(side, "bottom") == 0 ) {tnote2->SetX(-35); tnote2->SetY(-2);}
        tnote2->SetTextAlign(22);
        tnote2->SetTextColor(kBlack);
        tnote2->SetTextFont(42);
        tnote2->SetTextSize(0.04);
        tnote2->SetTextAngle(0);
        tnote2->Draw();

        string saveFile2 = plotsFolder + inputFile + "_curvature_hist_" + direction[nFile] + ".pdf";  
        c2->SaveAs(saveFile2.c_str());
        cout << "saved " << saveFile2 << endl;
        c2->Close();


    }

    gSystem->Exit(0);
  
}
    

      

    

  
   
