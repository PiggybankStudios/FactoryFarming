#include "export.h"
#include "../player.h"
#include "../ui.h"
#include "../sprite.h"
#include "../buildings/special.h"

/// ///

void doExport() {
  // noop
}



void populateInfoExport() {
  const uint16_t selectedID =  getUIContentID();
  updateExport();

  pd->sprite->setVisible(getCannotAffordSprite(), false);

  // INFO
  LCDBitmap* infoBitmap = getInfoBitmap();
  static char textA[128] = " ";
  static char textB[128] = " ";
  static char textC[128] = " ";
  static char strFloat[16] = " ";

  setRoobert10();
  pd->graphics->clearBitmap(infoBitmap, kColorClear);
  pd->graphics->pushContext(infoBitmap);
  roundedRect(1, TILE_PIX*18, TILE_PIX*2, TILE_PIX/2, kColorBlack);
  roundedRect(3, TILE_PIX*18, TILE_PIX*2, TILE_PIX/2, kColorWhite);
  snprintf(textA, 128, "Exporting: %s", toStringCargoByType(selectedID, /*plural=*/true));
  snprintf(textB, 128, "This Plot: %s/s", ftos(getThisWorldCargoExport(selectedID), 16, strFloat));
  snprintf(textC, 128, "All Plots: %s/s", ftos(getTotalCargoExport(selectedID), 16, strFloat));
  pd->graphics->setDrawMode(kDrawModeFillBlack);
  pd->graphics->drawText(textA, 128, kASCIIEncoding, 1*TILE_PIX, +2);
  pd->graphics->drawText(textB, 128, kASCIIEncoding, 1*TILE_PIX, TILE_PIX - 2);
  pd->graphics->drawText(textC, 128, kASCIIEncoding, 9*TILE_PIX, TILE_PIX - 2);
  pd->graphics->setDrawMode(kDrawModeCopy);
  pd->graphics->popContext();
  
}

bool populateContentExport() {
  updateExport();
  int16_t column = 0, row = 0;
  bool empty = true;
  setUIContentHeader(row, kUICatCargo);
  ++row;
  for (int32_t i = 0; i < getNSubTypes(kUICatCargo); ++i) {
    const float tot = getTotalCargoExport(i);
    if (!tot) {
      continue; // Am not exporting this type of cargo
    }
    if (column == ROW_WDTH) { // Only start a new row if we are sure we have something to put in it
      ++row;
      column = 0;
    }
    setUIContentItem(row, column++, kUICatCargo, i, 0);
    empty = false;
  }
  return empty;
}