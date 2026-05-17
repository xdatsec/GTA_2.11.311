//
// Created by x1y2z on 21.11.2023.
//

#include "TextureListingContainer.h"
#include "../vendor/armhook/patch.h"

RwRaster *TextureListingContainer::CreateRaster(const TextureDatabaseEntry *forEntry) {
    return CHook::CallFunction<RwRaster*>(g_libGTASA + 0x7950F4, this, forEntry);
}
