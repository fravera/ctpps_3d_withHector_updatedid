{
  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  c1->Divide(2,3);
  c1->cd(1);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023227392");
  c1->cd(2);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023292928");
  c1->cd(3);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023358464");
  c1->cd(4);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023424000");
  c1->cd(5);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023489536");
  c1->cd(6);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2023555072");

  TCanvas *c2 = new TCanvas("c2","c2",1000,600);
  c2->Divide(2,3);
  c2->cd(1);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040004608");
  c2->cd(2);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040070144");
  c2->cd(3);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040135680");
  c2->cd(4);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040201216");
  c2->cd(5);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040266752");
  c2->cd(6);
Events->Draw("PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.localPosition().y()","PSimHits_g4SimHits_CTPPSPixelHits_CTPPS.obj.detUnitId()==2040332288");



}
