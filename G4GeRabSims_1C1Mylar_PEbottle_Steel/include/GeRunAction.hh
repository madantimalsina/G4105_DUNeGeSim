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
/// \file GeRunAction.hh
/// \brief Definition of the GeRunAction class

#ifndef GeRunAction_h
#define GeRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class GeRunAction : public G4UserRunAction
{
  public:
    GeRunAction();
    virtual ~GeRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddEdep (G4double edep);
    void AddEdep1 (G4double edep1);
    void AddEdep4 (G4double edep4);
    //void AddEdepPE (G4double edepPE, G4ThreeVector position);

  private:
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4double> fEdep1;
    G4Accumulable<G4double> fEdep2;
    G4Accumulable<G4double> fEdep3;
    G4Accumulable<G4double> fEdep4;
    G4Accumulable<G4double> fEdep5;

    //G4Accumulable<G4double> fEdepPE;
    //G4Accumulable<G4ThreeVector> fpos;
    
};

#endif

