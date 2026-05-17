//
// Created by x1y2z on 23.11.2023.
//

#include "NodeName.h"
#include "../vendor/armhook/patch.h"

const RwChar* GetFrameNodeName(RwFrame* frame) {
    //dangerous 32bit to be converted if use?
    //return CHook::CallFunction<const RwChar*>(g_libGTASA + 0x48248C + 1, frame);
}