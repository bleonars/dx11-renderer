#pragma once

#include "includes.h"
#include "color.h"
#include "vector.h"

namespace dx {
    /**
     * @brief This class contains the directx Vertex container
    */
    class Vertex {
    public:
        /**
         * @brief The default constructor for the Vertex class
        */
        FORCEINLINE Vertex() : m_coordinates{}, m_color{} {

        }

        /**
         * @brief The pass-by-reference constructor
         * @param coordinates vector coordinates
         * @param color rgba color
        */
        FORCEINLINE Vertex( const Vector3 &coordinates, const Color &color ) : m_coordinates{ coordinates }, m_color{ color } {

        }
          
        /**
         * @brief This function initializes the vertex
         * @param coordinates vector coordinates
         * @param color rgba color
        */
        FORCEINLINE void init( const Vector3 &coordinates, const Color &color ) {
            m_coordinates = coordinates;
            m_color = color;
        }

        /**
         * @brief This function returns the vertex 3-dimensional coordinates
         * @return coordinate vector
        */
        FORCEINLINE Vector3 &coordinates() {
            return m_coordinates;
        }

        /**
         * @brief This functionr eturns the rgba color
         * @return rgba color
        */
        FORCEINLINE Color &color() {
            m_color;
        }

    private:
        Vector3 m_coordinates; // 3-dimensional coordinate vector
        Color   m_color;       // color rgba
    };
}