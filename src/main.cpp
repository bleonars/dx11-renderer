#include "includes.h"
#include "environment.h"

int __stdcall WinMain( HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show ) {
    dx::Environment env;
    uint32_t        ret{};

    // create directx 11 environment
    env.create( instance, cmd_show );

    // run application loop
    ret = env.perform();

    // destroy environment
    env.destroy();

    return ret;
}