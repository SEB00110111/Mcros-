/* Macro reads DST file produced by macro reco.C */

/* 							*
 * Macro básico para el tercer taller de análisis 	*
 * de la colaboración MexNICA                     	*
 * https://indico.nucleares.unam.mx/event/1659		*
 * 							*
 * modificado por:					*
 * 							*
 * Ivonne Alicia Maldonado Cervantes			*
 * ivonne.alicia.maldonado@gmail.com			*
 * 							*
 * Puede leer los archivos de salida *.root:       	*
 * 							*
 * evetest.root - para la salida del macro runMC.C	*
 *  - lee el árbol mpdsim (cbmsim versión anterior) 	*
 *  							*
 * mpddst.root - para la salida del macro reco.C 	*
 *  - lee el árbol mpdsim (cbmsim versión anterior)	*
 * 							*
 * corrected.root - para archivos corregidos por dca    *
 *  - lee el árbol mpdsim				*
 *  							*
 * Crea el archivo de salida "salida.root" que contiene *
 * un histograma de momento transverso obtenida de la   *
 * rama MCTrack con información MC y un histograma con  *
 * la posición del vértice primario, obtenido de la rama*
 * Vertex.						*
 * 							*
 * A cada rama hay que asociar la clase correspondiente *
 * para leer las distintas variables			*
 *    							*/

#include <Rtypes.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <TTree.h>

R__ADD_INCLUDE_PATH($VMCWORKDIR)
#include "mpdloadlibs.C"

char name[50];
Float_t ppxMC, ppyMC, ppzMC;
Float_t LpxMC, LpyMC, LpzMC;
Float_t Lpolx, Lpoly, Lpolz;
Float_t EPangleMC, bMC;
Float_t VtxX,VtxY,VtxZ;
Float_t RotX,RotY;
Int_t Laprim, BaryonN, dpdg;

void MyreadDST() {

    TStopwatch timer;
    timer.Start();

    //TFile out("salidanots.root","recreate");
    TFile out("salidabasic.root","recreate");
    //TFile out("salidaNoSmear.root","recreate");

    TH1F *h1 = new TH1F("h1","Momento Transverso;p_{T}(GeV/c;Entries)",50,0,10);
    TH2F *h2 = new TH2F("h2","x;y;Entries)",50,-10,10,50,-10,10);
    TH1F *hz = new TH1F("hz","z;Entries",50,-10,10);

    TH1F *h3 = new TH1F("h3","Momento Transverso #Lambda #bar{#Lambda}; p_{T}(GeV/c;Entries)",50,0,10);


    TH1F *hM1 = new TH1F("hM1","Multiplicidad |#eta|<0.5;N_{ch};Events)",1000,0,1000);
    TH1F *hM1eta = new TH1F("hM1eta","Multiplicidad |#eta|<1.5;N_{ch};Events)",1000,0,1000);
    TH1F *hM2 = new TH1F("hM2","Multiplicidad |#eta|<0.5;N_{#pi};Events)",1000,0,1000);
    TH1F *hM3 = new TH1F("hM3","Multiplicidad |#eta|<0.5;N_{K};Events)",1000,0,1000);
    TH1F *hM4 = new TH1F("hM4","Multiplicidad |#eta|<0.5;N_{p};Events)",1000,0,1000);

    TH1F *hM1all = new TH1F("hM1all","Multiplicidad;N_{ch};Events)",1000,0,1000);
    TH1F *hM2all = new TH1F("hM2all","Multiplicidad;N_{#pi};Events)",1000,0,1000);
    TH1F *hM3all = new TH1F("hM3all","Multiplicidad;N_{K};Events)",1000,0,1000);
    TH1F *hM4all = new TH1F("hM4all","Multiplicidad;N_{p};Events)",1000,0,1000);

    TH1F *hM1prim = new TH1F("hM1prim","Multiplicidad |#eta|<0.5 p;N_{ch};Events)",1000,0,1000);
    TH1F *hM2prim = new TH1F("hM2prim","Multiplicidad |#eta|<0.5 p;N_{#pi};Events)",1000,0,1000);
    TH1F *hM3prim = new TH1F("hM3prim","Multiplicidad |#eta|<0.5 p;N_{K};Events)",1000,0,1000);
    TH1F *hM4prim = new TH1F("hM4prim","Multiplicidad |#eta|<0.5 p;N_{p};Events)",1000,0,1000);

//Macros for comparison of multiplicity
  TH1F *hRefMult = new TH1F("hRefMultSTAR","hRefMultSTAR",2500,0,2500);
  TH2F *hBvsRefMult = new TH2F("hBvsRefMult","hBvsRefMult",2500,0,2500,200,0.,20.);


  TH1F *hRefMultMC = new TH1F("hRefMultSTARMC","hRefMultSTARMC",2500,0,2500);
  TH2F *hBvsRefMultMC = new TH2F("hBvsRefMultMC","hBvsRefMultMC",2500,0,2500,200,0.,20.);

  const Double_t cut_pt = 0.15; // default: 0.15 GeV/c
  const Double_t cut_eta = 0.5; // default: 0.5
  const Int_t cut_nhits = 16;   // default: 16
  const Double_t dca_cut = 0.5; // default: 0.5 cm


    for(Int_t nf=1; nf<6;nf++){
    //for(Int_t nf2=0; nf2<4;nf2++){

    //sprintf(name,"/storage/mexnica/ivonne/GEN/BiBi/9BiBiPHSD/mpddst%d.root",nf);
//    sprintf(name,"/storage/mexnica/datosNICA/eos/eosnica.jinr.ru/nica/mpd/sim/data/exp/dst-BiBi-09GeV-mp07-20-pwg3-250ev/BiBi/09.0GeV-0-14fm/UrQMD/BiBi-09GeV-mp07-20-pwg3-250ev-1/urqmd-BiBi-09.0GeV-0-14fm-eos0-250-10%d-%d.reco.root",nf,nf2);
    sprintf(name,"/storage/mexnica/mmedina/mpddst%d.root",nf);

    TString in=name;

    if (in.IsNull()) {
        cout << "Please, provide an input DST-file!" << endl;
        return;
    }

    TChain *dstTree = new TChain("mpdsim");  // cambiar a cbmsim para versiones anteriores
    dstTree->Add(in.Data());

    // Activate branches
    MpdEvent *event = nullptr;
    dstTree->SetBranchAddress("MPDEvent.", &event); // trazas globales
    TClonesArray *fMCTracks = nullptr;
    dstTree->SetBranchAddress("MCTrack", &fMCTracks);  // trazas MC

    TClonesArray *fvtxs = nullptr;
    dstTree->SetBranchAddress("Vertex", &fvtxs); // vértice primario

    FairMCEventHeader *mchead = 0x0;
    dstTree->SetBranchAddress("MCEventHeader.", &mchead); //



    Int_t events = dstTree->GetEntries();
    cout << " Number of events = " << events << "  in DST files = "<< nf << endl;
    for (Int_t i = 0; i < events; i++) {
        dstTree->GetEntry(i);

	MpdVertex *vtx = (MpdVertex*)fvtxs->First();

	TVector3 primVert;
	vtx->Position(primVert);
	Double_t Vt = primVert.Phi();

//	cout<< "Vt(angulo) =" << Vt << endl;
	Double_t posx= vtx->GetX();
	Double_t posy= vtx->GetY();
	Double_t posz= vtx->GetZ();
	h2->Fill(posx,posy);
	hz->Fill(posz);

        Int_t nvtx = fvtxs->GetEntriesFast();

//	cout << "Número de entradas = " << nvtx << endl;

	// Info MC del evento

        EPangleMC = mchead->GetRotZ();
        RotX = mchead->GetRotX();
        RotY = mchead->GetRotY();
	bMC = mchead->GetB();
	//Loop sobre tracks MC

	Int_t NtracksMC = fMCTracks->GetEntriesFast();
//        cout << " Number of MC tracks = " << NtracksMC << endl;
Int_t refMultMC=0;
Int_t nChp=0, npip=0, nKp=0, npp=0;
	for (Int_t iTrack = 0; iTrack < NtracksMC; iTrack++) {
        MpdMCTrack* mcTr = (MpdMCTrack*) fMCTracks->UncheckedAt(iTrack);

        Double_t ptMC = mcTr->GetPt();
        Double_t etaMC =  0.5*TMath::Log((mcTr->GetP()+mcTr->GetPz())/(mcTr->GetP()-mcTr->GetPz()+1.e-13));
// Cuts to compare with MPD Global Tracks
           if (ptMC < cut_pt)continue;
	   if (TMath::Abs(etaMC) > cut_eta) continue;

//end cuts	

        Int_t pdgcoded=mcTr->GetPdgCode();
        if(mcTr->GetMotherId()!=-1)continue; // me quedo con los tracks secundarios
         
        if (TMath::Abs(pdgcoded)== 211 ||
            TMath::Abs(pdgcoded)== 321 ||
            TMath::Abs(pdgcoded)==2212){

      refMultMC++;
}
        h3->Fill(mcTr->GetP());

//	   if(mcTr->GetMotherId()!=-1) continue; //sólo primarias
	   nChp +=1;
           if(TMath::Abs(pdgcoded) == 211)npip +=1;
           if(TMath::Abs(pdgcoded) == 321)nKp +=1;
           if(TMath::Abs(pdgcoded) == 2212)npp +=1;
               
            
	}// End sobre tracks MC

    hRefMultMC->Fill(refMultMC);
    hBvsRefMultMC->Fill(refMultMC,bMC);

	//Loop sobre tracks globales

        Int_t Ntracks = event->GetGlobalTracks()->GetEntriesFast();
//        cout << " Number of tracks = " << Ntracks << endl;
Int_t nCh=0, npi=0, nK=0, np=0, nCheta;
Int_t nCha=0, npia=0, nKa=0, npa=0;

Double_t ptpart, etapart;
Int_t nhits, refMult=0;
	for (Int_t iTrack = 0; iTrack < Ntracks; iTrack++) {

           MpdTrack* track = (MpdTrack*) event->GetGlobalTracks()->UncheckedAt(iTrack);
           ptpart = track->GetPt();
           etapart =  track->GetEta();
	   nhits = track->GetNofHits();
  //         if((nhits < 16)||(ptpart < 0.15))continue;
           if (ptpart < cut_pt)continue;
	   if (TMath::Abs(etapart) > cut_eta) continue;
           if (nhits <  cut_nhits) continue;

      // Primary track selection
      if (TMath::Sqrt(TMath::Power(track->GetDCAX(),2) + TMath::Power(track->GetDCAY(),2) + TMath::Power(track->GetDCAZ(),2)) > dca_cut) continue;
      
      refMult +=1;
      //refMult++;
      



	   h1->Fill(ptpart);
	   
	   Double_t px=track->GetPx();
	   Double_t py=track->GetPy();
	   Double_t pz=track->GetPz();
	   Double_t P = TMath::Sqrt(px*px+py*py+pz*pz);
           //Double_t eta =  0.5*TMath::Log((P+pz)/(P-pz+1.e-13));
           Int_t id = track->GetID();
	   MpdMCTrack* mctr = (MpdMCTrack*)fMCTracks->UncheckedAt(id);
	   Int_t pdgcode = mctr->GetPdgCode();
	   nCha +=1;
           if(TMath::Abs(pdgcode) == 211)npia +=1;
           if(TMath::Abs(pdgcode) == 321)nKa +=1;
           if(TMath::Abs(pdgcode) == 2212)npa +=1;

	   if(TMath::Abs(etapart)<1.5)nCheta +=1;
	   if(TMath::Abs(etapart)>0.5)continue;
	   nCh +=1;
           if(TMath::Abs(pdgcode) == 211)npi +=1;
           if(TMath::Abs(pdgcode) == 321)nK +=1;
           if(TMath::Abs(pdgcode) == 2212)np +=1;
            /* See mpddata/MpdTrack.h for more methods */

        } // track loop

hM1->Fill(nCh);
hM1eta->Fill(nCheta);
hM2->Fill(npi);
hM3->Fill(nK);
hM4->Fill(np);
hM1all->Fill(nCha);
hM2all->Fill(npia);
hM3all->Fill(nKa);
hM4all->Fill(npa);
hM1prim->Fill(nChp);
hM2prim->Fill(npip);
hM3prim->Fill(nKp);
hM4prim->Fill(npp);


    hRefMult->Fill(refMult);
    hBvsRefMult->Fill(refMult,bMC);

    } // event loop



//cout << "Lambdas = "  << nLambda << "  protones = " << nprotons << endl;

    }//loop files

//cout << "Lambdas = "<< nLambdaprim << " protones = "<< nprotons<< "  Todas Lambdas = "<< nLambda << endl;
    
    h1->Draw();

    timer.Print();

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
  //  exit(0);
  //
  out.Write();
  out.Close();
}		
