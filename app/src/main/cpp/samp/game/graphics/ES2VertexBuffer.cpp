//
// Created on 25.05.2023.
//

#include "ES2VertexBuffer.h"
#include "vendor/armhook/patch.h"

void ES2VertexBuffer::InjectHooks() {
    CHook::Write(g_libGTASA + 0x84CFF8, &ES2VertexBuffer::curCPUBuffer);
}