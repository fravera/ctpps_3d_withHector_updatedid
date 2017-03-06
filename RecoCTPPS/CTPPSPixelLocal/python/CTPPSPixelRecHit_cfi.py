import FWCore.ParameterSet.Config as cms

rechitProd = cms.EDProducer("CTPPSPixelRecHitProducer",
                                     label=cms.untracked.string("clusterProd"),
                                     RPixVerbosity = cms.int32(0),
                            RPixActiveEdgeSmearing = cms.double(0.020),
                            RPixActiveEdgePosition = cms.double(0.150)

)
