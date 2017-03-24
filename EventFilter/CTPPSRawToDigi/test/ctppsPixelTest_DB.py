import FWCore.ParameterSet.Config as cms

process = cms.Process("myCTPPSRawToDigi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)
#Database output service

process.load("CondCore.CondDB.CondDB_cfi")
# input database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:../../../CalibForward/CTPPSPixelCalibration/test/CTPPSPixel_DAQMapping_AnalysisMask_TEST.db'

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('CTPPSPixelDAQMappingRcd'),
        tag = cms.string("PixelDAQMapping"),
        label = cms.untracked.string("RPix")
      ),
      cms.PSet(
        record = cms.string('CTPPSPixelAnalysisMaskRcd'),
        tag = cms.string("PixelAnalysisMask"),
        label = cms.untracked.string("RPix")
      )
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
# fileNames =  cms.untracked.vstring('file:rawdata.root')
labelRawDataLikeMC = cms.untracked.bool(False),
fileNames =  cms.untracked.vstring(
#"file:./PixelAlive_1294_153_RAW_v3.root"
"file:/afs/cern.ch/work/k/kas/public/PXtrees/PixelAlive_1294_151_RAW_v2.root"

 )
)

process.load("EventFilter.CTPPSRawToDigi.CTPPSPixelRawToDigi_cfi")

process.ctppsPixelDigis.InputLabel = 'source'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('ctppsPixelDigis'),
    destinations = cms.untracked.vstring('r2d'),
    r2d = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG'))
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName =  cms.untracked.string('file:digis_PixelAlive_1294_151_RAW_v2_DB.root'),
    outputCommands = cms.untracked.vstring("keep *")
)

process.p = cms.Path(process.ctppsPixelDigis)
process.ep = cms.EndPath(process.out)
