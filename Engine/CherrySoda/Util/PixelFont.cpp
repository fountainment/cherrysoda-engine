#include <CherrySoda/Util/PixelFont.h>

#include <CherrySoda/Graphics/Atlas.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::PixelFont;
using cherrysoda::PixelFontCharacter;
using cherrysoda::PixelFontSize;

using cherrysoda::Atlas;
using cherrysoda::JsonUtil;
using cherrysoda::MTexture;
using cherrysoda::String;
using cherrysoda::StringUtil;


const PixelFontSize PixelFont::AddFontSize(const String& path, Atlas* atlas/* = nullptr*/, bool outline/* = false*/)
{
	cherrysoda::json::Document json;
	JsonUtil::ReadJsonFile(json, path);
	return AddFontSize(path, &json, atlas, outline);
}

const PixelFontSize PixelFont::AddFontSize(const String& path, const cherrysoda::json::Value* jsonValue, Atlas* atlas/* = nullptr*/, bool outline/* = false*/)
{
	// check if size already exists
	auto data = jsonValue->GetObject();
	float size = data["info"].GetObject()["size"].GetFloat();
	for (auto& fs : m_sizes) {
		if (fs.Size() == size) {
			return fs;
		}
	}

	// get textures
	STL::Vector<MTexture> textures;
	const auto& pages = data["pages"].GetArray();
	for (const auto& page : pages) {
		String file(page["file"].GetString());
		String atlasPath = StringUtil::Path_GetFileNameWithoutExtension(file);
		if (atlas != nullptr && atlas->Has(atlasPath)) {
			STL::Add(textures, (*atlas)[atlasPath]);
		}
		else {
			String dir = StringUtil::Path_GetDirectoryName(path);
			STL::Add(textures, MTexture::FromFile(dir + file));
		}
	}

	// create font size
	PixelFontSize fontSize;
	fontSize.m_textures = textures;
	fontSize.m_lineHeight = data["common"].GetObject()["lineHeight"].GetInt();
	fontSize.m_size = size;
	fontSize.m_outline = outline;

	// get characters
	const auto& chars = data["chars"].GetArray(); 
	for (const auto& character : chars) {
		cherrysoda::type::UInt32 id = character["id"].GetInt();
		int page = character["page"].GetInt();
		STL::Add(fontSize.m_characters, STL::MakePair(id, PixelFontCharacter(id, textures[page], &character)));
	}

	// get kerning
	if (data.HasMember("kerning") && data["kerning"].IsArray()) {
		// TODO: add kerning reading
	}

	// add font size
	STL::Add(m_sizes, fontSize);
	STL::Sort(m_sizes, [](const auto& a, const auto& b){ return a.Size() < b.Size(); });

	return fontSize;
}

const PixelFontSize PixelFont::Get(float size) const
{
	for (auto& fontSize : m_sizes) {
		if (fontSize.Size() >= size) {
			return fontSize;
		}
	}
	return STL::Back(m_sizes);
}
