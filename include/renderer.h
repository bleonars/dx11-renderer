#pragma once

#include "includes.h"
#include "vertex.h"

namespace dx {
    /**
     * @brief This class contains the backup of the current DirectX 11 render state
    */
    class RenderStateBackup {
    public:
        /**
         * @brief The constructor for the RenderStateBackup class
        */
        NOINLINE RenderStateBackup() : m_dev_ctx{}, m_scissor_rects_count {}, m_viewports_count{}, m_scissor_rects{}, m_viewports{}, m_rasterizer_state{}, m_blend_state{},
            m_blend_factor{}, m_sample_mask{}, m_stencil_ref{}, m_depth_stencil_state{}, m_shader_resource{}, m_sampler{}, m_pixel_shader{}, m_vertex_shader{}, m_geometry_shader{},
            m_ps_instance_count{}, m_vs_instance_count{}, m_gs_instance_count{}, m_ps_instances{}, m_vs_instances{}, m_gs_instances{}, m_primitive_topology{}, 
            m_index_buffer{}, m_vertex_buffer{}, m_constant_buffer{}, m_index_buffer_offset{}, m_vertex_buffer_stride{}, m_vertex_buffer_offset{}, m_index_buffer_format{}, m_input_layout{} {

        }

        /**
         * @brief This function creates the backup of the current DirectX 11 render state
         * @param dev_ctx directx device context
         * @return true, if able to capture stateblock. false, otherwise
        */
        NOINLINE bool capture( ID3D11DeviceContext *dev_ctx );

        /**
         * @brief This function restores the stored backup of the current DirectX 11 render state
        */
        NOINLINE void apply();

    private:
        static constexpr size_t  MAX_D3D11_CLASS_INSTANCE = 256;

        ID3D11DeviceContext      *m_dev_ctx;

        size_t                   m_scissor_rects_count, m_viewports_count;
        D3D11_RECT               m_scissor_rects[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
        D3D11_VIEWPORT           m_viewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
        ID3D11RasterizerState    *m_rasterizer_state;
        ID3D11BlendState         *m_blend_state;
        float                    m_blend_factor[ 4 ];
        size_t                   m_sample_mask;
        size_t                   m_stencil_ref;
        ID3D11DepthStencilState  *m_depth_stencil_state;
        ID3D11ShaderResourceView *m_shader_resource;
        ID3D11SamplerState       *m_sampler;
        ID3D11PixelShader        *m_pixel_shader;
        ID3D11VertexShader       *m_vertex_shader;
        ID3D11GeometryShader     *m_geometry_shader;
        size_t                   m_ps_instance_count, m_vs_instance_count, m_gs_instance_count;
        ID3D11ClassInstance      *m_ps_instances[ MAX_D3D11_CLASS_INSTANCE ], *m_vs_instances[ MAX_D3D11_CLASS_INSTANCE ], *m_gs_instances[ MAX_D3D11_CLASS_INSTANCE ];
        D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology;
        ID3D11Buffer             *m_index_buffer, *m_vertex_buffer, *m_constant_buffer;
        size_t                   m_index_buffer_offset, m_vertex_buffer_stride, m_vertex_buffer_offset;
        DXGI_FORMAT              m_index_buffer_format;
        ID3D11InputLayout        *m_input_layout;
    };

    /**
     * @brief This struct holds the batch of topology, index and vertex counts
    */
    struct Batch_t {
        D3D11_PRIMITIVE_TOPOLOGY m_topology;     // primitive topology
        size_t                   m_vertex_count; // vertex count
        size_t                   m_index_count;  // index count

        /**
         * @brief This constructor initializes the batch with its topology, index and vertex count
         * @param topology primitive topology
         * @param vertex_count count of vertices
         * @param index_count count of indices
        */
        FORCEINLINE Batch_t( D3D11_PRIMITIVE_TOPOLOGY topology, const size_t vertex_count = 0, const size_t index_count = 0 ) : m_topology{ topology }, m_vertex_count{ vertex_count }, m_index_count{ index_count } {

        }
    };

    /**
     * @brief This class holds the render list of indices, vertices, and batches
    */
    class RenderList {
    public:
        /**
         * @brief This default constructor for the RenderList class
        */
        FORCEINLINE RenderList() : m_vertices{}, m_indices{}, m_batches{} {

        }

        /**
         * @brief This function clears the render list
        */
        FORCEINLINE void clear() {
            m_vertices.clear();
            m_indices.clear();
            m_batches.clear();
        }

        /**
         * @brief This function returns the underlying vertices
         * @return vertices vector
        */
        std::vector< Vertex > &vertices() {
            return m_vertices;
        }

        /**
         * @brief This function returns the underlying indices
         * @return indices vector
        */
        std::vector< uint32_t > &indices() {
            return m_indices;
        }

        /**
         * @brief This function returns the underlying batches
         * @return batches vector
        */
        std::vector< Batch_t > &batches() {
            return m_batches;
        }

    private:
        std::vector< Vertex   > m_vertices; // vertices
        std::vector< uint32_t > m_indices;  // indices
        std::vector< Batch_t  > m_batches;  // batches
    };

    /**
     * @brief This class contains the DirectX 11 renderer including its initialization,
     * destruction, and drawing functions
    */
    class Renderer {
    public:
        /**
         * @brief The constructor for the Renderer class
        */
        FORCEINLINE Renderer() : m_dev_ctx{}, m_dev{}, m_vertex_shader{}, m_pixel_shader{}, m_input_layout{}, 
            m_blend_state{}, m_vertex_buffer{}, m_index_buffer{}, m_proj_buffer{}, m_max_vertices{ MAX_VERTICES }, m_max_indices{ MAX_INDICES },
            m_screen_size{}, m_render_list{} {

        }

        /**
         * @brief This function creates the renderer environment
         * @param dev directx device
         * @param dev_ctx directx device context
         * @return true if created. false, otherwise
        */
        NOINLINE bool create( ID3D11Device *dev, ID3D11DeviceContext *dev_ctx );

        /**
         * @brief This function destroys the renderer enviroment
        */
        NOINLINE void destroy();

        /**
         * @brief This function performs the rendering
        */
        NOINLINE void perform();

        /**
         * @brief This function draws a line of specific thickness
         * @param start start position
         * @param end end position
         * @param color rgba color
         * @param thickness pixel thickness
        */
        NOINLINE void draw_line( const Vector2 &start, const Vector2 &end, const Color &color, const float thickness = 1.f );
        
        /**
         * @brief This function draws a line of specific thickness
         * @param start_x start x-position
         * @param start_y start y-position
         * @param end_x end x-position
         * @param end_y end y-position
         * @param color rgba color
         * @param thickness pixel thickness
        */
        NOINLINE void draw_line( const float start_x, const float start_y, const float end_x, const float end_y, const Color &color, const float thickness = 1.f );

        /**
         * @brief This function draws a filled rectangle
         * @param pos position
         * @param size dimensions
         * @param color rgba color
        */
        NOINLINE void draw_filled_rect( const Vector2 &pos, const Vector2 &size, const Color &color );

        /**
         * @brief This function draws a filled rectangle
         * @param x start x-position
         * @param y start y-position
         * @param w width
         * @param h height
         * @param color rgba color
        */
        NOINLINE void draw_filled_rect( const float x, const float y, const float w, const float h, const Color &color );

        /**
         * @brief This function draws a filled rectangle
         * @param pos position
         * @param size dimensions
         * @param color rgba color
         * @param thickness pixel thickness
        */
        NOINLINE void draw_rect( const Vector2 &pos, const Vector2 &size, const Color &color, const float thickness = 1.f );

        /**
         * @brief This function draws a filled rectangle
         * @param x start x-position
         * @param y start y-position
         * @param w width
         * @param h height
         * @param color rgba color
         * @param thickness pixel thickness
        */
        NOINLINE void draw_rect( const float x, const float y, const float w, const float h, const Color &color, const float thickness = 1.f );

        /**
         * @brief This function draws an outlined filled rectangle
         * @param pos position
         * @param size dimensions
         * @param fill_color inside color
         * @param outline_color border color
        */
        NOINLINE void draw_outlined_filled_rect( const Vector2 &pos, const Vector2 &size, const Color &fill_color, const Color &outline_color );

        /**
         * @brief This function draws an outlined filled rectangle
         * @param x start x-position
         * @param y start y-position
         * @param w width
         * @param h height
         * @param fill_color inside color
         * @param outline_color border color
        */
        NOINLINE void draw_outlined_filled_rect( const float x, const float y, const float w, const float h, const Color &fill_color, const Color &outline_color );

        /**
         * @brief This function draws an outlined rectangle
         * @param pos position
         * @param size dimensions
         * @param fill_color inside color
         * @param outline_color border color
        */
        NOINLINE void draw_outlined_rect( const Vector2 &pos, const Vector2 &size, const Color &inner_color, const Color &outline_color );

        /**
         * @brief This function draws an outlined rectangle
         * @param x start x-position
         * @param y start y-position
         * @param w width
         * @param h height
         * @param fill_color inside color
         * @param outline_color border color
        */
        NOINLINE void draw_outlined_rect( const float x, const float y, const float w, const float h, const Color &inner_color, const Color &outline_color );

        /**
         * @brief This function draws a circle
         * @param pos position
         * @param radius circle radius
         * @param color rgba color
         * @param segment_count number of circle segments
        */
        NOINLINE void draw_circle( const Vector2 &pos, const float radius, const Color &color, const size_t segment_count = 32 );

        /**
         * @brief This function draws a circle
         * @param x start x-position
         * @param y start y-position
         * @param radius circle radius
         * @param color rgba color
         * @param segment_count number of circle segments
        */
        NOINLINE void draw_circle( const float x, const float y, const float radius, const Color &color, const size_t segment_count = 32 );

        /**
         * @brief This function draws a filled circle
         * @param pos position
         * @param radius circle radius
         * @param color rgba color
         * @param segment_count number of circle segments
        */
        NOINLINE void draw_filled_circle( const Vector2 &pos, const float radius, const Color &color, const size_t segment_count = 32 );

        /**
         * @brief This function draws a filled circle
         * @param x start x-position
         * @param y start y-position
         * @param radius circle radius
         * @param color rgba color
         * @param segment_count number of circle segments
        */
        NOINLINE void draw_filled_circle( const float x, const float y, const float radius, const Color &color, const size_t segment_count = 32 );

    private:
        static constexpr size_t MAX_VERTICES = 1024; // max number of vertices
        static constexpr size_t MAX_INDICES  = 1024; // max number of indices

        static constexpr size_t VERTEX_BUFFER_STRIDE = sizeof( Vertex ); // stride of vertex buffer
        static constexpr size_t VERTEX_BUFFER_OFFSET = 0;                // offset of vertex buffer

        ID3D11DeviceContext *m_dev_ctx; // directx device context
        ID3D11Device        *m_dev;     // directx device

        ID3D11VertexShader *m_vertex_shader; // directx vertex shader
        ID3D11PixelShader  *m_pixel_shader;  // directx pixel shader
        ID3D11InputLayout  *m_input_layout;  // directx input layout
        ID3D11BlendState   *m_blend_state;   // directx blend state

        ID3D11Buffer *m_vertex_buffer; // vertex buffer
        ID3D11Buffer *m_index_buffer;  // index buffer
        ID3D11Buffer *m_proj_buffer;   // projection buffer

        size_t m_max_vertices; // max vertex count
        size_t m_max_indices;  // max index count

        Vector2 m_screen_size; // current screen size

        RenderStateBackup m_render_state_backup; // render state backup
        RenderList m_render_list;                // render list

        /**
         * @brief This function draws the batched vertices
        */
        NOINLINE void flush();

        /**
         * @brief This function sets the custom render state settings
        */
        NOINLINE void set_custom_state();
        
        /**
         * @brief This function allocates the directx vertex and index buffers
         * @return true, if allocated. false, otherwise
        */
        NOINLINE bool allocate();

        /**
         * @brief This function creates the directx projection matrix and allocates its buffer
         * @return true, if allocated. false, otherwise
        */
        NOINLINE bool project();

        /**
         * @brief This function retrieves the current screen size from viewport
         * @return viewport width and height
        */
        NOINLINE Vector2 get_screen_size();

        /**
         * @brief This function adds the vertices and indices to the render list
         * @param vertex_array primitive vertices
         * @param vertex_count number of vertices
         * @param index_array primitive indices
         * @param index_count number of indices
         * @param topology primitive topology
        */
        NOINLINE void add_vertices( Vertex *vertex_array, const size_t vertex_count, uint32_t *index_array, const size_t index_count, D3D11_PRIMITIVE_TOPOLOGY topolog );
    };
}
