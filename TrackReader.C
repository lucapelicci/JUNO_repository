#include "CLHEP/Vector/LorentzVector.h"
#include <ostream>

void reco_edm_reader() {
  


  ofstream output_text;
  output_text.open("output_text.txt");

  TFile *f = TFile::Open("/junofs/users/dbasilico/RealData/Run2648_wp_rec.root");


  TTree *navigator = (TTree*)f->Get("Meta/navigator");
  JM::EvtNavigator* nav=0;
  
  navigator->SetBranchAddress("EvtNavigator",&nav);
  vector<TTimeStamp> t1;

  Int_t nentries_nav = 0;
  for(Int_t i = 0; i< navigator->GetEntries(); i++){
  
  navigator->GetEntry(i);

  //cout << nav->getDetectorType() << endl;

 if(nav->getDetectorType() == 1){
          t1.push_back(nav->TimeStamp());
 //         cout << nav->TimeStamp() << endl;
	nentries_nav++;
	cout << nentries_nav << endl;
 }
  }

  TTree *reco_tree = (TTree*)f->Get("/Event/WpRec/WpRecEvt");

  JM::WpRecEvt *Rec_obj = new JM::WpRecEvt();
  
  reco_tree->SetBranchAddress("WpRecEvt", &Rec_obj);

  Int_t nentries = reco_tree->GetEntries();

  cout << "\nTotal number of reconstructed events = " << nentries << endl;
  
  if(nentries != nentries_nav){
	  cout << "ERROR: rec events number =/= navigator WP entries " << nentries << "   " << nentries_nav << endl;
	  return ;
  }

  for (Int_t i = 0; i < nentries; i=i+1) {

    reco_tree->GetEntry(i);

    if(i%int(10)==0 && i) cout << " " << i << "/" <<  nentries << " (" << 100*i/nentries << "%)" << endl;
   
    const vector<JM::RecTrack*>* tracks = &Rec_obj->tracks();
    cout << tracks->size() << endl;
    
    for(Int_t i_track = 0; i_track < tracks->size(); i_track++){

    	Double_t peSum = tracks->at(i_track)->peSum();
	Float_t quality = tracks->at(i_track)->quality();
    	CLHEP::HepLorentzVector direction = tracks->at(i_track)->direction();
    	CLHEP::HepLorentzVector start = tracks->at(i_track)->start();
    	CLHEP::HepLorentzVector end = tracks->at(i_track)->end();

    	//cout << i << "\t"  << i_track << "\t" << peSum << "\t" << direction[0] << "\t" << direction[1] << "\t" << direction[2] << "\t" << start[0] << "\t" << end[0] << endl;
    	output_text << i << "\t" << i_track << "\t"  << t1.at(i) << "\t" << peSum << "\t" << direction[0] << "\t" << direction[1] << "\t" << direction[2] << "\t" << start[0] << "\t" << start[1] << "\t" << start[2] << "\t" << start[3] << "\t" << end[0] << "\t" << end[1] << "\t" << end[2] << "\t" << end[3] << "\t" << quality << endl;
    }
    
    }

output_text.close();

}
