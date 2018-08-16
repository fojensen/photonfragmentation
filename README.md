Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate. Remember this is defined for BTags==0!


Further information about the study may be found in AN-2016/350:

http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=SUS-16-033


This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:

https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc



*Step 1:*
To see what is going on, plot the madMinPhotonDeltaR distribution for the GJets, GJets_0p4 and QCD HT-binned MC samples:

`root madMinPhotonDeltaR.c+`

This is Figure 34 of AN-2016/350.


A fragmentation photon is one defined such that deltaR<0.4. The fragmentation fraction is defined as the the number of events with deltaR>0.4 divided by the total number of events.

The higher statistics GJets_0p4 sample has a hard cutoff <0.4! So we need to do something different. We use the sum of the lower-statistics GJets, with the hard cutoff at 0.05, and QCD samples to get the number of photons with deltaR<0.4. In order to not overcount, a _stitch point_ is defined where QCD is used for values less then the stitch point and GJets is used for values greater than the stitch points. This value is nominally set to 0.4, but values 0.05 -> 0.4 make perfect sense.

This uses the GJets and QCD MC samples to calculate the fraction. GJets_0p4 is used in the other components of the Zinv estimate, so this is independent of the 'primary' MC sample, which I guess is nice.


*Step 2:*
Make the root file containing the fragmentation histograms:

`root fragmentation.c+`

`fragmentation f`

`f.run()`


This code is able to calculate the fraction the low-deltaphi estimate as well (the lines are all there, just comment/uncomment appropriately.


*Step 3:*
Plot the fraction for all the analysis bins:

`fragPlot.c+`

This produces Figure 35 of AN-2016/350. It also produces some usual _suspects_...
