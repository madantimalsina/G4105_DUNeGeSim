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
/// \file GeDetectorConstruction.cc
/// \brief Implementation of the GeDetectorConstruction class

#include "GeDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

// added my madan 
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GeDetectorConstruction::GeDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  fScoringVolume1(0),
  fScoringVolume2(0),
  fScoringVolume3(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GeDetectorConstruction::~GeDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* GeDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  //nistManager->FindOrBuildMaterial("G4_Fe");
  
  // Envelope parameters
  //
  G4double env_sizeXY = 100.0*mm, env_sizeZ = 100.0*mm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        1.0*env_sizeXY, 1.0*env_sizeXY, 1.0*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  // Create disk shape for Canberra Be3820
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_Ge");
  G4ThreeVector pos1 = G4ThreeVector(0., 0., -15.6*mm);
  
  G4double innerRadius = 0.*mm;
  G4double outerRadius = 34.779*mm;
  G4double hz = 10.*mm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;
  
  G4Tubs* solidShape1 
    = new G4Tubs("Shape1",
                 innerRadius,
                 outerRadius,
                 hz,
                 startAngle,
                 spanningAngle);
  
  G4LogicalVolume* logicShape1 =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                
  
  // Create disk for carbon window
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_C");
  G4ThreeVector pos2 = G4ThreeVector(0., 0., -0.3*mm);
  
  innerRadius = 0.*mm;
  outerRadius = 34.779*mm;
  hz = 0.3*mm;
  startAngle = 0.*deg;
  spanningAngle = 360.*deg;
  
  G4Tubs* solidShape2
    = new G4Tubs("Shape2",
                 innerRadius,
                 outerRadius,
                 hz,
                 startAngle,
                 spanningAngle);
  
  G4LogicalVolume* logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  
  // Create disk for test source
  
  // Add material definition for Mylar
  G4double A,Z,d;  // atomic mass (mass of a mole)
  
  A = 1.01*g/mole;
  G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);
  A = 16.00*g/mole;
  G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);
  A = 12.011*g/mole;
  G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);
  d = 1.19*g/cm3;
  G4Material* matplexiglass = new G4Material("Mylar",d,3);
  matplexiglass->AddElement(elH,0.041959);
  matplexiglass->AddElement(elC,0.33303);
  matplexiglass->AddElement(elO,0.625011); 
  
  G4Material* shape3_mat = nist->FindOrBuildMaterial("Mylar");
  G4ThreeVector pos3 = G4ThreeVector(0., 0., 0.749*mm);
  
  innerRadius = 0.*mm;
  outerRadius = 11.9*mm;
  hz = 0.1255*mm;
  startAngle = 0.*deg;
  spanningAngle = 360.*deg;
  
  G4Tubs* solidShape3
    = new G4Tubs("Shape3",
                 innerRadius,
                 outerRadius,
                 hz,
                 startAngle,
                 spanningAngle);
  
  G4LogicalVolume* logicShape3 =                         
    new G4LogicalVolume(solidShape3,         //its solid
                        shape3_mat,          //its material
                        "Shape3");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos3,                    //at position
                    logicShape3,             //its logical volume
                    "Shape3",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // 
  // 125ml standard PE bottle being flush at detector face (1 mm gap) 
  //  
/*  G4double innerRadius_PEb = 0.;
  G4double outerRadius_PEb = 2.4*cm;
  G4double hz_PEb = 3.455*cm; //half height
  G4double translX = 2.5*cm;
*/

  G4RotationMatrix* rotD1 = new G4RotationMatrix();
  G4RotationMatrix* rotD2 = new G4RotationMatrix();
  G4RotationMatrix* rotD3 = new G4RotationMatrix();
  rotD1->rotateZ(90.*deg);
  rotD2->rotateX(90.*deg);
  rotD3->rotateY(90.*deg);


  // Silicon for Quartz
  G4Element* elSi = new G4Element("Silicon", "Si", Z=14., A=28.09*g/mole);
  
  //Define Quartz (SiO2)
  G4Material* SiO2 = new G4Material("Quartz",d= 1.746*g/cm3, 2, kStateSolid);
  SiO2->AddElement(elSi, 1);
  SiO2->AddElement(elO , 2);

  //
/*
  // Geometry for 125ml standard PE bottle
  G4Tubs* PEbottle = 
    new G4Tubs("Source", innerRadius_PEb, outerRadius_PEb, hz_PEb, startAngle, spanningAngle);

  G4LogicalVolume* logicRing =                         
    new G4LogicalVolume(PEbottle,      //its solid 
                        //SiO2,
                        env_mat,        //its material
                        "Source"); 
 // At center
  new G4PVPlacement(
                 rotD2,                // no rotation
                 G4ThreeVector(0., 0., (1.*mm + outerRadius_PEb)), // its position 
                 logicRing,            // its logical volume                         
                 "Source",            // its name
                 logicEnv,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps);  // checking overlaps 


  //translated position where two bottles are right next to each other, flush with detector face and centered     
  new G4PVPlacement(
                 rotD2,                // no rotation
                 G4ThreeVector(translX, 0., (1.*mm + outerRadius_PEb)), // its position 
                 logicRing,            // its logical volume                         
                 "Source",            // its name
                 logicEnv,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps);  // checking overlaps 


  //At Side Of Puck    
  new G4PVPlacement(
                 rotD2,                // no rotation
                 G4ThreeVector(6.945*cm, 0., -1.56*cm), // its position 
                 logicRing,            // its logical volume                         
                 "Source",            // its name
                 logicEnv,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps);  // checking overlaps                
   */


  // Geometry for the steel sample
  G4double l_steel = (2*4.0)*cm;
  G4double w_steel = (2*4.0)*cm;
  G4double hz_steel = (2*1.35)*cm;
  //G4double hz_steel = (0.95)*cm;


  // Fe with density 8.0*g/cm3
  //G4NistManager* nist = G4NistManager::Instance();
  nist->FindOrBuildMaterial("G4_Fe");
  d = 8.0*g/cm3;
  nist->BuildMaterialWithNewDensity("Fe_8","G4_Fe",d);
  auto steel_mat = G4Material::GetMaterial("Fe_8");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl; 

  // Defination of Geometry for steel
  auto SBox 
    = new G4Box("Source",             // its name
                 l_steel/2, w_steel/2, hz_steel/2); // its size
                         
  auto logicRing // Just to make same name
    = new G4LogicalVolume(
                 SBox,             // its solid
                 env_mat, 
                 //steel_mat,         // its material
                 "Source");        // its name
                                   
      new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., 1.45*cm), // its position
                 logicRing,            // its logical volume                         
                 "Source",            // its name
                 logicEnv,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps);  // checking overlaps 

  // Set Shape2 as scoring volume
  //
  fScoringVolume1 = logicShape1;
  fScoringVolume = logicShape2;
  fScoringVolume2 = logicShape3;
  fScoringVolume3 = logicRing;
  //fScoringVolume3 = SBox;
  //                                        
  // Visualization attributes
  //
  //G4VisAttributes* blue_clear = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.4));
  G4VisAttributes* aqua_clear = new G4VisAttributes(G4Colour(0.0, 0.7, 1.0, 0.4));

  logicShape1->SetVisAttributes(G4VisAttributes(G4Colour::Brown()));
  logicShape2->SetVisAttributes(G4VisAttributes(G4Colour::Red()));
  logicShape3->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
  //logicRing->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));
  logicRing->SetVisAttributes(aqua_clear);
  //logicShape1->SetVisAttributes(aqua_clear);


  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
