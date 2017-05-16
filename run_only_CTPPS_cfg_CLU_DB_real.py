import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CTPPS2',eras.Run2_25ns)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
# process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
############### using only CTPPS geometry 
process.load("Configuration.Geometry.geometry_CTPPS_cfi")
#process.load("CondFormats.CTPPSReadoutObjects.CTPPSPixelDAQMappingESSourceXML_cfi")
##########SimTransport###########
process.load('SimTransport.HectorProducer.HectorTransportCTPPS_cfi')

process.load("SimG4Core.Application.g4SimHits_cfi")
process.g4SimHits.Generator.ApplyPCuts          = False
process.g4SimHits.Generator.ApplyPhiCuts        = False
process.g4SimHits.Generator.ApplyEtaCuts        = False
process.g4SimHits.Generator.HepMCProductLabel   = 'LHCTransport'
process.g4SimHits.Generator.MinEtaCut        = -13.0
process.g4SimHits.Generator.MaxEtaCut        = 13.0
process.g4SimHits.Generator.Verbosity        = 0

process.g4SimHits.G4TrackingManagerVerbosity = cms.untracked.int32(3)
process.g4SimHits.SteppingAction.MaxTrackTime = cms.double(2000.0)
process.g4SimHits.StackingAction.MaxTrackTime = cms.double(2000.0)

process.load("IOMC.RandomEngine.IOMC_cff")
process.RandomNumberGeneratorService.generator.initialSeed = 456789
process.RandomNumberGeneratorService.g4SimHits.initialSeed = 9876
process.RandomNumberGeneratorService.VtxSmeared.initialSeed = 123456789
#process.RandomNumberGeneratorService.RPixDetDigitizer = cms.PSet(initialSeed =cms.untracked.uint32(137137))


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(3)
        )

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_v2', '')
process.load('PhysicsTools.HepMCCandAlgos.genParticles_cfi')

import math
phi_min = -math.pi
phi_max = math.pi
t_min   = 0.
t_max   = 2.
xi_min  = 0.01
xi_max  = 0.3
ecms = 13000.

process.generator = cms.EDProducer("RandomtXiGunProducer",
        PGunParameters = cms.PSet(
            PartID = cms.vint32(2212),
            MinPhi = cms.double(phi_min),
            MaxPhi = cms.double(phi_max),
            ECMS   = cms.double(ecms),
            Mint   = cms.double(t_min),
            Maxt   = cms.double(t_max),
            MinXi  = cms.double(xi_min),
            MaxXi  = cms.double(xi_max)
            ),
        Verbosity = cms.untracked.int32(0),
        psethack = cms.string('single protons'),
        FireBackward = cms.bool(True),
        FireForward  = cms.bool(True),
        firstRun = cms.untracked.uint32(1)
        )


process.source = cms.Source("PoolSource",
# fileNames =  cms.untracked.vstring('file:rawdata.root')
labelRawDataLikeMC = cms.untracked.bool(False),
fileNames =  cms.untracked.vstring(
#"file:./PixelAlive_1294_153_RAW_v3.root"
#'file:../../../CMSSW_9_1_0_pre1_test220/CMSSW_9_1_0_pre1/src/simevent_CTPPS_DIG_CLU_REC_5000_rid.root'
'file:/afs/cern.ch/user/f/fabferro/public/digis_run293910_RAW_FF_stream.root'
 ),
duplicateCheckMode = cms.untracked.string("checkEachFile")
)

#from CondCore.DBCommon.CondDBSetup_cfi import *
#process.CTPPSPixelGainCalibrationRcd = cms.ESSource("PoolDBESSource",
#        CondDBSetup,
#connect = cms.string('sqlite_file:ctppspixnew1.db'),
#
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('CTPPSPixelGainCalibrationsRcd'),
#        tag = cms.string("CTPPSPixelGainCalibNew_v1")
#    )
#
#)
#)
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


#process.ProductionFilterSequence = cms.Sequence(process.generator*process.VtxSmeared)
process.ProductionFilterSequence = cms.Sequence(process.generator)

############
process.o1 = cms.OutputModule("PoolOutputModule",
#        process.FEVTSIMEventContent,
        outputCommands = cms.untracked.vstring('keep *'),
        fileName = cms.untracked.string('simevent_CTPPS_CLU_real.root')
        )

process.common_maximum_timex = cms.PSet( # need to be localy redefined
        MaxTrackTime  = cms.double(2000.0),  # need to be localy redefined
        MaxTimeNames  = cms.vstring('ZDCRegion'), # need to be localy redefined
        MaxTrackTimes = cms.vdouble(10000.0),  # need to be localy redefined
        DeadRegions = cms.vstring()
        )

#process.load("SimGeneral.MixingModule.MYmixNoPU_cfi")
#process.load("SimCTPPS.CTPPSPixelDigiProducer.RPixDetConf_cfi")
#process.RPixDetDigitizer.RPixVerbosity = cms.int32(2)

process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)

'''
process.g4SimHits.Watchers = cms.VPSet(cms.PSet(
    type = cms.string('CTPPSAnalysisRP'),
    CTPPSAnalysisRP = cms.PSet(
        Names = cms.vstring('CTPPSTimingHits'),
        FileName = cms.string('CTPPSTimingHits.root'),
    Verbosity = cms.bool(True)
    )
))
'''

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

#process.clusterProd.RPixVerbosity = cms.int32(2)
process.load("RecoCTPPS.CTPPSPixelLocal.CTPPSPixelRecHit_cfi")
process.rechitProd.RPixVerbosity = cms.int32(2)



process.g4Simhits_step = cms.Path(process.g4SimHits)

process.mixedigi_step = cms.Path(process.clusterProd
#*process.rechitProd
)

# Transport
process.transport_step = cms.Path(process.LHCTransport)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.mixedigi_step,process.outpath)

# filter all path with the production filter sequence
for path in process.paths:
    getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq


