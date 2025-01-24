#include "Identifier/CdID.h"
#include "Identifier/IDService.h"


void test_edm() {

  TFile *f = TFile::Open("RUN2035.root");

  TTree *calib_tree = (TTree*)f->Get("Event/CdLpmtCalib/CdLpmtCalibEvt");

  JM::CdLpmtCalibEvt *calib_obj = new JM::CdLpmtCalibEvt();
  calib_tree->SetBranchAddress("CdLpmtCalibEvt", &calib_obj);


  int nentries = 1;//calib_tree->GetEntries();

  double totalPE = 0;

  for(size_t i=0; i<nentries;i++){

    if (i%1000==0) cout << i << "/" << nentries << " --> " << 100*i/nentries << "%" << endl;

    calib_tree->GetEntry(i);

    const list<JM::CalibPmtChannel*> calibPMTCol = calib_obj->calibPMTCol();
    list<JM::CalibPmtChannel*>::const_iterator calibHitIterator = calibPMTCol.begin();

    for (int j = 0; j < calibPMTCol.size(); j++) {  //loop over all pmts
      const JM::CalibPmtChannel *calibVar = *calibHitIterator;
      //PMT ID
      int pmtIdVar = calibVar->pmtId();
      Identifier id = Identifier(pmtIdVar);
      unsigned int detID = CdID::id(static_cast<unsigned int>(pmtIdVar));
      int pmtId = CdID::module(Identifier(detID));

      cout << "PMT " << pmtId << ": " << endl;
      for (int countPhoton = 0; countPhoton < calibVar->size(); countPhoton++) {

        cout << calibVar->time(countPhoton) << " " << calibVar->charge(countPhoton) << endl;
        totalPE += calibVar->charge(countPhoton);

      }
      cout << "Charge = " << totalPE << endl;
      cout << endl;

      calibHitIterator++;

    }

  }


}
