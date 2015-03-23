/*
 * =====================================================================================
 *
 *       Filename:  pionCalibFloat.cc
 *
 *    Description:  
 *
 *         Author:  Laura Dodd, laura.dodd@cern.ch Evan Friis, evan.friis@cern.ch
 *        Company:  UW Madison
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



using namespace std;
using namespace edm;


class pionCalibFloat : public edm::EDAnalyzer {
	public:
		explicit pionCalibFloat(const edm::ParameterSet& pset);
		static const unsigned int N_TOWER_PHI;
		static const unsigned int N_TOWER_ETA;
	private:
		virtual void analyze(const edm::Event& evt, const edm::EventSetup& es);
		double egPhysicalEt(const L1CaloEmCand& cand) const {
			return egLSB_*cand.rank();
		}



		TTree* tree;
		unsigned int run_;
		unsigned int lumi_;
		unsigned long int event_;

		InputTag scalerSrc_;
		InputTag l1Digis_;
		InputTag pvSrc_;
		InputTag ecalSrc_;
		InputTag hcalSrc_;


		float maxEg2x1Pt_;

		float instLumi_;
		int TPGSum_;
		int TPGDiff_;
		int TPGE_;
		int cTPGE_;
		int TPGH_;
		int cTPGH_;
		int TPGHtwr_;
		int TPGEtwr_;
		int TPG5x5_;
		int cTPG5x5_;
		int TPG5x5_gcteta_;
		int TPG5x5_tpgeta_;
		int TPGh5x5_;
		int TPGe5x5_;
		int cTPGh5x5_;
		int cTPGe5x5_;
		int ptbin_;
		unsigned int npvs_;

		vector<double> TPGSF1_;
		vector<double> TPGSF2_;
		vector<double> TPGSFp_;
		vector<double> TPGSFp1_;

		Handle<L1CaloRegionCollection> newRegions;
		Handle<L1CaloEmCollection> newEMCands;
		Handle<LumiScalersCollection> lumiScalers;
		Handle<reco::VertexCollection> vertices;
		Handle<EcalTrigPrimDigiCollection> ecal;
		Handle<HcalTrigPrimDigiCollection> hcal;


		vector<double> sinPhi;
		vector<double> cosPhi;

		int egammaSeed;
		bool ECALOn;
		double egLSB_;
		double regionLSB_;

		double LSB = 0.5;

		vector<vector<float>> eTowerETCode;
		vector<vector<float>> eCorrTowerETCode;
		vector<vector<float>> hTowerETCode;
		vector<vector<float>> hCorrTowerETCode;
};

// THESE ARE IN HELPERS-- this is a reference
//double getPhiTPG(int iPhi) {
// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
//	return convertTPGPhi(iPhi-1);
//}

//int TPGEtaRange(int ieta){
//	unsigned int iEta = 0;
// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
// And we have mapped [-28, -1], [1, 28] onto [0, 55]   
//	if(ieta < 0)
//		iEta = ieta + 28;
//	else if(ieta > 0)
//		iEta = ieta + 27;
//	return iEta;
//}

//double getEtaTPG(int ieta) {
//	unsigned int iEta = 0;
//	// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
//	// And we have mapped [-28, -1], [1, 28] onto [0, 55]
//
//	if(ieta < 0)
//		iEta = ieta + 28;
//	else if(ieta > 0)
//		iEta = ieta + 27;
//	return convertTPGEta(iEta);
//}

unsigned int const pionCalibFloat::N_TOWER_PHI = 72;
unsigned int const pionCalibFloat::N_TOWER_ETA = 56;

pionCalibFloat::pionCalibFloat(const edm::ParameterSet& pset):
	eTowerETCode(N_TOWER_PHI, vector<float>(N_TOWER_ETA)),
	eCorrTowerETCode(N_TOWER_PHI, vector<float>(N_TOWER_ETA)),
	hTowerETCode(N_TOWER_PHI, vector<float>(N_TOWER_ETA)),
	hCorrTowerETCode(N_TOWER_PHI, vector<float>(N_TOWER_ETA))
{
	// Initialize the ntuple builder
	edm::Service<TFileService> fs;
	tree = fs->make<TTree>("Ntuple", "Ntuple");
	tree->Branch("TPGSum", &TPGSum_, "TPGSum/i");
	tree->Branch("TPGDiff", &TPGDiff_, "TPGDiff/i");
	tree->Branch("TPGH", &TPGH_, "TPGH_/i");
	tree->Branch("cTPGH", &cTPGH_, "cTPGH_/i");
	tree->Branch("TPGE", &TPGE_, "TPGE/i");
	tree->Branch("cTPGE", &cTPGE_, "cTPGE/i");
	tree->Branch("TPG5x5", &TPG5x5_, "TPG5x5_/i");
	tree->Branch("cTPG5x5", &cTPG5x5_, "cTPG5x5_/i");
	tree->Branch("TPG5x5_gcteta", &TPG5x5_gcteta_, "TPG5x5_gcteta_/i");
	tree->Branch("TPG5x5_tpgeta", &TPG5x5_tpgeta_, "TPG5x5_tpgeta_/i");
	tree->Branch("TPGh5x5", &TPGh5x5_, "TPGh5x5_/i");
	tree->Branch("cTPGh5x5", &cTPGh5x5_, "cTPGh5x5_/i");
	tree->Branch("TPGe5x5", &TPGe5x5_, "TPGe5x5_/i");
	tree->Branch("cTPGe5x5", &cTPGe5x5_, "cTPGe5x5_/i");
	tree->Branch("ptbin", &ptbin_, "ptbin_/i");
	tree->Branch("run", &run_, "run/i");
	tree->Branch("lumi", &lumi_, "lumi/i");
	tree->Branch("evt", &event_, "evt/l");
	tree->Branch("npvs", &npvs_, "npvs/i");
	tree->Branch("instlumi", &instLumi_, "instlumi/F");
	tree->Branch("maxEg2x1Pt", &maxEg2x1Pt_, "maxEg2x1Pt/F");
	scalerSrc_ = pset.exists("scalerSrc") ? pset.getParameter<InputTag>("scalerSrc") : InputTag("scalersRawToDigi");
	// UCT variables
	pvSrc_ = pset.exists("pvSrc") ? pset.getParameter<InputTag>("pvSrc") : InputTag("offlinePrimaryVertices");
	ecalSrc_ = pset.exists("ecalSrc") ? pset.getParameter<InputTag>("ecalSrc"): InputTag("ecalDigis:EcalTriggerPrimitives");
	hcalSrc_ = pset.exists("hcalSrc") ? pset.getParameter<InputTag>("hcalSrc"): InputTag("hcalDigis");
	TPGSF1_= pset.getParameter<vector<double> >("TPGSF1");
	TPGSF2_= pset.getParameter<vector<double> >("TPGSF2");
	TPGSFp_= pset.getParameter<vector<double> >("TPGSFp");
	TPGSFp1_= pset.getParameter<vector<double> >("TPGSFp1");
	regionLSB_ = pset.getParameter<double>("regionLSB");
	egLSB_ = pset.getParameter<double>("egammaLSB");
	egammaSeed = pset.getParameter<int>("egammaSeed");
	ECALOn = pset.getParameter<bool>("ECALOn");
}


void pionCalibFloat::analyze(const edm::Event& evt, const edm::EventSetup& es) {

	// Setup meta info
	run_ = evt.id().run();
	lumi_ = evt.id().luminosityBlock();
	event_ = evt.id().event();

	// Get instantaneous lumi from the scalers
	// thx to Carlo Battilana
	//Handle<LumiScalersCollection> lumiScalers;
	//Handle<L1CaloRegionCollection> newRegions;
	//edm::DetSetVector<L1CaloRegionCollection> newRegion;
	//edm::Handle<L1CaloRegionCollection>::const_iterator newRegion;

	evt.getByLabel(scalerSrc_, lumiScalers);
	evt.getByLabel("l1Digis", newRegions);
	evt.getByLabel("l1Digis", newEMCands);
	evt.getByLabel(pvSrc_, vertices);
	evt.getByLabel(ecalSrc_, ecal);
	evt.getByLabel(hcalSrc_, hcal);

	// EVENT INFO 
	instLumi_ = -1;
	npvs_ = 0;

	npvs_ = vertices->size();

	if (lumiScalers->size())
		instLumi_ = lumiScalers->begin()->instantLumi();

	//Reset important things
	std::cout<<"Reset important things"<<std::endl;

	maxEg2x1Pt_ = 0;


	TPGSum_ = 0;
	TPGDiff_ = 0;


	TPGE_ =0;
	cTPGE_ =0;
	TPGH_=0;
	cTPGH_=0;

	cTPG5x5_=0;
	cTPGh5x5_=0;
	cTPGe5x5_=0;

	ptbin_=0;

	TPG5x5_=0;
	TPG5x5_gcteta_=999;
	TPG5x5_tpgeta_=999;
	TPGh5x5_=0;
	TPGe5x5_=0;

	// TPG TEST
	int maxTPGPt = 0;
	int maxTPGEPt = 0;
	int maxTPGHPt = 0;
	int maxTPGPt_phi = 999;
	int maxTPGHPt_phi = 999;
	int maxTPGEPt_phi = 999;
	int maxTPGPt_eta = 999;
	int maxTPGHPt_eta = 999;
	int maxTPGEPt_eta = 999;




	std::cout << "TPGS" << std::endl;
	//	std::cout << "ECAL TPGS" << std::endl;
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
		TPGSum_ +=et;
		TPGE_ +=et;
		eTowerETCode[iphi][ieta] = et; 
		int etbin;

		if(et<10){etbin=0;}
		else if(et<15){etbin=1;}
		else if(et<20){etbin=2;}
		else if(et<25){etbin=3;}
		else if(et<30){etbin=4;}
		else if(et<35){etbin=5;}
		else if(et<40){etbin=6;}
		else if(et<45){etbin=7;}
		else {etbin=8;}

		//		double alpha = TPGSF1_[etbin*56+ieta]; //v1


		//v2 v3
		double alpha = TPGSF1_[etbin*56+ieta]*TPGSF2_[ieta];


		eCorrTowerETCode[iphi][ieta] = alpha*et;
		cTPGE_ +=alpha*et;
		//		if (et>0){cout<<"eTowerETCode: "<<eTowerETCode[iphi][ieta]<<endl;}
		if ((*ecal)[i].compressedEt() > 0) {
			//			std::cout << "ecal eta/phi=" << ieta << "/" << iphi
			//				<< " = (" << getEtaTPG(cal_ieta) << "/" << getPhiTPG(cal_iphi) << ") "
			//				<< " et="<< (*ecal)[i].compressedEt()*egLSB_ << " fg=" << (*ecal)[i].fineGrain()
			//				<< " rctEta="<< twrEta2RegionEta(ieta) << " rctPhi=" << twrPhi2RegionPhi(cal_iphi)
			//				<< std::endl;
		}
		if (et>maxTPGEPt){
			maxTPGEPt=et;
			maxTPGEPt_phi = iphi; //this one starts at 0-72
			maxTPGEPt_eta = ieta; //this one is 0-54
		} 
	}
	ESHandle<L1CaloHcalScale> hcalScale;
	es.get<L1CaloHcalScaleRcd>().get(hcalScale);

	std::cout << "HCAL TPGS" << std::endl;
	for (size_t i = 0; i < hcal->size(); ++i) {
		int ieta = (*hcal)[i].id().ieta();
		int iphi = (*hcal)[i].id().iphi();
		int hniphi = iphi-1;
		int hnieta = TPGEtaRange(ieta);
		short absieta = std::abs((*hcal)[i].id().ieta());
		short zside = (*hcal)[i].id().zside();

		if (ieta >= -1000 && ieta <= 1000 &&
				iphi >= -1000 && ieta <= 1000) {
			double energy = hcalScale->et(
					//(*hcal)[i].SOI_compressedEt(), absieta, zside); 
					(*hcal)[i].SOI_compressedEt(), absieta, zside)*LSB; //*LSB

			hTowerETCode[hniphi][hnieta] = energy;
			TPGSum_ +=energy;
			TPGH_ += energy;
			/*
			   if(energy<10){hetbin=0;}
			   else if(energy<15){hetbin=1;}
			   else if(energy<20){hetbin=2;}
			   else if(energy<25){hetbin=3;}
			   else if(energy<30){hetbin=4;}
			   else if(energy<35){hetbin=5;}
			   else if(energy<40){hetbin=6;}
			   else if(energy<45){hetbin=7;}
			   else {hetbin=8;}
			   */
			//	double alpha_h = 1; //v1 v2
			double alpha_h = TPGSFp_[hnieta]; //v3
			//	double alpha_h2 = TPGSFp1_[hnieta]; //v4
			hCorrTowerETCode[hniphi][hnieta] = alpha_h*energy;
			cTPGH_ += alpha_h*energy;

			if (energy > 0) {
				//				std::cout << "hcal eta/phi=" << ieta << "/" << iphi
				//					<< " = (" << getEtaTPG(ieta) << "/" << getPhiTPG(iphi) << ") "
				//					<< " et=" << (*hcal)[i].SOI_compressedEt()
				//					<< " energy=" << energy
				//					<< " rctEta="<< twrEta2RegionEta(hnieta) << " rctPhi=" << twrPhi2RegionPhi(hniphi)
				//					<< " fg=" << (*hcal)[i].SOI_fineGrain() << std::endl;
			}
			if (energy>maxTPGHPt){
				maxTPGHPt=energy;
				maxTPGHPt_phi = hniphi; //this one starts at 0-72
				maxTPGHPt_eta = hnieta; //this one is 0-54
			} 
		}
	}//end HCAL TPG
	//if(ECALOn || maxTPGHPt<maxTPGEPt){
	if(ECALOn){
		maxTPGPt = maxTPGEPt;
		maxTPGPt_phi= maxTPGEPt_phi;
		maxTPGPt_eta= maxTPGEPt_eta;
	} 
	else{
		maxTPGPt = maxTPGHPt;
		maxTPGPt_phi= maxTPGHPt_phi;
		maxTPGPt_eta= maxTPGHPt_eta;
	}

	// tpg5x5 calculation
	if (maxTPGPt >0){
		TPG5x5_gcteta_ = twrEta2RegionEta(maxTPGPt_eta);
		TPG5x5_tpgeta_ = maxTPGPt_eta;
		for (int j = -5; j < 6; ++j) {//phi
			for (int k = -5; k < 6; ++k) { //eta
				int tpgsquarephi= maxTPGPt_phi+j;
				int tpgsquareeta= maxTPGPt_eta+k;	
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
	}

	if (maxTPGPt>0.9*(TPGH_+TPGE_)){TPGDiff_=1;}
	if(maxTPGPt<10){ptbin_=0;}
	else if(maxTPGPt<15){ptbin_=1;}
	else if(maxTPGPt<20){ptbin_=2;}
	else if(maxTPGPt<25){ptbin_=3;}
	else if(maxTPGPt<30){ptbin_=4;}
	else if(maxTPGPt<35){ptbin_=5;}
	else if(maxTPGPt<40){ptbin_=6;}
	else if(maxTPGPt<45){ptbin_=7;}
	else {ptbin_=8;}



	std::cout<<"egt Cand"<<std::endl;
	for(L1CaloEmCollection::const_iterator egtCand =
			newEMCands->begin();
			egtCand != newEMCands->end(); egtCand++){
		double eget = egPhysicalEt(*egtCand);
		if(eget > egammaSeed && eget > maxEg2x1Pt_)
		{
			maxEg2x1Pt_ = eget; 
		}
	}//end egtCAnd


	std::cout<<"Fill Tree"<<std::endl;

	tree->Fill();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(pionCalibFloat);
