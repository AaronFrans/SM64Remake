//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    const float4 sampledColor = gTexture.Sample(samPoint, input.TexCoord);

    const float4 underwaterColor = float4(0.0f, 0.2f, 0.4f, 1.0f);
    const float4 tintedColor = sampledColor * underwaterColor;

    const float depth = 0.5f;
    const float fogDensity = 0.2f;
    const float4 fogColor = float4(0.4f, 0.7f, 1.0f, 1.0f);
    
    const float4 fog = fogColor * (1.0f - exp(-depth * fogDensity));

    // Combine the tinted color and fog
    return tintedColor + fog;
}


//TECHNIQUE
//---------
technique11 Grayscale
{
    pass P0
    {          
        // Set states...
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

