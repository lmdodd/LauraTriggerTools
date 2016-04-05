/*
 * =====================================================================================
 *
 *       Filename:  Helpers.h
 *
 *    Description:  Common UCT functions.
 *
 *         Author:  M. Cepeda, S. Dasu, E. Friis
 *        Company:  UW Madison
 *
 * =====================================================================================
 */

#ifndef HELPERS_W9QK6HND
#define HELPERS_W9QK6HND

class L1CaloRegion;

// Compute the difference in phi between two towers, wrapping at phi = N
int deltaPhiWrapAtN(unsigned int N, int phi1, int phi2);

int deltaGctPhi(const L1CaloRegion& r1, const L1CaloRegion& r2);

// Calo detector mapping.
//
// See: https://twiki.cern.ch/twiki/bin/view/CMS/RCTMap

// Get the physical phi for a given TPG index.
double convertTPGPhi(int iPhi);

// Get the physical eta for a given TPG index
double convertTPGEta(int iEta);

// Convert a region index into physical phi (at center of region)
double convertRegionPhi(int iPhi);

// Convert a region index into physical eta (at center of region)
double convertRegionEta(int iEta);

// Convert a physical eta into a gct region index (L. Dodd)
int convertGenEta(double genEta);

// Convert a physical eta into an HF index (L. Dodd)
int convertHFGenEta(double genEta);

// Convert a physical phi into a trigger phi (L. Dodd)
int convertGenPhi(double genPhi);

// Convert a physical eta into an HF index (L. Dodd)
double convertHFTPGEta( int tpgEta);

// Get the effective area of a region in a given eta slice.
double getRegionArea(int gctEta);

// Find GCT index of a given tower.
int twrPhi2RegionPhi(int iPhi);
int twrEta2RegionEta(int iEta);

// TPG iPhi starts at 1 and goes to 72.  Let's index starting at zero.
double getPhiTPG(int iPhi);

// So here, -28 becomes 0.  -1 be comes 27.  +1 becomes 28. +28 becomes 55.
// And we have mapped [-28, -1], [1, 28] onto [0, 55]   

int TPGEtaRange(int ieta);

double getEtaTPG(int ieta); 

#endif /* end of include guard: HELPERS_W9QK6HND */
