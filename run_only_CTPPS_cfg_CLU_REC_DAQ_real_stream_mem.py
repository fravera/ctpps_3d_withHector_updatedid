import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("CondFormats.CTPPSReadoutObjects.CTPPSPixelDAQMappingESSourceXML_cfi")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(2)
        )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('clu_info'),
    clu_info = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
)
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/44589413-F73F-E711-9E8D-02163E014337.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/4493383A-F63F-E711-A67F-02163E011870.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/5C6E42A5-0640-E711-9C3E-02163E011B62.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/98F7DD15-F93F-E711-87E1-02163E019D62.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/A01D8813-0A40-E711-A300-02163E01A211.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/B83C33DB-F63F-E711-82FC-02163E019DF3.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/736/00000/D6706DD8-F93F-E711-9F6D-02163E019D7F.root',
#'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias2/AOD/PromptReco-v1/000/294/741/00000/0A6D50A6-6340-E711-B74E-02163E019E3C.root'
'file:/eos/cms/tier0/store/data/Run2017A/ZeroBias9/AOD/PromptReco-v1/000/295/000/00000/24691370-1145-E711-BAF1-02163E0142A4.root'
),
duplicateCheckMode = cms.untracked.string("checkEachFile")
)


#from CondCore.CondDB.CondDB_cfi import *
#process.load("CondCore.CondDB.CondDB_cfi")
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#        CondDB.clone(
#connect = cms.string('sqlite_file:CondTools/CTPPS/test/CTPPSPixel_DAQMapping_AnalysisMask.db')),
#   toGet = cms.VPSet(
#      cms.PSet(
#        record = cms.string('CTPPSPixelDAQMappingRcd'),
#        tag = cms.string("PixelDAQMapping"),
#        label = cms.untracked.string("RPix")
#      ),
#      cms.PSet(
#        record = cms.string('CTPPSPixelAnalysisMaskRcd'),
#        tag = cms.string("PixelAnalysisMask"),
#        label = cms.untracked.string("RPix")
#      )
#)
#)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
#process.load('Configuration.Geometry.geometry_CTPPS_alaTotem_cfi')
process.load('Configuration.Geometry.geometry_CTPPS_cfi')
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

############
process.o1 = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('drop *',
                                               'keep CTPPSPixelClusteredmDetSetVector_clusterProd_*_*',
                                               'keep CTPPSPixelRecHitedmDetSetVector_rechitProd_*_*',
),
        fileName = cms.untracked.string('simevent_CTPPS_CLU_REC_DAQ_real_mem_295000.root')
        )


#process.load("RecoCTPPS.CTPPSPixelLocal.CTPPSPixelClusterizer_cfi")

process.clusterProd = cms.EDProducer("CTPPSPixelClusterProducer",
                                     label=cms.untracked.string("ctppsPixelDigis"),
                                     RPixVerbosity = cms.int32(0),
                                     SeedADCThreshold = cms.int32(10),
                                     ADCThreshold = cms.int32(10),
                                     ElectronADCGain = cms.double(135.0),
                                     VCaltoElectronOffset = cms.int32(-411),
                                     VCaltoElectronGain = cms.int32(50),
                                     CalibrationFile = cms.string("Gain_Fed_1462-1463_Run_107.root"),
                                     DAQCalibration = cms.bool(True),
                                     doSingleCalibration = cms.bool(False)
)
process.load("RecoCTPPS.CTPPSPixelLocal.CTPPSPixelRecHit_cfi")

process.mixedigi_step = cms.Path(process.clusterProd
*process.rechitProd
)

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.mixedigi_step,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
  #  getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq
    getattr(process,path)._seq =  getattr(process,path)._seq


