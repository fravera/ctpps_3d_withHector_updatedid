import FWCore.ParameterSet.Config as cms

process = cms.Process("FF")

# Specify the maximum events to simulate
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)



process.source = cms.Source("PoolSource",
duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
     fileNames =
 cms.untracked.vstring(
     'file:./simevent_CTPPS_DIG_CLU_REC_5000_rid.root'
  )
)


process.recoAnal = cms.EDAnalyzer("CTPPSPixelRecoAnalyzer",
      label=cms.untracked.string("rechitProd"),
     Verbosity = cms.int32(3),
                                  RPixVerbosity = cms.int32(0),
   RPixActiveEdgeSmearing = cms.double(0.020),
    RPixActiveEdgePosition = cms.double(0.150)
)
########

process.p1 = cms.Path(

process.recoAnal


)


