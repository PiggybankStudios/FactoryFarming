#include "new.h"
#include "../player.h"
#include "../sound.h"
#include "../buildings/special.h"

uint32_t m_nextPrice;
enum kBuildingType m_nextBuilding;
union kSubType m_nextSubType;

/// ///

enum kBuildingType getNewBuildingType() {
  return m_nextBuilding;
}

uint32_t getNewID() {
  return m_nextSubType.raw;
}

bool checkHasNewToShow(struct Player_t* _p) {
  const uint32_t currentLevel = _p->m_buildingsUnlockedTo;
  if (UnlockDecs[currentLevel].type == FINAL_UNLOCK_TYPE && UnlockDecs[currentLevel].subType.raw == FINAL_UNLOCK_SUBTYPE) {
    // This is always the last unlock - cannot go on from here
    return false;
  }
  const bool haveUnlocked = (_p->m_soldCargo[ UnlockDecs[ currentLevel+1 ].ofCargo ] >= UnlockDecs[ currentLevel+1 ].fromSelling);
  // Tutorial
  if (getTutorialStage() < kTutBuildConveyor && UnlockDecs[currentLevel+1].type == kConveyor) {
    return false; // Need to progress the tutorial too to unlock conveyors
  }
  // Tutorial
  if (getTutorialStage() < kTutBuildQuarry && UnlockDecs[currentLevel+1].type == kExtractor && UnlockDecs[currentLevel+1].subType.extractor == kChalkQuarry) {
    return false; // Need to progress the tutorial too to unlock chalk quarry
  }
  // Tutorial
  if (getTutorialStage() < kTutBuildVitamin && UnlockDecs[currentLevel+1].type == kFactory) {
    return false; // Need to progress the tutorial too to unlock factory
  }

  return haveUnlocked;
}

bool checkShowNew() {
  struct Player_t* p = getPlayer();

  if (!checkHasNewToShow(p)) {
    return false;
  }

  const uint32_t testLevel = p->m_buildingsUnlockedTo + 1;
  m_nextBuilding = UnlockDecs[testLevel].type;
  m_nextSubType = UnlockDecs[testLevel].subType;
  p->m_buildingsUnlockedTo += 1;
  setGameMode(kMenuNew);
  sfx(kSfxUnlock);
  return true;
}

const char* getNewText() {
  switch(m_nextBuilding) {
    case kConveyor: switch (m_nextSubType.conveyor) {
      case kBelt: return "Moves Cargo North, South, East or West";
      case kSplitI: return  "Splits incoming cargo 2 ways (180 degrees)";
      case kSplitL: return "Splits incoming cargo 2 ways (90 degrees)";
      case kSplitT: return "Splits incoming cargo 3 ways";
      case kFilterI: return "Filters on the first Cargo to pass over";
      case kFilterL: return "Filters on the first Cargo to pass over";
      case kTunnelIn: case kTunnelOut: return "Moves Cargo underground for 1 tile";
      case kSpareConv0: case kSpareConv1: case kSpareConv2: case kSpareConv3:
      case kSpareConv4: case kSpareConv5: case kSpareConv6: case kSpareConv7:
      case kNConvSubTypes:;
    }
    case kPlant: switch (m_nextSubType.plant) {
      case kCarrotPlant: return "Plant to grow Carrots";
      case kSunflowerPlant: return "Plant to harvest Sunflowers";
      case kPotatoPlant: return "Plant to grow Potatoes";
      case kAppleTree: return "Plant to grow Apples";
      case kCornPlant: return "Plant to grow Corn";
      case kSeaweedPlant: return  "Seaweed grows in Lakes";
      case kWheatPlant: return "Plant to grow Wheat";
      case kCactusPlant: return "Plant to grow Cactus";
      case kLimeTree: return "Plant to grow Limes";
      case kStrawberryPlant:  return "Plant to grow Strawberries";  
      case kCocoaPlant: return "Plant to grow Coco Beans";
      case kCoffeePlant: return "Plant to grow Coffee Beans";
      case kBambooPlant: return "Plant to grow Bamboo Stalks";
      case kSeaCucumberPlant: return "Place in the Ocean to farm Sea Cucumbers";
      case kRosePlant: return "Tend to these Roses in your retirement.";
      case kSparePlant0: case kSparePlant1: case kSparePlant2: case kSparePlant3:
      case kSparePlant4: case kSparePlant5: case kSparePlant6: case kSparePlant7: 
      case kSparePlant8: case kSparePlant9: case kSparePlant10: case kSparePlant11:
      case kSparePlant12: case kSparePlant13: case kSparePlant14: case kSparePlant15: 
      case kNPlantSubTypes:;
    }
    case kUtility: switch (m_nextSubType.utility) {
      case kPath: return "Movement speed is enhanced while on a path";
      case kSign: return "Displays a Cargo of your choosing";
      case kBin: return "Cargo placed here will be destroyed";
      case kWell: return "Place to dampen the surrounding soil";
      case kStorageBox: return "Stores up to 3 different types of Cargo";
      case kBuffferBox: return "Like the Storage Box, but also self-empties";
      case kConveyorGrease: return "Upgrades Conveyor speed from x1 to x2";
      case kObstructionRemover: return "Clears Obstructed Ground allowing it to be built on";
      case kLandfill: return "Fills in water. Can built on, but not planted on.";
      case kRetirement: return "A relaxing space to enjoy your amassed wealth.";
      case kFence: return "A charming wooden fence.";
      case kSpareUtil0: case kSpareUtil1: case kSpareUtil2: case kSpareUtil3:
      case kSpareUtil4: case kSpareUtil5: case kSpareUtil6: case kSpareUtil7: 
      case kNUtilitySubTypes:;
    }
    case kExtractor: switch (m_nextSubType.extractor) {
      case kCropHarvesterSmall: return "Collects Cargo, can hold three different types";
      case kPump: return "Must be built on water. Produced Water Barrels";
      case kChalkQuarry: return "Must be built on Chalky Soil. Produces Chalk";
      case kSaltMine: return "Must be built on Peaty Soil. Produces Salt";
      case kCropHarvesterLarge: return "Collects Cargo, can hold three different types";
      case kCO2Extractor: return "Liquefies Carbon Dioxide right out of the air";
      case kSpareExtractor0: case kSpareExtractor1: case kSpareExtractor2: case kSpareExtractor3:
      case kSpareExtractor4: case kSpareExtractor5: case kSpareExtractor6: case kSpareExtractor7:
      case kNExtractorSubTypes:;
    }
    case kFactory: switch (m_nextSubType.factory) {
      case kVitiminFac: return "Manufactures low-grade Vitamin Pills";
      case kVegOilFac: return "Crush Vegetable Oil from Sunflowers";
      case kCrispsFac: return "Manufactures Potato Chips. A fried favorite!";
      case kEthanolFac: return "Brew & distill pure alcohol"; 
      case kHardCiderFac: return "Mix up some cheap Hard 'Cider'"; 
      case kAbattoir: return "Feed goes in, 'Meat' comes out";
      case kHydrogenFac: return "Splits water and captures the Hydrogen";
      case kHOILFac: return "Hydrogenates oil to prevent spoiling";
      case kCornDogFac: return "Crunchy and Meaty";
      case kBeerFac: return "Produces something which resembles beer";
      case kTequilaFac: return "At least it has some Agave in it";
      case kHFCSFac: return "So very, very, sweet...";
      case kGelatinFac: return "Used to set a variety of liquids";
      case kJellyFac: return "A wobbly favorite";
      case kJelloShotFac: return "An interesting mix of flavors";
      case kEmulsifierFac: return "Stops things from separating out";
      case kIceCreamFac: return "It's dairy free! Is that a good thing?";
      case kBatteryFarm: return "Corn goes in, Eggs come out";
      case kProteinFac: return "Maximum bulk, minimum fuss";
      case kChocolateFac: return "It's mostly sugar...";
      case kPieFac: return "What's for dinner? Meat Pie!";
      case kMSGFac: return "Makes everything taste better";
      case kTVDinnerFac: return "A meal for one, served in a tray";
      case kCaffeineFac: return "Extracts the caffeine from coffee beans";
      case kEnergyDrinkFac: return "Caffeine high & sugar rush all-in-one";
      case kRaveJuiceFac: return "... why not add alcohol too?";
      case kPerkPillFac: return "Will keep you awake far too long";
      case kCakeFac: return "Creamy, soft, and an infinite shelf life";
      case kDessertFac: return "Sweet tooth?";
      case kPackagingFac: return "Scaling up with custom packaging";
      case kCateringKitFac: return "Packaged Catering Kits";
      case kPartyPackFac: return "Everything a good party needs";
      case kParfumeFac: return "Fancy";
      case kMedicalSuplimentFac: return "Complete with dubious medical claims";
      case kTurkishDelightFac: return "A Delectable Delicacy";
      case kSpareFac0: case kSpareFac1: case kSpareFac2: case kSpareFac3:
      case kSpareFac4: case kSpareFac5: case kSpareFac6: case kSpareFac7: 
      case kSpareFac8: case kSpareFac9: case kSpareFac10: case kSpareFac11:
      case kSpareFac12: case kSpareFac13: case kSpareFac14: case kSpareFac15: 
      case kNFactorySubTypes:;
    }
    default: break;
  }
  return "??";
}