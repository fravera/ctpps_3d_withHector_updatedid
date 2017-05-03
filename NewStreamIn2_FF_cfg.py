import FWCore.ParameterSet.Config as cms

process = cms.Process("TRANSFER")

import FWCore.Framework.test.cmsExceptionsFatal_cff
process.options = FWCore.Framework.test.cmsExceptionsFatal_cff.options

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0001_streamA_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0002_streamA_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0003_streamA_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0004_streamA_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0005_streamA_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0006_streamA_StorageManager.dat'

),
    inputFileTransitionsEachEvent = cms.untracked.bool(True)
    #firstEvent = cms.untracked.uint64(10123456835)
)

process.a1 = cms.EDAnalyzer("StreamThingAnalyzer",
    product_to_get = cms.string('m1')
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('run292996_FF.root')
)

process.end = cms.EndPath(process.a1*process.out)
