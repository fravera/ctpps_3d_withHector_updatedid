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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

#process.source = cms.Source("PoolSource",
#labelRawDataLikeMC = cms.untracked.bool(False),
#fileNames =  cms.untracked.vstring(
#'file:./run292996_FF.root'
# ),
#duplicateCheckMode = cms.untracked.string("checkEachFile")
#)

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0310_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0311_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0312_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0313_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0314_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0315_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0316_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0317_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0318_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0319_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0320_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0321_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0322_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0323_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0324_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0325_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0326_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0327_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0328_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0329_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0330_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0331_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0332_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0333_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0334_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0335_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0336_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0337_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0338_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0339_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0340_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0341_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0342_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0343_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0344_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0345_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0346_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0347_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0348_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0349_streamExpress_StorageManager.dat',

'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0350_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0351_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0352_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0353_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0354_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0355_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0356_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0357_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0358_streamExpress_StorageManager.dat',
'file:/eos/cms/store/t0streamer/Data/Express/000/293/765/run293765_ls0359_streamExpress_StorageManager.dat'


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
    r2dFF = cms.untracked.PSet( threshold = cms.untracked.string('ERROR'))
)

process.out = cms.OutputModule("PoolOutputModule",
#    fileName =  cms.untracked.string('file:digis_PixelAlive_1462_2_RAW.root'),
    fileName =  cms.untracked.string('file:digis_run293765_RAW_FF_stream.root'),

    outputCommands = cms.untracked.vstring("keep *", "drop FEDRawDataCollection_*__*")
)

process.p = cms.Path(process.ctppsPixelDigis)
process.ep = cms.EndPath(process.out)
