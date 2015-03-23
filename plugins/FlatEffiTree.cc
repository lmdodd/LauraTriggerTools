/*
 * =====================================================================================
 *
 *       Filename:  FlatEffiTree.cc
 *
 *    Description:
 *
 *         Author:  Laura Dodd, laura.dodd@cern.ch
 *        Company:  UW Madison
 *
 * =====================================================================================
 */
#include <stdio.h>
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


typedef std::vector<edm::InputTag> VInputTag;
//typedef std::vector<unsigned int> PackedUIntCollection;
using namespace std;
using namespace edm;

namespace {

	// Predicate to sort candidates by descending pt
	class CandPtSorter {
		public:
			bool operator()(const reco::Candidate* candA, const reco::Candidate* candB)
				const {
					return candA->pt() > candB->pt();
				}
	};

	// Turn a set of VInputTags into a colleciton of candidate pointers.
	std::vector<const reco::Candidate*> getCollections(
			const edm::Event& evt, const VInputTag& collections) {
		std::vector<const reco::Candidate*> output;
		// Loop over collections
		for (size_t i = 0; i < collections.size(); ++i) {
			edm::Handle<edm::View<reco::Candidate> > handle;
			evt.getByLabel(collections[i], handle);
			// Loop over objects in current collection
			for (size_t j = 0; j < handle->size(); ++j) {
				const reco::Candidate& object = handle->at(j);
				output.push_back(&object);
			}
		}
		return output;
	}

}

class FlatEffiTree : public edm::EDAnalyzer {
	public:
		explicit FlatEffiTree(const edm::ParameterSet& pset);
		static const unsigned int N_TOWER_PHI;
		static const unsigned int N_TOWER_ETA;
	private:
		virtual void analyze(const edm::Event& evt, const edm::EventSetup& es);
		double egPhysicalEt(const L1CaloEmCand& cand) const {
			return egLSB_*cand.rank();
		}
		double regionPhysicalEt(const L1CaloRegion& cand) const {
			return regionLSB_*cand.et();
		}


		TTree* tree;
		InputTag scalerSrc_;
		InputTag l1Digis_;
		InputTag pvSrc_;

		VInputTag recoSrc_;
		VInputTag l1Src_;

		//initialize run info
		unsigned int run_;
		unsigned int lumi_;
		unsigned long int event_;
		unsigned int npvs_;
		float instLumi_;


		//reco information
		vector<Float_t>* genpts_;
		vector<Float_t>* genetas_;
		vector<Float_t>* genphis_;

		//L1 information
		vector<Float_t>* pts_;
		vector<Float_t>* etas_;
		vector<Float_t>* phis_;

		//efficiency information
		vector<Int_t>* genMatches_;

		//handles
		Handle<L1CaloRegionCollection> newRegions;
		Handle<L1CaloEmCollection> newEMCands;
		Handle<LumiScalersCollection> lumiScalers;
		Handle<reco::VertexCollection> vertices;


		vector<double> sinPhi;
		vector<double> cosPhi;

		double egLSB_;
		double regionLSB_;
};





FlatEffiTree::FlatEffiTree(const edm::ParameterSet& pset){

	// Initialize the ntuple builder
	edm::Service<TFileService> fs;
	tree = fs->make<TTree>("Ntuple", "Ntuple");

	//run informaiton
	tree->Branch("run", &run_, "run/i");
	tree->Branch("lumi", &lumi_, "lumi/i");
	tree->Branch("evt", &event_, "evt/l");
	tree->Branch("npvs", &npvs_, "npvs/i"); //check 73X pileup
	tree->Branch("instlumi", &instLumi_, "instlumi/F");

	//calibration vectors
	tree->Branch("recoMatch", "std::vector<int>", &genMatches_); //event matched

	tree->Branch("recoPt", "std::vector<float>", &genpts_); //reco eg rct pt
	tree->Branch("recoGctEta", "std::vector<float>", &genetas_);//reco eta
	tree->Branch("recoGctPhi", "std::vector<float>", &genphis_);//reco phi

	tree->Branch("l1Pt", "std::vector<float>", &pts_); //reco eg rct pt
	tree->Branch("l1Eta", "std::vector<float>", &etas_);//rct gct eta?
	tree->Branch("l1Phi", "std::vector<float>", &phis_);//rct gct phi?

	scalerSrc_ = pset.exists("scalerSrc") ? pset.getParameter<InputTag>("scalerSrc") : InputTag("scalersRawToDigi");
	pvSrc_ = pset.exists("pvSrc") ? pset.getParameter<InputTag>("pvSrc") : InputTag("offlinePrimaryVertices");

	// Input variables
	recoSrc_ = pset.getParameter<VInputTag>("recoSrc");
	l1Src_ = pset.getParameter<VInputTag>("l1Src");
	//l1Digis_ = pset.exists("l1Digis") ? pset.getParameter<VInputTag>("l1Digis") : VInputTag("l1Digis");

	regionLSB_ = pset.getParameter<double>("regionLSB");//currently unused
	egLSB_ = pset.getParameter<double>("egammaLSB");
}


unsigned int const FlatEffiTree::N_TOWER_PHI = 72;
unsigned int const FlatEffiTree::N_TOWER_ETA = 56;


void FlatEffiTree::analyze(const edm::Event& evt, const edm::EventSetup& es) {
	// Setup meta info
	run_ = evt.id().run();
	lumi_ = evt.id().luminosityBlock();
	event_ = evt.id().event();
	evt.getByLabel(scalerSrc_, lumiScalers);
	evt.getByLabel("l1Digis", newRegions);
	evt.getByLabel("l1Digis", newEMCands);
	evt.getByLabel(pvSrc_, vertices);


	// EVENT INFO
	instLumi_ = -1;
	npvs_ = 0;
	npvs_ = vertices->size();

	if (lumiScalers->size())
		instLumi_ = lumiScalers->begin()->instantLumi();


	//get reco eg pt
	std::vector<const reco::Candidate*> objects = getCollections(evt, l1Src_);
	std::vector<const reco::Candidate*> genObjects = getCollections(evt, recoSrc_);

	std::sort(objects.begin(), objects.end(), CandPtSorter());
	std::sort(genObjects.begin(), genObjects.end(), CandPtSorter());



	//Reset important things
	std::cout<<"Reset important things"<<std::endl;
	pts_->clear();
	etas_->clear();
	phis_->clear();

	genMatches_->clear();
	genpts_->clear();
	genetas_->clear();
	genphis_->clear();

	std::cout<<"L1 Objects!"<<std::endl;
	for (size_t i = 0; i < objects.size(); ++i) {
		//  std::cout<<objects[i]->pt()<<"   "<<objects[i]->eta()<<"   "<<objects[i]->phi()<<std::endl;
		pts_->push_back(objects[i]->pt());
		etas_->push_back(objects[i]->eta());
		phis_->push_back(objects[i]->phi());
	}
	// std::cout<<"Gen Objects!"<<std::endl;
	for (size_t i = 0; i < genObjects.size(); ++i) {
		//  std::cout<<genObjects[i]->pt()<<"   "<<genObjects[i]->eta()<<"   "<<genObjects[i]->phi()<<std::endl;
		genpts_->push_back(genObjects[i]->pt());
		genetas_->push_back(genObjects[i]->eta());
		genphis_->push_back(genObjects[i]->phi());
		int match=-1;
		double minAngle=0.5;
		for (size_t j=0; j<pts_->size(); j++){
			double deltaEta=(etas_->at(j) - genObjects[i]->eta() );
			double deltaPhi=reco::deltaPhi(phis_->at(j),genObjects[i]->phi()); 
			double dR=sqrt( deltaEta*deltaEta + deltaPhi*deltaPhi)   ;
			if(dR<minAngle) {minAngle=dR; match=j;}
		}
		genMatches_->push_back(match);
		//  std::cout<<"-----> matched to"<<match<<std::endl;
	}

	//Dummy variables for now

	std::cout<<"Fill Tree"<<std::endl;

	tree->Fill();
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(FlatEffiTree);
