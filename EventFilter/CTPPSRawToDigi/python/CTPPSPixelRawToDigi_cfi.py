import FWCore.ParameterSet.Config as cms


ctppsPixelDigis = cms.EDProducer(
    "CTPPSPixelRawToDigi",
    #Timing = cms.untracked.bool(False)
#    IncludeErrors = cms.bool(False),
    InputLabel = cms.InputTag("ctppsPixelRawData"),
#    UseQualityInfo = cms.bool(False),
    ## ErrorList: list of error codes used by tracking to invalidate modules
    #ErrorList = cms.vint32(29)
    ## UserErrorList: list of error codes used by Pixel experts for investigation
    #UserErrorList = cms.vint32(40)
    ##  Use pilot blades
    #UsePilotBlade = cms.bool(False)
    ##  Use phase1
    #UsePhase1 = cms.bool(False)
    ## Empty Regions PSet means complete unpacking
    #Regions = cms.PSet( ) 
    mappingLabel = cms.string("RPix")
    
    #from Configuration.Eras.Modifier_phase1Pixel_cff import phase1Pixel
    #phase1Pixel.toModify(ctppsPixelDigis, UsePhase1=True)
)
