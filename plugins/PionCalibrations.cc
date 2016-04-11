// system include files
#include <memory>
#include <unordered_map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"

#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "CondFormats/DataRecord/interface/L1CaloGeometryRecord.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"

#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"
#include "CondFormats/DataRecord/interface/L1RCTParametersRcd.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "L1Trigger/LauraTriggerTools/interface/helpers.h"


#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TTree.h"

#include "TLorentzVector.h"
//
// class declaration
//

using namespace std;
using namespace edm;

class PionCalibrations : public edm::EDAnalyzer {
	public:
		explicit PionCalibrations(const edm::ParameterSet&);
		static const unsigned int N_TOWER_PHI;
		static const unsigned int N_TOWER_ETA;
		~PionCalibrations();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);

		// ----------member data ---------------------------
		edm::EDGetTokenT<HcalTrigPrimDigiCollection> digis_;
		edm::EDGetTokenT<EcalTrigPrimDigiCollection> Edigis_;
		edm::EDGetTokenT<std::vector<reco::GenParticle>> genSrc_;
		bool detid_;
		double threshold_;
		bool doClosure_;

		//The square sums
		vector<vector<unsigned int>> eTowerETCode;
		vector<vector<unsigned int>> eCorrTowerETCode;
		vector<vector<unsigned int>> hTowerETCode;
		vector<vector<unsigned int>> hCorrTowerETCode;


		int event_;


		TTree *pions_;
		double pion_pt_;
		double pion_et_;
		double pion_eta_;
		double pion_phi_;
		int pion_ieta_;
		int pion_iphi_;

		TTree *matched_;
		double gen_et_;
		double gen_pt_;
		double gen_eta_;
		double gen_phi_;
		int gen_ieta_;
		int gen_iphi_;
		double l1_summed55_;
		double l1_summed55_e_;
		double l1_summed55_h_;


		TTree *Htps_;
		TTree *Etps_;

		int tp_ieta_;
		int tp_iphi_;
		double tp_phi_;
		double tp_eta_;
		int tp_depth_;
		int tp_version_;
		int tp_soi_;
		double tp_et_;

		double etp_et_;
		int etp_ieta_;
		int etp_iphi_;
		double etp_phi_;
		double etp_eta_;
	
		float bin0[12] = { 1.80,1.54,1.41,1.37,1.36,1.34,1.39,1.47,1.56,1.68,1.92,2.07}; 
		float bin1[12] = { 2.23,1.78,1.58,1.51,1.45,1.48,1.51,1.60,1.73,1.87,2.46,2.59}; 
		float bin2[12] = { 2.30,1.70,1.59,1.51,1.47,1.47,1.53,1.58,1.67,1.88,2.51,2.49}; 
		float bin3[12] = { 2.19,1.59,1.56,1.49,1.44,1.47,1.51,1.56,1.66,1.83,2.39,2.43}; 
};


unsigned int const PionCalibrations::N_TOWER_PHI = 72;
unsigned int const PionCalibrations::N_TOWER_ETA = 82; //41 towers! This includes HF  

PionCalibrations::PionCalibrations(const edm::ParameterSet& config) :
	edm::EDAnalyzer(),
	digis_(consumes<HcalTrigPrimDigiCollection>(config.getParameter<edm::InputTag>("triggerPrimitives"))),
	Edigis_(consumes<EcalTrigPrimDigiCollection>(config.getParameter<edm::InputTag>("eTriggerPrimitives"))),
	genSrc_(consumes<std::vector<reco::GenParticle>>(config.getParameter<edm::InputTag>("genSrc"))),
	detid_(config.getUntrackedParameter<bool>("useDetIdForUncompression", true)),
	threshold_(config.getUntrackedParameter<double>("threshold", 0.)),
	doClosure_(config.getUntrackedParameter<bool>("doClosure", false)),
	eTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	eCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA))
{
	edm::Service<TFileService> fs;


	Htps_ = fs->make<TTree>("Htps", "Trigger primitives");
	Htps_->Branch("event", &event_);
	Htps_->Branch("ieta", &tp_ieta_);
	Htps_->Branch("iphi", &tp_iphi_);
	Htps_->Branch("phi", &tp_phi_);
	Htps_->Branch("eta", &tp_eta_);
	Htps_->Branch("depth", &tp_depth_);
	Htps_->Branch("version", &tp_version_);
	Htps_->Branch("soi", &tp_soi_);
	Htps_->Branch("et", &tp_et_);


	Etps_ = fs->make<TTree>("Etps", "Trigger primitives");
	Etps_->Branch("event", &event_);
	Etps_->Branch("ieta", &etp_ieta_);
	Etps_->Branch("iphi", &etp_iphi_);
	Etps_->Branch("phi", &etp_phi_);
	Etps_->Branch("eta", &etp_eta_);
	Etps_->Branch("et", &etp_et_);

	pions_ = fs->make<TTree>("pions", "Pion quantities");
	pions_->Branch("event", &event_);
	pions_->Branch("et" , &pion_et_);
	pions_->Branch("eta" , &pion_eta_);
	pions_->Branch("phi" , &pion_phi_);
	pions_->Branch("ieta" , &pion_ieta_);
	pions_->Branch("iphi" , &pion_iphi_);

	matched_ = fs->make<TTree>("matched", "Matched quantities");
	matched_->Branch("event", &event_);
	matched_->Branch("gen_pt" , &gen_pt_);
	matched_->Branch("gen_et" , &gen_et_);
	matched_->Branch("gen_eta" , &gen_eta_);
	matched_->Branch("gen_phi" , &gen_phi_);
	matched_->Branch("gen_ieta" , &gen_ieta_);
	matched_->Branch("gen_iphi" , &gen_iphi_);
	matched_->Branch("l1_summed55" , &l1_summed55_);
	matched_->Branch("l1_summed55_e" , &l1_summed55_e_);
	matched_->Branch("l1_summed55_h" , &l1_summed55_h_);

}

PionCalibrations::~PionCalibrations() {}

	void
PionCalibrations::analyze(const edm::Event& event, const edm::EventSetup& setup)
{

	event_ = event.id().event();

	Handle<HcalTrigPrimDigiCollection> digis;
	if (!event.getByToken(digis_, digis)) {
		LogError("PionCalibrations") <<
			"Can't find hcal trigger primitive digi collection"<<std::endl;
		return;
	}

	Handle<EcalTrigPrimDigiCollection> Edigis;
	if (!event.getByToken(Edigis_, Edigis)) {
		LogError("PionCalibrations") <<
			"Can't find ecal trigger primitive digi collection" << std::endl;
		return;
	}

	edm::Handle<std::vector<reco::GenParticle> > objects;
	if (!event.getByToken(genSrc_, objects)) {
		LogError("PionCalibrations") <<
			"Can't find genParticle collection" << std::endl;
		return;
	}

        

	for(const auto& pion: *objects){
		pion_et_=pion.et();
		pion_eta_=pion.eta();
		pion_phi_=pion.phi();
		pion_ieta_=convertGenEta(pion.eta());
		pion_iphi_=convertGenPhi(pion.phi());
		pions_->Fill();
	}



	ESHandle<CaloTPGTranscoder> decoder;
	setup.get<CaloTPGRecord>().get(decoder);

	std::unordered_map<int, std::unordered_map<int, double>> new_ets;
	std::unordered_map<int, std::unordered_map<int, int>> new_counts;


	ESHandle<HcalTrigTowerGeometry> tpd_geo;
	setup.get<CaloGeometryRecord>().get(tpd_geo);

	std::map<HcalTrigTowerDetId, HcalTriggerPrimitiveDigi> ttids;
	for (const auto& digi: *digis) {
		//	if (digi.id().version() == 1 || digi.id().ieta()>29) continue; //No HF
		ttids[digi.id()] = digi;
		HcalTrigTowerDetId id = digi.id();
		if (detid_)
			tp_et_ = decoder->hcaletValue(id, digi.t0());
		else
			tp_et_ = decoder->hcaletValue(tp_ieta_, tp_iphi_, tp_soi_);
		if (tp_et_ < threshold_) continue;
		tp_ieta_ = id.ieta();
		tp_iphi_ = id.iphi();
                if (abs(tp_ieta_)>28) continue; //ignore HF for now in HCAL vecotr 
                // TPG iEta starts at 0 and goes to 55 for ECAL; FIXME in helpers? Will be different for hcal 
                // TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
                int ieta = TPGEtaRange(tp_ieta_);//avoid negative eta
                int iphi =  tp_iphi_-1; //zero index
		tp_eta_ = convertTPGEta(ieta); //FIXME
		tp_phi_ = convertTPGPhi(iphi); //CHECKME/FIXME should require zero index
		tp_depth_ = id.depth();
		tp_version_ = id.version();
		tp_soi_ = digi.SOI_compressedEt();
                hTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 
                if ( ieta<0 ||iphi<0 ||ieta>55){
                        cout<<"Original iEta: "<< tp_ieta_ <<" is transformed to "<<ieta<<" for saving to vector; Real Eta: "<<tp_eta_<<endl;
                        cout<<"Original iPhi: "<< tp_iphi_ <<" is transformed to "<<iphi<<" for saving to vector; Real Phi: "<<tp_phi_<<endl;
                }

		Htps_->Fill();


	}//end for of hcal digis

	for (const auto& Edigi: *Edigis) {
		double ecalet = Edigi.compressedEt(); //0.5 GeV LSB
		//std::cout<<"ECalET et: "<< ecalet<<std::endl;
		etp_et_ = ecalet;
		etp_ieta_ = Edigi.id().ieta();
		etp_iphi_ = Edigi.id().iphi();
		// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
		// TPG iEta starts at 0 and goes to 55 for ECAL; FIXME in helpers? Will be different for hcal 
		int iphi =  etp_iphi_ -1; //zero index
		int ieta = TPGEtaRange(etp_ieta_); // get rid of negative etas
		etp_phi_ = convertTPGPhi(iphi);//should require zero index
		etp_eta_ = convertTPGEta(ieta);//should not allow negatives Range 0-55 (only EBEE)
		if ( ieta<0 ||iphi<0 ||ieta>55){
			cout<<"Original iEta: "<< etp_ieta_ <<" is transformed to "<<ieta<<" for saving to vector; Real Eta: "<<etp_eta_<<endl;
			cout<<"Original iPhi: "<< etp_iphi_ <<" is transformed to "<<iphi<<" for saving to vector; Real Phi: "<<etp_phi_<<endl;
		}
		eTowerETCode[iphi][ieta] = ecalet; //compressed et!!! easily save the et in a vector of ints (divinde by 2 later) 
		Etps_->Fill();
	}

	for (const auto& pion: *objects){
		l1_summed55_=0;
		l1_summed55_e_=0;
		l1_summed55_h_=0;
		if (std::abs(pion.eta())>2.976) continue; //ignore HF pions for now go up to ieta 29 boundary, include 28. 
		gen_pt_=pion.pt();
		gen_et_=pion.et();
		gen_eta_=pion.eta();
		gen_phi_=pion.phi();
		gen_ieta_=convertGenEta(pion.eta());
		gen_iphi_=convertGenPhi(pion.phi());
		double TPGh5x5_=0;
		double TPGe5x5_=0;
		double TPG5x5_=0;


		for (int j = -5; j < 6; ++j) {//phi
			for (int k = -5; k < 6; ++k) { //eta
				int tpgsquarephi= gen_ieta_+j;
				int tpgsquareeta= gen_iphi_+k;	
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
				TPGh5x5_ += hTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPGe5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPG5x5_ += hTowerETCode[tpgsquarephi][tpgsquareeta];	
				TPG5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
			}
		}
		//FILL ISOLATION PROCEDURE
		//LSB = 0.5 
		l1_summed55_h_=TPGh5x5_*0.5;
		l1_summed55_e_=TPGe5x5_*0.5;
		l1_summed55_=TPG5x5_*0.5;
		matched_->Fill();
	}

}

void
PionCalibrations::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PionCalibrations);
