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
		double HoE_;
		double sumCorr_;
		double sumCorr_e_;
		double sumCorr_h_;
		int ptbin_;


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
	
		float ecal[252] = {1.1847, 1.16759, 1.17779, 1.19955, 1.21125, 1.214, 1.21503, 1.22515, 1.24151, 1.27836, 1.30292, 1.33526, 1.42338, 1.4931, 1.49597, 1.50405, 1.52785, 1.81552, 1.59856, 1.75692, 1.76496, 1.77562, 1.69527, 1.66827, 1.61861, 1.56645, 1.56646, 1.56646, 1.1351, 1.12589, 1.12834, 1.13725, 1.14408, 1.1494, 1.14296, 1.14852, 1.1578, 1.17634, 1.18038, 1.19386, 1.23758, 1.27605, 1.27818, 1.28195, 1.34881, 1.71053, 1.37338, 1.52571, 1.54801, 1.53316, 1.4397, 1.40497, 1.37743, 1.33914, 1.33914, 1.33914, 1.18043, 1.17823, 1.1751, 1.17608, 1.19152, 1.196, 1.20125, 1.2068, 1.22584, 1.22476, 1.22395, 1.22302, 1.25137, 1.28097, 1.29871, 1.2862, 1.33489, 1.60937, 1.28365, 1.41367, 1.42521, 1.42041, 1.36784, 1.34922, 1.32754, 1.29825, 1.29825, 1.29825, 1.11664, 1.11852, 1.11861, 1.12367, 1.12405, 1.14814, 1.14304, 1.15337, 1.16607, 1.18698, 1.17048, 1.17463, 1.2185, 1.23842, 1.23214, 1.24744, 1.30047, 1.47152, 1.22868, 1.33121, 1.34841, 1.35178, 1.30048, 1.28537, 1.27012, 1.24159, 1.24159, 1.24159, 1.08422, 1.08146, 1.08706, 1.08906, 1.08636, 1.10092, 1.10363, 1.11102, 1.1186, 1.13301, 1.12369, 1.14377, 1.16477, 1.17801, 1.18782, 1.17168, 1.24593, 1.36835, 1.20252, 1.28349, 1.29828, 1.30328, 1.26848, 1.25817, 1.2464, 1.22259, 1.22259, 1.22259, 1.07444, 1.06774, 1.06883, 1.0707, 1.07881, 1.08859, 1.08285, 1.08747, 1.09736, 1.10678, 1.10008, 1.10717, 1.12858, 1.15383, 1.15826, 1.14855, 1.19911, 1.32567, 1.17553, 1.25976, 1.27926, 1.28459, 1.24524, 1.23706, 1.22597, 1.20006, 1.20006, 1.20006, 1.06224, 1.05968, 1.05767, 1.06254, 1.06729, 1.0691, 1.07125, 1.07312, 1.08124, 1.08966, 1.08695, 1.08826, 1.10611, 1.13115, 1.12641, 1.13093, 1.17074, 1.28958, 1.16217, 1.22844, 1.24812, 1.25352, 1.22065, 1.21287, 1.20544, 1.18344, 1.18344, 1.18344, 1.03589, 1.03224, 1.03229, 1.03623, 1.03979, 1.04403, 1.04574, 1.049, 1.04821, 1.06183, 1.0588, 1.06655, 1.08582, 1.10289, 1.10052, 1.10506, 1.143, 1.27373, 1.1459, 1.2156, 1.23455, 1.23968, 1.20753, 1.20127, 1.19629, 1.16809, 1.16809, 1.16809, 1.03456, 1.02955, 1.03079, 1.03509, 1.03949, 1.0437, 1.04236, 1.04486, 1.0517, 1.05864, 1.05516, 1.06167, 1.07738, 1.0985, 1.09317, 1.09559, 1.13557, 1.26076, 1.14118, 1.20545, 1.22137, 1.22802, 1.19936, 1.19676, 1.19088, 1.16709, 1.16709, 1.16709};
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
	pions_->Branch("pt" , &pion_pt_);
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
	matched_->Branch("ptbin" , &ptbin_);
	matched_->Branch("HoERatio" , &HoE_);
	matched_->Branch("sumCorr" , &sumCorr_);
	matched_->Branch("sumCorr_e" , &sumCorr_e_);
	matched_->Branch("sumCorr_h" , &sumCorr_h_);

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
		pion_pt_=pion.pt();
		pion_eta_=pion.eta();
		pion_phi_=pion.phi();
		pion_ieta_=convertTPGGenEta(pion.eta());
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
		tp_et_ = decoder->hcaletValue(id, digi.t0());
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
                hCorrTowerETCode[iphi][ieta] = tp_et_*2; //add "uncompressed et" e.g. divide this by two later for 0.5 GeV precision 
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
		int temp = floor((ecalet*0.5)/5);
		int ptbin = temp -1;
		if (ptbin<0) ptbin=0; 
		else if (ptbin>8) ptbin=8;
	
		eTowerETCode[iphi][ieta] = ecalet; //compressed et!!! easily save the et in a vector of ints (divinde by 2 later) 
		eCorrTowerETCode[iphi][ieta] = ecalet*ecal[ptbin*9+ieta]; //compressed et!!! easily save the et in a vector of ints (divinde by 2 later) 
		Etps_->Fill();
	}

	for (const auto& pion: *objects){
		HoE_=0;
		l1_summed55_=0;
		sumCorr_=0;
		l1_summed55_e_=0;
		sumCorr_e_=0;
		l1_summed55_h_=0;
		sumCorr_h_=0;
		if (std::abs(pion.eta())>2.868) continue; //ignore HF pions for now go up to ieta 29 boundary, include 28. 
		//cout<<"GenParticle PdgId: "<< pion.pdgId() <<endl;
		gen_pt_=pion.pt();
		gen_eta_=pion.eta();
		gen_phi_=pion.phi();
		//cout<<"Pion Eta: "<<gen_eta_<<endl;
		gen_ieta_=convertTPGGenEta(pion.eta());
		//cout<<"iEta: "<<gen_ieta_<<endl; 
		gen_iphi_=convertGenPhi(pion.phi());
		//cout<<"GenParticle Pt: "<< gen_pt_ <<" Eta: "<<gen_eta_<<" Phi: "<<gen_phi_<<" iEta: "<<gen_ieta_<<" iPhi: "<<gen_iphi_ <<endl;
                //iETA NEGATIVE
		ptbin_=0;
		double TPGh5x5_=0;
		double cTPGh5x5_=0;
		double TPGe5x5_=0;
		double cTPGe5x5_=0;
		double TPG5x5_=0;
		double cTPG5x5_=0;

		for (int j = -2; j < 3; ++j) {//eta
			for (int k = -2; k < 3; ++k) { //phi
				int tpgsquarephi= gen_iphi_+k;
				int tpgsquareeta= gen_ieta_+j;	

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
				cTPGh5x5_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPGe5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];		
				cTPGe5x5_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
				TPG5x5_ += hTowerETCode[tpgsquarephi][tpgsquareeta];	
				TPG5x5_ += eTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG5x5_ += hCorrTowerETCode[tpgsquarephi][tpgsquareeta];	
				cTPG5x5_ += eCorrTowerETCode[tpgsquarephi][tpgsquareeta];		
			}
		}
		//FILL ISOLATION PROCEDURE
		//
		ptbin_ = floor(gen_pt_/5) -1;
		if (ptbin_<0) ptbin_=0; 
		else if (ptbin_>8) ptbin_=8;
	
		//LSB = 0.5 
		l1_summed55_h_=TPGh5x5_*0.5;
		sumCorr_h_=cTPGh5x5_*0.5;
		l1_summed55_e_=TPGe5x5_*0.5;
		sumCorr_e_=cTPGe5x5_*0.5;
		l1_summed55_=TPG5x5_*0.5;
		sumCorr_=cTPG5x5_*0.5;
		HoE_=TPGh5x5_/TPGe5x5_;
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
