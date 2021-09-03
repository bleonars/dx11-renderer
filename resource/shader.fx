cbuffer proj_buffer : register( b0 ) {
    matrix proj_matrix;
};

struct VS_Output_t {
    float4 m_pos : SV_POSITION;
    float4 m_col : COLOR;
};

struct VS_Input_t {
    float4 m_pos : POSITION;
    float4 m_col : COLOR;
};

VS_Output_t vertex_shader( VS_Input_t vs_in ) {
    VS_Output_t ret;

    ret.m_pos = mul( proj_matrix, float4( vs_in.m_pos.xy, 0.f, 1.f ) );
    ret.m_col = vs_in.m_col;

    return ret;
}

float4 pixel_shader( VS_Output_t ps_in ) : SV_TARGET {
    return ps_in.m_col;
}