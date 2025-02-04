#include <cstdlib>
#include <map>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class JUNO_PMTs {

    private :
        map <int,tuple<int,double,double,double,double,double>> Map_PMTs;
        string CdPmts_Path;
        string WpPmts_Path;
        string BottomPmts_Path;

    public : 

        JUNO_PMTs();
        JUNO_PMTs(string);
        JUNO_PMTs(string, string, string);

        ~JUNO_PMTs();

        void SetCdPmts(string);
        void SetWpPmts(string);
        void SetBottomPmts(string);

        string GetCdFileName() {return CdPmts_Path;}
        string GetWpFileName() {return WpPmts_Path;}
        string GetBottomFileName() {return BottomPmts_Path;}

        int GetType(int PmtNo) {return get<0>(Map_PMTs[PmtNo]);}
        double GetX(int PmtNo) {return get<1>(Map_PMTs[PmtNo]);}
        double GetY(int PmtNo) {return get<2>(Map_PMTs[PmtNo]);}
        double GetZ(int PmtNo) {return get<3>(Map_PMTs[PmtNo]);}
        double GetTheta(int PmtNo) {return get<4>(Map_PMTs[PmtNo]);}
        double GetPhi(int PmtNo) {return get<5>(Map_PMTs[PmtNo]);}

        tuple <double, double, double> GetCartesian (int PmtNo) {return make_tuple(get<1>(Map_PMTs[PmtNo]),get<2>(Map_PMTs[PmtNo]),get<3>(Map_PMTs[PmtNo]));}
        tuple <double, double> GetSpherical (int PmtNo) {return make_tuple(get<4>(Map_PMTs[PmtNo]),get<5>(Map_PMTs[PmtNo]));}
        tuple <int,double,double,double,double,double> GetAll (int PmtNo) {return Map_PMTs[PmtNo];}

        bool isHama(int PmtNo);
        bool isNNVT(int PmtNo);

};

void Read_file (string FileName, std::map <int,tuple<int,double,double,double,double,double>>& out_map, bool is_Wp) {

    ifstream PMT_File(FileName);

    if (!PMT_File.is_open()) {
        cerr << "Error: Could not open file.\n";
        exit(1);
    }

    string line,value;

    for (int i=0;i<4;i++) getline (PMT_File,line); //Four header lines in file

    while (getline(PMT_File,line)) {

        stringstream ss(line);
        vector <string> separated_values;

        while (getline(ss,value,',')) {
            separated_values.push_back(value);
        }

        tuple <int,double,double,double,double,double> PMT_Pos;

        if (is_Wp) PMT_Pos = make_tuple(stoi(separated_values[6]),stod(separated_values[10]),stod(separated_values[11]),stod(separated_values[12]),stod(separated_values[13]),stod(separated_values[14]));
        else PMT_Pos = make_tuple(stoi(separated_values[6]),stod(separated_values[9]),stod(separated_values[10]),stod(separated_values[11]),stod(separated_values[12]),stod(separated_values[13]));

        out_map[stoi(separated_values[0])] = PMT_Pos;

        separated_values.clear();

    }

    PMT_File.close();


};

JUNO_PMTs::JUNO_PMTs () {};

JUNO_PMTs::JUNO_PMTs (std::string FileName) {

    CdPmts_Path = FileName;

    map <int,tuple<int,double,double,double,double,double>> Data_map;

    Read_file(FileName,Data_map,false);

    Map_PMTs = Data_map;
    
};

JUNO_PMTs::JUNO_PMTs (string CdFileName, string WpFileName, string BottomFilename) {

    CdPmts_Path = CdFileName;
    WpPmts_Path = WpFileName;
    BottomPmts_Path = BottomFilename;

    map <int,tuple<int,double,double,double,double,double>> Data_map;
    
    Read_file(CdFileName,Data_map,false);
    Read_file(WpFileName,Data_map,true);
    Read_file(BottomFilename,Data_map,true);

    Map_PMTs = Data_map;
    
}

JUNO_PMTs::~JUNO_PMTs () {
    Map_PMTs.clear();
}

bool JUNO_PMTs::isHama(int PmtNo) {

    if (get<0>(Map_PMTs[PmtNo]) == 1) return true;
    else return false;

}

bool JUNO_PMTs::isNNVT(int PmtNo) {

    if (get<0>(Map_PMTs[PmtNo]) != 1) return true;
    else return false;
    
}

void JUNO_PMTs::SetCdPmts(string CdFileName) {

    if (!CdPmts_Path.empty()) {
        cout << "WARNING: you are resetting an already present Cd PMTs file " << endl;
    } 

    Read_file(CdFileName,Map_PMTs,false);

    return;
}

void JUNO_PMTs::SetWpPmts(string WpFileName) {

    if (!WpPmts_Path.empty()) {
        cout << "WARNING: you are resetting an already present Wp PMTs file " << endl;
    } 

    Read_file(WpFileName,Map_PMTs,true);

    return;
}

void JUNO_PMTs::SetBottomPmts(string BottomFileName) {

    if (!BottomPmts_Path.empty()) {
        cout << "WARNING: you are resetting an already present Bottom PMTs file " << endl;
    } 

    Read_file(BottomFileName,Map_PMTs,true);

    return;
}
