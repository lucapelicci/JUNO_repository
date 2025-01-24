void reco_edm_reader() {

  TFile *f = TFile::Open("/junofs/users/epercalli/Light_off_tests/J25.1.0/root/RUN2035_rec.root");

  TTree *reco_tree = (TTree*)f->Get("Event/CdVertexRec/CdVertexRecEvt");

  JM::CdVertexRecEvt *Rec_obj = new JM::CdVertexRecEvt();
  reco_tree->SetBranchAddress("CdVertexRecEvt", &Rec_obj);

  Int_t nentries = reco_tree->GetEntries();

  cout << "\nTotal number of reconstructed events = " << nentries << endl;

  TH1F *herec_npe = new TH1F("herec_npe", "herec_npe", 210, -10, 2000);
  TH1F *herec_mev = new TH1F("herec_mev", "herec_mev", 180000, 0, 180);
  TH1F *hrecx = new TH1F("hrecx", "hrecx", 20000, -20000, 20000);
  TH1F *hrecy = new TH1F("hrecy", "hrecy", 20000, -20000, 20000);
  TH1F *hrecz = new TH1F("hrecz", "hrecz", 20000, -20000, 20000);
  TH1F *hrecr = new TH1F("hrecr", "hrecr", 20000, 0, 20000);

  TFile *fout = new TFile("run_2035_reco.root","RECREATE");

  Double_t xRec, yRec, zRec, rRec;
  Double_t epeRec, emevRec;
  Double_t energyQuality, positionQuality;
  TTree *tree = new TTree("tree","tree");
  tree->Branch("xRec",&xRec);
  tree->Branch("yRec",&yRec);
  tree->Branch("zRec",&zRec);
  tree->Branch("rRec",&rRec);
  tree->Branch("epeRec",&epeRec);
  tree->Branch("emevRec",&emevRec);
  tree->Branch("energyQuality",&energyQuality);
  tree->Branch("positionQuality",&positionQuality);

  for (Int_t i = 0; i < nentries; i=i+1) {

    reco_tree->GetEntry(i);

    if(i%int(500)==0 && i) cout << " " << i << "/" <<  nentries << " (" << 100*i/nentries << "%)" << endl;

    const vector<JM::RecVertex*>* vertices = &Rec_obj->vertices();

    Double_t x = vertices->at(0)->x();
    Double_t y = vertices->at(0)->y();
    Double_t z = vertices->at(0)->z();
    Double_t r = sqrt( x*x + y*y + z*z);
    Double_t en_npe = vertices->at(0)->peSum();
    Double_t en_mev = vertices->at(0)->energy();
    herec_npe->Fill(en_npe);
    herec_mev->Fill(en_mev);
    hrecx->Fill(vertices->at(0)->x());
    hrecy->Fill(vertices->at(0)->y());
    hrecz->Fill(vertices->at(0)->z());
    hrecr->Fill(r);
    energyQuality = vertices->at(0)->energyQuality();
    positionQuality = vertices->at(0)->positionQuality();

    xRec = x;
    yRec = y;
    zRec = z;
    rRec = r;

    epeRec = en_npe;
    emevRec = en_mev;

    tree->Fill();

  }

  fout->cd();
  tree->Write();
  herec_npe->Write();
  herec_mev->Write();
  hrecx->Write();
  hrecy->Write();
  hrecz->Write();
  hrecr->Write();
  fout->Close();
}
