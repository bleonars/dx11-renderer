#include "environment.h"

using namespace dx;

void Environment::create( HINSTANCE instance, int cmd_show ) {
    create_window( instance, cmd_show, WND_X, WND_Y, WND_WIDTH, WND_HEIGHT );

    m_renderer.create();
}

void Environment::destroy() {
    m_renderer.destroy();
}

uint32_t Environment::perform() {
    MSG msg{};

    while ( true ) {
        // peek message and dispatch to windowproc
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

            if ( msg.message == WM_QUIT )
                break;
        }

        // perform rendering functions
        else {
            m_renderer.perform();
        }
    }

    return msg.wParam;
}

void Environment::create_window( HINSTANCE instance, int cmd_show, const size_t pos_x, const size_t pos_y,
                                 const size_t width, const size_t height ) {
    WNDCLASSEX wnd_class;
    RECT       dimensions;

    // initialize window
    ZeroMemory( &wnd_class, sizeof( WNDCLASSEX ) );
    wnd_class.cbSize = sizeof( WNDCLASSEX );
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc = WindowProc;
    wnd_class.hInstance = instance;
    wnd_class.hCursor = LoadCursor( NULL, IDC_ARROW );
    wnd_class.hbrBackground = ( HBRUSH ) COLOR_WINDOW;
    wnd_class.lpszClassName = L"env_wc";

    RegisterClassEx( &wnd_class );

    // adjust window dimensions
    dimensions = { 0, 0, ( long ) width, ( long ) height };
    AdjustWindowRect( &dimensions, WS_OVERLAPPEDWINDOW, FALSE );

    // create window
    m_wnd = CreateWindowEx( NULL, L"env_wc", L"dx11_renderer", WS_OVERLAPPEDWINDOW,
                            pos_x, pos_y, width, height, NULL, NULL, instance, NULL );

    // display window
    ShowWindow( m_wnd, cmd_show );
}

LRESULT __stdcall Environment::WindowProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
    switch ( msg ) {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;

    default:
        break;
    }

    return DefWindowProc( wnd, msg, wparam, lparam );
}