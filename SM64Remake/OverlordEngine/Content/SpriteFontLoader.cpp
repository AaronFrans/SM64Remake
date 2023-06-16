#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...

	auto identification0 = pReader->Read<byte>();
	auto identification1 = pReader->Read<byte>();
	auto identification2 = pReader->Read<byte>();

	if (identification0 != 66 || identification1 != 77 || identification2 != 70)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}
	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	auto versionByte = pReader->Read<byte>();
	if (versionByte < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;

	}



	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]
	//...

	auto blockIdentifier = pReader->Read<byte>();
	auto blockSize = pReader->Read<int>();


	fontDesc.fontSize = pReader->Read<short>();



	pReader->MoveBufferPosition(12);
	fontDesc.fontName = pReader->ReadNullString();


	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	//...

	blockIdentifier = pReader->Read<byte>();
	blockSize = pReader->Read<int>();

	pReader->MoveBufferPosition(4);

	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();


	auto pageCount = pReader->Read<short>();

	if (pageCount > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only one texture per font is allowed!");
		return nullptr;

	}



	pReader->MoveBufferPosition(5);





	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]


	blockIdentifier = pReader->Read<byte>();
	blockSize = pReader->Read<int>();

	auto pageName = pReader->ReadNullString();

	auto fullPath = loadInfo.assetFullPath.parent_path().append(pageName);

	fontDesc.pTexture = ContentManager::Load<TextureData>(fullPath);


	//**********
	// BLOCK 4 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	blockIdentifier = pReader->Read<byte>();
	blockSize = pReader->Read<int>();

	constexpr int charInfoSize{ 20 };

	const int nrChars{ blockSize / charInfoSize };

	for (size_t i = 0; i < nrChars; ++i)
	{
		auto charId = static_cast<wchar_t>(pReader->Read<int>());
		FontMetric metric{};

		metric.character = charId;
		short xPos{ pReader->Read<short>() };
		short yPos{ pReader->Read<short>() };
		metric.width = pReader->Read<short>();
		metric.height = pReader->Read<short>();
		metric.offsetX = pReader->Read<short>();
		metric.offsetY = pReader->Read<short>();
		metric.advanceX = pReader->Read<short>();
		metric.page = pReader->Read<byte>();

		auto channelByte = pReader->Read<byte>();
		switch (channelByte)
		{
		case 0X1:
			metric.channel = 2;
			break;
		case 0X2:
			metric.channel = 1;
			break;
		case 0X4:
			metric.channel = 0;
			break;
		case 0X8:
			metric.channel = 3;
			break;
		}


		metric.texCoord = { static_cast<float>(xPos) / fontDesc.textureWidth, static_cast<float>(yPos) / fontDesc.textureHeight };

		fontDesc.metrics[metric.character] = metric;
	}


	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
