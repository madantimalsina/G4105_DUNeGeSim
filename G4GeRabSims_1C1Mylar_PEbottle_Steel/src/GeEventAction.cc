//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file GeEventAction.cc
/// \brief Implementation of the GeEventAction class

#include "GeEventAction.hh"
#include "GeRunAction.hh"
#include "GeAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GeEventAction::GeEventAction(GeRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fEdep1(0.),
  fEdep4(0.),
  fEdepPE(0.),
  fpos{0.}
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GeEventAction::~GeEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GeEventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fEdep1 = 0.;
  fEdep4 = 0.;
  fEdepPE = 0.;

  fpos[0] = 0.0;
  fpos[1] = 0.0;
  fpos[2] = 0.0;

/*  G4AnalysisManager* analysisManager_vtx = G4AnalysisManager::Instance();
    analysisManager_vtx->FillH1(4, fpos[0]);
    analysisManager_vtx->FillH1(5, fpos[1]);
    analysisManager_vtx->FillH1(6, fpos[2]);*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GeEventAction::EndOfEventAction(const G4Event*)
{   
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4ThreeVector position = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex()->GetPosition();

  //G4cout << "Position: " <<position << G4endl;
  //G4cout << "fpos: " <<fpos[0] << "," << fpos[1] << ", " << fpos[2] << G4endl;

  // fill histograms
  if (fEdep > 0) analysisManager->FillH1(0, fEdep);
  if (fEdep1 > 0) analysisManager->FillH1(1, fEdep1);
  if (fEdep4 > 0) analysisManager->FillH1(2, fEdep4);
  if (fEdepPE > 0) analysisManager->FillH1(3, fEdep4);
  analysisManager->FillH1(4, position[0]);
  analysisManager->FillH1(5, position[1]);
  analysisManager->FillH1(6, position[2]);
  analysisManager->FillH1(7, std::sqrt((position[0]*position[0])+((position[2]-26.*mm)*(position[2]-26.*mm))));

/*analysisManager->FillH1(4, fpos[0]);
  analysisManager->FillH1(5, fpos[1]);
  analysisManager->FillH1(6, fpos[2]);
*/

  // 2D histogram
  analysisManager->FillH2(0, position[0], position[1]); 
  analysisManager->FillH2(1, position[1], position[2]); 
  analysisManager->FillH2(2, position[2], position[0]);

/* analysisManager->FillH2(0, fpos[0], fpos[1]); 
  analysisManager->FillH2(1, fpos[1], fpos[2]); 
  analysisManager->FillH2(2, fpos[2], fpos[0]);  
*/


  
  //fill ntuple
  analysisManager->FillNtupleDColumn(0, fEdep);
  analysisManager->FillNtupleDColumn(1, fEdep1);
  analysisManager->FillNtupleDColumn(2, fEdep4);
  analysisManager->FillNtupleDColumn(3, fEdepPE);
  analysisManager->FillNtupleDColumn(4, position[0]);
  analysisManager->FillNtupleDColumn(5, position[1]);
  analysisManager->FillNtupleDColumn(6, position[2]);
  analysisManager->AddNtupleRow();
  
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddEdep1(fEdep1);
  fRunAction->AddEdep4(fEdep4);
  //fRunAction->AddEdepPE(fEdepPE, position);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
