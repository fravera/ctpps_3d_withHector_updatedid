import FWCore.ParameterSet.Config as cms

# geometry
from Geometry.VeryForwardGeometry.geometry_CTPPS_alaTotem_RECO_cfi import *

# reco hit production
from RecoCTPPS.CTPPSPixelLocal.CTPPSPixelClusterizer_cfi import *

# local track fitting
from RecoCTPPS.CTPPSPixelLocal.CTPPSPixelRecHit_cfi import *

ctppsPixelLocalReconstruction = cms.Sequence(
    clusterProd*rechitProd
)
