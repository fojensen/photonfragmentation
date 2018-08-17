
## Code to calculate the photon fragmentation fraction for the RA2/b Zinv estimate.<br>

<br>

Further information about the study may be found in AN-2016/350:

http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=SUS-16-033

<br>

This makes use of the madMinPhotonDeltaR variable, defined in the TreeMaker module here:

https://github.com/TreeMaker/TreeMaker/blob/Run2_2017/Utils/src/MinDeltaRDouble.cc

<br>

A fragmentation photon is one defined such that dr<0.4. The fragmentation fraction is defined as the the number of photons with dr>0.4 divided by the total number of photons. One needs to include these events in the MC as the cutoff is only artificial and does not match data.

The higher statistics GJets_0p4 sample has a hard cutoff requiring dr>0.4. So we need to make an estimate of the number of fragmentation photons to scale GJets_0p4 appropriately as this sample is used in the Zinv estimation. We use the sum of the lower-statistics GJets, with the hard cutoff requiring dr>0.05, and QCD samples to get the number of photons with dr<0.4. In order to not overcount, a _stitch point_ is defined where QCD is used for values less then the stitch point and GJets is used for values greater than the stitch point (but always less than 0.4). The stitch point value is nominally set to 0.4, but values 0.05 -> 0.4 make perfect sense and can be used for an estimation of the systematic errors associated with this choice.

<br>

### Step 1:

To see what is going on with these distributions, plot the madMinPhotonDeltaR for the GJets, GJets_0p4 and QCD HT-binned MC samples:

`root madMinPhotonDeltaR.c+`

This is Figure 34 of AN-2016/350.


### Step 2:

Make the root file containing the fragmentation histograms:

`root fragmentation.c+`

`fragmentation f`

`f.run()`

Three files need to made for stitch points of 0.2, 0.3, 0.4, to be used for systematic errors (comment/uncomment appropriately).
This code is able to calculate the fraction the low-deltaphi estimate as well (comment/uncomment appropriately).

### Step 3:

Plot the fraction for all the analysis bins:

`root fragPlot.c+`

This produces Figure 35 of AN-2016/350. It also produces some _usual suspects_...


### Step 4:

For a systematic error, see how the fraction changes when using a stitch point of 0.2 or 0.3:

`root dfover1mf.c+`

If you want a simpler value to assign for the systematic error, and not bin by bin, we can look at the scatter of the central value of the differences (i.e. ignoring the statistical error seen in the plot):

`root scatter.c+`
