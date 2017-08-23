
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load("CondFormats.CTPPSReadoutObjects.CTPPSPixelDAQMappingESSourceXML_cfi")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(10000)
        )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet( threshold = cms.untracked.string('ERROR'))
)
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )


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


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_relval', '')

process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

#process.patternProd = cms.EDProducer("patternProducer",
 #                                    label=cms.untracked.string("ctppsPixelRecHits"),
  #                                   RPixVerbosity = cms.untracked.int32(0)

#)
process.load("RecoCTPPS.PixelLocal.patternProd_cfi")
process.patternProd.RPixVerbosity = cms.untracked.int32(0)

############
process.o1 = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('keep *',
                                               'keep CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters_*_*',
                                               'keep CTPPSPixelRecHitedmDetSetVector_ctppsPixelRecHits_*_*',
),
        fileName = cms.untracked.string('pro.root')
        )


process.mixedigi_step = cms.Path(
 process.ctppsRawToDigi *
process.recoCTPPS
*process.patternProd
)

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.mixedigi_step,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
  #  getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq
    getattr(process,path)._seq =  getattr(process,path)._seq


