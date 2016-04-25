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
		edm::InputTag Edigis_;
		bool detid_;
		double threshold_;
		bool doClosure_;

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
		double l1_summed33_;
		double l1_summed55_;
		double l1_summed77_;


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

		float bin0[12] = { 1.80,1.54,1.41,1.37,1.36,1.34,1.39,1.47,1.56,1.68,1.92,2.07}; 
		float bin1[12] = { 2.23,1.78,1.58,1.51,1.45,1.48,1.51,1.60,1.73,1.87,2.46,2.59}; 
		float bin2[12] = { 2.30,1.70,1.59,1.51,1.47,1.47,1.53,1.58,1.67,1.88,2.51,2.49}; 
		float bin3[12] = { 2.19,1.59,1.56,1.49,1.44,1.47,1.51,1.56,1.66,1.83,2.39,2.43}; 

		float ecal[252] = {1.1847, 1.16759, 1.17779, 1.19955, 1.21125, 1.214, 1.21503, 1.22515, 1.24151, 1.27836, 1.30292, 1.33526, 1.42338, 1.4931, 1.49597, 1.50405, 1.52785, 1.81552, 1.59856, 1.75692, 1.76496, 1.77562, 1.69527, 1.66827, 1.61861, 1.56645, 1.56646, 1.56646, 1.1351, 1.12589, 1.12834, 1.13725, 1.14408, 1.1494, 1.14296, 1.14852, 1.1578, 1.17634, 1.18038, 1.19386, 1.23758, 1.27605, 1.27818, 1.28195, 1.34881, 1.71053, 1.37338, 1.52571, 1.54801, 1.53316, 1.4397, 1.40497, 1.37743, 1.33914, 1.33914, 1.33914, 1.18043, 1.17823, 1.1751, 1.17608, 1.19152, 1.196, 1.20125, 1.2068, 1.22584, 1.22476, 1.22395, 1.22302, 1.25137, 1.28097, 1.29871, 1.2862, 1.33489, 1.60937, 1.28365, 1.41367, 1.42521, 1.42041, 1.36784, 1.34922, 1.32754, 1.29825, 1.29825, 1.29825, 1.11664, 1.11852, 1.11861, 1.12367, 1.12405, 1.14814, 1.14304, 1.15337, 1.16607, 1.18698, 1.17048, 1.17463, 1.2185, 1.23842, 1.23214, 1.24744, 1.30047, 1.47152, 1.22868, 1.33121, 1.34841, 1.35178, 1.30048, 1.28537, 1.27012, 1.24159, 1.24159, 1.24159, 1.08422, 1.08146, 1.08706, 1.08906, 1.08636, 1.10092, 1.10363, 1.11102, 1.1186, 1.13301, 1.12369, 1.14377, 1.16477, 1.17801, 1.18782, 1.17168, 1.24593, 1.36835, 1.20252, 1.28349, 1.29828, 1.30328, 1.26848, 1.25817, 1.2464, 1.22259, 1.22259, 1.22259, 1.07444, 1.06774, 1.06883, 1.0707, 1.07881, 1.08859, 1.08285, 1.08747, 1.09736, 1.10678, 1.10008, 1.10717, 1.12858, 1.15383, 1.15826, 1.14855, 1.19911, 1.32567, 1.17553, 1.25976, 1.27926, 1.28459, 1.24524, 1.23706, 1.22597, 1.20006, 1.20006, 1.20006, 1.06224, 1.05968, 1.05767, 1.06254, 1.06729, 1.0691, 1.07125, 1.07312, 1.08124, 1.08966, 1.08695, 1.08826, 1.10611, 1.13115, 1.12641, 1.13093, 1.17074, 1.28958, 1.16217, 1.22844, 1.24812, 1.25352, 1.22065, 1.21287, 1.20544, 1.18344, 1.18344, 1.18344, 1.03589, 1.03224, 1.03229, 1.03623, 1.03979, 1.04403, 1.04574, 1.049, 1.04821, 1.06183, 1.0588, 1.06655, 1.08582, 1.10289, 1.10052, 1.10506, 1.143, 1.27373, 1.1459, 1.2156, 1.23455, 1.23968, 1.20753, 1.20127, 1.19629, 1.16809, 1.16809, 1.16809, 1.03456, 1.02955, 1.03079, 1.03509, 1.03949, 1.0437, 1.04236, 1.04486, 1.0517, 1.05864, 1.05516, 1.06167, 1.07738, 1.0985, 1.09317, 1.09559, 1.13557, 1.26076, 1.14118, 1.20545, 1.22137, 1.22802, 1.19936, 1.19676, 1.19088, 1.16709, 1.16709, 1.16709};
};


unsigned int const JetHFCalib::N_TOWER_PHI = 72;
unsigned int const JetHFCalib::N_TOWER_ETA = 82;

JetHFCalib::JetHFCalib(const edm::ParameterSet& config) :
	edm::EDAnalyzer(),
	digis_(config.getParameter<edm::InputTag>("triggerPrimitives")),
	Edigis_(config.getParameter<edm::InputTag>("eTriggerPrimitives")),
	detid_(config.getUntrackedParameter<bool>("useDetIdForUncompression", true)),
	threshold_(config.getUntrackedParameter<double>("threshold", 0.)),
	doClosure_(config.getUntrackedParameter<bool>("doClosure", false))
{
	edm::Service<TFileService> fs;

	genSrc_    = mayConsume<std::vector<reco::GenJet> >(config.getParameter<edm::InputTag>("genSrc"));

	consumes<HcalTrigPrimDigiCollection>(digis_);
	consumes<EcalTrigPrimDigiCollection>(Edigis_);

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
	matched_->Branch("l1_summed33" , &l1_summed33_);
	matched_->Branch("l1_summed55" , &l1_summed55_);
	matched_->Branch("l1_summed77" , &l1_summed77_);

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

	Handle<EcalTrigPrimDigiCollection> Edigis;
	if (!event.getByLabel(Edigis_, Edigis)) {
		LogError("JetHFCalib") <<
			"Can't find ecal trigger primitive digi collection with tag '" <<
			Edigis_ << "'" << std::endl;
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
			tp_ieta_ = id.ieta();
			tp_iphi_ = id.iphi();
			tp_phi_ = convertTPGPhi(id.iphi());
			tp_eta_ = convertHFTPGEta(id.ieta()); //gets gen value
			tp_depth_ = id.depth();
			tp_version_ = id.version();
			tp_soi_ = digi.SOI_compressedEt();


			tp_et_ = decoder->hcaletValue(id, digi.t0());
			if (tp_et_ < threshold_)
				continue;
			tps_->Fill();
		}//end 1x1 upgrade
	}//end for of digis

	for(const auto& jet: *objects){
		l1_summed33_=0;
		l1_summed55_=0;
		l1_summed77_=0;

		if (std::abs(jet.eta())<2.967) continue;//ignore jets in ieta 29
		gen_pt_=jet.pt();
		gen_et_=jet.et();
		gen_eta_=jet.eta();
		gen_phi_=jet.phi();
		gen_ieta_=convertHFGenEta(jet.eta());
		gen_iphi_=convertGenPhi(jet.phi());
		std::cout<<"JET INFO \t  gen_ieta: "<<gen_ieta_<<" eta: "<< gen_eta_<<std::endl;
		for (const auto& digi: *digis) {
			if (digi.id().version() == 1||abs(digi.id().ieta())<29){ //1x1 upgrade or ignore < 29
				HcalTrigTowerDetId id = digi.id();
				double tempet = decoder->hcaletValue(id, digi.t0());
			        tp_et_ = (floor(tempet*2)) / 2.0;	
				int absieta = abs(id.ieta())-30;//how to find bin. ieta 30 ->bin 0
				//if (tp_et_>1){
				//std::cout<<"TPG INFO \t  tpg_et: "<<tp_et_<<" ieta: "<< id.ieta()<<" iphi: "<<id.iphi()<<std::endl;
				//std::cout<<"TPG INFO \t  version: "<<id.version()<<std::endl;
				//}
				if (doClosure_){
					if (tp_et_<5)  tp_et_ = tp_et_; 
					else if (tp_et_<20)  tp_et_ = tp_et_*bin0[absieta] ; 
					else if (tp_et_<30)  tp_et_ = tp_et_*bin1[absieta] ; 
					else if (tp_et_<50)  tp_et_ = tp_et_*bin2[absieta] ; 
					else  tp_et_ = tp_et_*bin3[absieta] ; 
					tp_et_= (int (tp_et_*2))/2.0;
				}


				if (tp_et_ < threshold_) continue;
			  	double ecalet=0;	
				if (abs(id.ieta())==29 || gen_ieta_==-999) continue;

				if (abs(id.ieta())<29){ //if in endcap find ecal et for that tp!!!
					for (const auto& Edigi: *Edigis) {
						if (Edigi.id().ieta()==id.ieta() && Edigi.id().iphi()==id.iphi()){ 
							ecalet = Edigi.compressedEt();
							int ieta = abs(id.ieta())-1;
							int temp = floor((ecalet*0.5)/5);
							int ptbin = temp -1;
							if (ptbin<0) ptbin=0; 
							else if (ptbin>8) ptbin=8;
							ecalet *= ecal[ptbin*9+ieta]; //compressed et!!! easily save the et in a vector of ints (divinde by 2 later) 

							if (ecalet>1) std::cout<<"ECal Tp \t  et: "<< ecalet<<" ieta: "<<Edigi.id().ieta()<<" iphi: "<<Edigi.id().iphi()<<std::endl;
							break;}
					}
				}
				if (abs(gen_ieta_)==30&&abs(gen_iphi_-id.iphi())<2) {
					if ((id.ieta()==28||abs(gen_ieta_-id.ieta())<2)) {l1_summed33_+=tp_et_;l1_summed33_+=ecalet;}
					if ((id.ieta()==28||id.ieta()==27||abs(gen_ieta_-id.ieta())<2)) {l1_summed55_+=tp_et_;l1_summed55_+=ecalet;}
					if ((id.ieta()==26||id.ieta()==28||id.ieta()==27||abs(gen_ieta_-id.ieta())<2)) {l1_summed77_+=tp_et_;l1_summed77_+=ecalet; }
				} 
				else if (abs(gen_ieta_)==31&&abs(gen_iphi_-id.iphi())<2) {
					if ((abs(gen_ieta_-id.ieta())<2)) l1_summed33_+=tp_et_;
					if ((id.ieta()==28||abs(gen_ieta_-id.ieta())<2)) {l1_summed55_+=tp_et_;l1_summed55_+=ecalet;}
					if ((id.ieta()==28||id.ieta()==27||abs(gen_ieta_-id.ieta())<2)) {l1_summed77_+=tp_et_;l1_summed77_+=ecalet; }
				} 
				else{
					if (abs(gen_ieta_-id.ieta())<2&&abs(gen_iphi_-id.iphi())<2) l1_summed33_+=tp_et_;
					if (abs(gen_ieta_-id.ieta())<3&&abs(gen_iphi_-id.iphi())<3) l1_summed55_+=tp_et_;
					if (abs(gen_ieta_-id.ieta())<4&&abs(gen_iphi_-id.iphi())<4) l1_summed77_+=tp_et_;
				}
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
