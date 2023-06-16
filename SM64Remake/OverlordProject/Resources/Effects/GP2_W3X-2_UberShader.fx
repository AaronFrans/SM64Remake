/*
******************
* DAE Ubershader *
******************

**This Shader Contains:

- Diffuse (Texture & Color)
	- Regular Diffuse
- Specular
	- Specular Level (Texture & Value)
	- Shininess (Value)
	- Models
		- Blinn
		- Phong
- Ambient (Color)
- EnvironmentMapping (CubeMap)
	- Reflection + Fresnel Falloff
	- Refraction
- Normal (Texture)
- Opacity (Texture & Value)

-Techniques
	- WithAlphaBlending
	- WithoutAlphaBlending
*/

//GLOBAL MATRICES
//***************
// The World View Projection Matrix
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
// The ViewInverse Matrix - the third row contains the camera position!
float4x4 gMatrixViewInverse : VIEWINVERSE;
// The World Matrix
float4x4 gMatrixWorld : WORLD;



//STATES
//******
RasterizerState gRS_FrontCulling 
{ 
	CullMode = FRONT; 
};

BlendState gBS_EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

//SAMPLER STATES
//**************
SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//LIGHT
//*****
float3 gLightDirection : DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.577f, 0.577f, 0.577f);

//DIFFUSE
//*******
bool gUseTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Bool";
> = false;

float4 gColorDiffuse
<
	string UIName = "Diffuse Color";
	string UIWidget = "Color";
> = float4(1, 1, 1, 1);

Texture2D gTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Texture";
>;

//SPECULAR
//********
float4 gColorSpecular
<
	string UIName = "Specular Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Texture";
>;

bool gUseTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Bool";
> = false;

int gShininess
<
	string UIName = "Shininess";
	string UIWidget = "Slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1f;
> = 15;

//AMBIENT
//*******
float4 gColorAmbient
<
	string UIName = "Ambient Color";
	string UIWidget = "Color";
> = float4(0,0,0,1);

float gAmbientIntensity
<
	string UIName = "Ambient Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 0.0f;

//NORMAL MAPPING
//**************
bool gFlipGreenChannel
<
	string UIName = "Flip Green Channel";
	string UIWidget = "Bool";
> = false;

bool gUseTextureNormal
<
	string UIName = "Normal Mapping";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureNormal
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
> ;

//ENVIRONMENT MAPPING
//*******************
TextureCube gCubeEnvironment
<
	string UIName = "Environment Cube";
	string ResourceType = "Cube";
>;

bool gUseEnvironmentMapping
<
	string UIName = "Environment Mapping";
	string UIWidget = "Bool";
> = false;

float gReflectionStrength
<
	string UIName = "Reflection Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionStrength
<
	string UIName = "Refraction Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionIndex
<
	string UIName = "Refraction Index";
>  = 0.3f;

//FRESNEL FALLOFF
//***************
bool gUseFresnelFalloff
<
	string UIName = "Fresnel FallOff";
	string UIWidget = "Bool";
> = false;


float4 gColorFresnel
<
	string UIName = "Fresnel Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

float gFresnelPower
<
	string UIName = "Fresnel Power";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0f;

float gFresnelMultiplier
<
	string UIName = "Fresnel Multiplier";
	string UIWidget = "slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0;

float gFresnelHardness
<
	string UIName = "Fresnel Hardness";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 0;

//OPACITY
//***************
float gOpacityIntensity
<
	string UIName = "Opacity Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 1.0f;

bool gTextureOpacityIntensity
<
	string UIName = "Opacity Map";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureOpacity
<
	string UIName = "Opacity Map";
	string UIWidget = "Texture";
>;


//SPECULAR MODELS
//***************
bool gUseSpecularBlinn
<
	string UIName = "Specular Blinn";
	string UIWidget = "Bool";
> = false;

bool gUseSpecularPhong
<
	string UIName = "Specular Phong";
	string UIWidget = "Bool";
> = false;

bool gUseLight
<
	string UIName = "Use Light";
	string UIWidget = "Bool";
> = true;


//Shadow
//***********

Texture2D gShadowMap;
float4x4 gWorldViewProj_Light;

float gShadowMapBias = 0.01f;
SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};



//VS IN & OUT
//***********
struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition: COLOR0;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
	float4 lPos : TEXCOORD1;
};

float3 CalculateSpecularBlinn(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specularColor = float3(0,0,0);
	
	if (gUseTextureSpecularIntensity)
	{
		float3 halfVec = normalize(gLightDirection + -viewDirection);
		float dotResult = saturate(dot(halfVec, normal));
		specularColor = pow(dotResult, gShininess) *
		gTextureSpecularIntensity.Sample(gTextureSampler, texCoord);
		
	}
	return specularColor * gColorSpecular;
}

float Phong(float ks, float exp, float3 l, float3 v, float3 n)
{
	float3 reflectedLightVector = reflect(l,n);
	float reflectedViewDot = saturate(dot(reflectedLightVector, v));
	float phong = ks * pow(reflectedViewDot, exp);

	return phong;
}

float3 CalculateSpecularPhong(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specularColor = float3(0,0,0);
	if (gUseTextureSpecularIntensity)
	{
	  float phongExp = gShininess;
	  specularColor = Phong(1.0f, phongExp, -gLightDirection, viewDirection, normal) *
	  gTextureSpecularIntensity.Sample(gTextureSampler, texCoord); 
	  
	}
	
	return specularColor * gColorSpecular;
}

float3 CalculateSpecular(float3 viewDirection, float3 normal, float2 texCoord)
{
	float3 specColor = CalculateSpecularBlinn(viewDirection, normal, texCoord) * gUseSpecularBlinn +
					   CalculateSpecularPhong(viewDirection, normal, texCoord) * gUseSpecularPhong;
				
	return specColor;
}

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	
	if(gUseTextureNormal)
	{
	
		float3 binormal = cross(normal, tangent);
		if(gFlipGreenChannel)
			binormal = -binormal;
	
		float4x4 tangentSpaceAxis = float4x4(
			float4(tangent, 0.0f),
			float4(binormal, 0.0f),
			float4(normal, 0.0f),
			float4(0.0f, 0.0f, 0.0f, 1.0f)
		);

		float3 currentNormalMap = 2.0f * gTextureNormal.Sample(gTextureSampler, texCoord).rgb - float3(1.0f, 1.0f, 1.0f);


		return mul(float4(currentNormalMap, 0.0f), tangentSpaceAxis) ;
	}
	else
	{
		return 	normal;
	}
}

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float diffuseStrength = 1;
	if(gUseLight)
	{
		float diffuseStrength = saturate(dot(gLightDirection, normal));
	}

	float3 diffColor = gColorDiffuse * diffuseStrength;

	if(gUseTextureDiffuse)
	 	diffColor *= gTextureDiffuse.Sample(gTextureSampler, texCoord).rgb;
	
	return diffColor;
}

float3 CalculateFresnelFalloff(float3 normal, float3 viewDirection, float3 environmentColor)
{
	float3 fresnelFalloff = gUseEnvironmentMapping * environmentColor;
	
	if(gUseFresnelFalloff)
	{
		float fresnel = (pow(1- saturate(abs(dot(normal, viewDirection))), gFresnelPower)) * gFresnelMultiplier;
		float fresnelMask = pow(1 - saturate(dot(float3(0, -1, 0), normal)), gFresnelHardness);
		if (gUseEnvironmentMapping)
			fresnelFalloff = fresnel * fresnelMask * environmentColor;
		else
			fresnelFalloff = fresnel * fresnelMask * gColorFresnel;
	}
	
	return fresnelFalloff;
	
}

float3 CalculateEnvironment(float3 viewDirection, float3 normal)
{
	float3 environment = float3(0,0,0);
	
	if(gUseEnvironmentMapping)
	{
		
		float3 reflectVec = reflect(viewDirection, normal);
	 	environment += gReflectionStrength * gCubeEnvironment.Sample(gTextureSampler, reflectVec).rgb;
		
		float3 refractVec = refract(viewDirection, normal, gRefractionIndex);
        environment += gRefractionStrength * gCubeEnvironment.Sample(gTextureSampler, refractVec).rgb;
	}
	
	return environment;
}

float CalculateOpacity(float2 texCoord)
{
	float opacity = gOpacityIntensity;
	if(gTextureOpacityIntensity)
	{
		opacity *= gTextureOpacity.Sample(gTextureSampler, texCoord);
	}
	
	return opacity;
}

// The main vertex shader
VS_Output MainVS(VS_Input input) {
	
	VS_Output output = (VS_Output)0;
	
	output.Position = mul(float4(input.Position, 1.0), gMatrixWVP);
	output.WorldPosition = mul(float4(input.Position,1.0), gMatrixWorld);
	output.Normal = mul(input.Normal, (float3x3)gMatrixWorld);
	output.Tangent = mul(input.Tangent, (float3x3)gMatrixWorld);
	output.TexCoord = input.TexCoord;
	output.lPos = mul(float4(input.Position, 1.0), gWorldViewProj_Light);
	
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
		int nrRuns = 1.0f;
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

// The main pixel shader
float4 MainPS(VS_Output input) : SV_TARGET {
	
	//OPACITY
	float opacity = CalculateOpacity(input.TexCoord);
	clip(opacity - 0.3f);


	float shadowValue = EvaluateShadowMap(input.lPos);

	// NORMALIZE
	input.Normal = normalize(input.Normal);
	input.Tangent = normalize(input.Tangent);
	
	float3 viewDirection = normalize(input.WorldPosition.xyz - gMatrixViewInverse[3].xyz);
	
	//NORMAL
	float3 newNormal = CalculateNormal(input.Tangent, input.Normal, input.TexCoord);
		
	//SPECULAR
	float3 specColor = CalculateSpecular(viewDirection, newNormal, input.TexCoord);
		
	//DIFFUSE
	float3 diffColor = CalculateDiffuse(newNormal, input.TexCoord);
		
	//AMBIENT
	float3 ambientColor = gColorAmbient * gAmbientIntensity;
		
	//ENVIRONMENT MAPPING
	float3 environmentColor = CalculateEnvironment(viewDirection, newNormal);
	
	//FRESNEL FALLOFF
	environmentColor = CalculateFresnelFalloff(newNormal, viewDirection, environmentColor);
		
	//FINAL COLOR CALCULATION
	float3 finalColor = diffColor + specColor + environmentColor + ambientColor;
	

	
	return float4(finalColor * shadowValue, opacity);
}

// Default Technique
technique10 WithAlphaBlending {
	pass p0 {
		SetBlendState(gBS_EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

// Default Technique
technique10 WithoutAlphaBlending {
	pass p0 {
		SetRasterizerState(NULL);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
