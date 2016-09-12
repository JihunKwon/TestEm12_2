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
/// \file electromagnetic/TestEm12/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 78723 2014-01-20 10:32:17Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
	fWorldMaterial(0), fAbsorMaterial(0), fTargetMaterial(0),
   fWorld(0),
   fDetectorMessenger(0)
{
  // default parameter values
  fWorldSizeX = fWorldSizeYZ = 15 * um;
  fAbsorRadius = 10*um;
  fNbOfLayersB = 100;
  fTargetRadius = 1000 * nm;
  
  fWorldMaterial = fAbsorMaterial = 0;

  DefineMaterials();
  SetMaterial("G4_WATER");

  // create commands for interactive definition of the detector  
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  G4NistManager* man = G4NistManager::Instance();
  
  man->FindOrBuildMaterial("G4_Al");
  man->FindOrBuildMaterial("G4_Si");
  man->FindOrBuildMaterial("G4_Fe");
  man->FindOrBuildMaterial("G4_Ge");
  man->FindOrBuildMaterial("G4_W");
  man->FindOrBuildMaterial("G4_Pb");
  
  man->FindOrBuildMaterial("G4_AIR");
  man->FindOrBuildMaterial("G4_WATER");
  
 ///G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  // World
  //
  G4Box*
	  sWorld = new G4Box("World",                                      //name
		  fWorldSizeX / 2, fWorldSizeYZ / 2, fWorldSizeYZ / 2);   //dimensions


  fWorldMaterial = fAbsorMaterial;

  G4LogicalVolume*
	  lWorld = new G4LogicalVolume(sWorld,                        //shape
		  fWorldMaterial,                //material
		  "World");                       //name

  //G4VPhysicalVolume*
	  fWorld = new G4PVPlacement(0,                           //no rotation
		  G4ThreeVector(),             //at (0,0,0)
		  lWorld,                      //logical volume
		  "World",                     //name
		  0,                           //mother  volume
		  false,                       //no boolean operation
		  0);                          //copy number
  
 /*
  // Absorber
  //
  G4Sphere* 
  sAbsor = new G4Sphere("Absorber",                           //name
                     0., fAbsorRadius, 0., twopi, 0., pi);    //size

  G4LogicalVolume*
  lAbsor = new G4LogicalVolume(sAbsor,                        //solid
                                     fAbsorMaterial,          //material
                                    "Absorber");              //name

  G4VPhysicalVolume*
  fAbsor = new G4PVPlacement(0,                         //no rotation
                             G4ThreeVector(),           //at (0,0,0)
                             lAbsor,                    //logical volume
                            "Absorber",                 //name
                             lWorld,                         //mother  volume
                             false,                     //no boolean operation
                             0);                        //copy number
*/
  // LayersB(Outer)
  //
  fLayerBThickness = (fAbsorRadius-fTargetRadius)/fNbOfLayersB;
 
  //G4LogicalVolume* lLayer = NULL;
  G4LogicalVolume* pMotherVolume = lWorld;	// lAbsor;

  //for (G4int i=1; i<=fNbOfLayers; i++) 
	for (G4int i = fNbOfLayersB; i > 0; i--)
	  {
		char szName[16];
		sprintf(szName, "LayerB%d", i);

		G4Sphere*
		sLayer = new G4Sphere(szName, /*(i-1)*fLayerBThickness+fTargetRadius*/0, i*fLayerBThickness+fTargetRadius,
								0., twopi, 0., pi);
		G4cout << i*fLayerBThickness << G4endl;
                 
		//G4LogicalVolume* 
		G4LogicalVolume* lLayer = new G4LogicalVolume(sLayer,                //shape
										fAbsorMaterial,        //material
										szName);              //name
                                 
					new G4PVPlacement(0,                      //no rotation
									G4ThreeVector(),        //at (0,0,0)
									lLayer,                 //logical volume
									szName,                //name
									pMotherVolume,	//lAbsor,                 //mother  volume
									false,                  //no boolean operation
									i);                     //copy number

					pMotherVolume = lLayer;
	}  

	/*		 
   // Target
   //
	G4Orb*
		sTarget = new G4Orb("Target", fTargetRadius);

	fTargetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");
	//fTargetMaterial = G4Material::GetMaterial("G4_Au");

	G4LogicalVolume*
		lTarget = new G4LogicalVolume(sTarget,                //shape
			fTargetMaterial,        //material
			"Target");              //name

	new G4PVPlacement(0,                      //no rotation
		G4ThreeVector(),        //at (0,0,0)
		lTarget,                 //logical volume
		"Target",                //name
		lLayer,                 //mother  volume
		false,                  //no boolean operation
		0);                     //copy number
		*/

                                                 

  PrintParameters();
  
  //
  //always return the root volume
  //  
  return fWorld;	// fAbsor;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << "---> The Absorber is a sphere of " 
         << G4BestUnit(fAbsorRadius,"Length") << " radius of "
         << fAbsorMaterial->GetName() << " divided in " << fNbOfLayersB 
         << " slices of " << G4BestUnit(fLayerBThickness,"Length")
         << "\n \n" << fAbsorMaterial << G4endl;
  G4cout << "\n---------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetRadius(G4double value)
{
  fAbsorRadius = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) fAbsorMaterial = pttoMaterial;
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfLayers(G4int value)
{
  fNbOfLayersB = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();   
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
