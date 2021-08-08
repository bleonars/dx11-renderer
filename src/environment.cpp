#include "environment.h"
#include "color.h"

using namespace dx;

void Environment::create( HINSTANCE instance, int cmd_show ) {
    create_window( instance, cmd_show, WND_X, WND_Y, WND_WIDTH, WND_HEIGHT );

    create_directx();

    m_renderer.create();
}

void Environment::destroy() {
    m_renderer.destroy();

    destroy_directx();
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
            m_dev_ctx->ClearRenderTargetView( m_render_target, D3DXCOLOR( 0.f, 0.f, 0.f, 1.f ) );

            m_renderer.perform();

            m_swapchain->Present( 0, 0 );
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
    wnd_class.cbSize        = sizeof( WNDCLASSEX );
    wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc   = WindowProc;
    wnd_class.hInstance     = instance;
    wnd_class.hCursor       = LoadCursor( NULL, IDC_ARROW );
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

void Environment::create_directx() {
    DXGI_SWAP_CHAIN_DESC swapchain_desc;
    ID3D11Texture2D      *back_buffer;
    D3D11_VIEWPORT       viewport;

    // initialize swapchain
    ZeroMemory( &swapchain_desc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
    swapchain_desc.BufferCount       = 1;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow      = m_wnd;
    swapchain_desc.SampleDesc.Count  = 4;
    swapchain_desc.Windowed          = TRUE;

    // create swapchain, device, and device context
    D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, 
                                   D3D11_SDK_VERSION, &swapchain_desc, &m_swapchain, &m_dev,
                                   NULL, &m_dev_ctx );

    // retrieve backbuffer address 
    m_swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID * ) &back_buffer );

    // create backbuffer as render target
    m_dev->CreateRenderTargetView( back_buffer, NULL, &m_render_target );
    back_buffer->Release();

    m_dev_ctx->OMSetRenderTargets( 1, &m_render_target, NULL );

    // initialize viewport
    ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width    = 640;
    viewport.Height   = 480;

    m_dev_ctx->RSSetViewports( 1, &viewport );
}

void Environment::destroy_directx() {
    m_swapchain->Release();
    m_render_target->Release();
    m_dev_ctx->Release();
    m_dev->Release();
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