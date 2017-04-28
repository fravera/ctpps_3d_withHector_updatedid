import FWCore.ParameterSet.Config as cms

process = cms.Process('RECODQM')

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

# raw data source
process.source = cms.Source("PoolSource",
#fileNames=cms.untracked.vstring('file:/afs/cern.ch/user/j/jkaspar/public/run273062_ls0001-2_stream.root')
labelRawDataLikeMC = cms.untracked.bool(False),
fileNames = 
#cms.untracked.vstring('file:/afs/cern.ch/user/p/popov/scratch_bk/data/run273062_ls0001-2_stream.root')
#cms.untracked.vstring('file:/afs/cern.ch/user/p/popov/scratch_bk/data/simevent_CTPPS_DIG_CLU_100.root')
#cms.untracked.vstring('file:/afs/cern.ch/user/p/popov/scratch_bk/data/simevent_CTPPS_DIG_CLU_2_TEST_5000.root')
#cms.untracked.vstring('file:/afs/cern.ch/user/p/popov/scratch_bk/data/digis_PixelAlive_1294_153_RAW_v3.root')
cms.untracked.vstring('file:/afs/cern.ch/user/f/fabferro/WORKSPACE/private/CMSSW_9_1_0_pre1/src/EventFilter/CTPPSRawToDigi/test/digis_run292779_RAW.root')
#cms.untracked.vstring('file:/afs/cern.ch/user/p/popov/scratch_bk/data/digis_PixelAlive_1294_151_RAW_v2_900p1.root')

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# raw-to-digi conversion
#process.load("EventFilter.TotemRawToDigi.totemRawToDigi_cff")

# local RP reconstruction chain with standard settings
#process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# CTPPS DQM modules
#process.load("DQM.CTPPS.totemDQM_cff")
process.load("DQM.CTPPS.rPixDQM_cff")

process.options = cms.untracked.PSet(
#    Rethrow = cms.untracked.vstring('ProductNotFound',
    SkipEvent = cms.untracked.vstring('ProductNotFound',
        'TooManyProducts',
        'TooFewProducts')
)

process.path = cms.Path(
#  process.totemTriggerRawToDigi *
#  process.totemRPRawToDigi *

#  process.recoCTPPS *

#  process.totemDQM +
  process.rPixDQM
)

process.end_path = cms.EndPath(
  process.dqmEnv +
  process.dqmSaver
)

process.schedule = cms.Schedule(
  process.path,
  process.end_path
)
