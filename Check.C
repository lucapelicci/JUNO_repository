// How to use it
// root -l
// .L Check.C
// test_edm("/junofs/users/claudiocoletta/Light_off/J25.1.0/root/RUN2598_single.root")



void test_edm(const char* inputFile) {

  TFile *f = TFile::Open(inputFile);

  TTree *EvtNavigator_tree = (TTree*)f->Get("Meta/navigator");
  int nentries_EvtNavigator_tree = EvtNavigator_tree->GetEntries();
  cout << "Meta/Navigator entries = " << nentries_EvtNavigator_tree << endl;

  TTree *CD_Trigger_tree = (TTree*)f->Get("Event/CdTrigger/CdTriggerEvt");
  //JM::CdTriggerEvt *CD_Trigger_obj = new JM::CdTriggerEvt();
  //CD_Trigger_tree->SetBranchAddress("CdTriggerEvt", &CD_Trigger_obj);
  int nentries_CD_Trigger_tree = CD_Trigger_tree->GetEntries();
  cout << "Event/CdTrigger/CdTriggerEvt entries = " << nentries_CD_Trigger_tree << endl;
  
  TTree *Wp_Trigger_tree = (TTree*)f->Get("Event/WpTrigger/WpTriggerEvt");
  //JM::WpTriggerEvt *Wp_Trigger_obj = new JM::WpTriggerEvt();
  //Wp_Trigger_tree->SetBranchAddress("WpTriggerEvt", &Wp_Trigger_obj);
  int nentries_Wp_Trigger_tree = Wp_Trigger_tree->GetEntries();
  cout << "Event/WpTrigger/WpTriggerEvt entries = " << nentries_Wp_Trigger_tree << endl;
  
  TTree *CD_calib_tree = (TTree*)f->Get("Event/CdLpmtCalib/CdLpmtCalibEvt");
  //JM::CdLpmtCalibEvt *CD_calib_obj = new JM::CdLpmtCalibEvt();
  //CD_calib_tree->SetBranchAddress("CdLpmtCalibEvt", &CD_calib_obj);
  int nentries_CD_calib_tree = CD_calib_tree->GetEntries();
  cout << "Event/CdLpmtCalib/CdLpmtCalibEvt entries = " << nentries_CD_calib_tree << endl;
  
  TTree *CD_OECRaw_tree = (TTree*)f->Get("/Event/CdLpmtElec_OECRaw/CdLpmtElecEvt");
  //JM::CdLpmtElecEvt *CD_CdLpmtElecEvt_obj = new JM::CdLpmtElecEvt();
  //CD_OECRaw_tree->SetBranchAddress("CdLpmtElecEvt", &CD_CdLpmtElecEvt_obj);
  int nentries_CD_OECRaw_tree = CD_OECRaw_tree->GetEntries();
  cout << "Event/CdLpmtElec_OECRaw/CdLpmtElecEvt entries = " << nentries_CD_OECRaw_tree << endl;  
  
  TTree *WP_OECRaw_tree = (TTree*)f->Get("/Event/WpElec_OECRaw/WpElecEvt");
  //JM::WpElecEvt *WP_WpElecEvt_obj = new JM::WpElecEvt();
  //WP_OECRaw_tree->SetBranchAddress("WpElecEvt", &WP_WpElecEvt_obj);
  int nentries_WP_OECRaw_tree = WP_OECRaw_tree->GetEntries();
  cout << "Event/WpElec_OECRaw/WpElecEvt entries = " << nentries_WP_OECRaw_tree << endl;    
 
   TTree *CD_FPGA_tree = (TTree*)f->Get("/Event/CdLpmtElec_FPGA/CdLpmtElecEvt");
  //JM::CdLpmtElecEvt *CD_CdLpmtElecEvt_FPGA_obj = new JM::CdLpmtElecEvt();
  //CD_FPGA_tree->SetBranchAddress("CdLpmtElec", &CD_CdLpmtElecEvt_FPGA_obj);
  int nentries_CD_FPGA_tree = CD_FPGA_tree->GetEntries();
  cout << "Event/CdLpmtElec_FPGA/CdLpmtElecEvt entries = " << nentries_CD_FPGA_tree << endl;

  TTree *WP_FPGA_tree = (TTree*)f->Get("/Event/WpElec_FPGA/WpElecEvt");
  int nentries_WP_FPGA_tree = WP_FPGA_tree->GetEntries();
  cout << "Event/WpLpmtElec_FPGA/WpLpmtElecEvt entries = " << nentries_WP_FPGA_tree << endl;  

}
