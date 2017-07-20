{
  TCanvas *cc = new TCanvas("plus","plus",400,1200);
  cc->Divide(1,6);
  cc->cd(1);
    planep0->Draw("colz");
  cc->cd(2);
    planep1->Draw("colz");
  cc->cd(3);
    planep2->Draw("colz");
  cc->cd(4);
    planep3->Draw("colz");
  cc->cd(5);
    planep4->Draw("colz");
  cc->cd(6);
    planep5->Draw("colz");


  TCanvas *cc2 = new TCanvas("minus","minus",400,1200);
  cc2->Divide(1,6);
  cc2->cd(1);
    planem0->Draw("colz");
   cc2->cd(2);
   planem1->Draw("colz");
   cc2->cd(3);
   planem2->Draw("colz");
   cc2->cd( 4);
   planem3->Draw("colz");
   cc2->cd( 5);
   planem4->Draw("colz");
   cc2->cd( 6);
   planem5->Draw("colz");


}
