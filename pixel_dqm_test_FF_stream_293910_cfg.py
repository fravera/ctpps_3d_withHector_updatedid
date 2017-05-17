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
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0310_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0311_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0312_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0313_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0314_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0315_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0316_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0317_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0318_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0319_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0320_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0321_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0322_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0323_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0324_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0325_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0326_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0327_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0328_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0329_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0330_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0331_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0332_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0333_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0334_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0335_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0336_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0337_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0338_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0339_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0340_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0341_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0342_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0343_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0344_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0345_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0346_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0347_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0348_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0349_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0350_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0351_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0352_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0353_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0354_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0355_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0356_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0357_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0358_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0359_streamExpress_StorageManager.dat'



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
