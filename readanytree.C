/*****************************************************
 *                                                   *
 * Simply macro to read mpddst files without mpdroot *
 *                                                   *
 * written by: Ivonne Maldonado                      *
 * ivonne.alicia.maldonado@gmail.com                 *
 *
 * Follow tutorials and add instructions
 * *         
 *****************************************************/
TString filename ="outtestf141443.root";
void readanytree(){
TFile *f = TFile::Open(filename);
TTree *t1 = (TTree*)f->Get("mpdsim");
Double_t px[10000];
Double_t py[10000];
Double_t pt[10000];
t1->SetBranchAddress("MCTrack.fPx",&px);
t1->SetBranchAddress("MCTrack.fPx",&py);
TH1F *hpt =new TH1F("pttransverso","Distribucion del pt;pt2;Entries",100,0,5.0);

for (int iEntry = 0; t1->LoadTree(iEntry) >= 0; ++iEntry) {
   // Load the data for the given tree entry
   t1->GetEntry(iEntry);
    for(int i=0; i<10; i++){
     cout << px[i] << ",  event  = " << iEntry << endl;
pt[i] = TMath::Sqrt(px[i]*px[i] + py[i]+py[i]);
	cout << "otra linea " << pt[i] << ", event = " << iEntry << endl;
hpt->Fill(pt[i]);

}
}
hpt->Draw();
TFile *fo = new TFile("salida.root","recreate");
fo->cd();
hpt->Write();
fo->Close();
}
