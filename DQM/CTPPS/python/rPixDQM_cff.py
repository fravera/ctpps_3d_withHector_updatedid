import FWCore.ParameterSet.Config as cms

#from DQM.CTPPS.totemDAQTriggerDQMSource_cfi import *
#from DQM.CTPPS.totemRPDQMHarvester_cfi import *
#from DQM.CTPPS.totemRPDQMSource_cfi import *

from DQM.CTPPS.rPixDQMSource_cfi import *

rPixDQM = cms.Sequence(
#  totemDAQTriggerDQMSource *totemRPDQMSource *totemRPDQMHarvester
 rPixDQMSource
)
