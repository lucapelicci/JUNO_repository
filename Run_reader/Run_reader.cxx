#define SNIPER_VERSION_2 1
#include "Run_reader.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "Event/SimHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdLpmtCalibEvt.h"
#include "Event/CdTriggerHeader.h"
#include "Event/CdTriggerEvt.h"
#include "RootWriter/RootWriter.h"
#include <numeric>

#include "TH1F.h"
#include "TTree.h"

DECLARE_ALGORITHM(Run_reader);

Run_reader::Run_reader(const std::string& name) 
	: AlgBase(name),
	  m_iEvt(0),
	  m_buf(0)
{
}

bool Run_reader::initialize() {

    LogDebug << "initializing" << std::endl;
    auto toptask = getRoot();
	
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");

    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
	
    m_buf = navBuf.data();

    SniperPtr<RootWriter> rw(getParent(), "RootWriter");
    if (rw.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enable it in your job option file."
                 << std::endl;
         return false;
    }
	
    events = rw->bookTree(*m_par,"tree/CdEvents","Events Tree");
    events->Branch("timestamp",&timestamp);
    events->Branch("charge",&charge);
    events->Branch("time",&time);
    events->Branch("npe",&total_npe,"npe/I");
    events -> Branch("TriggerType",&trigger_type);

    return true;
}

bool Run_reader::execute() {

    LogDebug << "=====================================" << std::endl;
    LogDebug << "executing: " << m_iEvt++
             << std::endl;
                 
    JM::CdLpmtCalibEvt* calibevent = 0;
    JM::CdTriggerEvt* triggerevent = 0;

    auto nav = m_buf->curEvt();

    auto calibheader = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
    if (calibheader) calibevent = (JM::CdLpmtCalibEvt*)calibheader->event();
	
    if (!calibevent) {
        LogInfo << "No CalibEvt found, skipping..." << std::endl;
        return true;
    }

    auto triggerheader = JM::getHeaderObject<JM::CdTriggerHeader>(nav);
    if (triggerheader) triggerevent = (JM::CdTriggerEvt*) triggerheader -> event();

    if (!triggerevent) {
        LogInfo << "No CdTriggerEvt found for this event " << std::endl;
        trigger_type = "None";
    } else {
        const auto& triggerelement = triggerevent -> triggerType();
        if (triggerelement.size() != 1) LogInfo << "------------- Strange trigger element! -------------" << std::endl;
        else trigger_type = triggerelement[0];
    }

    charge.clear();
    time.clear();
    PMTID.clear();

    for (const auto& element : calibevent->calibPMTCol()) {

        PMTID.push_back(element->pmtId());

        for (auto pmtChannel : element -> charge() ) {
            charge.push_back(pmtChannel);
        }
        for (auto pmtChannel : element -> time() ) {
            time.push_back(pmtChannel);
        }

    }

    total_npe = std::accumulate(charge.begin(),charge.end(),0.0);
    timestamp = nav->TimeStamp();

    events -> Fill();
    return true;

}

bool Run_reader::finalize() {

    return true;
    
}
