#pragma once

#include "includes.h"

namespace dx {
    /**
     * @brief This class contains the directx Color container
    */
    class Color {
    public:
        /**
         * @brief The default constructor for the Color class
        */
        FORCEINLINE Color() : m_rgba{} {

        }

        /**
         * @brief The default copy constructor for Color class
         * @param color constant reference to instance of color class
        */
        FORCEINLINE Color( const Color &color ) : m_rgba{ color.m_rgba } {

        }

        /**
         * @brief The pass-by-reference constructor for the Color class
         * @param rgba array of rgba components
        */
        FORCEINLINE Color( const std::array< float, 4 > &rgba ) : m_rgba{ rgba } {

        }

        /**
         * @brief The value default constructor for the Color class
         * @param r red channel
         * @param g green channel
         * @param b blue channel
         * @param a alpha channel
        */
        FORCEINLINE Color( const float r, const float g, const float b, const float a = 1.f ) : m_rgba{ r, g, b, a } {

        }

        /**
         * @brief This operator sets this instance of color class equal to another
         * @param color other color instance constant refernece
         * @return this color instance
        */
        FORCEINLINE Color &operator = ( const Color &color ) {
            m_rgba = color.m_rgba;
            return *this;
        }

        /**
         * @brief This operator sets this instance of color class equal to an rgba array
         * @param rgba array of rgba componenets
         * @return this color instance
        */
        FORCEINLINE Color &operator = ( const std::array< float, 4 > &rgba ) {
            m_rgba = rgba;
            return *this;
        }

        /**
         * @brief This operator adds another color's rgba to this instance of color's rgba value
         * @param color other color instance
         * @return this color instance
        */
        FORCEINLINE Color &operator += ( const Color &color ) {
            *this += color.m_rgba;
            return *this;
        }

        /**
         * @brief This operator adds a rgba array to this instance of color's rgba value
         * @param rgba array of rgba components
         * @return this color instance
        */
        FORCEINLINE Color &operator += ( const std::array< float, 4 > &rgba ) {
            *this += rgba;
            return *this;
        }

        /**
         * @brief This operator subtracts a rgba value from instance of color's rgba
         * @param color other color instance
         * @return this color instance
        */
        FORCEINLINE Color &operator -= ( const Color &color ) {
            *this -= color.m_rgba;
            return *this;
        }

        /**
         * @brief This operator subtracts a rgba value from instance of color's rgba
         * @param rgba array of rgba components
         * @return this color instance
        */
        FORCEINLINE Color &operator -= ( const std::array< float, 4 > &rgba ) {
            *this -= rgba;
            return *this;
        }

        /**
         * @brief This operator adds a rgba array to a Color instance
         * @param rgba array of rgba components
         * @return sum of addition of Color and rgba array
        */
        FORCEINLINE Color operator + ( const std::array< float, 4 > &rgba ) {
            auto ret = *this;
            ret += rgba;
            return ret;
        }

        /**
         * @brief This operator adds a Color instance to a Color instance
         * @param color other color instance
         * @return sum of addition of two color instances
        */
        FORCEINLINE Color operator + ( const Color &color ) {
            auto ret = *this;
            ret += color;
            return ret;
        }

        /**
         * @brief This operator subtracts an argba array from a Color instance
         * @param rgba array of rgba components
         * @return difference of Color and rgba array
        */
        FORCEINLINE Color operator - ( const std::array< float, 4 > &rgba ) {
            auto ret = *this;
            ret -= rgba;
            return ret;
        }

        /**
         * @brief This operator subtracts a Color instance from a Color instance
         * @param color other color instance
         * @return difference of two color instances
        */
        FORCEINLINE Color operator - ( const Color &color ) {
            auto ret = *this;
            ret -= color;
            return ret;
        }

        /**
         * @brief This function checks if this color instance is equal to another
         * @param color other color instance
         * @return true if equal. false, otherwise.
        */
        FORCEINLINE bool operator == ( const Color &color ) {
            return ( m_rgba == color.m_rgba );
        }

        /**
         * @brief This function checks if this color instance is not equal to another
         * @param color other color instance
         * @return true if not equal. false, otherwise.
        */
        FORCEINLINE bool operator != ( const Color &color ) {
            return !( *this == color );
        }

        /**
         * @brief This function returns the desired rgba component
         * @param idx index of the rgba component
         * @return rgba component
        */
        FORCEINLINE float &at( const size_t idx ) {
            return m_rgba[ idx ];
        }

        /**
         * @brief This function returns the desired rgbacomponent
         * @param idx index of the rgba component
         * @return rgba component
        */
        FORCEINLINE float &operator [] ( const size_t idx ) {
            return m_rgba[ idx ];
        }

        /**
         * @brief This function returns the underlying rgba data
         * @return rgba data
        */
        FORCEINLINE float *data() {
            return m_rgba.data();
        }

        /**
         * @brief This function returns the color red
         * @return instance of red Color
        */
        FORCEINLINE static Color red() {
            return Color( 1.f, 0.f, 0.f );
        }

        /**
         * @brief This function returns the color gren
         * @return instance of green Color
        */
        FORCEINLINE static Color green() {
            return Color( 0.f, 1.f, 0.f );
        }

        /**
         * @brief This function returns the color blue
         * @return instance of blue Color
        */
        FORCEINLINE static Color blue() {
            return Color( 0.f, 0.f, 1.f );
        }

        /**
         * @brief This fucntion returns the color black
         * @return instance of black Color
        */
        FORCEINLINE static Color black() {
            return Color( 0.f, 0.f, 0.f );
        }

        /**
         * @brief This function returns the color white
         * @return instance of white Color
        */
        FORCEINLINE static Color white() {
            return Color( 1.f, 1.f, 1.f );
        }

        /**
         * @brief This function returns the color yellow
         * @return instance of yellow Color
        */
        FORCEINLINE static Color yellow() {
            return Color( 1.f, 1.f, 0.f );
        }

        /**
         * @brief This function returns the color orange
         * @return instance of orange Color
        */
        FORCEINLINE static Color orange() {
            return Color( 1.f, 0.5f, 0.f );
        }

        /**
         * @brief This function returns the color purple
         * @return instance of purple Color
        */
        FORCEINLINE static Color purple() {
            return Color( 255.f, 0.f, 255.f );
        }

    private:
        std::array< float, 4 > m_rgba; // rgba components
    };
}