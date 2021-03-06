/*
 * =====================================================================================
 *
 *       Filename:  TauRecoCalib.cc
 *
 *    Description:
 *
 *         Author:  Laura Dodd, laura.dodd@cern.ch
 *        Company:  UW Madison
 *      This computes a root file to calibrate RCT based on a reco object
 *
 * =====================================================================================
 */
#include "L1Trigger/LauraTriggerTools/interface/ExpressionNtuple.h"
#include "L1Trigger/LauraTriggerTools/interface/helpers.h"
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <TTree.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"
#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"


typedef std::vector<edm::InputTag> VInputTag;

//typedef std::vector<unsigned int> PackedUIntCollection;
using namespace std;
using namespace edm;
class TauRecoCalib : public edm::EDAnalyzer {
	public:
		explicit TauRecoCalib(const edm::ParameterSet& pset);
		virtual ~TauRecoCalib();
		static const unsigned int N_TOWER_PHI;
		static const unsigned int N_TOWER_ETA;
		void analyze(const edm::Event& evt, const edm::EventSetup& es);
	private:


		TTree* tree;
		edm::EDGetTokenT<std::vector<LumiScalers>> scalerSrc_;
		edm::EDGetTokenT<std::vector<L1CaloRegion>> l1Digis_;
		edm::EDGetTokenT<std::vector<reco::Vertex>> pvSrc_;
		edm::EDGetTokenT<edm::SortedCollection<EcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> >> ecalSrc_;
		edm::EDGetTokenT<edm::SortedCollection<HcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<HcalTriggerPrimitiveDigi> >> hcalSrc_;

                edm::EDGetTokenT<pat::TauCollection> tauToken_;


		//initialize run info
		unsigned int run_;
		unsigned int lumi_;
		unsigned long int event_;
		unsigned int npvs_;
		float instLumi_;


		//L1 EG information
		vector<float> egCandPt_;
		vector<float> egCandEta_;
		vector<float> egCandPhi_;

		//Reco information
		vector<float>* pts_;
		vector<float>* dms_;
		vector<float>* etas_;
		vector<float>* phis_;

		//TPG MAX information
		vector<float> maxTPGPt_;
		vector<float> maxTPGPt_eta_;
		vector<float> maxTPGPt_phi_;

		//TPG E information
		float maxETPGPt;
		float maxETPGPt_eta;
		float maxETPGPt_phi;

		//TPG H information
		float maxHTPGPt;
		float maxHTPGPt_eta;
		float maxHTPGPt_phi;

		//TPG information for matching
		float maxTPGPt;
		float maxTPGPt_eta;
		float maxTPGPt_phi;


		//TPG information
		vector<float> TPGVeto_; //This determines majority in central TPG
		vector<int> ptbin_; //This determines majority in central TPG

		vector<int> TPG5x5_;
		vector<int> TPG5x5_gcteta_;
		vector<int> TPG5x5_tpgeta_;
		vector<int> TPG5x5_tpgphi_;
		vector<int> TPGh5x5_;
		vector<int> TPGe5x5_;

		//TPG calibration vectors
		vector<double> TPGSF1_;
		vector<double> TPGSFp_;


		//calibrated TPGcollections
		vector<int> cTPG5x5_;
		vector<int> cTPGh5x5_;
		vector<int> cTPGe5x5_;


		//handles
		Handle<LumiScalersCollection> lumiScalers;
		Handle<reco::VertexCollection> vertices;
		Handle<EcalTrigPrimDigiCollection> ecal;
		Handle<HcalTrigPrimDigiCollection> hcal;


		vector<double> sinPhi;
		vector<double> cosPhi;

		bool ECALOn;
		bool v_off;
		bool v1;
		bool v3;

		double LSB = 0.5; //used

		//Keep track of tower energies
		vector<vector<unsigned int>> eTowerETCode;
		vector<vector<unsigned int>> eCorrTowerETCode;
		vector<vector<unsigned int>> hTowerETCode;
		vector<vector<unsigned int>> hCorrTowerETCode;
};



TauRecoCalib::~TauRecoCalib() {
	delete pts_;
	delete dms_;
	delete etas_;
	delete phis_;
}


unsigned int const TauRecoCalib::N_TOWER_PHI = 72;
unsigned int const TauRecoCalib::N_TOWER_ETA = 56;


namespace {

	// Predicate to sort candidates by descending pt
	class CandPtSorter {
		public:
			bool operator()(const reco::Candidate* candA, const reco::Candidate* candB)
				const {
					return candA->pt() > candB->pt();
				}
	};

}




TauRecoCalib::TauRecoCalib(const edm::ParameterSet& pset):
        tauToken_(consumes<pat::TauCollection>(pset.getParameter<edm::InputTag>("taus"))),
	eTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	eCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA))
{

	// Initialize the ntuple builder
	edm::Service<TFileService> fs;
	tree = fs->make<TTree>("Ntuple", "Ntuple");
	pts_ = new std::vector<Float_t>();
	dms_ = new std::vector<Float_t>();
	etas_ = new std::vector<Float_t>();
	phis_ = new std::vector<Float_t>();

	//run informaiton
	tree->Branch("run", &run_, "run/i");
	tree->Branch("lumi", &lumi_, "lumi/i");
	tree->Branch("evt", &event_, "evt/l");
	tree->Branch("npvs", &npvs_, "npvs/i");
	tree->Branch("instlumi", &instLumi_, "instlumi/F");

	//calibration vectors
	tree->Branch("TPGVeto", "std::vector<float>", &TPGVeto_); //calibrate event or not
	tree->Branch("ptbin", "std::vector<int>", &ptbin_); //calibrate event or not


	//TPG5x5
	tree->Branch("TPG5x5_tpgeta_", "std::vector<int>", &TPG5x5_tpgeta_);
	tree->Branch("TPG5x5", "std::vector<int>", &TPG5x5_);
	tree->Branch("TPGh5x5","std::vector<int>", &TPGh5x5_);
	tree->Branch("TPGe5x5","std::vector<int>", &TPGe5x5_);

	tree->Branch("recoPt", "std::vector<float>", &pts_); //reco eg rct pt
	tree->Branch("recoDMs", "std::vector<float>", &dms_); //reco eg rct pt
	tree->Branch("recoGctEta", "std::vector<float>", &etas_);//reco eta
	tree->Branch("recoGctPhi", "std::vector<float>", &phis_);//reco phi

	tree->Branch("l1egPt", "std::vector<float>", &egCandPt_); //reco eg rct pt
	tree->Branch("l1egEta", "std::vector<float>", &egCandEta_);//rct gct eta?
	tree->Branch("l1egPhi", "std::vector<float>", &egCandPhi_);//rct gct phi?

	//cTPG5x5
	tree->Branch("cTPG5x5", "std::vector<int>", &cTPG5x5_);
	tree->Branch("cTPGh5x5","std::vector<int>", &cTPGh5x5_);
	tree->Branch("cTPGe5x5","std::vector<int>", &cTPGe5x5_);


	scalerSrc_ = consumes<std::vector<LumiScalers>>(InputTag("scalersRawToDigi"));
	pvSrc_ = consumes<std::vector<reco::Vertex>>(InputTag("offlineSlimmedPrimaryVertices"));
	ecalSrc_ = consumes<edm::SortedCollection<EcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> >>(InputTag("ecalDigis:EcalTriggerPrimitives"));
	//ecalSrc_ = pset.exists("ecalSrc") ? pset.getParameter<InputTag>("ecalSrc"): InputTag("ecalTriggerPrimitiveDigis");
	hcalSrc_ = consumes<edm::SortedCollection<HcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<HcalTriggerPrimitiveDigi> >>( InputTag("hcalDigis"));

	TPGSF1_= pset.getParameter<vector<double> >("TPGSF1");//calibration tables
	TPGSFp_= pset.getParameter<vector<double> >("TPGSFp");//calibration tables

	LSB = pset.getParameter<double>("regionLSB");//currently unused

	ECALOn = pset.getParameter<bool>("ECALOn"); //Calibrate  *if(ECALOn) maxTPGPt=maxTPGEPt)*
	v_off = pset.getParameter<bool>("v_off"); //Calibrate
	v1 = pset.getParameter<bool>("v1"); //Calibrate
	v3 = pset.getParameter<bool>("v3"); //Calibrate
}


void TauRecoCalib::analyze(const edm::Event& evt, const edm::EventSetup& es) {
	// Setup meta info
	//std::cout<<"Analyze loop"<<std::endl;
	//
	run_ = evt.id().run();
	lumi_ = evt.id().luminosityBlock();
	event_ = evt.id().event();
	evt.getByToken(scalerSrc_, lumiScalers);
	evt.getByToken(pvSrc_, vertices);
	evt.getByToken(ecalSrc_, ecal);
	evt.getByToken(hcalSrc_, hcal);


	// EVENT INFO
	instLumi_ = -1;
	npvs_ = 0;
	npvs_ = vertices->size();

	if (lumiScalers->size())
		instLumi_ = lumiScalers->begin()->instantLumi();



	//Reset important things
	//std::cout<<"Reset important things"<<std::endl;
	//std::cout<<"Reset Reco"<<std::endl;
	pts_->clear();
	dms_->clear();
	etas_->clear();
	phis_->clear();

	//std::cout<<"Reset EG"<<std::endl;
	egCandPt_.clear();
	egCandEta_.clear();
	egCandPhi_.clear();

	//std::cout<<"Reset TPG helpers"<<std::endl;
	TPGVeto_.clear();
	ptbin_.clear();

	//std::cout<<"Reset maxTPGS"<<std::endl;
	// TPG TESTING
	maxTPGPt_.clear();
	maxTPGPt_eta_.clear();
	maxTPGPt_phi_.clear();

	//std::cout<<"Reset TPG5x5s"<<std::endl;
	TPG5x5_.clear();
	TPG5x5_gcteta_.clear();
	TPG5x5_tpgeta_.clear();
	TPG5x5_tpgphi_.clear();
	TPGh5x5_.clear();
	TPGe5x5_.clear();

	//std::cout<<"Reset Corrected TPGS"<<std::endl;
	//clear corrected 5x5 tpgs
	cTPG5x5_.clear();
	cTPGh5x5_.clear();
	cTPGe5x5_.clear();


	//std::cout<<"Reset TPG=0s "<<std::endl;
	maxHTPGPt=0;
	maxHTPGPt_eta=0;
	maxHTPGPt_phi=0;
	maxETPGPt=0;
	maxETPGPt_eta=0;
	maxETPGPt_phi=0;
	maxTPGPt=0;
	maxTPGPt_eta=0;
	maxTPGPt_phi=0;



	edm::Handle<pat::TauCollection> taus;
	evt.getByToken(tauToken_, taus);
	for (const pat::Tau &tau : *taus) {
		//if (tau.tauID("decayModeFinding")&&abs(tau.eta())<2.3&&tau.tauID("againstMuonTight3")>0.5&&tau.tauID("againstElectronLooseMVA5")>0.5&&(tau.decayMode()==0||tau.decayMode()==10)&&tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")<2){
		if (abs(tau.eta())<3&&tau.tauID("againstMuonTight3")>0.5&&tau.tauID("againstElectronLooseMVA6")>0.5&&(tau.decayMode()!=1&&tau.decayMode()!=6)&&tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")<2){
			pts_->push_back(tau.pt());
			dms_->push_back(tau.decayMode());
			etas_->push_back(tau.eta());
			phis_->push_back(tau.phi());
		}
	}

	//EG cand vector filling
	//std::cout<<"L1 EG Cands"<<std::endl;
		egCandPt_.push_back(-999);
		egCandEta_.push_back(-999); //should be gctEta?
		egCandPhi_.push_back(-999); //should be gctPhi?


	//std::cout << "TPGS" << std::endl;
	//std::cout << "ECAL TPGS" << std::endl;
	//fill ET vector
	for (size_t i = 0; i < ecal->size(); ++i) {
		int cal_ieta = (*ecal)[i].id().ieta();
		int cal_iphi = (*ecal)[i].id().iphi();
		int iphi = cal_iphi-1;
		int ieta = TPGEtaRange(cal_ieta);
		// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
		// TPG ieta ideal goes from 0-55.
		//cout<<"Before filling eTower"
		//	<<"ieta:"<<ieta<<" cal_ieta:"<< cal_ieta<<" iphi:"<<iphi<<endl;
		double et= (*ecal)[i].compressedEt()*LSB;
		eTowerETCode[iphi][ieta] = et;
		//fill corrected tpgs here
		//eCorrTowerETCode
		//CORRECTIONS
		int etbin=0;
		double alpha=1.0;
		if(et<10){etbin=0;}
		else if(et<15){etbin=1;}
		else if(et<20){etbin=2;}
		else if(et<25){etbin=3;}
		else if(et<30){etbin=4;}
		else if(et<35){etbin=5;}
		else if(et<40){etbin=6;}
		else if(et<45){etbin=7;}
		else {etbin=8;}
		if((!v_off)&&v1) {
		     if ( ieta>27) {alpha = TPGSF1_[etbin*28+(ieta-28)];} //map 28-55 to 0-27
		     else if ( ieta<28) {alpha = TPGSF1_[etbin*28+(ieta-27)];} //map 0-27 to 0-27 (flip order)
                } //v1
		//std::cout<<"alpha: "<<alpha<<std::endl;
		//std::cout<<"E: "<<et<<std::endl;
		//std::cout<<"ECorr: "<<alpha*et<<std::endl;
		eCorrTowerETCode[iphi][ieta] = alpha*et;
		//std::cout<<"Ecal sf: "<<alpha<<std::endl;
	}//end ECAL TPGS

	//	std::cout << "HCAL TPGS" << std::endl;
	ESHandle<L1CaloHcalScale> hcalScale;
	es.get<L1CaloHcalScaleRcd>().get(hcalScale);

	for (size_t i = 0; i < hcal->size(); ++i) {
		int ieta = (*hcal)[i].id().ieta();
                //std::cout<<"HCAL ieta: "<<ieta<<std::endl;
		int iphi = (*hcal)[i].id().iphi();
		int hniphi = iphi-1;
		short absieta = std::abs((*hcal)[i].id().ieta());
                if (absieta>28) continue;
                int hnieta = TPGEtaRange(ieta);
		short zside = (*hcal)[i].id().zside();

		if (ieta >= -1000 && ieta <= 1000 &&
				iphi >= -1000 && ieta <= 1000) {
			double energy = hcalScale->et(
					(*hcal)[i].SOI_compressedEt(), absieta, zside);

			hTowerETCode[hniphi][hnieta] = energy;
			//fill corrected tpgs here
			//hCorrTowerETCode
			int hetbin=0;
			double alpha_h=1.0; //v_off, v1 
			if(energy<10){hetbin=0;}
			else if(energy<15){hetbin=1;}
			else if(energy<20){hetbin=2;}
			else if(energy<25){hetbin=3;}
			else if(energy<30){hetbin=4;}
			else if(energy<35){hetbin=5;}
			else if(energy<40){hetbin=6;}
			else if(energy<45){hetbin=7;}
			else {hetbin=8;}
			if (v3) {//required to check HCAL performance 
				if ( hnieta>27) {alpha_h = TPGSFp_[hetbin*28+(ieta-28)];} //1-32 is mapped 28-55 cutting off HF
				else if (hnieta<28) {alpha_h = TPGSFp_[hetbin*28+(ieta-27)];} //-32--1 is flipped, and mapped 0-27 cutting off HF
			}
			hCorrTowerETCode[hniphi][hnieta] = alpha_h*energy;

		}//end if
	}//end HCAL TPGs


	//MATCHING
	//std::cout << "MATCHING" << std::endl;

	//match tpg to reco eg object
	for (size_t i = 0; i < pts_->size(); ++i) {
		double closestDR=10e6;
		double closestDRET=10e6;
		int match=-1;
		if (ECALOn){
			for (size_t j = 0; j < ecal->size(); ++j) {
				int cal_ieta = (*ecal)[j].id().ieta();
				int cal_iphi = (*ecal)[j].id().iphi();
				int iphi = cal_iphi-1;
				int ieta = TPGEtaRange(cal_ieta);
				float phi = convertTPGPhi(iphi); //returns phi
				float eta = convertTPGEta(ieta);
				// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
				// TPG ieta ideal goes from 0-55.
				//	<<"ieta:"<<ieta<<" cal_ieta:"<< cal_ieta<<" iphi:"<<iphi<<endl;
				double et= (*ecal)[j].compressedEt()*LSB;
				//fill corrected tpgs here
				//eCorrTowerETCode[][]=et*alpha

				double deltaEta=(etas_->at(i) - eta);
				double deltaPhi=reco::deltaPhi(phis_->at(i),phi);
				double dR=sqrt( deltaEta*deltaEta + deltaPhi*deltaPhi);

				if (dR<.5 && dR<closestDR && pts_->at(i)<90 && fabs(pts_->at(i)-et) < closestDRET) { //require dR<.5, closestDr that
					closestDRET=abs(pts_->at(i)-et);
					closestDR= dR;
					match=j;
					//maxETPGPt = et;
					//maxETPGPt_eta = ieta;
					//maxETPGPt_phi = iphi;
					maxTPGPt = et;
					maxTPGPt_eta = ieta;
					maxTPGPt_phi = iphi;
				}//end if
			}//end ecal size
		}//end ecal on
		else{ //ECALOn ==false
			//HCAL SF derivation
			for (size_t j = 0; j < hcal->size(); ++j) {
				int cal_ieta = (*hcal)[j].id().ieta();
				int cal_iphi = (*hcal)[j].id().iphi();
				int iphi = cal_iphi-1;
				short absieta = std::abs((*hcal)[j].id().ieta());
                                if (absieta>28) continue;
				int ieta = TPGEtaRange(cal_ieta);
				float phi = convertTPGPhi(iphi); //returns phi
				float eta = convertTPGEta(ieta);
				short zside = (*hcal)[j].id().zside();

				if (ieta >= -1000 && ieta <= 1000 &&
						iphi >= -1000 && ieta <= 1000) {
					double et = hcalScale->et(
							(*hcal)[j].SOI_compressedEt(), absieta, zside); 

					double deltaEta=(etas_->at(i) - eta);
					double deltaPhi=reco::deltaPhi(phis_->at(i),phi);
					double dR=sqrt( deltaEta*deltaEta + deltaPhi*deltaPhi);

					if (dR<.5 && dR<closestDR && pts_->at(i)<90 && fabs(pts_->at(i)-et) < closestDRET) { //require dR<.5, closestDr that
						closestDRET=abs(pts_->at(i)-et);
						closestDR= dR;
						match=j;
						//maxHTPGPt = et;
						//maxHTPGPt_phi = iphi;
						//maxHTPGPt_eta = ieta;
						maxTPGPt = et;
						maxTPGPt_eta = ieta;
						maxTPGPt_phi = iphi;
					}//end if
				}//end if et in range
			}//end hcal size
		} //end !ECALOn

		if (match==-1){
			maxTPGPt_.push_back(0);
			maxTPGPt_eta_.push_back(999);
			maxTPGPt_phi_.push_back(999);
		}
		else {
			maxTPGPt_.push_back(maxTPGPt);
			//cout<<"MaxEgTPG: "<<maxETPGPt<<endl;
			maxTPGPt_eta_.push_back(maxTPGPt_eta);
			maxTPGPt_phi_.push_back(maxTPGPt_phi);
		}//end else
	}//end pts_ loop Matching

	//Tpg5x5 calculation, require maxTPGPt_eta and maxTPGPt_phi
	//TPGh5x5, TGPe5x5 calculated
	//Corrected: cTPGe5x5,cTPGh5x5
	//if maxTPGPt is set for a reco pt object
	//std::cout<<"TPG5x5 Calculation"<<std::endl;
	for(size_t i = 0; i < maxTPGPt_.size(); ++i){
		//std::cout<<"maxPtSize_ "<<maxTPGPt_.size()<<std::endl;
		//std::cout<<"reco Pt size_ "<<pts_->size()<<std::endl;
		//std::cout<<"iterator i "<< i <<std::endl;
		if (maxTPGPt_.at(i)>0){
			//cout<<"MaxTPG: "<<maxTPGPt_.at(i)<<endl;
			TPG5x5_gcteta_.push_back(twrEta2RegionEta(maxTPGPt_eta_.at(i)));
			TPG5x5_tpgeta_.push_back(maxTPGPt_eta_.at(i));
			TPG5x5_tpgphi_.push_back(maxTPGPt_phi_.at(i));
			int TPGh5x5=0;
			int TPGe5x5=0;
			int TPG5x5=0;
			int cTPGh5x5=0;
			int cTPGe5x5=0;
			int cTPG5x5=0;
			for (int j = -2; j < 3; ++j) {//}//phi
				//for (int j = -5; j < 6; ++j) {//}//phi
				for (int k = -2; k < 3; ++k) {//} //eta
					//for (int k = -5; k < 6; ++k) { //}//eta
					//std::cout<<"Inside j k for "<<std::endl;
					int tpgsquarephi= TPG5x5_tpgphi_.at(i)+j;
			//std::cout<<"tpgsquarephi "<<tpgsquarephi<< std::endl;
			int tpgsquareeta= TPG5x5_tpgeta_.at(i)+k;
			//std::cout<<"tpgsquareeta "<<tpgsquareeta<< std::endl;
			if (tpgsquarephi==-1) {tpgsquarephi=71;}
			if (tpgsquarephi==-2) {tpgsquarephi=70;}
			if (tpgsquarephi==-3) {tpgsquarephi=69;}
			if (tpgsquarephi==-4) {tpgsquarephi=68;}
			if (tpgsquarephi==-5) {tpgsquarephi=67;}
			if (tpgsquarephi==72) {tpgsquarephi=0;}
			if (tpgsquarephi==73) {tpgsquarephi=1;}
			if (tpgsquarephi==74) {tpgsquarephi=2;}
			if (tpgsquarephi==75) {tpgsquarephi=3;}
			if (tpgsquarephi==76) {tpgsquarephi=4;}
			if (tpgsquareeta>55 || tpgsquareeta<0) {continue;}//No Eta values beyond

			TPGh5x5+=hTowerETCode[tpgsquarephi][tpgsquareeta];
			TPGe5x5+=eTowerETCode[tpgsquarephi][tpgsquareeta];
			TPG5x5+=hTowerETCode[tpgsquarephi][tpgsquareeta];
			TPG5x5+=eTowerETCode[tpgsquarephi][tpgsquareeta];
			cTPGh5x5+=hCorrTowerETCode[tpgsquarephi][tpgsquareeta];
			cTPGe5x5+=eCorrTowerETCode[tpgsquarephi][tpgsquareeta];
			cTPG5x5+=hCorrTowerETCode[tpgsquarephi][tpgsquareeta];
			cTPG5x5+=eCorrTowerETCode[tpgsquarephi][tpgsquareeta];
		}
	}
	TPGh5x5_.push_back(TPGh5x5);
	TPGe5x5_.push_back(TPGe5x5);
	TPG5x5_.push_back(TPG5x5);
	cTPGh5x5_.push_back(cTPGh5x5);
	cTPGe5x5_.push_back(cTPGe5x5);
	cTPG5x5_.push_back(cTPG5x5);
}//end if max tpg
else { 
	TPGh5x5_.push_back(0);
	TPGe5x5_.push_back(0);
	TPG5x5_.push_back(0);
	cTPG5x5_.push_back(0);
	cTPGe5x5_.push_back(0);
	cTPGh5x5_.push_back(0);
	TPG5x5_gcteta_.push_back(0);
	TPG5x5_tpgeta_.push_back(0);
	TPG5x5_tpgphi_.push_back(0);

}
}//end for


//Fill Pt bin
for(size_t i = 0; i < maxTPGPt_.size(); ++i){
	if (maxTPGPt_.at(i)>0){
		TPGVeto_.push_back(maxTPGPt_.at(i)/TPG5x5_.at(i));
	}
	else {TPGVeto_.push_back(-1);}
	if(maxTPGPt_.at(i)<10){ptbin_.push_back(0);}
	else if(maxTPGPt_.at(i)<15){ptbin_.push_back(1);}
	else if(maxTPGPt_.at(i)<20){ptbin_.push_back(2);}
	else if(maxTPGPt_.at(i)<25){ptbin_.push_back(3);}
	else if(maxTPGPt_.at(i)<30){ptbin_.push_back(4);}
	else if(maxTPGPt_.at(i)<35){ptbin_.push_back(5);}
	else if(maxTPGPt_.at(i)<40){ptbin_.push_back(6);}
	else if(maxTPGPt_.at(i)<45){ptbin_.push_back(7);}
	else {ptbin_.push_back(8);}
}

//	std::cout<<"Fill Tree"<<std::endl;

tree->Fill();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauRecoCalib);
