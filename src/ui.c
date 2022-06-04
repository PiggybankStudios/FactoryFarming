#include "ui.h"
#include "sprite.h"
#include "player.h"
#include "input.h"
#include "cargo.h"
#include "building.h"

uint16_t m_UIIcons[] = {0,3,  0,4,  4,3,  4,4,  1,1,  2,1,  1,2};

LCDSprite* m_UISpriteBottom;

LCDSprite* m_UISpriteRight;

LCDSprite* m_UISpriteTop;

LCDBitmap* m_UIBitmapBottom;

LCDBitmap* m_UIBitmapRight;

LCDBitmap* m_UIBitmapTop;

bool m_UIDirtyBottom = false;

bool m_UIDirtyRight = false;

int16_t m_UISelectedID = 0;

int16_t m_UISelectedRotation = 0;

void drawUIBottom(void);

void drawUIRight(void);

const char* getRotationAsString(void);

/// ///

void UIDirtyBottom() {
  m_UIDirtyBottom = true;
}

void UIDirtyRight() {
  m_UIDirtyRight = true;
}

uint16_t getUISelectedID() {
  return m_UISelectedID;
}

uint16_t getUISelectedRotation() {
  return m_UISelectedRotation;
}

void modUISelectedID(bool _increment) {
  if (_increment) {
    m_UISelectedID = (m_UISelectedID == UI_ITEMS-1 ? 0 : m_UISelectedID + 1);
  } else {
    m_UISelectedID = (m_UISelectedID == 0 ? UI_ITEMS-1 : m_UISelectedID - 1);
  }
  UIDirtyRight();
}

void modUISelectedRotation(bool _increment) {
  if (_increment) {
    m_UISelectedRotation = (m_UISelectedRotation == kConvDirN-1 ? 0 : m_UISelectedRotation + 1);
  } else {
    m_UISelectedRotation = (m_UISelectedRotation == 0 ? kConvDirN-1 : m_UISelectedRotation - 1);
  }
  UIDirtyRight();
}

void updateUI(int _fc, enum kGameMode _gm) {
  
  if (_gm == kMenuSelect && _fc % (TICK_FREQUENCY/4) == 0) {
    // Flashing cursor
    UIDirtyRight();
  } else if (_gm == kMenuOptionSelected && _fc % (TICK_FREQUENCY/4) == 0) {
    // Flashing blueprint 
    pd->sprite->setVisible(getPlayer()->m_blueprint[getZoom()], _fc % (TICK_FREQUENCY/2) < TICK_FREQUENCY/4);
  } else if (_gm ==kWander && _fc % FAR_TICK_FREQUENCY == 0) {
    // Update bottom ticker
    UIDirtyBottom();
  }

  if (m_UIDirtyRight) {
    m_UIDirtyRight = false;
    drawUIRight();
  }
  if (m_UIDirtyBottom) {
    m_UIDirtyBottom = false;
    drawUIBottom();
  }
}

void updateBlueprint() {
  uint8_t zoom = getZoom();
  if (getGameMode() == kMenuOptionSelected) {
    switch (m_UISelectedID) {
      case kMenuConveyor:; pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UISelectedRotation+0, 3, zoom), kBitmapUnflipped); break;
      case kMenuSplitI:;   pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UISelectedRotation+0, 4, zoom), kBitmapUnflipped); break;
      case kMenuSplitL:;   pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UISelectedRotation+4, 3, zoom), kBitmapUnflipped); break;
      case kMenuSplitT:;   pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UISelectedRotation+4, 4, zoom), kBitmapUnflipped); break;     
      case kMenuApple:;    pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UIIcons[8], m_UIIcons[9], zoom), kBitmapUnflipped); break;     
      case kMenuCheese:;   pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UIIcons[10], m_UIIcons[11], zoom), kBitmapUnflipped); break;     
      case kMenuBin:;      pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(m_UIIcons[12], m_UIIcons[13], zoom), kBitmapUnflipped); break;     
    }
  } else { // Clear blueprint
    pd->sprite->setImage(getPlayer()->m_blueprint[zoom], getSprite16(0, 0, zoom), kBitmapUnflipped); 
  }
}

void addUIToSpriteList() {
  pd->sprite->addSprite(m_UISpriteBottom);
  pd->sprite->addSprite(m_UISpriteTop);
  pd->sprite->addSprite(m_UISpriteRight);
}

const char* getRotationAsString() {
  switch (m_UISelectedID) {
    case kMenuConveyor:;
      switch (m_UISelectedRotation) {
        case 0: return "N";
        case 1: return "E";
        case 2: return "S";
        case 3: return "W";
      }
    case kMenuSplitI:;
      switch (m_UISelectedRotation) {
        case 0: return "W, E";
        case 1: return "N, S";
        case 2: return "W, E";
        case 3: return "N, S";
      }
    case kMenuSplitL:;
      switch (m_UISelectedRotation) {
        case 0: return "N, E";
        case 1: return "E, S";
        case 2: return "S, W";
        case 3: return "W, N";
      }
    case kMenuSplitT:;
      switch (m_UISelectedRotation) {
        case 0: return "W, N, E";
        case 1: return "N, E, S";
        case 2: return "E, S, W";
        case 3: return "S, W, N";
      }
  }
  return "!";
}

void drawUIBottom() {
  static char text[128];
  pd->graphics->pushContext(m_UIBitmapBottom);
  pd->graphics->fillRect(0, 0, DEVICE_PIX_X, TILE_PIX, kColorBlack);
  setRoobert10();
  pd->graphics->setDrawMode(kDrawModeFillWhite);
  const enum kGameMode gm = getGameMode();
  if (gm == kWander) {
    static bool mode = true;
    if (pd->system->getElapsedTime() > 3.0f) {
      mode = !mode;
      pd->system->resetElapsedTime();
    }
    if (mode) {
      snprintf(text, 128, "Conveyors:%u", getNConveyors());
      pd->graphics->drawText(text, 128, kASCIIEncoding, 0, 0);
      snprintf(text, 128, "Cargo:%u", getNCargo());
      pd->graphics->drawText(text, 128, kASCIIEncoding, 7*TILE_PIX, 0);
      snprintf(text, 128, "Sprite List:%u", pd->sprite->getSpriteCount());
      pd->graphics->drawText(text, 128, kASCIIEncoding, 12*TILE_PIX, 0);
    } else {
      snprintf(text, 128, "Near Ticks:%u", getNearTickCount());
      pd->graphics->drawText(text, 128, kASCIIEncoding, 0, 0);
      snprintf(text, 128, "Far Ticks:%u", getFarTickCount());
      pd->graphics->drawText(text, 128, kASCIIEncoding, 7*TILE_PIX, 0);
    }

  } else if (gm == kMenuSelect || gm == kMenuOptionSelected) {
    switch (m_UISelectedID) {
      case kMenuConveyor:; snprintf(text, 128, "Conveyor Belt (%s)", getRotationAsString()); break;
      case kMenuSplitI:;   snprintf(text, 128, "'I' Conveyor Splitter (%s)", getRotationAsString()); break;
      case kMenuSplitL:;   snprintf(text, 128, "'L' Conveyor Splitter (%s)", getRotationAsString()); break;
      case kMenuSplitT:;   snprintf(text, 128, "'T' Conveyor Splitter (%s)", getRotationAsString()); break;
      case kMenuApple:;    snprintf(text, 128, "Apples, put them on a conveyor"); break;
      case kMenuCheese:;   snprintf(text, 128, "Cheese (Gouda), put it on a conveyor?"); break;
      case kMenuBin:;      snprintf(text, 128, "Remove conveyors & cargo"); break;
    }
    pd->graphics->drawText(text, 128, kASCIIEncoding, TILE_PIX, 0);
  }
  pd->graphics->popContext();
}

void drawUIRight() {
  pd->graphics->pushContext(m_UIBitmapRight);
  pd->graphics->fillRect(0, 0, TILE_PIX, DEVICE_PIX_Y, kColorBlack);
  const enum kGameMode gm = getGameMode();
  for (uint32_t i = 0; i < UI_ITEMS; ++i) {
    const uint16_t y = TILE_PIX*2*i + TILE_PIX;
    const uint16_t offset = i >= kMenuApple ? 0 : getUISelectedRotation();
    pd->graphics->drawBitmap(getSprite16(m_UIIcons[i*2] + offset, m_UIIcons[(i*2)+1], 1), 0, y, kBitmapUnflipped);
    if ((gm == kMenuSelect || gm == kMenuOptionSelected) && i == getUISelectedID()) {
      LCDBitmapDrawMode m = ((gm == kMenuSelect && getFrameCount() % (TICK_FREQUENCY/2) < TICK_FREQUENCY/4) ? kDrawModeInverted : kDrawModeCopy );
      pd->graphics->setDrawMode(m);
      pd->graphics->drawBitmap(getSprite16(0, 2, 1), 0, y, kBitmapUnflipped);
      pd->graphics->setDrawMode(kDrawModeCopy);
    }
  }
  pd->graphics->popContext();
  UIDirtyBottom();
}

void drawUITop(const char* _text) {
  pd->sprite->setVisible(m_UISpriteTop, !(_text == NULL) );
  if (_text == NULL) {
    return;
  }
  pd->graphics->pushContext(m_UIBitmapTop);
  pd->graphics->setLineCapStyle(kLineCapStyleRound);
  pd->graphics->drawLine(TILE_PIX, TILE_PIX, SCREEN_PIX_X/2 - TILE_PIX, TILE_PIX, TILE_PIX*2, kColorBlack);
  pd->graphics->setDrawMode(kDrawModeFillWhite);
  setRoobert24();
  pd->graphics->drawText(_text, 16, kASCIIEncoding, TILE_PIX, 0);
  pd->graphics->popContext();
}

void initiUI() {
  m_UISpriteTop = pd->sprite->newSprite();
  m_UISpriteBottom = pd->sprite->newSprite();
  m_UISpriteRight = pd->sprite->newSprite();

  
  m_UIBitmapTop = pd->graphics->newBitmap(SCREEN_PIX_X/2, TILE_PIX*2, kColorClear);
  m_UIBitmapBottom = pd->graphics->newBitmap(DEVICE_PIX_X, TILE_PIX, kColorBlack);
  m_UIBitmapRight = pd->graphics->newBitmap(TILE_PIX, DEVICE_PIX_Y, kColorBlack);


  PDRect boundTop = {.x = 0, .y = 0, .width = SCREEN_PIX_X/2, .height = TILE_PIX*2};
  pd->sprite->setBounds(m_UISpriteTop, boundTop);
  pd->sprite->setImage(m_UISpriteTop, m_UIBitmapTop, kBitmapUnflipped);
  pd->sprite->moveTo(m_UISpriteTop, SCREEN_PIX_X/2, TILE_PIX*2);
  pd->sprite->setZIndex(m_UISpriteTop, Z_INDEX_UI);
  pd->sprite->setIgnoresDrawOffset(m_UISpriteTop, 1);
  pd->sprite->setVisible(m_UISpriteTop, 1);

  PDRect boundBottom = {.x = 0, .y = 0, .width = DEVICE_PIX_X, .height = TILE_PIX};
  pd->sprite->setBounds(m_UISpriteBottom, boundBottom);
  pd->sprite->setImage(m_UISpriteBottom, m_UIBitmapRight, kBitmapUnflipped);
  pd->sprite->moveTo(m_UISpriteBottom, SCREEN_PIX_X + TILE_PIX/2, DEVICE_PIX_Y/2);
  pd->sprite->setZIndex(m_UISpriteBottom, Z_INDEX_UI);
  pd->sprite->setIgnoresDrawOffset(m_UISpriteBottom, 1);

  PDRect boundRight = {.x = 0, .y = 0, .width = DEVICE_PIX_X, .height = TILE_PIX};
  pd->sprite->setBounds(m_UISpriteRight, boundRight);
  pd->sprite->setImage(m_UISpriteRight, m_UIBitmapBottom, kBitmapUnflipped);
  pd->sprite->moveTo(m_UISpriteRight, DEVICE_PIX_X/2, SCREEN_PIX_Y + TILE_PIX/2);
  pd->sprite->setZIndex(m_UISpriteRight, Z_INDEX_UI);
  pd->sprite->setIgnoresDrawOffset(m_UISpriteRight, 1);

  UIDirtyBottom();
  UIDirtyRight();
}