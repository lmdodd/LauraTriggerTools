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


#include "DataFormats/JetReco/interface/GenJet.h"

#include "L1Trigger/LauraTriggerTools/src/helpers.cc"


#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TTree.h"

#include "TLorentzVector.h"
//
// class declaration
//

class JetHFCalib : public edm::EDAnalyzer {
	public:
		explicit JetHFCalib(const edm::ParameterSet&);
		static const unsigned int N_TOWER_PHI;
		static const unsigned int N_TOWER_ETA;
		~JetHFCalib();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);

		// ----------member data ---------------------------
		edm::EDGetTokenT<std::vector<reco::GenJet>> genSrc_;
		edm::InputTag digis_;
		bool detid_;
		double threshold_;

		int event_;


		TTree *jets_;
		double jet_pt_;
		double jet_et_;
		double jet_eta_;
		double jet_phi_;
		int jet_ieta_;
		int jet_iphi_;

		TTree *matched_;
		double gen_et_;
		double gen_pt_;
		double gen_eta_;
		double gen_phi_;
		int gen_ieta_;
		int gen_iphi_;
		double l1_summed22_;
		double l1_summed33_;
		double l1_summed44_;


		TTree *tps_;

		int tp_ieta_;
		int tp_iphi_;
		double tp_phi_;
		double tp_eta_;
		int tp_depth_;
		int tp_version_;
		int tp_soi_;
		double tp_et_;

		TTree *ev_;
		double ev_tp_v0_et_;
		double ev_tp_v1_et_;
};


unsigned int const JetHFCalib::N_TOWER_PHI = 72;
unsigned int const JetHFCalib::N_TOWER_ETA = 82;

JetHFCalib::JetHFCalib(const edm::ParameterSet& config) :
	edm::EDAnalyzer(),
	digis_(config.getParameter<edm::InputTag>("triggerPrimitives")),
	detid_(config.getUntrackedParameter<bool>("useDetIdForUncompression", true)),
	threshold_(config.getUntrackedParameter<double>("threshold", 0.))
{
	edm::Service<TFileService> fs;

	genSrc_    = mayConsume<std::vector<reco::GenJet> >(config.getParameter<edm::InputTag>("genSrc"));

	consumes<HcalTrigPrimDigiCollection>(digis_);

	tps_ = fs->make<TTree>("tps", "Trigger primitives");
	tps_->Branch("event", &event_);
	tps_->Branch("ieta", &tp_ieta_);
	tps_->Branch("iphi", &tp_iphi_);
	tps_->Branch("phi", &tp_phi_);
	tps_->Branch("eta", &tp_eta_);
	tps_->Branch("depth", &tp_depth_);
	tps_->Branch("version", &tp_version_);
	tps_->Branch("soi", &tp_soi_);
	tps_->Branch("et", &tp_et_);

	ev_ = fs->make<TTree>("evs", "Event quantities");
	ev_->Branch("event", &event_);
	ev_->Branch("tp_v0_et", &ev_tp_v0_et_);
	ev_->Branch("tp_v1_et", &ev_tp_v1_et_);


	jets_ = fs->make<TTree>("jets", "Jet quantities");
	jets_->Branch("event", &event_);
	jets_->Branch("et" , &jet_et_);
	jets_->Branch("eta" , &jet_eta_);
	jets_->Branch("phi" , &jet_phi_);
	jets_->Branch("ieta" , &jet_ieta_);
	jets_->Branch("iphi" , &jet_iphi_);

	matched_ = fs->make<TTree>("matched", "Matched quantities");
	matched_->Branch("event", &event_);
	matched_->Branch("gen_pt" , &gen_pt_);
	matched_->Branch("gen_et" , &gen_et_);
	matched_->Branch("gen_eta" , &gen_eta_);
	matched_->Branch("gen_phi" , &gen_phi_);
	matched_->Branch("gen_ieta" , &gen_ieta_);
	matched_->Branch("gen_iphi" , &gen_iphi_);
	matched_->Branch("l1_summed22" , &l1_summed22_);
	matched_->Branch("l1_summed33" , &l1_summed33_);
	matched_->Branch("l1_summed44" , &l1_summed44_);

}

JetHFCalib::~JetHFCalib() {}

	void
JetHFCalib::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
	using namespace edm;

	event_ = event.id().event();

	Handle<HcalTrigPrimDigiCollection> digis;
	if (!event.getByLabel(digis_, digis)) {
		LogError("JetHFCalib") <<
			"Can't find hcal trigger primitive digi collection with tag '" <<
			digis_ << "'" << std::endl;
		return;
	}

	edm::Handle<std::vector<reco::GenJet> > objects;
	event.getByToken(genSrc_,objects);


	for(const auto& jet: *objects){
		if (std::abs(jet.eta())<2.976) continue;
		jet_et_=jet.et();
		jet_eta_=jet.eta();
		jet_phi_=jet.phi();
		jet_ieta_=convertHFGenEta(jet.eta());
		jet_iphi_=convertGenPhi(jet.phi());
		jets_->Fill();
	}



	ESHandle<CaloTPGTranscoder> decoder;
	setup.get<CaloTPGRecord>().get(decoder);

	std::unordered_map<int, std::unordered_map<int, double>> new_ets;
	std::unordered_map<int, std::unordered_map<int, int>> new_counts;


	ESHandle<HcalTrigTowerGeometry> tpd_geo;
	setup.get<CaloGeometryRecord>().get(tpd_geo);

	std::map<HcalTrigTowerDetId, HcalTriggerPrimitiveDigi> ttids;
	for (const auto& digi: *digis) {
		if (digi.id().version() == 1){ //1x1 upgrade
			ttids[digi.id()] = digi;
			HcalTrigTowerDetId id = digi.id();

			if (detid_)
				tp_et_ = decoder->hcaletValue(id, digi.t0());
			else
				tp_et_ = decoder->hcaletValue(tp_ieta_, tp_iphi_, tp_soi_);
			if (tp_et_ < threshold_)
				continue;

			tp_ieta_ = id.ieta();
			tp_iphi_ = id.iphi();
			tp_phi_ = convertTPGPhi(id.iphi());
			tp_eta_ = convertHFTPGEta(id.ieta());
			tp_depth_ = id.depth();
			tp_version_ = id.version();
			tp_soi_ = digi.SOI_compressedEt();

		tps_->Fill();
		}//end 1x1 upgrade





	}//end for of digis
	l1_summed22_=0;
	l1_summed33_=0;
	l1_summed44_=0;
	for(const auto& jet: *objects){
		if (std::abs(jet.eta())<2.976) continue;
		gen_pt_=jet.pt();
		gen_et_=jet.et();
		gen_eta_=jet.eta();
		gen_phi_=jet.phi();
		gen_ieta_=convertHFGenEta(jet.eta());
		gen_iphi_=convertGenPhi(jet.phi());
		for (const auto& digi: *digis) {
			if (digi.id().version() == 1){ //1x1 upgrade
				HcalTrigTowerDetId id = digi.id();
				if (detid_) tp_et_ = decoder->hcaletValue(id, digi.t0());
				else tp_et_ = decoder->hcaletValue(tp_ieta_, tp_iphi_, tp_soi_);
				if (tp_et_ < threshold_) continue;
				if (abs(gen_ieta_-id.ieta())<2&&abs(gen_iphi_-id.iphi())<2) l1_summed22_+=tp_et_;
				if (abs(gen_ieta_-id.ieta())<3&&abs(gen_iphi_-id.iphi())<3) l1_summed33_+=tp_et_;
				if (abs(gen_ieta_-id.ieta())<4&&abs(gen_iphi_-id.iphi())<4) l1_summed44_+=tp_et_;
			}//end 1x1 upgrade

		}
                matched_->Fill();
	}



	ev_->Fill();
}

void
JetHFCalib::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetHFCalib);
