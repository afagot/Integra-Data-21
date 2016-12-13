#include "../include/fonctions.h"

using namespace std;

int main(int argc ,char *argv[]){
	stringstream converter;
	converter << argv[0];
	string program;
	converter >> program;
	converter.clear();

	if(argc != 1){
			cout << "[Integrals] expects to have 1 parameter\n";
			cout << "[Offline] USAGE is : " << program << "\n";
			return -1;
	} else if(argc == 1){
		//intput A1 ROOT file
		TFile A1ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_A1_triger_cosmic.root","READ");
		TH1I* A1Profile = (TH1I*)A1ROOTinput.Get("StripProfile");

		//intput A2 ROOT file
		TFile A2ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_A2_triger_cosmic.root","READ");
		TH1I* A2Profile = (TH1I*)A2ROOTinput.Get("StripProfile");

		//Canvas to save a copy of the profile
		TCanvas* Canvas = new TCanvas("Profile","Hit profile");
		Canvas->Divide(1,3);

		//Merged histogram to get the results from an entire partition
		TList* listA = new TList;
		listA->Add(A1Profile);
		listA->Add(A2Profile);

		TH1I* ProfileA = (TH1I*)A1Profile->Clone("ProfileA");
		ProfileA->Reset();
		ProfileA->Merge(listA);
		Canvas->cd(1);
		ProfileA->Draw();

		//intput C1 ROOT file
		TFile C1ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_C1_triger_cosmic.root","READ");
		TH1I* C1Profile = (TH1I*)C1ROOTinput.Get("StripProfile");

		//intput C2 ROOT file
		TFile C2ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_C2_triger_cosmic.root","READ");
		TH1I* C2Profile = (TH1I*)C2ROOTinput.Get("StripProfile");

		//Merged histogram to get the results from
		TList* listC = new TList;
		listC->Add(C1Profile);
		listC->Add(C2Profile);

		TH1I* ProfileC = (TH1I*)C1Profile->Clone("ProfileC");
		ProfileC->Reset();
		ProfileC->Merge(listC);
		Canvas->cd(3);
		ProfileC->Draw();

		//intput B1 ROOT file
		TFile B1ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_B1_triger_cosmic.root","READ");
		TH1I* B1Profile = (TH1I*)B1ROOTinput.Get("StripProfile");

		//intput B2 ROOT file
		TFile B2ROOTinput("../../Gamma_off/Run210814/RootData_Run210814/ROOT_RE-2-BARC-161_HV_9200_source_off_B2_triger_cosmic.root","READ");
		TH1I* B2Profile = (TH1I*)B2ROOTinput.Get("StripProfile");

		//Merged histogram to get the results from an entire partition
		TList* listB = new TList;
		listB->Add(B1Profile);
		listB->Add(B2Profile);

		TH1I* ProfileB = (TH1I*)B1Profile->Clone("ProfileB");
		ProfileB->Reset();
		ProfileB->Merge(listB);
		Canvas->cd(2);
		ProfileB->Draw();

		//output csv file with integrals results
		ofstream CSVFile("Data.csv", ios::out);

		//*****************************************************************************************************

		TF1* AllMuons = new TF1("fit","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))+[5]*exp(-0.5*((x-[6])/[7])**2) / (1 + exp(-[8]*(x-[9])))",33,64);
		AllMuons->SetParLimits(0,1,200000);
		AllMuons->SetParameter(1,58);
		AllMuons->SetParLimits(1,33,64);
		AllMuons->SetParameter(2,6);
		AllMuons->SetParLimits(2,1,32);
		AllMuons->SetParameter(3,-1);
		AllMuons->SetParLimits(3,-10,-0.01);
		AllMuons->SetParameter(4,56);
		AllMuons->SetParLimits(4,33,64);
		AllMuons->SetParLimits(5,1,50000);
		AllMuons->SetParameter(6,59);
		AllMuons->SetParLimits(6,33,64);
		AllMuons->SetParameter(7,2);
		AllMuons->SetParLimits(7,1,32);
		AllMuons->SetParameter(8,1);
		AllMuons->SetParLimits(8,0.01,10);
		AllMuons->SetParameter(9,56);
		AllMuons->SetParLimits(9,33,64);
		ProfileB->Fit(AllMuons,"QR");
		Canvas->cd(2);
		ProfileB->Draw();

		cout << "What to do?\n";
		cin.get();

		//*****************************************************************************************************

		TF1* ForwardMuons = new TF1("fit_f","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))",33,64);
		ForwardMuons->FixParameter(0,AllMuons->GetParameter(0));
		ForwardMuons->FixParameter(1,AllMuons->GetParameter(1));
		ForwardMuons->FixParameter(2,AllMuons->GetParameter(2));
		ForwardMuons->FixParameter(3,AllMuons->GetParameter(3));
		ForwardMuons->FixParameter(4,AllMuons->GetParameter(4));

		CSVFile << ProfileB->GetEntries() << '\t'
				<< ForwardMuons->Integral(33.,64.) << '\t'
				<< AllMuons->Integral(33.,48.) << '\t'
				<< ProfileB->GetMaximum() << '\t'
				<< ProfileB->GetMaximumBin()+32 << '\t'
				<< ForwardMuons->GetMaximum() << '\t'
				<< ForwardMuons->GetMaximumX() << '\t';

		//*****************************************************************************************************

		TF1* BackwardMuons = new TF1("fit_b","[0]*exp(-0.5*((x-[1])/[2])**2) / (1 + exp(-[3]*(x-[4])))",33,64);
		BackwardMuons->FixParameter(0,AllMuons->GetParameter(5));
		BackwardMuons->FixParameter(1,AllMuons->GetParameter(6));
		BackwardMuons->FixParameter(2,AllMuons->GetParameter(7));
		BackwardMuons->FixParameter(3,AllMuons->GetParameter(8));
		BackwardMuons->FixParameter(4,AllMuons->GetParameter(9));

		CSVFile << BackwardMuons->Integral(33.,64.) << '\t';

		ProfileB->GetXaxis()->SetRange(10.5,16.5);

		CSVFile << ProfileB->GetMaximum() << '\t'
				<< ProfileB->GetMaximumBin()+32 << '\t'
				<< BackwardMuons->GetMaximum(57.5,64) << '\t'
				<< BackwardMuons->GetMaximumX(57.5,64) << '\t';

		CSVFile.close();


		TFile ROOToutput("Cumulated.root","RECREATE");
		AllMuons->Write();
		ForwardMuons->Write();
		BackwardMuons->Write();
		Canvas->Write();

		A1ROOTinput.Close();
		A2ROOTinput.Close();
		B1ROOTinput.Close();
		B2ROOTinput.Close();
		C1ROOTinput.Close();
		C2ROOTinput.Close();

		ROOToutput.Close();

		return 0;
	}
}
