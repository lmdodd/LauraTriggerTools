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
		double l1_summed33_;
		double l1_summed55_;


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
	
		TTree *ev_;
		double ev_tp_v0_et_;
		double ev_tp_v1_et_;

		float bin0[12] = { 1.80,1.54,1.41,1.37,1.36,1.34,1.39,1.47,1.56,1.68,1.92,2.07}; 
		float bin1[12] = { 2.23,1.78,1.58,1.51,1.45,1.48,1.51,1.60,1.73,1.87,2.46,2.59}; 
		float bin2[12] = { 2.30,1.70,1.59,1.51,1.47,1.47,1.53,1.58,1.67,1.88,2.51,2.49}; 
		float bin3[12] = { 2.19,1.59,1.56,1.49,1.44,1.47,1.51,1.56,1.66,1.83,2.39,2.43}; 
};


unsigned int const PionCalibrations::N_TOWER_PHI = 72;
unsigned int const PionCalibrations::N_TOWER_ETA = 82;

PionCalibrations::PionCalibrations(const edm::ParameterSet& config) :
	edm::EDAnalyzer(),
	digis_(consumes<HcalTrigPrimDigiCollection>(config.getParameter<edm::InputTag>("triggerPrimitives"))),
	Edigis_(consumes<EcalTrigPrimDigiCollection>(config.getParameter<edm::InputTag>("eTriggerPrimitives"))),
	genSrc_(consumes<std::vector<reco::GenParticle>>(config.getParameter<edm::InputTag>("genSrc"))),
	detid_(config.getUntrackedParameter<bool>("useDetIdForUncompression", true)),
	threshold_(config.getUntrackedParameter<double>("threshold", 0.)),
	doClosure_(config.getUntrackedParameter<bool>("doClosure", false))
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

	ev_ = fs->make<TTree>("evs", "Event quantities");
	ev_->Branch("event", &event_);
	ev_->Branch("tp_v0_et", &ev_tp_v0_et_);
	ev_->Branch("tp_v1_et", &ev_tp_v1_et_);


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
	matched_->Branch("l1_summed33" , &l1_summed33_);
	matched_->Branch("l1_summed55" , &l1_summed55_);

}

PionCalibrations::~PionCalibrations() {}

	void
PionCalibrations::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
	using namespace edm;

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

		tp_phi_ = convertTPGPhi(id.iphi());
		tp_eta_ = convertTPGEta(id.ieta());
		tp_depth_ = id.depth();
		tp_version_ = id.version();
		tp_soi_ = digi.SOI_compressedEt();

		Htps_->Fill();


	}//end for of hcal digis

	for (const auto& Edigi: *Edigis) {
		double ecalet = Edigi.compressedEt()*0.5;
		std::cout<<"ECalET et: "<< ecalet<<std::endl;
	}


	/*
	   for(const auto& jet: *objects){
	   l1_summed33_=0;
	   l1_summed55_=0;

	   if (std::abs(jet.eta())>2.976) continue; //no HF pions
	   gen_pt_=jet.pt();
	   gen_et_=jet.et();
	   gen_eta_=jet.eta();
	   gen_phi_=jet.phi();
	   gen_ieta_=convertHFGenEta(jet.eta());
	   gen_iphi_=convertGenPhi(jet.phi());

	   for (const auto& digi: *digis) {
	   if (digi.id().version() == 1||digi.id().ieta()>29){ //1x1 upgrade or ignore 29
	   HcalTrigTowerDetId id = digi.id();
	   if (detid_) tp_et_ = decoder->hcaletValue(id, digi.t0());
	   else tp_et_ = decoder->hcaletValue(tp_ieta_, tp_iphi_, tp_soi_);
	   int absieta = abs(tp_ieta_)-30;
	   if (doClosure_){
	   if (tp_et_<5)  tp_et_ = tp_et_*bin0[absieta] ; 
	   else if (tp_et_<20)  tp_et_ = tp_et_*bin0[absieta] ; 
	   else if (tp_et_<30)  tp_et_ = tp_et_*bin1[absieta] ; 
	   else if (tp_et_<50)  tp_et_ = tp_et_*bin2[absieta] ; 
	   else  tp_et_ = tp_et_*bin3[absieta] ; 
	   tp_et_= (int (tp_et_*2))/2.0;
	   }


	   if (tp_et_ < threshold_) continue;
	   double ecalet=0;	
	   if (id.ieta()==29)continue;

	   if (id.ieta()<29){
	   for (const auto& Edigi: *Edigis) {
	   if (Edigi.id().ieta()==id.ieta() && Edigi.id().iphi()==id.iphi()){ 
	   ecalet = Edigi.compressedEt()*0.5;
	   std::cout<<"ECalET set: "<< ecalet<<std::endl;
	   break;}
	   }
	   }
	   if (gen_ieta_==30&&abs(gen_iphi_-id.iphi())<2) {
	   if ((id.ieta()==28||abs(gen_ieta_-id.ieta())<2)) {l1_summed33_+=tp_et_;l1_summed33_+=ecalet;}
	   if ((id.ieta()==28||id.ieta()==27||abs(gen_ieta_-id.ieta())<2)) {l1_summed55_+=tp_et_;l1_summed55_+=ecalet;}
	   } 
	   else if (gen_ieta_==31&&abs(gen_iphi_-id.iphi())<2) {
	   if ((abs(gen_ieta_-id.ieta())<2)) l1_summed33_+=tp_et_;
	   if ((id.ieta()==28||abs(gen_ieta_-id.ieta())<2)) {l1_summed55_+=tp_et_;l1_summed55_+=ecalet;}
	   } 
	   else{
	   if (abs(gen_ieta_-id.ieta())<2&&abs(gen_iphi_-id.iphi())<2) l1_summed33_+=tp_et_;
	   if (abs(gen_ieta_-id.ieta())<3&&abs(gen_iphi_-id.iphi())<3) l1_summed55_+=tp_et_;
	   }
	   }//end 1x1 upgrade

	   }
	   matched_->Fill();
	   }
	   */



	ev_->Fill();
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
