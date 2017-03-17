import FWCore.ParameterSet.Config as cms

process = cms.Process('test')


process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# load a mapping
process.load("CondFormats.CTPPSReadoutObjects.CTPPSPixelDAQMappingESSourceXML_cfi")
process.ctppsPixelDAQMappingESSourceXML.mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/rpix_mapping_220_far.xml")
process.ctppsPixelDAQMappingESSourceXML.maskFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/rpix_channel_mask_220_far.xml")


#Database output service
process.load("CondCore.CondDB.CondDB_cfi")
# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:CTPPSPixel_DAQMapping_AnalysisMask_220_far.db'


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
    cms.PSet(
        record = cms.string('CTPPSPixelDAQMappingRcd'),
        tag = cms.string('PixelDAQMapping')
    ),
    cms.PSet(
        record = cms.string('CTPPSPixelAnalysisMaskRcd'),
        tag = cms.string('PixelAnalysisMask')
    )
  )
)


# print the mapping
process.writeCTPPSPixelDAQMapping = cms.EDAnalyzer("WriteCTPPSPixelDAQMapping"
)

process.path = cms.Path(
  process.writeCTPPSPixelDAQMapping
)
