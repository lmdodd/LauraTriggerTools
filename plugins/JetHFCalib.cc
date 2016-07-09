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

using namespace edm;
using namespace std;


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

		//The square sums
		vector<vector<unsigned int>> eTowerETCode;
		vector<vector<unsigned int>> eCorrTowerETCode;
		vector<vector<unsigned int>> hTowerETCode;
		vector<vector<unsigned int>> hCorrTowerETCode;


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

		float bin0[12] = {1.85, 1.86, 1.83, 1.84, 1.83, 1.84, 1.95, 2.02, 2.12, 2.22, 2.39, 2.43};
		float bin1[12] = {2.18, 2.18, 2.13, 2.02, 1.91 , 1.97, 1.99, 2.04, 2.19, 2.44, 2.81, 2.92};
		float bin2[12] = {1.95, 1.95, 1.90, 1.82, 1.72, 1.74, 1.79, 1.83, 1.94, 2.15, 2.72, 2.75};
		float bin3[12] = {1.68, 1.68, 1.67, 1.60, 1.55, 1.58, 1.61, 1.67, 1.78, 1.94, 2.49, 2.62};

		float ecal[252] = {1.1847, 1.16759, 1.17779, 1.19955, 1.21125, 1.214, 1.21503, 1.22515, 1.24151, 1.27836, 1.30292, 1.33526, 1.42338, 1.4931, 1.49597, 1.50405, 1.52785, 1.81552, 1.59856, 1.75692, 1.76496, 1.77562, 1.69527, 1.66827, 1.61861, 1.56645, 1.56646, 1.56646, 1.1351, 1.12589, 1.12834, 1.13725, 1.14408, 1.1494, 1.14296, 1.14852, 1.1578, 1.17634, 1.18038, 1.19386, 1.23758, 1.27605, 1.27818, 1.28195, 1.34881, 1.71053, 1.37338, 1.52571, 1.54801, 1.53316, 1.4397, 1.40497, 1.37743, 1.33914, 1.33914, 1.33914, 1.18043, 1.17823, 1.1751, 1.17608, 1.19152, 1.196, 1.20125, 1.2068, 1.22584, 1.22476, 1.22395, 1.22302, 1.25137, 1.28097, 1.29871, 1.2862, 1.33489, 1.60937, 1.28365, 1.41367, 1.42521, 1.42041, 1.36784, 1.34922, 1.32754, 1.29825, 1.29825, 1.29825, 1.11664, 1.11852, 1.11861, 1.12367, 1.12405, 1.14814, 1.14304, 1.15337, 1.16607, 1.18698, 1.17048, 1.17463, 1.2185, 1.23842, 1.23214, 1.24744, 1.30047, 1.47152, 1.22868, 1.33121, 1.34841, 1.35178, 1.30048, 1.28537, 1.27012, 1.24159, 1.24159, 1.24159, 1.08422, 1.08146, 1.08706, 1.08906, 1.08636, 1.10092, 1.10363, 1.11102, 1.1186, 1.13301, 1.12369, 1.14377, 1.16477, 1.17801, 1.18782, 1.17168, 1.24593, 1.36835, 1.20252, 1.28349, 1.29828, 1.30328, 1.26848, 1.25817, 1.2464, 1.22259, 1.22259, 1.22259, 1.07444, 1.06774, 1.06883, 1.0707, 1.07881, 1.08859, 1.08285, 1.08747, 1.09736, 1.10678, 1.10008, 1.10717, 1.12858, 1.15383, 1.15826, 1.14855, 1.19911, 1.32567, 1.17553, 1.25976, 1.27926, 1.28459, 1.24524, 1.23706, 1.22597, 1.20006, 1.20006, 1.20006, 1.06224, 1.05968, 1.05767, 1.06254, 1.06729, 1.0691, 1.07125, 1.07312, 1.08124, 1.08966, 1.08695, 1.08826, 1.10611, 1.13115, 1.12641, 1.13093, 1.17074, 1.28958, 1.16217, 1.22844, 1.24812, 1.25352, 1.22065, 1.21287, 1.20544, 1.18344, 1.18344, 1.18344, 1.03589, 1.03224, 1.03229, 1.03623, 1.03979, 1.04403, 1.04574, 1.049, 1.04821, 1.06183, 1.0588, 1.06655, 1.08582, 1.10289, 1.10052, 1.10506, 1.143, 1.27373, 1.1459, 1.2156, 1.23455, 1.23968, 1.20753, 1.20127, 1.19629, 1.16809, 1.16809, 1.16809, 1.03456, 1.02955, 1.03079, 1.03509, 1.03949, 1.0437, 1.04236, 1.04486, 1.0517, 1.05864, 1.05516, 1.06167, 1.07738, 1.0985, 1.09317, 1.09559, 1.13557, 1.26076, 1.14118, 1.20545, 1.22137, 1.22802, 1.19936, 1.19676, 1.19088, 1.16709, 1.16709, 1.16709};

		float hcal[252] = {1.511112, 1.519900, 1.499483, 1.488560, 1.528111, 1.475114, 1.476616, 1.514163, 1.515306, 1.542464, 1.511663, 1.593745, 1.493667, 1.485315, 1.419925, 1.349169, 1.312518, 1.423302, 1.478461, 1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 1.383350, 1.365700, 1.368470, 1.354610, 1.348480, 1.329720, 1.272250, 1.301710, 1.322210, 1.360860, 1.333850, 1.392200, 1.403060, 1.394870, 1.322050, 1.244570, 1.206910, 1.321870, 1.344160, 1.403270, 1.403270, 1.403270, 1.403270, 1.403270, 1.403270, 1.403270, 1.403270, 1.403270, 1.245690, 1.238320, 1.245420, 1.234830, 1.243730, 1.249790, 1.179450, 1.213620, 1.219030, 1.252130, 1.209560, 1.250710, 1.280490, 1.262800, 1.254060, 1.186810, 1.127830, 1.260000, 1.275140, 1.305850, 1.305850, 1.305850, 1.305850, 1.305850, 1.305850, 1.305850, 1.305850, 1.305850, 1.189940, 1.189120, 1.177120, 1.179690, 1.185510, 1.150590, 1.151830, 1.167860, 1.154310, 1.163190, 1.161700, 1.136100, 1.161870, 1.195050, 1.153910, 1.117900, 1.106750, 1.208120, 1.160020, 1.232800, 1.232800, 1.232800, 1.232800, 1.232800, 1.232800, 1.232800, 1.232800, 1.232800, 1.122540, 1.129520, 1.125080, 1.115150, 1.118250, 1.096190, 1.108170, 1.087490, 1.109750, 1.099780, 1.081000, 1.050610, 1.078270, 1.079460, 1.047740, 1.041400, 1.041750, 1.116880, 1.097730, 1.125780, 1.125780, 1.125780, 1.125780, 1.125780, 1.125780, 1.125780, 1.125780, 1.125780, 1.110470, 1.117340, 1.115980, 1.088490, 1.088260, 1.078230, 1.062720, 1.054690, 1.053270, 1.086640, 1.050620, 1.038470, 1.046440, 1.059130, 1.012240, 1.039030, 1.036040, 1.088460, 1.078880, 1.090600, 1.090600, 1.090600, 1.090600, 1.090600, 1.090600, 1.090600, 1.090600, 1.090600, 1.115970, 1.111010, 1.113170, 1.079390, 1.076850, 1.063730, 1.039300, 1.049910, 1.040100, 1.025820, 1.015830, 1.015850, 1.010810, 1.014210, 0.980321, 1.023580, 1.045990, 1.073220, 1.057750, 1.059850, 1.059850, 1.059850, 1.059850, 1.059850, 1.059850, 1.059850, 1.059850, 1.059850, 1.061180, 1.059770, 1.071210, 1.064420, 1.065340, 1.043070, 1.041400, 1.022680, 1.017410, 1.017690, 1.005610, 1.006360, 0.999420, 0.990866, 0.986723, 0.989036, 0.995116, 1.045620, 1.024330, 1.040660, 1.040660, 1.040660, 1.040660, 1.040660, 1.040660, 1.040660, 1.040660, 1.040660, 1.083150, 1.067090, 1.083180, 1.061010, 1.075640, 1.051640, 1.038760, 1.042670, 1.010910, 1.011580, 1.006560, 0.984468, 0.986642, 0.985799, 0.968133, 1.000290, 1.011210, 1.046690, 1.016670, 1.020470, 1.020470, 1.020470, 1.020470, 1.020470, 1.020470, 1.020470, 1.020470, 1.020470};

};


unsigned int const JetHFCalib::N_TOWER_PHI = 72;
unsigned int const JetHFCalib::N_TOWER_ETA = 82;

JetHFCalib::JetHFCalib(const edm::ParameterSet& config) :
	edm::EDAnalyzer(),
	digis_(config.getParameter<edm::InputTag>("triggerPrimitives")),
	Edigis_(config.getParameter<edm::InputTag>("eTriggerPrimitives")),
	detid_(config.getUntrackedParameter<bool>("useDetIdForUncompression", true)),
	threshold_(config.getUntrackedParameter<double>("threshold", 0.)),
	doClosure_(config.getUntrackedParameter<bool>("doClosure", false)),
	eTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	eCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA)),
	hCorrTowerETCode(N_TOWER_PHI, vector<unsigned int>(N_TOWER_ETA))
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

	//cout<<"=================================================="<<endl;
	ESHandle<HcalTrigTowerGeometry> tpd_geo;
	setup.get<CaloGeometryRecord>().get(tpd_geo);

	std::map<HcalTrigTowerDetId, HcalTriggerPrimitiveDigi> ttids;
	for (const auto& digi: *digis) {
		if (digi.id().version() == 1){ //1x1 upgrade
			ttids[digi.id()] = digi;
			HcalTrigTowerDetId id = digi.id();
			tp_ieta_ = id.ieta();
			int ieta = HCALTPGEtaRange(id.ieta());
			tp_iphi_ = id.iphi();
			int iphi = tp_iphi_-1;
			tp_phi_ = convertTPGPhi(id.iphi());
			tp_eta_ = convertHFTPGEta(id.ieta()); //gets gen value
			tp_depth_ = id.depth();
			tp_version_ = id.version();
			tp_soi_ = digi.SOI_compressedEt();


			tp_et_ = decoder->hcaletValue(id, digi.t0());
			hTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 
			int tmpieta = abs(id.ieta())-1;
			int temp = floor((tp_et_)/5);
			int ptbin = temp -1;
			if (ptbin<0) ptbin=0; 
			else if (ptbin>8) ptbin=8;
			if (tmpieta<28) tp_et_*=hcal[ptbin*9+tmpieta]; //tmpieta 0-27. apply HCAL SF in the barrel
			if (doClosure_) {
				if (tp_et_<5)  tp_et_ = tp_et_; 
				else if (tp_et_<20)  tp_et_ = tp_et_*bin0[tmpieta] ; 
				else if (tp_et_<30)  tp_et_ = tp_et_*bin1[tmpieta] ; 
				else if (tp_et_<50)  tp_et_ = tp_et_*bin2[tmpieta] ; 
				else  tp_et_ = tp_et_*bin3[tmpieta] ; 
			} //compressed et!!! easily save the et in a vector of ints (divide by 2 later) 
			hCorrTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 

			if (tp_et_ < threshold_)
				continue; // only fill treenon-zero
			tps_->Fill();
		}//end 1x1 upgrade
	}//end for of digis

	for (const auto& Edigi: *Edigis) {
		double ecalet = Edigi.compressedEt();
		//find ecal SF 
		int ieta = abs(Edigi.id().ieta())-1;
		int temp = floor((ecalet*0.5)/5);
		int ptbin = temp -1;
		if (ptbin<0) ptbin=0; 
		else if (ptbin>8) ptbin=8;
		//ecalet *= ecal[ptbin*9+ieta]; //compressed et!!! easily save the et in a vector of ints (divide by 2 later) 

		int ieta_range = HCALTPGEtaRange(Edigi.id().ieta());
		int iphi = Edigi.id().iphi()-1;
		eCorrTowerETCode[iphi][ieta_range] = ecalet*ecal[ptbin*9+ieta]; //compressed et!!! easily save the et in a vector of ints (divide by 2 later) 
		eTowerETCode[iphi][ieta_range] = ecalet; //compressed et!!! easily save the et in a vector of ints (divide by 2 later) 

		if (ieta>26 && Edigi.compressedEt()>1){
			//cout<<"EDIGI INFO \t CompET: "<<Edigi.compressedEt()<<" \t id.ieta: "<<Edigi.id().ieta()<<" \t id.iphi: "<<Edigi.id().iphi()<<endl;
			//cout<<"           \t SF: "<<ecal[ptbin*9+ieta]<<" \t ptbin: "<<ptbin<<" \t newComp et: "<< ecalet*ecal[ptbin*9+ieta]<<endl;
			//cout<<"           \t ieta_rng: "<<ieta_range <<" \t iphi_rng: "<<iphi<<endl;
		} 
	}

	for(const auto& jet: *objects){
		l1_summed33_=0;
		l1_summed55_=0;
		l1_summed77_=0;

		if (std::abs(jet.eta())<2.967 || jet.pt()<5) continue;//ignore jets in ieta 29
		gen_pt_=jet.pt();
		gen_et_=jet.et();
		gen_eta_=jet.eta();
		gen_phi_=jet.phi();
		gen_ieta_=convertHFGenEta(jet.eta());
		int ieta_range = HCALTPGEtaRange(gen_ieta_); 
		gen_iphi_=convertGenPhi(jet.phi());
		int iphi_range=gen_iphi_-1;
		//cout<<"======== \t JET"<<endl;
		//cout<<"Jet INFO \t ieta: "<<ieta_range<<" \t iphi: "<<iphi_range<<endl; 
		//cout<<"Jet INFO \t pt: "<<gen_pt_<<" \t et: "<<gen_et_<<endl; 
		//cout<<"Jet INFO \t  gen_ieta: "<<gen_ieta_<<" \t eta: "<< gen_eta_<<std::endl;
		//now sum in squares around jet eta 
		//

		double TPGh3x3_=0;
		double cTPGh3x3_=0;
		double TPGe3x3_=0;
		double cTPGe3x3_=0;
		double TPG3x3_=0;
		double cTPG3x3_=0;

		////cout<<"3x3 TPG INFO"<<endl;
		int counter=0;
		for (int j = -1; j < 2; ++j) {//eta
			for (int k = -1; k < 2; ++k) { //phi
				int tpgsquarephi= iphi_range+k;
				int tpgsquareeta= ieta_range+j;	
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
				if (tpgsquareeta==14) {tpgsquareeta=15;}//ignore 29
				if (tpgsquareeta==13) {tpgsquareeta=14;}//ignore 29
				if (tpgsquareeta==12) {tpgsquareeta=13;}//ignore 29
				if (tpgsquareeta==67) {tpgsquareeta=66;}//ignore 29
				if (tpgsquareeta==68) {tpgsquareeta=67;}//ignore 29
				if (tpgsquareeta==69) {tpgsquareeta=68;}//ignore 29

				if (tpgsquareeta>81 || tpgsquareeta<0) {continue;}//No Eta values beyond 41
				TPGh3x3_ += hTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGh3x3_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPGe3x3_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGe3x3_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPG3x3_ += hTowerETCode[tpgsquarephi][tpgsquareeta];	
				TPG3x3_ += eTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG3x3_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG3x3_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				////cout<<"Adding "<<counter<<"tower eta "<<tpgsquareeta<<" phi "<<tpgsquarephi<<" with ecal energy "<<eCorrTowerETCode[tpgsquarephi][tpgsquareeta]<<" and hcal energy "<<hCorrTowerETCode[tpgsquarephi][tpgsquareeta]<<"."<<endl;
				counter++;
			}
		}


		double TPGh5x5_=0;
		double cTPGh5x5_=0;
		double TPGe5x5_=0;
		double cTPGe5x5_=0;
		double TPG5x5_=0;
		double cTPG5x5_=0;

		//cout<<"5x5 TPG INFO"<<endl;
		counter=0;
		for (int j = -2; j < 3; ++j) {//eta
			for (int k = -2; k < 3; ++k) { //phi
				int tpgsquarephi= iphi_range+k;
				int tpgsquareeta= ieta_range+j;	

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
				if (tpgsquareeta==14) {tpgsquareeta=15;}//ignore 29
				if (tpgsquareeta==13) {tpgsquareeta=14;}//ignore 29
				if (tpgsquareeta==12) {tpgsquareeta=13;}//ignore 29
				if (tpgsquareeta==67) {tpgsquareeta=66;}//ignore 29
				if (tpgsquareeta==68) {tpgsquareeta=67;}//ignore 29
				if (tpgsquareeta==69) {tpgsquareeta=68;}//ignore 29

				if (tpgsquareeta>81 || tpgsquareeta<0) {continue;}//No Eta values beyond 41
				TPGh5x5_ += hTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGh5x5_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPGe5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGe5x5_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPG5x5_ += hTowerETCode[tpgsquarephi][tpgsquareeta];	
				TPG5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG5x5_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG5x5_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				//cout<<"Adding "<<counter<<" tower eta "<<tpgsquareeta<<" phi "<<tpgsquarephi<<" with ecal energy "<<eCorrTowerETCode[tpgsquarephi][tpgsquareeta]<<" and hcal energy "<<hCorrTowerETCode[tpgsquarephi][tpgsquareeta]<<"."<<endl;
				counter++;
			}
		}
		//cout<<"hcal TPG: "<<cTPGh5x5_<<" \t ecal TPG: "<<cTPGe5x5_<<" \t TPG total: "<<cTPG5x5_<<endl;

		double TPGh7x7_=0;
		double cTPGh7x7_=0;
		double TPGe7x7_=0;
		double cTPGe7x7_=0;
		double TPG7x7_=0;
		double cTPG7x7_=0;

		for (int j = -3; j < 4; ++j) {//eta
			for (int k = -3; k < 4; ++k) { //phi
				int tpgsquarephi= iphi_range+k;
				int tpgsquareeta= ieta_range+j;	


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
				if (tpgsquareeta==14) {tpgsquareeta=15;}//ignore 29
				if (tpgsquareeta==13) {tpgsquareeta=14;}//ignore 29
				if (tpgsquareeta==12) {tpgsquareeta=13;}//ignore 29
				if (tpgsquareeta==67) {tpgsquareeta=66;}//ignore 29
				if (tpgsquareeta==68) {tpgsquareeta=67;}//ignore 29
				if (tpgsquareeta==69) {tpgsquareeta=68;}//ignore 29
				if (tpgsquareeta>81 || tpgsquareeta<0) {continue;}//No Eta values beyond 41
				TPGh7x7_ += hTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGh7x7_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPGe7x7_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGe7x7_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPG7x7_ += hTowerETCode[tpgsquarephi][tpgsquareeta];	
				TPG7x7_ += eTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG7x7_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG7x7_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
			}
		}

		//Goto real ET
		l1_summed33_ = cTPG3x3_*0.5;
		l1_summed55_ = cTPG5x5_*0.5;
		l1_summed77_ = cTPG7x7_*0.5;

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
