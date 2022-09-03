#include "input.h"
#include "location.h"
#include "player.h"
#include "sprite.h"
#include "render.h"
#include "building.h"
#include "sound.h"
#include "ui.h"
#include "io.h"
#include "buildings/special.h"
#include "buildings/utility.h"
#include "ui/inventory.h"
#include "ui/sell.h"
#include "ui/shop.h"
#include "ui/warp.h"
#include "ui/settings.h"
#include "ui/import.h"
#include "ui/export.h"
#include "ui/new.h"

uint8_t m_pressed[4] = {0};

uint8_t m_zoom = 1;

uint8_t m_pickRadius = 3;

int8_t  m_followConveyor = 0;

bool characterMoveInput(uint32_t _buttonPressed);

void clickHandleWander(uint32_t _buttonPressed);

void clickHandleMenuMain(uint32_t _buttonPressed);

void clickHandleMenuBuy(uint32_t _buttonPressed);

void clickHandleMenuNew(uint32_t _buttonPressed);

void clickHandleMenuSell(uint32_t _buttonPressed);

void clickHandleMenuWarp(uint32_t _buttonPressed);

void clickHandleMenuWarp(uint32_t _buttonPressed);

void clickHandleMenuImport(uint32_t _buttonPressed);

void clickHandleMenuExport(uint32_t _buttonPressed);

void clickHandleMenuPlayer(uint32_t _buttonPressed);

void clickHandleMenuCredits(uint32_t _buttonPressed);

void clickHandleBuilding(uint32_t _buttonPressed);

void clickHandlePlanting(uint32_t _buttonPressed);

void clickHandlePick(uint32_t _buttonPressed);

void clickHandleInspect(uint32_t _buttonPressed);

void clickHandleDestroy(uint32_t _buttonPressed);

void clickHandleTitles(uint32_t _buttonPressed);

void rotateHandleWander(float _rotation);

void rotateHandlePlacement(float _rotation);

void rotateHandlePick(float _rotation);

void rotateHandleCredits(float _rotation);

void rotateHandleSettings(float _rotation);

void rotateHandleMultiplier(float _rotation);

bool holdBRotateInput(uint32_t _buttonPressed);

bool holdBRadiusInput(uint32_t _buttonPressed);

bool holdBMultiplierInput(uint32_t _buttonPressed);

void toggleZoom(void);

void modRadius(bool _inc);

uint8_t m_b;

/// ///

void modRadius(bool _inc) {
  sfx(kSfxRotate);
  if (_inc) {
    m_pickRadius += 2;
    if (m_pickRadius == 7) m_pickRadius = 1;
  } else {
    if (m_pickRadius == 1) m_pickRadius = 7;
    m_pickRadius -= 2;
  }
  updateRenderList();
  updateBlueprint();
}

uint8_t getZoom() {
  return m_zoom;
}

uint8_t getRadius() {
 return m_pickRadius;
}

uint8_t getPressed(uint32_t _i) {
  return m_pressed[_i];
}

bool bPressed() {
  return m_b;
}

void unZoom() {
  m_zoom = 1;
}

void toggleZoom() {
  sfx(kSfxRotate); // TODO own sound?
  if (++m_zoom == ZOOM_LEVELS) {
    m_zoom = 1;
  }
  updateRenderList();
  updateBlueprint();
}

bool characterMoveInput(uint32_t _buttonPressed) {
  if (kButtonLeft == _buttonPressed) m_pressed[0] = 1;
  else if (kButtonRight == _buttonPressed) m_pressed[1] = 1;
  else if (kButtonUp == _buttonPressed) m_pressed[2] = 1;
  else if (kButtonDown == _buttonPressed) m_pressed[3] = 1;
  else return false;
  return true;
}

bool holdBRotateInput(uint32_t _buttonPressed) {
  if (kButtonLeft == _buttonPressed) setCursorRotation(3);
  else if (kButtonRight == _buttonPressed) setCursorRotation(1);
  else if (kButtonUp == _buttonPressed) setCursorRotation(0);
  else if (kButtonDown == _buttonPressed) setCursorRotation(2);
  else return false;
  return true;
}

bool holdBRadiusInput(uint32_t _buttonPressed) {
  if (kButtonUp == _buttonPressed) modRadius(true);
  else if (kButtonDown == _buttonPressed) modRadius(false);
  else return false;
  return true;
}

bool holdBMultiplierInput(uint32_t _buttonPressed) {
  if (kButtonUp == _buttonPressed) modMultiplier(true);
  else if (kButtonDown == _buttonPressed) modMultiplier(false);
  else return false;
  return true;
}

void gameClickConfigHandler(uint32_t _buttonPressed) {
  switch (getGameMode()) {
    case kWanderMode: return clickHandleWander(_buttonPressed);
    case kMenuSettings: return clickHandleMenuMain(_buttonPressed);
    case kMenuBuy: return clickHandleMenuBuy(_buttonPressed);
    case kMenuNew: return clickHandleMenuNew(_buttonPressed);
    case kMenuSell: return clickHandleMenuSell(_buttonPressed);
    case kMenuPlayer: return clickHandleMenuPlayer(_buttonPressed);
    case kMenuWarp: return clickHandleMenuWarp(_buttonPressed);
    case kMenuExport: return clickHandleMenuExport(_buttonPressed);
    case kMenuImport: return clickHandleMenuImport(_buttonPressed);
    case kMenuCredits: return clickHandleMenuCredits(_buttonPressed);
    case kPlaceMode: case kBuildMode: return clickHandleBuilding(_buttonPressed);
    case kPlantMode: return clickHandlePlanting(_buttonPressed);
    case kPickMode: return clickHandlePick(_buttonPressed);
    case kInspectMode: return clickHandleInspect(_buttonPressed);
    case kDestroyMode: return clickHandleDestroy(_buttonPressed);
    case kTitles: return clickHandleTitles(_buttonPressed);
    case kNGameModes: break;
  }
}

void clickHandleWander(uint32_t _buttonPressed) {
  if (characterMoveInput(_buttonPressed)) { /*noop*/ }
  else if (kButtonA == _buttonPressed) {
    // 254: tutorial finised, 255: tutorial disabled
    const bool ic = isCamouflaged();
    if (getTutorialStage() < TUTORIAL_FINISHED && checkReturnDismissTutorialMsg()) { /*noop*/ } // NOTE: The second function call has side-effects
    else if (distanceFromBuy() < ACTIVATE_DISTANCE) { 
      if (!checkShowNew()) { // has own sfx
        setGameMode(kMenuBuy);
        sfx(kSfxMenuOpen);
      } 
    } else if (distanceFromSell() < ACTIVATE_DISTANCE) {
      setGameMode(kMenuSell);
      sfx(kSfxMenuOpen);
    } else if (!ic && distanceFromWarp() < ACTIVATE_DISTANCE) {
      setGameMode(kMenuWarp);
      sfx(kSfxMenuOpen);
    } else if (!ic && distanceFromOut() < ACTIVATE_DISTANCE) {
      setGameMode(kMenuExport);
      sfx(kSfxMenuOpen);
    } else if (!ic && distanceFromIn() < ACTIVATE_DISTANCE) {
      setGameMode(kMenuImport);
      sfx(kSfxMenuOpen);
    } else if (!ic && distanceFromRetirement() < ACTIVATE_DISTANCE) {
      setGameMode(kMenuCredits);
      redrawAllSettingsMenuLines();
      chooseMusic(N_MUSIC_TRACKS);
    } else {
      setGameMode(kMenuPlayer);
      sfx(kSfxMenuOpen);
    }
  } else if (kButtonB == _buttonPressed) {
    if (getTutorialStage() < TUTORIAL_FINISHED) checkReturnDismissTutorialMsg();
  }

  //else if (kButtonB == _buttonPressed) toggleZoom(); // Press B to change zoom?
  // Hold down B to go faster?
}

void clickHandleTitles(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) {
    sfx(kSfxA);
    setSave(getTitleCursorSelected());
    if (hasSaveData(getTitleCursorSelected())) {
      setForceSlot(-1); // -1 loads from the slot stored in the player's save file
      doIO(kDoLoad, /*and then*/ kDoNothing);
    } else {
      setForceSlot(0); // This will be the ID of the world we generate
      doIO(kDoResetPlayer, /*and then*/ kDoNewWorld);
    }
  } else if (kButtonB == _buttonPressed) {
    sfx(kSfxNo);
  } else if (kButtonLeft == _buttonPressed) {
    modTitleCursor(false);
  } else if (kButtonRight == _buttonPressed) {
    modTitleCursor(true);
  }
}

void clickHandleMenuMain(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) {
    sfx(kSfxA);
    doSettings();
  } else if (kButtonB == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
  } else {
    moveCursor(_buttonPressed);
  }
}

void clickHandleMenuBuy(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) {
    doPurchace();
  } else if (bPressed() && holdBMultiplierInput(_buttonPressed)) {
    // noop
  } else if (kButtonB == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
    // Tutorial
    if (getTutorialStage() == kTutWelcomeBuySeeds && getTutorialProgress() >= 10) {
      nextTutorialStage();
    }
  } else {
    moveCursor(_buttonPressed);
  }
}

void clickHandleMenuNew(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed || kButtonB == _buttonPressed) {
    if (kButtonA == _buttonPressed) sfx(kSfxA);
    else sfx(kSfxB);
    if (!checkShowNew()) {
      setGameMode(kMenuBuy);
    }
  }
}

void clickHandleMenuSell(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) {
    // Has own sfx
    doSale();
  } else if (bPressed() && holdBMultiplierInput(_buttonPressed)) {
    // noop
  } else if (kButtonB == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
    // Tutorial
    if (getTutorialStage() == kTutSellCarrots && getTutorialProgress() >= 10) {
      nextTutorialStage();
    }
  } else {
    moveCursor(_buttonPressed);
  }
}

void clickHandleMenuPlayer(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) { sfx(kSfxA); doInventoryClick(); }
  else if (bPressed() && holdBRotateInput(_buttonPressed)) { /* noop */ }
  else if (kButtonB == _buttonPressed) { sfx(kSfxB); setGameMode(kWanderMode); }
  else moveCursor(_buttonPressed);
}

void clickHandleMenuWarp(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) doWarp(); // has own sfx
  else if (kButtonB == _buttonPressed) { sfx(kSfxB); setGameMode(kWanderMode); }
  else moveCursor(_buttonPressed);
}

void clickHandleMenuExport(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) { sfx(kSfxA); doExport(); }
  else if (kButtonB == _buttonPressed) { sfx(kSfxB); setGameMode(kWanderMode); }
  else moveCursor(_buttonPressed);
}

void clickHandleMenuImport(uint32_t _buttonPressed) {
  if (kButtonA == _buttonPressed) { sfx(kSfxA); doImport(); }
  else if (kButtonB == _buttonPressed) { sfx(kSfxB); setGameMode(kWanderMode); }
  else moveCursor(_buttonPressed);
}

void clickHandleBuilding(uint32_t _buttonPressed) {
  if (bPressed() && holdBRotateInput(_buttonPressed)) {
    // noop
  } else if (characterMoveInput(_buttonPressed)) {
    // noop
  } else if (kButtonA    == _buttonPressed) {
    doPlace(); // Sfx handled internally
  } else if (kButtonB    == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
    updateBlueprint();
    // Tutorial
    if (getTutorialStage() == kTutPlantCarrots && getTutorialProgress() >= 10) {
      nextTutorialStage();
    }
    // Tutorial
    if (getTutorialStage() == kTutBuildHarvester && getTutorialProgress()) {
      nextTutorialStage();
    }
    // Tutorial 
    if (getTutorialStage() == kTutBuildQuarry && getTutorialProgress()) {
      nextTutorialStage();
    }
  }
}

void clickHandlePlanting(uint32_t _buttonPressed) {
if (characterMoveInput(_buttonPressed)) {
    // noop
  } else if (kButtonA    == _buttonPressed) {
    doPlace(); // sfx handled internally
  } else if (kButtonB    == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
    updateBlueprint();
    // Tutorial
    if (getTutorialStage() == kTutPlantCarrots && getTutorialProgress() >= 10) {
      nextTutorialStage();
    }
  }
}

void clickHandlePick(uint32_t _buttonPressed) {
  if (bPressed() && holdBRadiusInput(_buttonPressed)) {
    /*noop*/
  } else if (characterMoveInput(_buttonPressed)) {
    // noop
  } else if (kButtonA    == _buttonPressed) {
    doPick(); // Sfx handled internally
  } else if (kButtonB    == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
    // Tutorial
    if (getTutorialStage() == kTutGetCarrots && getTutorialProgress() >= 10) {
      nextTutorialStage();
    }
  }
}

void clickHandleInspect(uint32_t _buttonPressed) {
  if (characterMoveInput(_buttonPressed)) { /*noop*/} 
  // Note: The tutorial and inspect boxes share the samme UI component
  else if (kButtonA == _buttonPressed) { checkReturnDismissTutorialMsg(); setGameMode(kWanderMode); sfx(kSfxA); }
  else if (kButtonB == _buttonPressed) { checkReturnDismissTutorialMsg(); setGameMode(kWanderMode); sfx(kSfxB); }
}

void clickHandleDestroy(uint32_t _buttonPressed) {
  if (bPressed() && holdBRadiusInput(_buttonPressed)) { /*noop*/ }
  else if (characterMoveInput(_buttonPressed)) { /*noop*/ }
  else if (kButtonA == _buttonPressed) doDestroy(); // sfx handled internally
  else if (kButtonB == _buttonPressed) { setGameMode(kWanderMode); sfx(kSfxB); }
}

void clickHandleMenuCredits(uint32_t _buttonPressed) {
  if (kButtonB == _buttonPressed) {
    sfx(kSfxB);
    setGameMode(kWanderMode);
  }
}

void rotateHandleWander(float _rotation) {
  // Follow conveyor
  if (_rotation) {
    struct Location_t* loc = getPlayerLocation();
    if (loc && loc->m_building && loc->m_building->m_type == kConveyor) {
      if (_rotation > 0) m_followConveyor = 16;
      else               m_followConveyor = -16;
      return;
    }
  }

  // Zoom control
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION) {
    sfx(kSfxRotate); // TODO different here?
    rot = 0.0f;
    if (++m_zoom == ZOOM_LEVELS) m_zoom = ZOOM_LEVELS-1;
    #ifdef DEV
    pd->system->logToConsole("ZOOM IN");
    #endif
    updateRenderList();
    updateBlueprint();
  } else if (rot < -UI_ROTATE_ACTION) {
    sfx(kSfxRotate); // TODO different here?
    rot = 0.0f;
    #ifdef DEV
    pd->system->logToConsole("ZOOM OUT");
    #endif
    if (--m_zoom == 0) m_zoom = 1;
    updateRenderList();
    updateBlueprint();
  }
}

void rotateHandlePlacement(float _rotation) {
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION) {
    rot = 0.0f;
    rotateCursor(true);
  } else if (rot < -UI_ROTATE_ACTION) {
    rot = 0.0f;
    rotateCursor(false);
  }
}

void rotateHandlePick(float _rotation) {
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION) {
    rot = 0.0f;
    modRadius(true);
  } else if (rot < -UI_ROTATE_ACTION) {
    rot = 0.0f;
    modRadius(false);
  }
}

void rotateHandleMultiplier(float _rotation) {
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION) {
    rot = 0.0f;
    modMultiplier(true);
  } else if (rot < -UI_ROTATE_ACTION) {
    rot = 0.0f;
    modMultiplier(false);
  }
}

void rotateHandleCredits(float _rotation) {
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION/8) {
    rot = 0.0f;
    modCredits(true);
  } else if (rot < -UI_ROTATE_ACTION/8) {
    rot = 0.0f;
    modCredits(false);
  }
}

void rotateHandleSettings(float _rotation) {
  static float rot = 0.0f;
  rot += _rotation;
  if (rot > UI_ROTATE_ACTION/8) {
    rot = 0.0f;
    moveCursor(kButtonRight);
  } else if (rot < -UI_ROTATE_ACTION/8) {
    rot = 0.0f;
    moveCursor(kButtonLeft);
  }
}

int8_t getAndReduceFollowConveyor() {
  if (m_followConveyor > 0) {
    return m_followConveyor--;
  } else if (m_followConveyor < 0) {
    return m_followConveyor++;
  }
  return false;
}

// Temporary until the playdate eventHandler is functional for inputs
void clickHandlerReplacement() {
  static uint8_t multiClickCount = 16, multiClickNext = 16;
  enum kGameMode gm = getGameMode();
  PDButtons current, pushed, released = 0;
  pd->system->getButtonState(&current, &pushed, &released);
  if (pushed & kButtonUp) gameClickConfigHandler(kButtonUp);
  if (pushed & kButtonRight) gameClickConfigHandler(kButtonRight);
  if (pushed & kButtonDown) gameClickConfigHandler(kButtonDown);
  if (pushed & kButtonLeft) gameClickConfigHandler(kButtonLeft);
  if (current & kButtonB) ++m_b;
  if (released & kButtonB) {
    if (m_b < BUTTON_PRESSED_FRAMES) gameClickConfigHandler(kButtonB);
    m_b = 0;
  }
  if (released & kButtonA) {
    gameClickConfigHandler(kButtonA);
    multiClickCount = 8;
    multiClickNext = 8;
  } else if (current & kButtonA)  {
    if (gm == kPlaceMode || gm == kPickMode || gm == kPlantMode || gm == kDestroyMode) {
      gameClickConfigHandler(kButtonA); // Special, allow pick/placing rows of conveyors
    } else if (gm >= kMenuBuy) {
      if (--multiClickCount == 0) {
        gameClickConfigHandler(kButtonA); // Special, allow speed buying/selling
        if (multiClickNext > 2) --multiClickNext;
        multiClickCount = multiClickNext;
      }
    }
  }

  if (released & kButtonLeft) m_pressed[0] = 0;
  if (released & kButtonRight) m_pressed[1] = 0;
  if (released & kButtonUp) m_pressed[2] = 0;
  if (released & kButtonDown) m_pressed[3] = 0;

  switch (gm) {
    case kWanderMode: rotateHandleWander(pd->system->getCrankChange()); break;
    case kBuildMode: case kPlaceMode: rotateHandlePlacement(pd->system->getCrankChange()); break;
    case kPickMode: case kDestroyMode: rotateHandlePick(pd->system->getCrankChange()); break;
    case kMenuCredits: rotateHandleCredits(pd->system->getCrankChange()); break;
    case kMenuBuy: case kMenuSell: rotateHandleMultiplier(pd->system->getCrankChange()); break; 
    case kMenuPlayer: case kMenuSettings: case kMenuExport: case kMenuImport: case kMenuWarp: rotateHandleSettings(pd->system->getCrankChange()); break;
    case kMenuNew: case kPlantMode: case kInspectMode: case kTitles: case kNGameModes: break;
  }

}