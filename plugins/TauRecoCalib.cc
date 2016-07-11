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
#include <unordered_map>
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
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"



typedef std::vector<edm::InputTag> VInputTag;

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
		edm::EDGetTokenT<HcalTrigPrimDigiCollection> hcalSrc_;
		edm::EDGetTokenT<EcalTrigPrimDigiCollection> ecalSrc_;
	
		edm::EDGetTokenT<pat::TauCollection> tauToken_;


		//initialize run info
		unsigned int run_;
		unsigned int lumi_;
		unsigned long int event_;
		unsigned int npvs_;
		float instLumi_;


		//Reco information
		vector<float>* pts_;
		vector<float>* dms_;
		vector<float>* etas_;
		vector<float>* phis_;

		//TPG MAX information
		vector<float> centralTPGPt_;
		vector<float> centraleTPGPt_;
		vector<float> centralhTPGPt_;

		//TPG information
		vector<float> TPGVeto_; //This determines majority in central TPG
		vector<int> ptbin_; //This determines majority in central TPG

		vector<double> TPG5x5_;
		vector<double> TPG5x5_tpgeta_;
		vector<double> TPG5x5_tpgphi_;
		vector<double> TPGh5x5_;
		vector<double> TPGe5x5_;

		//TPG calibration vectors
		vector<double> TPGSF1_;
		vector<double> TPGSFp_;


		//calibrated TPGcollections
		vector<double> cTPG5x5_;
		vector<double> cTPGh5x5_;
		vector<double> cTPGe5x5_;


		//handles
		Handle<LumiScalersCollection> lumiScalers;
		Handle<reco::VertexCollection> vertices;
		Handle<EcalTrigPrimDigiCollection> ecal;
		Handle<HcalTrigPrimDigiCollection> hcal;



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
	tree->Branch("TPG5x5_tpgeta_", "std::vector<double>", &TPG5x5_tpgeta_);
	tree->Branch("TPG5x5", "std::vector<double>", &TPG5x5_);
	tree->Branch("TPGh5x5","std::vector<double>", &TPGh5x5_);
	tree->Branch("TPGe5x5","std::vector<double>", &TPGe5x5_);
	tree->Branch("centralTPGPt","std::vector<double>", &centralTPGPt_);
	tree->Branch("centraleTPGPt","std::vector<double>", &centraleTPGPt_);
	tree->Branch("centralhTPGPt","std::vector<double>", &centralhTPGPt_);

	tree->Branch("recoPt", "std::vector<float>", &pts_); //reco eg rct pt
	tree->Branch("recoDMs", "std::vector<float>", &dms_); //reco eg rct pt
	tree->Branch("recoGctEta", "std::vector<float>", &etas_);//reco eta
	tree->Branch("recoGctPhi", "std::vector<float>", &phis_);//reco phi

	//cTPG5x5
	tree->Branch("cTPG5x5", "std::vector<double>", &cTPG5x5_);
	tree->Branch("cTPGh5x5","std::vector<double>", &cTPGh5x5_);
	tree->Branch("cTPGe5x5","std::vector<double>", &cTPGe5x5_);


	scalerSrc_ = consumes<std::vector<LumiScalers>>(InputTag("scalersRawToDigi"));
	pvSrc_ = consumes<std::vector<reco::Vertex>>(InputTag("offlineSlimmedPrimaryVertices"));
	ecalSrc_ = consumes<edm::SortedCollection<EcalTriggerPrimitiveDigi,edm::StrictWeakOrdering<EcalTriggerPrimitiveDigi> >>(InputTag("ecalDigis:EcalTriggerPrimitives"));
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

	//std::cout<<"Reset TPG helpers"<<std::endl;
	TPGVeto_.clear();
	ptbin_.clear();

	//std::cout<<"Reset maxTPGS"<<std::endl;
	// TPG TESTING
	centralTPGPt_.clear();

	//std::cout<<"Reset TPG5x5s"<<std::endl;
	TPG5x5_.clear();
	TPG5x5_tpgeta_.clear();
	TPG5x5_tpgphi_.clear();
	TPGh5x5_.clear();
	TPGe5x5_.clear();

	//std::cout<<"Reset Corrected TPGS"<<std::endl;
	//clear corrected 5x5 tpgs
	cTPG5x5_.clear();
	cTPGh5x5_.clear();
	cTPGe5x5_.clear();


	//std::cout << "TPGS" << std::endl;
	std::cout << "ECAL TPGS" << std::endl;
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
			if ( ieta>27) {alpha = TPGSF1_[etbin*28+(ieta-28)]; std::cout<<"bin used in LUT"<<etbin*28+(ieta-28)<<std::endl;} //map 28-55 to 0-27
			else if ( ieta<28) {alpha = TPGSF1_[etbin*28+abs(ieta-27)]; std::cout<<"bin used in LUT"<<etbin*28+abs(ieta-27)<<std::endl;} //map 0-27 to 0-27 (flip order)
		} //v1

		if( et>5) {
			std::cout<<"====NEW DIGI ===="<<std::endl;
			std::cout<<"etbin: "<<etbin<<std::endl;
			std::cout<<"ieta: "<<ieta<<std::endl;
			std::cout<<"iphi: "<<iphi<<std::endl;
			std::cout<<"alpha: "<<alpha<<std::endl;
			std::cout<<"et: "<<et<<std::endl;
			std::cout<<"etCorr: "<<alpha*et<<std::endl;
		}
		eCorrTowerETCode[iphi][ieta] = alpha*et;
	}//end ECAL TPGS

	//	std::cout << "HCAL TPGS" << std::endl;
	ESHandle<CaloTPGTranscoder> decoder;
	es.get<CaloTPGRecord>().get(decoder);

	std::unordered_map<int, std::unordered_map<int, double>> new_ets;
	std::unordered_map<int, std::unordered_map<int, int>> new_counts;

	ESHandle<HcalTrigTowerGeometry> tpd_geo;
	es.get<CaloGeometryRecord>().get(tpd_geo);

	std::map<HcalTrigTowerDetId, HcalTriggerPrimitiveDigi> ttids;
	for (const auto& digi: *hcal) {
		//	if (digi.id().version() == 1 || digi.id().ieta()>29) continue; //No HF
		ttids[digi.id()] = digi;
		HcalTrigTowerDetId id = digi.id();
		double tp_et_ = decoder->hcaletValue(id, digi.t0());
		int tp_ieta_ = id.ieta();
		int tp_iphi_ = id.iphi();
		std::cout<<" HCAL ieta: "<<tp_ieta_ <<std::endl;
		if (abs(tp_ieta_)>28) continue; //ignore HF for now in HCAL vecotr 
		// TPG iEta starts at 0 and goes to 55 for ECAL; FIXME in helpers? Will be different for hcal 
		// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
		int ieta = TPGEtaRange(tp_ieta_);//avoid negative eta
		int iphi =  tp_iphi_-1; //zero index
		int tp_eta_ = convertTPGEta(ieta); //FIXME
		int tp_phi_ = convertTPGPhi(iphi); //CHECKME/FIXME should require zero index
		hTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 
		hCorrTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 
		if ( ieta<0 ||iphi<0 ||ieta>55){
			cout<<"Original iEta: "<< tp_ieta_ <<" is transformed to "<<ieta<<" for saving to vector; Real Eta: "<<tp_eta_<<endl;
			cout<<"Original iPhi: "<< tp_iphi_ <<" is transformed to "<<iphi<<" for saving to vector; Real Phi: "<<tp_phi_<<endl;
		}

	}//end for of hcal digis


	//MATCHING
	//std::cout << "MATCHING" << std::endl;


	edm::Handle<pat::TauCollection> taus;
	evt.getByToken(tauToken_, taus);
	for (const pat::Tau &tau : *taus) {
		if (abs(tau.eta())<2.868&&tau.tauID("againstMuonTight3")>0.5&&tau.tauID("againstElectronTightMVA6")>0.5&&(tau.decayMode()!=5&&tau.decayMode()!=6)&&tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")<2){
			pts_->push_back(tau.pt());
			dms_->push_back(tau.decayMode());
			etas_->push_back(tau.eta());
			phis_->push_back(tau.phi());
			int tau_ieta_=convertTPGGenEta(tau.eta());
			int tau_iphi_=convertGenPhi(tau.phi());

			int temp = floor((tau.pt())/5);
			int ptbin = temp -1;
			if (ptbin<0) ptbin=0; 
			else if (ptbin>8) ptbin=8;

 	
			//cout<<"TauParticle Pt: "<< pts_ <<" Eta: "<<tau_eta_<<" Phi: "<<tau_phi_<<" iEta: "<<tau_ieta_<<" iPhi: "<<tau_iphi_ <<endl;
			//iETA NEGATIVE
			ptbin_.push_back(ptbin);
			double TPGh5x5=0;
			double cTPGh5x5=0;
			double TPGe5x5=0;
			double cTPGe5x5=0;
			double TPG5x5=0;
			double cTPG5x5=0;
			double chTPG=0;
			double ceCorrTPG=0;

			for (int j = -2; j < 3; ++j) {//eta
				for (int k = -2; k < 3; ++k) { //phi
					int tpgsquarephi= tau_iphi_+k;
					int tpgsquareeta= tau_ieta_+j;	
					chTPG +=hTowerETCode[tau_iphi_][tau_ieta_];
					ceCorrTPG +=eCorrTowerETCode[tau_iphi_][tau_ieta_];

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
					if (tpgsquareeta>55 || tpgsquareeta<0) {continue;}//No Eta values beyond FIX ME IN NEXT ITERATION
					TPGh5x5 += hTowerETCode[tpgsquarephi][tpgsquareeta];		
					cTPGh5x5 += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
					TPGe5x5 += eTowerETCode[tpgsquarephi][tpgsquareeta];		
					cTPGe5x5 += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
					TPG5x5 += hTowerETCode[tpgsquarephi][tpgsquareeta];	
					TPG5x5 += eTowerETCode[tpgsquarephi][tpgsquareeta];	
					cTPG5x5 += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];	
					cTPG5x5 += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				}
			}//end tpg sum
			TPG5x5_.push_back(TPG5x5);
			TPGh5x5_.push_back(TPGh5x5);
			TPGe5x5_.push_back(TPGe5x5);
			TPG5x5_tpgeta_.push_back(tau_ieta_);
			TPG5x5_tpgphi_.push_back(tau_iphi_);

			cTPG5x5_.push_back(cTPG5x5);
			cTPGh5x5_.push_back(cTPGh5x5);
			cTPGe5x5_.push_back(cTPGe5x5);
			TPGVeto_.push_back(TPGh5x5/cTPGe5x5);
		        centralTPGPt_.push_back(ceCorrTPG+chTPG);
		        centraleTPGPt_.push_back(ceCorrTPG);
		        centralhTPGPt_.push_back(chTPG);

		}//end good taus
	}//end tau loop



	tree->Fill();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauRecoCalib);
