
## Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate.<br>

<br>

Further information about the study may be found in AN-2016/350:

http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=SUS-16-033

<br>

This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:

https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc

<br>

A fragmentation photon is one defined such that deltaR<0.4. The fragmentation fraction is defined as the the number of events with deltaR>0.4 divided by the total number of events. One needs to include these events in the MC as the cutoff is only artificial.

The higher statistics GJets_0p4 sample has a hard cutoff at deltaR>0.4. So we need to make an estimate of the number of fragmentation photons so we can scale this MC sample appropriately. We use the sum of the lower-statistics GJets, with the hard cutoff at 0.05, and QCD samples to get the number of photons with deltaR<0.4. In order to not overcount, a _stitch point_ is defined where QCD is used for values less then the stitch point and GJets is used for values greater than the stitch points. The stictch point value is nominally set to 0.4, but values 0.05 -> 0.4 make perfect sense and can be used as an estimation of the systematic errors.

This uses the GJets and QCD MC samples to calculate the fraction. GJets_0p4 is used in the other components of the Zinv estimate, so this is independent of the 'primary' MC sample, which is nice.

<br>

### Step 1:

To see what is going on, plot the madMinPhotonDeltaR distribution for the GJets, GJets_0p4 and QCD HT-binned MC samples:

`root madMinPhotonDeltaR.c+`

This is Figure 34 of AN-2016/350.


### Step 2:

Make the root file containing the fragmentation histograms:

`root fragmentation.c+`

`fragmentation f`

`f.run()`

This code is able to calculate the fraction the low-deltaphi estimate as well (the lines are all there, just comment/uncomment appropriately. Three files were made for three different stitch points of 0.2, 0.3, 0.4, to be used for systematic errors, comment the appropriate lines out to do each sp file in turn...


### Step 3:

Plot the fraction for all the analysis bins:

`root fragPlot.c+`

This produces Figure 35 of AN-2016/350. It also produces some _usual suspects_...


### Step 4:

For a systematic error, see how the fraction changes when using a stitch point of 0.2 or 0.3:

`root dfover1mf.c+`

If you want a simpler value to assign for the systematic error, and not bin by bin, we can look at the scatter of the central value of the differences (i.e. ignoring the statistical error seen in the plot):

`root scatter.c+`
