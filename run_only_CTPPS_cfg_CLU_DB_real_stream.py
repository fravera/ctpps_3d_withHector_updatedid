import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
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

process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring(
#'file:/eos/cms/store/t0streamer/Data/Express/000/293/910/run293910_ls0359_streamExpress_StorageManager.dat'
'file:/eos/cms/store/t0streamer/Data/Express/000/294/929/run294929_ls0001_streamExpress_StorageManager.dat'

),
    inputFileTransitionsEachEvent = cms.untracked.bool(True)
    #firstEvent = cms.untracked.uint64(10123456835)
)



from CondCore.CondDB.CondDB_cfi import *
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        CondDB.clone(
connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS')),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSPixelGainCalibrationsRcd'),
        tag = cms.string("CTPPSPixelGainCalibNew_v4_test")
    ),
      cms.PSet(
        record = cms.string('CTPPSPixelDAQMappingRcd'),
        tag = cms.string("CTPPSPixelDAQMapping_v1_test"),
        label = cms.untracked.string("RPix")
      ),
      cms.PSet(
        record = cms.string('CTPPSPixelAnalysisMaskRcd'),
        tag = cms.string("CTPPSPixelAnalysisMask_v1_test"),
        label = cms.untracked.string("RPix")
      )


)
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
#process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

############
process.o1 = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('keep *'),
        fileName = cms.untracked.string('simevent_CTPPS_CLU_DB_real.root')
        )

process.load("RecoCTPPS.PixelLocal.CTPPSPixelClusterProducer_cfi")

#process.ctppsPixelClusters = cms.EDProducer("CTPPSPixelClusterProducer",
#                                     label=cms.untracked.string("ctppsPixelDigis"),
#                                     RPixVerbosity = cms.int32(2),
#                                     SeedADCThreshold = cms.int32(10),
#                                     ADCThreshold = cms.int32(10),
#                                     ElectronADCGain = cms.double(135.0),
#                                     VCaltoElectronOffset = cms.int32(-411),
#                                     VCaltoElectronGain = cms.int32(50),
#                                     CalibrationFile = cms.string("file:/eos/cms/store/group/dpg_ctpps/comm_ctpps/Gain_Fed_1462-1463_Run_107.root"),
#                                     DAQCalibration = cms.bool(True),
#                                     doSingleCalibration = cms.bool(False)
#)


process.mixedigi_step = cms.Path(process.ctppsPixelDigis*process.ctppsPixelClusters
#*process.rechitProd
)

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.mixedigi_step,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
  #  getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq
    getattr(process,path)._seq =  getattr(process,path)._seq


