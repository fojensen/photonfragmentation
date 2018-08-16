
## Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate.<br>


Further information about the study may be found in AN-2016/350:

http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=SUS-16-033

<br>

This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:

https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc


### Step 1:

To see what is going on, plot the madMinPhotonDeltaR distribution for the GJets, GJets_0p4 and QCD HT-binned MC samples:

`root madMinPhotonDeltaR.c+`

This is Figure 34 of AN-2016/350.


### Step 2:

Make the root file containing the fragmentation histograms:

`root fragmentation.c+`

`fragmentation f`

`f.run()`

This code is able to calculate the fraction the low-deltaphi estimate as well (the lines are all there, just comment/uncomment appropriately.


### Step 3:

Plot the fraction for all the analysis bins:

`fragPlot.c+`

This produces Figure 35 of AN-2016/350. It also produces some _usual suspects_...

### Step 4:
