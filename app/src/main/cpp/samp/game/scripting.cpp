#include "../main.h"
#include "scripting.h"

int32_t ScriptCommand(const SCRIPT_COMMAND *command, ...) {

    va_list ap{};
    va_start(ap, command);

    CRunningScript GameScript;

    GameScript.SetOpcode(command->Opcode);

    const auto *parameter = command->Params;
    while (*parameter) {
        switch (*parameter) {

            case 'i': {
                int i = va_arg(ap, int);
                GameScript.AddIntParam(i);
                break;
            }

            case 'f': {
                float f = va_arg(ap, double);
                GameScript.AddFloatParam(f);
                break;
            }

            case 'v': {
                uintptr *v = va_arg(ap, uintptr*);
                GameScript.AddRefParam(v);
                break;
            }

            case 's': {
                char *str = va_arg(ap, char*);
                uint8_t len = strlen(str);
                GameScript.AddStringParam(str, len);
                break;
            }

            case 'z': {
                GameScript.AddZeroTerminator();
                break;
            }

            default: {
                return 0;
            }
        }

        ++parameter; // Next parameter
    }
    va_end(ap);

    auto result = GameScript.ProcessCommand();
    GameScript.RetrieveRefs();
    return result;
}

void InitScripting()
{


}