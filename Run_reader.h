#ifndef RUN_READER_H
#define RUN_READER_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

#include <fstream>

class TH1F;
class TTree;

class Run_reader : public AlgBase //Change the name to something more descriptive
{
    public :
    Run_reader() : Run_reader("Run_reader") {}
    Run_reader(const std::string& name); //Constructor, must have same name as the class

    // Following functions are needed by SNiPER, so they are mandatory
    bool initialize();
    bool execute();
    bool finalize();

    private :

        int m_iEvt; // To count the loops
        JM::NavBuffer* m_buf; // Our buffer with the events
        
        // Define variables that are globally used
	TTimeStamp timestamp;
        int total_npe;
        std::vector<int> PMTID; 
        std::vector<float> charge ,time;
        TString trigger_type;

        TTree* events;
};

#endif

