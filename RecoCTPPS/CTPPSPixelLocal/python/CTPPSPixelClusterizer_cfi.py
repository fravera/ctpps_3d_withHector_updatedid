import FWCore.ParameterSet.Config as cms

clusterProd = cms.EDProducer("CTPPSPixelClusterProducer",
                                     label=cms.untracked.string("RPixDetDigitizer"),
                                     RPixVerbosity = cms.int32(0),
                                     SeedADCThreshold = cms.int32(10),
                                     ADCThreshold = cms.int32(10),
                                     ElectronADCGain =cms.double(135.0)
)
