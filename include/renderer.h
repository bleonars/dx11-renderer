#pragma once

#include "includes.h"

namespace dx {
    /**
     * @brief This class contains the DirectX 11 renderer including its initialization,
     * destruction, and drawing functions
    */
    class Renderer {
    public:
        /**
         * @brief The constructor for the Renderer class
        */
        FORCEINLINE Renderer() {

        }

        /**
         * @brief 
         * @return 
        */
        NOINLINE void create();

        /**
         * @brief 
         * @return 
        */
        NOINLINE void destroy();

        /**
         * @brief 
         * @return 
        */
        NOINLINE void perform();

    private:

    };
}