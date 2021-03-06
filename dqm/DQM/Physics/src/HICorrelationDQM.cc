/*
 *  See header file for a description of this class.
 *
 *  $Date: 2010/09/17 19:01:00 $
 *  $Revision: 1.0 $
 *  \author Shengquan Tuo - Vanderbilt
 */

#include "DQM/Physics/src/HICorrelationDQM.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <vector>

#include <string>
#include <cmath>




#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/SimpleVector.h"
#include "Math/Vector3D.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Track/interface/CoreSimTrack.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/UpdatablePSimHit.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"


#define Pi 3.14159265358979


  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace math;
  using namespace HepMC;

HICorrelationDQM::HICorrelationDQM(const edm::ParameterSet& iConfig)//:
  //  centLabel_(iConfig.getUntrackedParameter<edm::InputTag>("hiCentrality"))
{

}

HICorrelationDQM::~HICorrelationDQM() { 
  

}


void HICorrelationDQM::beginJob() {
 
  theDbe = Service<DQMStore>().operator->();

  //Book MEs 
 
  theDbe->setCurrentFolder("Physics/HICorrelation");  

  //book histograms for multiplicity distribution
  //MEcontainer_["eHcal_eEcal"] = theDbe->book2D("eHcal_eEcal", "HCAL Energy vs. ECAL Energy;ECAL Energy;HCAL Energy", 2000, 0, 15000,  2000, 0, 14000);
  //MEcontainer_["eHF_eEcal"] = theDbe->book2D("eHF_eEcal", "HF Energy vs. ECAL Energy;ECAL Energy;HF Energy", 2000, 0, 18000,  2000, 0, 100000);
  ///MEcontainer_["eHF_eeHcal"] = theDbe->book2D("eHF_eeHcal", "HF Energy vs. HCAL Endcap Energy;HCAL EndCap Energy;HF Energy", 2000, 0, 12000,  2000, 0, 100000);
  //MEcontainer_["eHF_ebHcal"] = theDbe->book2D("eHF_ebHcal", "HF Energy vs. HCAL Barrel Energy;HCAL Barrel Energy;HF Energy", 2000, 0, 1200,  2000, 0, 100000);
  ///MEcontainer_["eHF_esZDC"] = theDbe->book2D("eHF_esZDC", "HF Energy vs. ZDC Energy;ZDC Energy;HF Energy", 2000, 0, 100000,  2000, 0, 100000);
  //MEcontainer_["epcaloeh"] = theDbe->book1D("epcaloeh", "EvtPlane(etECAL + etHCAL)", 80, -2, 2);
  //MEcontainer_["epcalohfeh"] = theDbe->book1D("epcalohfeh", "EvtPlane(etECAL + etHCAL + etHF)", 80, -2, 2);
  //book histograms for elliptic flow
  //MEcontainer_["event"] = theDbe->book1D("event", "Event distribution", 10, 0, 10);

  MEcontainer_["eHcal_eEcal"] = theDbe->book2D("eHcal_eEcal", "HCAL Energy vs. ECAL Energy", 200, 0, 1400,  200, 0, 1400);
  MEcontainer_["eHF_eEcal"] = theDbe->book2D("eHF_eEcal", "HF Energy vs. ECAL Energy", 200, 0, 1400,  200, 0, 10000);
  MEcontainer_["eHF_eeHcal"] = theDbe->book2D("eHF_eeHcal", "HF Energy vs. HCAL Endcap Energy", 200, 0, 1200,  200, 0, 10000);
  MEcontainer_["eHF_ebHcal"] = theDbe->book2D("eHF_ebHcal", "HF Energy vs. HCAL Barrel Energy", 200, 0, 120,  200, 0, 10000);
  MEcontainer_["eHF_esZDC"] = theDbe->book2D("eHF_esZDC", "HF Energy vs. ZDC Energy", 200, 0, 7000,  200, 0, 10000);
  MEcontainer_["esZDC_eEcal"] = theDbe->book2D("esZDC_eEcal", "ZDC Energy vs. ECAL Energy", 200, 0, 1400,  200, 0, 7000);

}

void HICorrelationDQM::endJob(void) {

}


void HICorrelationDQM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  double Ehcal=0;
  double Eecal=0;
  double Ehf=0;
  double Eehcal=0;
  double Ebhcal=0;
  //double Eszdc=0;
  //int Ntkpixel=0;
  //double Esrhithf=0;
  double Ezdc=0;
  long nhits=0;
  double temp=0;

  Handle<ZDCRecHitCollection> zdc_recHits_h;
  ESHandle<HcalDbService> conditions;
  iEvent.getByType(zdc_recHits_h);
  const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;

  iSetup.get<HcalDbRecord>().get(conditions);
  const HcalQIEShape* shape = conditions->getHcalShape();
  if(zdc_recHits){
		for (ZDCRecHitCollection::const_iterator zhit=zdc_recHits->begin();zhit!=zdc_recHits->end();zhit++){		
		  //int iSide      = (zhit->id()).zside();
		  //int iSection   = (zhit->id()).section();
		  //int iChannel   = (zhit->id()).channel();
		  //int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

		  //RecData[chid]=zhit->energy();
		  //RecData[chid+18]=zhit->time();
		  temp=zhit->energy();
		  if(temp<0) Ezdc +=-temp;
		  if(temp>=0) Ezdc +=temp;
                  //cout<<"### Ezdc00 = "<<zhit->energy()<<endl;
		  nhits++;
		}
		
		//ZDCRecoTree->Fill();
  }


      Handle<CaloTowerCollection> calotower;
      iEvent.getByLabel("towerMaker",calotower);
      
      if(!calotower.isValid()){
        cout << "Error! Can't get calotower product!" << endl;
       return ;
      }
      for (CaloTowerCollection::const_iterator j = calotower->begin();j !=calotower->end(); j++) {

	double tower_eta = j->eta();
	//double tower_phi = j->phi();
        double tower_energy_e   = j->emEnergy();
	double tower_energy_h   = j->hadEnergy();
	double tower_energy     = tower_energy_e + tower_energy_h;

	//double tower_energyet_e   = j->emEt();
	//double tower_energyet_h   = j->hadEt();
        //double tower_energyet     = tower_energyet_e + tower_energyet_h;


        if (fabs(tower_eta)>3.){ 

	Ehf +=tower_energy_h; ///// tower_energy_e=0 when |eta|>3.
	}
        if (fabs(tower_eta)<3.){ 
	Eecal +=tower_energy_e; /////
	Ehcal +=tower_energy_h; /////
        if (fabs(tower_eta)<1.5){ 
	  Ebhcal +=tower_energy_h; /////
	  //cout<<"Ebhcal = "<<Ebhcal<<endl;
	} 
        if (fabs(tower_eta)>1.5){
	  Eehcal +=tower_energy_h; /////
          //cout<<"Eehcal = "<<Eehcal<<endl;
	} 
	}
      }

	MEcontainer_["eHcal_eEcal"]->Fill(Eecal/10.0,Ehcal/10.0);
	MEcontainer_["eHF_eEcal"]->Fill(Eecal/10.0,Ehf/10.0);
	MEcontainer_["eHF_eeHcal"]->Fill(Eehcal/10.0,Ehf/10.0);
	MEcontainer_["eHF_ebHcal"]->Fill(Ebhcal/10.0,Ehf/10.0);
	MEcontainer_["eHF_esZDC"]->Fill(Ezdc/10.0,Ehf/10.0);
	MEcontainer_["esZDC_eEcal"]->Fill(Eecal/10.0,Ezdc/10.0);
    
}













