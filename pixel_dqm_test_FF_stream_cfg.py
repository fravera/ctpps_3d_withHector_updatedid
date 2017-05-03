import FWCore.ParameterSet.Config as cms

#process = cms.Process('RECODQM')
process = cms.Process('CTPPSDQM')

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

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


# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

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


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# CTPPS DQM modules
#process.load("DQM.CTPPS.totemDQM_cff")
process.load("DQM.CTPPS.ctppsDQM_cff")

process.options = cms.untracked.PSet(
#    Rethrow = cms.untracked.vstring('ProductNotFound',
    SkipEvent = cms.untracked.vstring('ProductNotFound',
        'TooManyProducts',
        'TooFewProducts')
)

process.path = cms.Path(
  process.ctppsPixelDigis *
#  process.recoCTPPS +
  process.ctppsDQM
)

process.end_path = cms.EndPath(
  process.dqmEnv +
  process.dqmSaver
)

process.schedule = cms.Schedule(
  process.path,
  process.end_path
)
