import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSReconstruction", eras.ctpps_2016)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# raw data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/0008EE9C-2964-E711-AE47-02163E019D98.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/1639787C-2D64-E711-89FE-02163E0140D5.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/2A501A5A-2D64-E711-8B27-02163E019D8B.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/2AC93208-2A64-E711-9A27-02163E019B9B.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/347BA704-2A64-E711-B315-02163E019CD7.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/3602647F-2D64-E711-8E01-02163E013676.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/4A3B8E53-2D64-E711-9EFB-02163E011E6F.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/6C7B8F15-3064-E711-84AC-02163E013867.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/7A936FD5-2D64-E711-81BB-02163E013480.root",
#		"/store/data/Run2017B/ZeroBias1/RAW/v1/000/298/596/00000/9C115C01-2A64-E711-901C-02163E01A3FA.root"
  ),

  inputCommands = cms.untracked.vstring(
    'drop *',
    'keep FEDRawDataCollection_*_*_*'
  )
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# geometry
process.load("Configuration.Geometry.geometry_CTPPS_alaTotem_cfi")


# strips: clusterization
process.load("RecoCTPPS.TotemRPLocal.totemRPClusterProducer_cfi")

# strips: reco hit production
process.load("RecoCTPPS.TotemRPLocal.totemRPRecHitProducer_cfi")

# strips: non-parallel pattern recognition
process.load("RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi")

# strips: local track fitting
process.load("RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi")


# diamonds: reco hit production
process.load("RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi")

# diamonds: local track fitting
process.load("RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi")


# pixels: clusterisation
process.clusterProd = cms.EDProducer("CTPPSPixelClusterProducer",
  label=cms.untracked.string("ctppsPixelDigis"),
  RPixVerbosity = cms.int32(0),
  SeedADCThreshold = cms.int32(10),
  ADCThreshold = cms.int32(10),
  ElectronADCGain = cms.double(135.0),
  VCaltoElectronOffset = cms.int32(-411),
  VCaltoElectronGain = cms.int32(50),
  CalibrationFile = cms.string("/afs/cern.ch/work/j/jkaspar/software/ctpps/development/pixel_from_Enrico/CMSSW_9_2_0/src/Gain_Fed_1462-1463_Run_107.root"),
  DAQCalibration = cms.bool(True),
  doSingleCalibration = cms.bool(False)
)

# pixels: rechit producer
process.load("RecoCTPPS.CTPPSPixelLocal.CTPPSPixelRecHit_cfi")
process.rechitProd.RPixVerbosity = cms.int32(0)

process.p = cms.Path(
  process.ctppsRawToDigi

  * process.totemRPClusterProducer
  * process.totemRPRecHitProducer
  * process.totemRPUVPatternFinder
  * process.totemRPLocalTrackFitter

  * process.ctppsDiamondRecHits
  * process.ctppsDiamondLocalTracks

  * process.clusterProd
  * process.rechitProd
)

# output configuration
process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string("output.root")
)

process.outpath = cms.EndPath(process.output)
