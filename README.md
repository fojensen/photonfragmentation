# photonfragmentation
Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate.
This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:
https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc

Step 1:
Plot the madMinPhotonDeltaR distribution for the GJets, GJets_0p4 and QCD HT-binned MC samples:
root madMinPhotonDeltaR.c+

Step 2:
