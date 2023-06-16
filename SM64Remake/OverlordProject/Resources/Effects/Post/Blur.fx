//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
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
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	uint textureWidth = 0;
	uint textureHeight = 0;
	uint numberOfLevels = 0;

	gTexture.GetDimensions(0, textureWidth, textureHeight,numberOfLevels); 

	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float2 dx = 1.0f / textureWidth;
	float2 dy = 1.0f / textureHeight;

	// Step 3: Create a double for loop (5 iterations each)
	float2 offset = float2(0.0f, 0.0f);
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	int numPasses = 0;

	for (int i = -2; i <= 2; ++i )
	{
		for (int j = -2; j <= 2; ++j)
		{
			// Inside the loop, calculate the offset in each direction
			offset.x = dx * i;
			offset.y = dy * j;

			// Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
			finalColor += gTexture.Sample(samPoint, input.TexCoord + offset).rgb; // Replace TextureSampler with the appropriate sampler state
			numPasses++;
		}
	}

	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	finalColor /= float(numPasses);

	// Step 5: return the final color
	return float4(finalColor, 1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}