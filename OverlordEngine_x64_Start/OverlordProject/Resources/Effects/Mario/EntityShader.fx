float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;


Texture2D gDiffuseMap;
Texture2D gOpacityMap;

Texture2D gShadowMap;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.color = input.color;
	output.texCoord = input.texCoord;
	return output;
}



float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
	float offset = float2(u,v) / float2(1280,780);
	return offset;
}

float EvaluateShadowMap(float4 lpos)
{
	lpos.xyz /= lpos.w;
 
    //if position is not visible to the light - dont illuminate it
    //results in hard light frustum
    if( lpos.x < -1.0f || lpos.x > 1.0f ||
        lpos.y < -1.0f || lpos.y > 1.0f ||
        lpos.z < 0.0f  || lpos.z > 1.0f ) return 1.f;
 
    //transform clip space coords to texture space coords (-1:1 to 0:1)
    lpos.x = lpos.x / 2.f + 0.5f;
    lpos.y = lpos.y /-2.f + 0.5f;
	lpos.z -= gShadowMapBias;
 
	//PCF sampling for shadow map
    	float sum = 0.f;
		int nrRuns = 2.f;
    	float x = 0.f, y = 0.f;
		int nrTexels = 0;
 
    //perform PCF filtering on a 4 x 4 texel neighborhood
    for (y = -nrRuns; y <= nrRuns; y += 0.5f)
    {
        for (x = -nrRuns; x <= nrRuns; x += 0.5f)
        {
            sum += gShadowMap.SampleCmpLevelZero( cmpSampler, lpos.xy + texOffset(x,y),lpos.z );
			++nrTexels;
        }
    }
	
    //sample shadow map - point sampler
    float shadowMapDepth = sum / nrTexels;

    //if clip space z value greater than shadow map value then pixel is in shadow
 
	return shadowMapDepth * 0.5f + 0.5f;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{


	float oppacity = gOpacityMap.Sample( samLinear,input.texCoord ).x;

	float4 color;
    if(oppacity == 1)
    {
    	color = gDiffuseMap.Sample( samLinear,input.texCoord );
    }
	else
	{
		color = input.color;
	}

	float3 color_rgb= color.rgb;
	float color_a = color.a;
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	return float4( color_rgb , color_a );
}
	

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
