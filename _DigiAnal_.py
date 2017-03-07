import FWCore.ParameterSet.Config as cms

process = cms.Process("TestFlatGun")

# Specify the maximum events to simulate
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2)
)



process.source = cms.Source("PoolSource",
duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
     fileNames =
 cms.untracked.vstring(
     'file:./simevent_CTPPS_DIG_CLU_REC_2.root'
  )
)



process.digiAnal = cms.EDAnalyzer("CTPPSPixelDigiAnalyzer",
      label=cms.untracked.string("RPixDetDigitizer"),
     Verbosity = cms.int32(3),
   RPixVerbosity = cms.int32(0),
   RPixActiveEdgeSmearing = cms.double(0.020),
    RPixActiveEdgePosition = cms.double(0.150)
)
########

process.p1 = cms.Path(

process.digiAnal


)


