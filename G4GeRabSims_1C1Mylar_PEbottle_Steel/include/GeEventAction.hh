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
/// \file GeEventAction.hh
/// \brief Definition of the GeEventAction class

#ifndef GeEventAction_h
#define GeEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class GeRunAction;

/// Event action class
///

class GeEventAction : public G4UserEventAction
{
  public:
    GeEventAction(GeRunAction* runAction);
    virtual ~GeEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    //void AddEdepPE(G4double de, G4double dl, G4ThreeVector position);

    void AddEdep(G4double edep) { fEdep += edep; }
    void AddEdep1(G4double edep1) { fEdep1 += edep1; }
    void AddEdep4(G4double edep4) { fEdep4 += edep4; }

    void AddEdepPE(G4double de, G4ThreeVector position) {
  
            fEdepPE += de; 

            fpos[0] = position.x();
            fpos[1] = position.y();
            fpos[2] = position.z();
        }

  private:
    GeRunAction* fRunAction;
    G4double     fEdep;
    G4double     fEdep1;
    G4double     fEdep4;
    G4double     fEdepPE;
    G4double     fpos[3];
    //G4ThreeVector fpos[3];

};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
