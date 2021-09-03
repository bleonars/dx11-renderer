#include "renderer.h"
#include "vertex.h"

#include "vertex_shader.h"
#include "pixel_shader.h"

#include <DirectXMath.h>

using namespace dx;
using namespace DirectX;

void Renderer::perform() {
    // capture current render state
    if ( !m_render_state_backup.capture( m_dev_ctx ) )
        return;
    
    // set custom rendering settings
    set_custom_state();

    // draw the vertices
    flush();

    // reapply previous render state
    m_render_state_backup.apply();
}

bool Renderer::create( ID3D11Device *dev, ID3D11DeviceContext *dev_ctx ) {
    D3D11_BLEND_DESC blend_desc{};
    HRESULT          hr;

    if ( !dev || !dev_ctx )
        return false;

    m_dev_ctx = dev_ctx;
    m_dev     = dev;

    hr = m_dev->CreateVertexShader( vertex_shader.data(), sizeof( vertex_shader ), nullptr, &m_vertex_shader );
    if ( FAILED( hr ) ) 
        return false;

    hr = m_dev->CreatePixelShader( pixel_shader.data(), sizeof( pixel_shader ), nullptr, &m_pixel_shader );
    if ( FAILED( hr ) )
        return false;

    const std::array< D3D11_INPUT_ELEMENT_DESC, 2 > input_layout_desc = {
        D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
        D3D11_INPUT_ELEMENT_DESC{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_dev->CreateInputLayout( input_layout_desc.data(), input_layout_desc.size(), 
                                    vertex_shader.data(), sizeof( vertex_shader ), &m_input_layout );
    if ( FAILED( hr ) )
        return false;

    // initialize blend state
    blend_desc.RenderTarget->BlendEnable           = TRUE;
    blend_desc.RenderTarget->SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    blend_desc.RenderTarget->DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget->SrcBlendAlpha         = D3D11_BLEND_INV_DEST_ALPHA;
    blend_desc.RenderTarget->DestBlendAlpha        = D3D11_BLEND_ONE;
    blend_desc.RenderTarget->BlendOp               = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget->BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = m_dev->CreateBlendState( &blend_desc, &m_blend_state );
    if ( FAILED( hr ) )
        return false;

    // initialize vertex and index buffers
    if ( !allocate() )
        return false;

    // initialize projection matrix and buffer
    if ( !project() )
        return false;

    return true;
}

void Renderer::destroy() {
    m_vertex_shader->Release();
    m_pixel_shader->Release();
    m_input_layout->Release();
    m_blend_state->Release();
    m_vertex_buffer->Release();
    m_index_buffer->Release();
}

bool Renderer::allocate() {
    D3D11_BUFFER_DESC        vertex_buffer_desc{};
    D3D11_BUFFER_DESC        index_buffer_desc{};
    D3D11_MAPPED_SUBRESOURCE resource{};
    HRESULT                  hr;

    // initialize vertex buffer
    vertex_buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
    vertex_buffer_desc.ByteWidth      = sizeof( Vertex ) * m_max_vertices;
    vertex_buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertex_buffer_desc.MiscFlags      = 0;

    hr = m_dev->CreateBuffer( &vertex_buffer_desc, NULL, &m_vertex_buffer );
    if ( FAILED( hr ) )
        return false;

    // initialize index buffer
    index_buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
    index_buffer_desc.ByteWidth      = sizeof( size_t ) * m_max_indices;
    index_buffer_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
    index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    index_buffer_desc.MiscFlags      = 0;

    hr = m_dev->CreateBuffer( &index_buffer_desc, NULL, &m_index_buffer );
    if ( FAILED( hr ) )
        return false;

    return true;
}

bool Renderer::project() {
    D3D11_BUFFER_DESC        proj_buffer_desc{};
    XMMATRIX                 proj_matrix{};
    D3D11_MAPPED_SUBRESOURCE resource{};
    HRESULT                  hr;

    m_screen_size = get_screen_size();

    // initialize projection buffer
    proj_buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
    proj_buffer_desc.ByteWidth      = sizeof( XMMATRIX );
    proj_buffer_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    proj_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    proj_buffer_desc.MiscFlags      = 0;

    hr = m_dev->CreateBuffer( &proj_buffer_desc, NULL, &m_proj_buffer );
    if ( FAILED( hr ) )
        return false;

    // initialize projection matrix
    proj_matrix = XMMatrixOrthographicOffCenterLH(
        0.f, m_screen_size.x, m_screen_size.y, 0.f, 0.f, 1.f
    );

    hr = m_dev_ctx->Map( m_proj_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
    if ( FAILED( hr ) )
        return false;

    memcpy( resource.pData, &proj_matrix, sizeof( XMMATRIX ) );

    m_dev_ctx->Unmap( m_proj_buffer, 0 );

    return true;
}

void Renderer::flush() {
    D3D11_MAPPED_SUBRESOURCE resource{};
    size_t                   ind_idx{};

    // copy render list contents to vertex buffer
    m_dev_ctx->Map( m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
    memcpy( resource.pData, m_render_list.vertices().data(), sizeof( Vertex ) * m_render_list.vertices().size() );
    m_dev_ctx->Unmap( m_vertex_buffer, 0 );

    // copy render list contents to index buffer
    m_dev_ctx->Map( m_index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
    memcpy( resource.pData, m_render_list.indices().data(), sizeof( uint32_t ) * m_render_list.indices().size() );
    m_dev_ctx->Unmap( m_index_buffer, 0 );

    // draw batched indices/vertices
    for ( const auto &b : m_render_list.batches() ) {
        m_dev_ctx->IASetPrimitiveTopology( b.m_topology );
        m_dev_ctx->DrawIndexed( b.m_index_count, ind_idx, 0 );

        ind_idx += b.m_index_count;
    }

    m_render_list.clear();
}

void Renderer::set_custom_state() {
    // set shader
    m_dev_ctx->VSSetShader( m_vertex_shader, nullptr, 0 );
    m_dev_ctx->PSSetShader( m_pixel_shader, nullptr, 0 );

    // set blend state
    m_dev_ctx->OMSetBlendState( m_blend_state, nullptr, 0xffffffff );

    // set layout
    m_dev_ctx->IASetInputLayout( m_input_layout );

    // set buffers
    m_dev_ctx->VSSetConstantBuffers( 0, 1, &m_proj_buffer );
    m_dev_ctx->IASetVertexBuffers( 0, 1, &m_vertex_buffer, &VERTEX_BUFFER_STRIDE, &VERTEX_BUFFER_OFFSET );
    m_dev_ctx->IASetIndexBuffer( m_index_buffer, DXGI_FORMAT_R32_UINT, 0 );
}

Vector2 Renderer::get_screen_size() {
    D3D11_VIEWPORT viewport;
    size_t         num_viewports{ 1 };

    m_dev_ctx->RSGetViewports( &num_viewports, &viewport );

    return Vector2( viewport.Width, viewport.Height );
}

void Renderer::add_vertices( Vertex *vertex_array, const size_t vertex_count, uint32_t *index_array, const size_t index_count, D3D11_PRIMITIVE_TOPOLOGY topology ) {
    size_t idx_offset{};

    // retrive list contents
    auto &vertices = m_render_list.vertices();
    auto &indices  = m_render_list.indices();
    auto &batches  = m_render_list.batches();

    // add vertices to render list
    for ( size_t i{}; i < vertex_count; ++i )
        vertices.push_back( vertex_array[ i ] );

    // calculate index offset
    if ( !indices.empty() )
        idx_offset = *std::max_element( indices.begin(), indices.end() ) + 1;

    // add indices to index list
    for ( size_t i{}; i < index_count; ++i )
        indices.push_back( index_array[ i ] + idx_offset );

    // create new patch if needed
    if ( batches.empty() || batches.back().m_topology != topology )
        batches.push_back( { topology } );

    batches.back().m_vertex_count += vertex_count;
    batches.back().m_index_count  += index_count;
}

void Renderer::draw_line( const Vector2 &start, const Vector2 &end, const Color &color, const float thickness ) {
    // draw a pixel thick line
    if ( thickness <= 1.f ) {
        std::array< Vertex,   2 > vertices_thin;
        std::array< uint32_t, 2 > indices_thin;

        vertices_thin[ 0 ] = { { start.x, start.y, 0.f }, color };
        vertices_thin[ 1 ] = { { end.x,   end.y,   0.f }, color };

        indices_thin = { 0, 1 };

        add_vertices( vertices_thin.data(), vertices_thin.size(), indices_thin.data(), indices_thin.size(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
    }

    // draw a line with some thickness
    // https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader
    else {
        std::array< Vertex,   4 > vertices_thick;
        std::array< uint32_t, 6 > indices_thick;
        Vector2                   diff, norm;
        Vector2                   a, b, c, d;

        // calculate the normal vector of this line.
        diff = end - start;
        norm = Vector2( -diff.y, diff.x ).normalized();

        // calculate corners for quad vertices.
        a = start - norm * thickness;
        b = start + norm * thickness;
        c = end   - norm * thickness;
        d = end   + norm * thickness;

        vertices_thick[ 0 ] = { { a.x, a.y, 0.f }, color };
        vertices_thick[ 1 ] = { { b.x, b.y, 0.f }, color };
        vertices_thick[ 2 ] = { { c.x, c.y, 0.f }, color };
        vertices_thick[ 3 ] = { { d.x, d.y, 0.f }, color };

        indices_thick = { 0, 2, 3, 3, 1, 0 };

        add_vertices( vertices_thick.data(), vertices_thick.size(), indices_thick.data(), indices_thick.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }
};

void Renderer::draw_line( const float start_x, const float start_y, const float end_x, const float end_y, const Color &color, const float thickness ) {
    draw_line( { start_x, start_y }, { end_x, end_y }, color, thickness );
}

void Renderer::draw_filled_rect( const Vector2 &pos, const Vector2 &size, const Color &color ) {
    std::array< Vertex,   4 > vertices;
    std::array< uint32_t, 6 > indices;

    vertices[ 0 ] = { { pos.x,          pos.y,          0.f }, color };
    vertices[ 1 ] = { { pos.x + size.x, pos.y,          0.f }, color };
    vertices[ 2 ] = { { pos.x + size.x, pos.y + size.y, 0.f }, color };
    vertices[ 3 ] = { { pos.x,          pos.y + size.y, 0.f }, color };

    indices = { 0, 1, 2, 2, 3, 0 };

    add_vertices( vertices.data(), vertices.size(), indices.data(), indices.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Renderer::draw_filled_rect( const float x, const float y, const float w, const float h, const Color &color ) {
    draw_filled_rect( { x, y }, { w, h }, color );
}

void Renderer::draw_rect( const Vector2 &pos, const Vector2 &size, const Color &color, const float thickness ) {
    // bottom line.
    draw_filled_rect( pos.x, pos.y + size.y - thickness, size.x, thickness, color );

    // top line.
    draw_filled_rect( pos.x, pos.y, size.x, thickness, color );

    // left line.
    draw_filled_rect( pos.x, pos.y, thickness, size.y, color );

    // right line.
    draw_filled_rect( pos.x + size.x - thickness, pos.y, thickness, size.y, color );
}

void Renderer::draw_rect( const float x, const float y, const float w, const float h, const Color &color, const float thickness ) {
    draw_rect( { x, y }, { w, h }, color, thickness );
}

void Renderer::draw_outlined_filled_rect( const Vector2 &pos, const Vector2 &size, const Color &fill_color, const Color &outline_color ) {
    draw_filled_rect( pos.x, pos.y, size.x, size.y, fill_color );
    draw_rect( pos.x - 1.f, pos.y - 1.f, size.x + 2.f, size.y + 2.f, outline_color );
}

void Renderer::draw_outlined_filled_rect( const float x, const float y, const float w, const float h, const Color &fill_color, const Color &outline_color ) {
    draw_outlined_filled_rect( { x, y }, { w, h }, fill_color, outline_color );
}

void Renderer::draw_outlined_rect( const Vector2 &pos, const Vector2 &size, const Color &inner_color, const Color &outline_color ) {
    // outline
    draw_rect( pos.x - 1.f, pos.y - 1.f, size.x + 2.f, size.y + 2.f, outline_color );
    draw_rect( pos.x + 1.f, pos.y + 1.f, size.x - 2.f, size.y - 2.f, outline_color );

    // inner line
    draw_rect( pos.x, pos.y, size.x, size.y, inner_color );
}

void Renderer::draw_outlined_rect( const float x, const float y, const float w, const float h, const Color &inner_color, const Color &outline_color ) {
    draw_outlined_rect( { x, y }, { w, h }, inner_color, outline_color );
}

void Renderer::draw_circle( const Vector2 &pos, const float radius, const Color &color, const size_t segment_count ) {
    std::vector< Vertex > vertices;
    std::vector< size_t > indices;
    float                 angle;
    float                 x_pos, y_pos;

    for ( size_t i{}; i <= segment_count; ++i ) {
        angle = 2.f * std::numbers::pi * ( float ) i / ( float ) segment_count;

        x_pos = pos.x + ( radius * std::cos( angle ) );
        y_pos = pos.y + ( radius * std::sin( angle ) );

        vertices.push_back( { { x_pos, y_pos, 0.f },  color } );
        indices.push_back( i );
    }

    add_vertices( vertices.data(), vertices.size(), indices.data(), indices.size(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
}

void Renderer::draw_circle( const float x, const float y, const float radius, const Color &color, const size_t segment_count ) {
    draw_circle( { x, y }, radius, color, segment_count );
}

void Renderer::draw_filled_circle( const Vector2 &pos, const float radius, const Color &color, const size_t segment_count ) {
    std::vector< Vertex > vertices;
    std::vector< size_t > indices;
    float                 angle;
    float                 x_pos, y_pos;
    size_t                idx{};

    vertices.push_back( { { pos.x, pos.y, 0.f }, color } );

    for ( size_t i{}; i <= segment_count; ++i ) {
        angle = 2.f * std::numbers::pi * ( float ) i / ( float ) segment_count;

        x_pos = pos.x + ( radius * std::cos( angle ) );
        y_pos = pos.y + ( radius * std::sin( angle ) );

        vertices.push_back( { { x_pos, y_pos, 0.f },  color } );
        indices.insert( indices.end(), { 0, idx, ++idx } );
    }

    add_vertices( vertices.data(), vertices.size(), indices.data(), indices.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Renderer::draw_filled_circle( const float x, const float y, const float radius, const Color &color, const size_t segment_count ) {
    draw_filled_circle( { x, y }, radius, color, segment_count );
}

bool RenderStateBackup::capture( ID3D11DeviceContext *dev_ctx ) {
    if ( !dev_ctx )
        return false;

    return true;
}

void RenderStateBackup::apply() {

}