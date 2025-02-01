#include "CLHEP/Vector/LorentzVector.h"

void reco_edm_reader() {

  TFile *f = TFile::Open("/junofs/users/dbasilico/RealData/Run2648_wp_rec.root");

  TTree *reco_tree = (TTree*)f->Get("/Event/WpRec/WpRecEvt");

  JM::WpRecEvt *Rec_obj = new JM::WpRecEvt();
  
  reco_tree->SetBranchAddress("WpRecEvt", &Rec_obj);

  Int_t nentries = reco_tree->GetEntries();

  cout << "\nTotal number of reconstructed events = " << nentries << endl;


  for (Int_t i = 0; i < nentries; i=i+1) {

    reco_tree->GetEntry(i);

    if(i%int(10)==0 && i) cout << " " << i << "/" <<  nentries << " (" << 100*i/nentries << "%)" << endl;

    const vector<JM::RecTrack*>* tracks = &Rec_obj->tracks();

    Double_t peSum = tracks->at(0)->peSum();
    CLHEP::HepLorentzVector direction = tracks->at(0)->direction();
    CLHEP::HepLorentzVector start = tracks->at(0)->start();
    CLHEP::HepLorentzVector end = tracks->at(0)->end();

    cout << i << "\t"  << peSum << "\t" << direction[0] << "\t" << direction[1] << "\t" << direction[2] << "\t" << start[0] << "\t" << end[0] << endl;
  }

}
