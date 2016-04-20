#include "L1Trigger/LauraTriggerTools/interface/helpers.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include <cmath>
#include <math.h>

int deltaPhiWrapAtN(unsigned int N, int phi1, int phi2) {
  int difference = phi1 - phi2;
  if (std::abs(phi1 - phi2) == N-1) {
    difference = -difference/std::abs(difference);
  }
  return difference;
}

int deltaGctPhi(const L1CaloRegion& r1, const L1CaloRegion& r2) {
  return deltaPhiWrapAtN(18, r1.gctPhi(), r2.gctPhi());
}

int deltaTPGPhi(int tpgPhi1, int tpgPhi2) {
  return deltaPhiWrapAtN(72, tpgPhi1, tpgPhi2);
}


double convertRegionPhi(int iPhi) {
  if (iPhi < 10)
    return 2. * M_PI * iPhi / 18.;
  if (iPhi < 18)
    return -M_PI + 2. * M_PI * (iPhi - 9) / 18.;
  return -9;
}

double convertTPGPhi(int iPhi) {
  if (iPhi < 37)
    return 2. * M_PI * iPhi / 72.;
  if (iPhi < 72)
    return -M_PI + 2. * M_PI * (iPhi - 36) / 72.;
  return -999;
}

int convertGenPhi(double genPhi) {
        //FIXME
	if (genPhi<0)
		return (72*( (genPhi/(2.*M_PI))+ (1/2.)) +36) ;
	if (genPhi>0)
		return ((72*genPhi/(2.*M_PI))+1);
        return -999;
}



double convertHFTPGEta( int tpgEta){
	const double hfEtaValues[13] = {
		2.964, //Boundary for 29/30 - we skip eta 29
		3.139,
		3.314,
		3.489,
		3.664,
		3.839, 
		4.013,
		4.191, 
		4.363,
		4.538,
		4.716,
		4.889,
		5.191 //End of HF
	};
	if (tpgEta<0){
		int ieta=abs(tpgEta+30);
		return -(((hfEtaValues[ieta])+(hfEtaValues[ieta+1]))/2.0);
	}
	else{
		int ieta=(tpgEta-30);
		return (((hfEtaValues[ieta])+(hfEtaValues[ieta+1]))/2.0);
	} 
}

int convertTPGGenEta(double genEta) {
	const double EtaValues[28] = {
		0.087,
		0.174,
		0.261,
		0.348,
		0.435,
		0.522,
		0.609,
		0.696,
		0.783,
		0.87,
		0.957,
		1.044,
		1.131,
		1.218,
		1.305,
		1.392,
		1.479,
		1.566,
		1.653,
		1.74,
		1.83,
		1.93,
		2.043,
		2.172,
		2.322,
		2.5,
		2.65,
		2.868  //only to eta 28
	};
	if (genEta > 0){ 
		for (int n=0; n<28; n++){
			if (genEta<EtaValues[n]) {
				int Eta = 28 + n; //FIXME
				return Eta;
				break;
			}
		}
	}
	else if (genEta<0){
		for (int n=0; n<28; n++){
			if  (std::abs(genEta) < EtaValues[n]){
				int Eta = 27-n; //FIXME
				return Eta;
				break;
			}
		}
	}
	return -999;
}


int convertHFGenEta(double genEta) {
	const double hfEtaValues[13] = {
		2.964, //Boundary for 29/30 - we skip eta 29
		3.139,
		3.314,
		3.489,
		3.664,
		3.839,
		4.013,
		4.191,
		4.363,
		4.538,
		4.716,
		4.889,
		5.191 //End of HF
	};
	if (genEta > 0){ 
		for (int n=0; n<13; n++){
			if (genEta<hfEtaValues[n]) {
				int hfEta = 29 + n; //FIXME
				return hfEta;
				break;
			}
		}
	}
	else if (genEta<0){
		for (int n=0; n<13; n++){
			if  (std::abs(genEta) < hfEtaValues[n]){
				int hfEta = -29-n; //FIXME
				return hfEta;
				break;
			}
		}
	}
	return -999;
}


int convertGenEta(double genEta) {
	const double rgnEtaValues[11] = {
		0.174, // HB and inner HE bins are 0.348 wide
		0.522,
		0.870,
		1.218,
		1.566,
		1.956, // Last two HE bins are 0.432 and 0.828 wide
		2.586,
		3.250, // HF bins are 0.5 wide
		3.750,
		4.250,
		4.750
	};
	if (genEta > 0){ 
		for (int n=0; n<11; n++){
			if (genEta<rgnEtaValues[n]) {
				int rgnEta = 11 + n;
				return rgnEta;
				break;
			}
		}
	}
	else if (genEta<0){
		for (int n=0; n<11; n++){
			if  (std::abs(genEta) < rgnEtaValues[n]){
				int rgnEta = -n+10;
				return rgnEta;
				break;
			}
		}
	}
	return -9;
}


double convertRegionEta(int iEta) {
	const double rgnEtaValues[11] = {
		0.174, // HB and inner HE bins are 0.348 wide
		0.522,
		0.870,
		1.218,
		1.566,
		1.956, // Last two HE bins are 0.432 and 0.828 wide
		2.586,
		3.250, // HF bins are 0.5 wide
		3.750,
		4.250,
		4.750
	};
	if(iEta < 11) {
		return -rgnEtaValues[-(iEta - 10)]; // 0-10 are negative eta values
	}
	else if (iEta < 22) {
		return rgnEtaValues[iEta - 11];     // 11-21 are positive eta values
	}
	return -9;
}

double convertTPGEta(int iEta) {
	// 0-27 are negative eta values, 28-55 are positive eta values

	int correctedIndex = iEta < 28 ? -(iEta - 27) : iEta - 28;

	double etaValue = -9;
	if (correctedIndex < 20) {
		etaValue = 0.0435 + correctedIndex * 0.087;
	} else {
		const double endcapEtaValues[8] = {  //not boundary values!!! 
			1.785,
			1.880,
			1.9865,
			2.1075,
			2.247, //or 2.252??? HArdcoded table sent by Jeremy has different eta bound than online response 
			2.411, //or 2.416??? Hardcoded table sent by Jeremy has different eta bound than online response 
			2.575,
			2.825
		};
		etaValue = endcapEtaValues[correctedIndex-20];
	}
	if (iEta < 28)
		return -etaValue;
	else
		return etaValue;
}

double getRegionArea(int gctEta) {
	switch (gctEta) {
		case 0: return 0.5*0.348;
		case 1: return 0.5*0.348;
		case 2: return 0.5*0.348;
		case 3: return 0.5*0.348;
		case 4: return 0.828*0.348;
		case 5: return 0.432*0.348;
		case 6: return 0.348*0.348;
		case 7: return 0.348*0.348;
		case 8: return 0.348*0.348;
		case 9: return 0.348*0.348;
		case 10: return 0.348*0.348;
		case 21: return 0.5*0.348;
		case 20: return 0.5*0.348;
		case 19: return 0.5*0.348;
		case 18: return 0.5*0.348;
		case 17: return 0.828*0.348;
		case 16: return 0.432*0.348;
		case 15: return 0.348*0.348;
		case 14: return 0.348*0.348;
		case 13: return 0.348*0.348;
		case 12: return 0.348*0.348;
		case 11: return 0.348*0.348;
	}
	return 0;
}

int twrPhi2RegionPhi(int iPhi) {
	unsigned int rgnIdx = (iPhi + 2)/4;
	// 70 and 71 are actually in GCT phi 0
	if (rgnIdx == 18)
		return 0;
	return rgnIdx;
}

int twrEta2RegionEta(int iEta) {
	// 4 towers per region.  Non-HF regions start at ieta=4 in RCT land.
	unsigned int rgnIdx = (iEta / 4) + 4;
	return rgnIdx;
}


double getPhiTPG(int iPhi) {
	// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
	return convertTPGPhi(iPhi-1);
}

int TPGEtaRange(int ieta){
	unsigned int iEta = 0;
	// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
	// And we have mapped [-28, -1], [1, 28] onto [0, 55]   
	if(ieta < 0)
		iEta = ieta + 28;
	else if(ieta > 0)
		iEta = ieta + 27;
	return iEta;
}

int HCALTPGEtaRange(int ieta){
	unsigned int iEta = 0;
	// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
	// And we have mapped [-28, -1], [1, 28] onto [0, 55]   
	if(ieta < 0)
		iEta = ieta + 28;
	else if(ieta > 0)
		iEta = ieta + 27;
	return iEta;
}


double getEtaTPG(int ieta) {
	unsigned int iEta = 0;
	// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
	// And we have mapped [-28, -1], [1, 28] onto [0, 55]

	if(ieta < 0)
		iEta = ieta + 28;
	else if(ieta > 0)
		iEta = ieta + 27;
	return convertTPGEta(iEta);
}



