#pragma once

#include "includes.h"
#include "renderer.h"

namespace dx {
    /**
     * @brief This class contains the DirectX 11 environment
    */
    class Environment {
    public:
        /**
         * @brief The constructor for the Environment class
        */
        FORCEINLINE Environment() : m_wnd{}, m_swapchain{}, m_render_target{}, m_dev_ctx{}, m_dev{}, m_renderer{} {

        }

        /**
         * @brief This function creates the directx environment and initializes the renderer
         * @param instance current application instance
         * @param cmd_show current application flags
        */
        NOINLINE void create( HINSTANCE instance, int cmd_show );

        /**
         * @brief This function destroys the directx environment and frees the renderer's resources
        */
        NOINLINE void destroy();

        /**
         * @brief This function performs the tasks of the main environment loop and handles the renderering
         * @return exit message status
        */
        NOINLINE uint32_t perform();

    private:
        /**
         * @brief window
        */
        static constexpr size_t WND_X = 200; // window start x-pos
        static constexpr size_t WND_Y = 200; // window start y-poz

        static constexpr size_t WND_WIDTH  = 640; // window start width
        static constexpr size_t WND_HEIGHT = 480; // window start height

        HWND m_wnd; // window handle

        /**
         * @brief directx
        */
        IDXGISwapChain         *m_swapchain;     // directx swapchain interface
        ID3D11RenderTargetView *m_render_target; // directx rendertarget
        ID3D11DeviceContext    *m_dev_ctx;       // directx device context
        ID3D11Device           *m_dev;           // directx device
       
        /**
         * @brief renderer
        */
        Renderer m_renderer; // directx renderer

        /**
         * @brief This function creates the win32 window for the directx environment
         * @param instance current application instance
         * @param cmd_show current application flags
         * @param pos_x window starting x position
         * @param pos_y window starting y position
         * @param width window starting width
         * @param height window starting height
        */
        NOINLINE void create_window( HINSTANCE instance, int cmd_show, const size_t pos_x, const size_t pos_y,
                                     const size_t width, const size_t height );

        /**
         * @brief This function creates the directx rendering utilities
        */
        NOINLINE void create_directx();

        /**
         * @brief This function destroys the directx rendering utilities 
        */
        NOINLINE void destroy_directx();

        /**
         * @brief This function handles the window callback messages
         * @param wnd current window instance
         * @param msg current processed message
         * @param wparam first additional parameter
         * @param lparam second additional parameter
         * @return result of message processing
        */
        static LRESULT __stdcall WindowProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
    };
}