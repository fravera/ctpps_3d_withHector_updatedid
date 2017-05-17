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
        input = cms.untracked.int32(100)
        )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('clu_info'),
    clu_info = cms.untracked.PSet( threshold = cms.untracked.string('INFO'))
)

process.source = cms.Source("PoolSource",

labelRawDataLikeMC = cms.untracked.bool(False),
fileNames =  cms.untracked.vstring(
#"file:./PixelAlive_1294_153_RAW_v3.root"
#'file:../../../CMSSW_9_1_0_pre1_test220/CMSSW_9_1_0_pre1/src/simevent_CTPPS_DIG_CLU_REC_5000_rid.root'
'file:/afs/cern.ch/user/f/fabferro/public/digis_run293910_RAW_FF_stream.root'
 ),
duplicateCheckMode = cms.untracked.string("checkEachFile")
)


from CondCore.CondDB.CondDB_cfi import *
process.CTPPSPixelGainCalibrationRcd = cms.ESSource("PoolDBESSource",
        CondDB.clone(
connect = cms.string('sqlite_file:ctppspixnew1.db')),

    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSPixelGainCalibrationsRcd'),
        tag = cms.string("CTPPSPixelGainCalibNew_v1")
    )

)
)
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
        CondDB.clone(
connect = cms.string('sqlite_file:CondTools/CTPPS/test/CTPPSPixel_DAQMapping_AnalysisMask_220_far.db')),
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


############
process.o1 = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('keep *'),
        fileName = cms.untracked.string('simevent_CTPPS_CLU_real.root')
        )

#process.load("RecoCTPPS.CTPPSPixelLocal.CTPPSPixelClusterizer_cfi")
process.clusterProd = cms.EDProducer("CTPPSPixelClusterProducer",
                                     label=cms.untracked.string("ctppsPixelDigis"),
                                     RPixVerbosity = cms.int32(2),
                                     SeedADCThreshold = cms.int32(10),
                                     ADCThreshold = cms.int32(10),
                                     ElectronADCGain = cms.double(135.0),
                                     VCaltoElectronOffset = cms.int32(0),
                                     VCaltoElectronGain = cms.int32(50),
#                                     CalibrationFile = cms.string("Gain_Fed_1462-1463_Run_107.root"),
#                                     DAQCalibration = cms.bool(True),
                                     doSingleCalibration = cms.bool(False)
)


process.mixedigi_step = cms.Path(process.clusterProd
#*process.rechitProd
)

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.mixedigi_step,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
  #  getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq
    getattr(process,path)._seq =  getattr(process,path)._seq


