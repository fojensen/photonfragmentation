# photonfragmentation

Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate.

Further information about the study may be found in AN-2016/350:

http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=SUS-16-033


This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:

https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc


Step 1:
Plot the madMinPhotonDeltaR distribution for the GJets, GJets_0p4 and QCD HT-binned MC samples:

`root madMinPhotonDeltaR.c+`

This is Figure 34 of AN-2016/350.

Step 2:
