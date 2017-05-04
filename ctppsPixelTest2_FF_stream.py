import FWCore.ParameterSet.Config as cms

process = cms.Process("myCTPPSRawToDigi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("CondFormats.CTPPSReadoutObjects.CTPPSPixelDAQMappingESSourceXML_cfi")
process.ctppsPixelDAQMappingESSourceXML.configuration = cms.VPSet(
    # example configuration block:
    cms.PSet(
        validityRange = cms.EventRange("1:min - 999999999:max"),
        mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/rpix_mapping_220_far.xml"),
        maskFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/rpix_channel_mask_220_far.xml")
        )
    )

process.load("Configuration.StandardSequences.Services_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500))

#process.source = cms.Source("PoolSource",
#labelRawDataLikeMC = cms.untracked.bool(False),
#fileNames =  cms.untracked.vstring(
#'file:./run292996_FF.root'
# ),
#duplicateCheckMode = cms.untracked.string("checkEachFile")
#)

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0001_streamA_StorageManager.dat',
#'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0002_streamA_StorageManager.dat',
#'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0003_streamA_StorageManager.dat',
#'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0004_streamA_StorageManager.dat',
#'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0005_streamA_StorageManager.dat',
#'file:/eos/cms/store/t0streamer/Minidaq/A/000/292/996/run292996_ls0006_streamA_StorageManager.dat'

),
    inputFileTransitionsEachEvent = cms.untracked.bool(True)
    #firstEvent = cms.untracked.uint64(10123456835)
)



process.load("EventFilter.CTPPSRawToDigi.ctppsPixelRawToDigi_cfi")

#process.ctppsPixelDigis.InputLabel = 'source'
process.ctppsPixelDigis.InputLabel = 'rawDataCollector'

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('ctppsPixelDigis'),
    destinations = cms.untracked.vstring('r2dFF'),
    r2dFF = cms.untracked.PSet( threshold = cms.untracked.string('DEBUG'))
)

process.out = cms.OutputModule("PoolOutputModule",
#    fileName =  cms.untracked.string('file:digis_PixelAlive_1462_2_RAW.root'),
    fileName =  cms.untracked.string('file:digis_run292996_RAW_FF_stream.root'),

    outputCommands = cms.untracked.vstring("keep *")
)

process.p = cms.Path(process.ctppsPixelDigis)
process.ep = cms.EndPath(process.out)
