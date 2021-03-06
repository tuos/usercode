/*
 *  See header file for a description of this class.
 *
 *  $Date: 2010/07/07 18:31:00 $
 *  $Revision: 1.0 $
 *  \author Shengquan Tuo - Vanderbilt
 */

#include "DQM/Physics/src/HIFlowDQM.h"


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

#define Pi 3.14159265358979


  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace math;
  using namespace HepMC;

HIFlowDQM::HIFlowDQM(const edm::ParameterSet& iConfig)//:
  //  centLabel_(iConfig.getUntrackedParameter<edm::InputTag>("hiCentrality"))
{

}

HIFlowDQM::~HIFlowDQM() { 
  

}


void HIFlowDQM::beginJob() {
 
  theDbe = Service<DQMStore>().operator->();

  //Book MEs 
 
  theDbe->setCurrentFolder("Physics/HIFlow");  

  //book histograms for multiplicity distribution
  MEcontainer_["eptrkpos"] = theDbe->book1D("eptrkpos", "EvtPlaneTracksPosEta;Event Plane Angle;Event", 80, -2, 2);
  MEcontainer_["eptrkmid"] = theDbe->book1D("eptrkmid", "EvtPlaneTracksMidEta;Event Plane Angle;Event", 80, -2, 2);
  MEcontainer_["eptrkneg"] = theDbe->book1D("eptrkneg", "EvtPlaneTracksNegEta;Event Plane Angle;Event", 80, -2, 2);
  MEcontainer_["epcalohf"] = theDbe->book1D("epcalohf", "EvtPlane(etHF);Event Plane Angle;Event", 80, -2, 2);
  MEcontainer_["epcaloeh"] = theDbe->book1D("epcaloeh", "EvtPlane(etECAL + etHCAL);Event Plane Angle;Event", 80, -2, 2);
  MEcontainer_["epcalohfeh"] = theDbe->book1D("epcalohfeh", "EvtPlane(etECAL + etHCAL + etHF);Event Plane Angle;Event", 80, -2, 2);
  //book histograms for elliptic flow
  //MEcontainer_["event"] = theDbe->book1D("event", "Event distribution", 10, 0, 10);



}

void HIFlowDQM::endJob(void) {

}


void HIFlowDQM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  
  double Psi;
  // calo part
  //double hfsin, ehsin, hfcos, ehcos;
  double hfehsin,hfehcos;
  double hfs=0;
  double ehs=0;
  double hfehs=0;
  double hfc=0;
  double ehc=0;
  double hfehc=0;
  
  //double trkpossin,trkmidsin,trknegsin,trkposcos,trkmidcos,trknegcos;
  //double trkposs=0;
  //double trkmids=0;
  //double trknegs=0;
  //double trkposc=0;
  //double trkmidc=0;
  //double trknegc=0;

      Handle<CaloTowerCollection> calotower;
      iEvent.getByLabel("towerMaker",calotower);
      
      if(!calotower.isValid()){
        cout << "Error! Can't get calotower product!" << endl;
       return ;
      }
      for (CaloTowerCollection::const_iterator j = calotower->begin();j !=calotower->end(); j++) {

	double tower_eta = j->eta();
	double tower_phi = j->phi();
        double tower_energy_e   = j->emEnergy();
	double tower_energy_h   = j->hadEnergy();
	double tower_energy     = tower_energy_e + tower_energy_h;

	double tower_energyet_e   = j->emEt();
	double tower_energyet_h   = j->hadEt();
        double tower_energyet     = tower_energyet_e + tower_energyet_h;

        //hfehsin=tower_energy*sin(2.*tower_phi);
        //hfehcos=tower_energy*cos(2.*tower_phi);
        hfehsin=tower_energyet*sin(2.*tower_phi);
        hfehcos=tower_energyet*cos(2.*tower_phi);

        hfehs +=hfehsin;
        hfehc +=hfehcos;
        if (fabs(tower_eta)>3.){ 
        hfs +=hfehsin;
        hfc +=hfehcos;
	}
        if (fabs(tower_eta)<3.){ 
        ehs +=hfehsin;
        ehc +=hfehcos;
	}
      }
      Psi=atan2(hfehs,hfehc)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      MEcontainer_["epcalohfeh"]->Fill(Psi);
      Psi=atan2(hfs,hfc)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      MEcontainer_["epcalohf"]->Fill(Psi);
      Psi=atan2(ehs,ehc)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      MEcontainer_["epcaloeh"]->Fill(Psi);


   // trakc part
   double track_eta;
   double track_phi;
   double track_pt;
   double track_charge;
   double trackSin_eta_mid = 0;
   double trackCos_eta_mid = 0;
   double trackSin_eta_pos = 0;
   double trackCos_eta_pos = 0;
   double trackSin_eta_neg = 0;
   double trackCos_eta_neg = 0;
   Handle<reco::TrackCollection> tracks;
   iEvent.getByLabel("hiSelectedTracks", tracks);
   // cout << " TRACKS Size " << tracks->size() << endl;
   if(!tracks.isValid()){
     cout << "Error! Can't get selectTracks!" << endl;
     return ;
   }
   for(reco::TrackCollection::const_iterator j = tracks->begin(); j != tracks->end(); j++){
    
     track_eta = j->eta();
     track_phi = j->phi();
     track_pt = j->pt();
     track_charge = j->charge();

     if(fabs(track_eta)<0.75){
       trackSin_eta_mid+=sin(2*track_phi);
       trackCos_eta_mid+=cos(2*track_phi);
     }
   
     if((track_eta >= 0.75) && (track_eta < 2.0)){
       trackSin_eta_pos+=sin(2*track_phi);
       trackCos_eta_pos+=cos(2*track_phi);
     }
     if((track_eta <= -0.75) && (track_eta > -2.0)){
       trackSin_eta_neg+=sin(2*track_phi);
       trackCos_eta_neg+=cos(2*track_phi);
     }
   }
      Psi=atan2(trackSin_eta_pos,trackCos_eta_pos)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      //if(Psi!=0)
      MEcontainer_["eptrkpos"]->Fill(Psi);
      Psi=atan2(trackSin_eta_mid,trackCos_eta_mid)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      //if(Psi!=0)
      MEcontainer_["eptrkmid"]->Fill(Psi);
      Psi=atan2(trackSin_eta_neg,trackCos_eta_neg)/2.;
      //if(Psi>Pi/2) Psi-=Pi;
      //if(Psi<-Pi/2) Psi+=Pi;
      //if(Psi!=0)
      MEcontainer_["eptrkneg"]->Fill(Psi);


    
}













