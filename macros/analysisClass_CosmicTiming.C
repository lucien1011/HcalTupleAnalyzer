
#include "analysisClass.h"
#include "HcalTupleTree.h"
#include "HLTFilterObjectCollectionHelper.h"

#include "HLTFilterObject.h"
#include "HBHECosmicDigi.h"
#include "HOCosmicDigi.h"
#include "CosmicMuon.h"

void analysisClass::loop(){
  
  //--------------------------------------------------------------------------------
  // Declare HCAL tree(s)
  //--------------------------------------------------------------------------------
  
  HcalTupleTree * tuple_tree = getTree<HcalTupleTree>("tuple_tree");
  int n_events = tuple_tree -> fChain -> GetEntries();

  //--------------------------------------------------------------------------------
  // Constant
  //--------------------------------------------------------------------------------
  const double energyCut = 2.;

  //--------------------------------------------------------------------------------
  // Turn on/off branches
  //--------------------------------------------------------------------------------
  
  tuple_tree -> fChain -> SetBranchStatus("*"                                , kFALSE);
  							                     	     
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiIEta"               , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiIPhi"               , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiEta"               , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiPhi"               , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiDepth"              , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiRecEnergy"          , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiRecTime"            , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiFC"                 , kTRUE );
  tuple_tree -> fChain -> SetBranchStatus("HBHECosmicDigiSize"               , kTRUE );
							                     	     
  //--------------------------------------------------------------------------------
  // Make histograms
  //--------------------------------------------------------------------------------
  char histName[100];
  char title[100];
  TH2F * h_iphi_vs_recTime = makeTH2F("h_iphi_vs_recTime",72,0.5,72.5,150,-49.5,100.5);
  
  //--------------------------------------------------------------------------------
  // Loop
  //--------------------------------------------------------------------------------
  
  for (int i = 0; i < n_events; ++i){
    
    tuple_tree -> GetEntry(i);
    if ( (i + 1) % 100 == 0 ) 
      std::cout << "Processing event " << i + 1 << "/" << n_events << std::endl;
 
    //-----------------------------------------------------------------
    // Fill HBHE histograms
    //-----------------------------------------------------------------
      
    CollectionPtr hbheDigis (new Collection(*tuple_tree, tuple_tree -> HBHECosmicDigiIEta -> size()));

    int nHBHE = hbheDigis -> GetSize();

    for (int iHBHE = 0; iHBHE < nHBHE; ++iHBHE){
      HBHECosmicDigi hbheDigi = hbheDigis -> GetConstituent<HBHECosmicDigi>(iHBHE);

      if ( hbheDigi.energy() < energyCut) continue;
      
      h_iphi_vs_recTime -> Fill( hbheDigi.iphi() , hbheDigi.recHitTime() );
      
    };
  };
};


      
