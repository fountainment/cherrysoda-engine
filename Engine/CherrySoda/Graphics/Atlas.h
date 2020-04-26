#ifndef _CHERRYSODA_GRAPHICS_ATLAS_H_
#define _CHERRYSODA_GRAPHICS_ATLAS_H_

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Json.h>

namespace cherrysoda {

class Atlas
{
public:
	enum class AtlasDataFormat
	{
		CrunchJson,
		CrunchBinary,
		CrunchJsonOrBinary,
		CrunchBinaryNoAtlas,
		Packer,
		PackerNoAtlas
	};

	Atlas()
	{}

	const STL::List<MTexture> GetAtlasSubtextures(const String& key, int startIndex = 0, int keyLength = 4)
	{
		STL::List<MTexture> list;

		if (!STL::TryGetValue(m_orderedTexturesCache, key, list)) {
			int index = startIndex;
			for (;;) {
				MTexture texture = GetAtlasSubtextureFromAtlasAt(key, index, keyLength);
				if (texture.IsValid())
					STL::Add(list, texture);
				else
					break;
				++index;
			}

			m_orderedTexturesCache[key] = list;
		}

		return list;		
	}

	const MTexture GetAtlasSubtextureFromAtlasAt(const String& key, int index = 0, int keyLength = 4)
	{
		if (index == 0 && STL::ContainsKey(m_textures, key)) {
			return m_textures[key];
		}

		String format = "%s%00d";
		format[4] += keyLength;
		String finalKey = CHERRYSODA_FORMAT(format.c_str(), key.c_str(), index);

		MTexture result;
		if (!STL::TryGetValue(m_textures, finalKey, result)) {
			CHERRYSODA_DEBUG_FORMAT("\"%s\" not found in atlas!\n", finalKey.c_str());
		}

		return result;
	}

	static Atlas FromAtlas(const String& path, AtlasDataFormat format = AtlasDataFormat::CrunchJson)
	{
		Atlas atlas;
		ReadAtlasData(atlas, path, format);
		return atlas;	
	}

	static void ReadAtlasData(Atlas& atlas, const String& path, AtlasDataFormat format)
	{
		switch (format) {
		case AtlasDataFormat::CrunchJson:
			json::Document doc;
			JsonUtil::ReadJsonFile(doc, path);
			const auto& at = doc["textures"];
			CHERRYSODA_ASSERT_FORMAT(at.IsArray(), "Atlas json parse failed: \"textures\" scope is not an array in \"%s\"!\n");
			for (const auto& tex : at.GetArray()) {
				String texturePath = path.substr(0, path.find_last_of("/\\") + 1) + tex["name"].GetString() + ".png";
				auto texture = Texture2D::FromFile(texturePath);
				auto mTexture = MTexture(texture);
				STL::Add(atlas.m_sources, texture);
				const auto& img = tex["images"];
				CHERRYSODA_ASSERT_FORMAT(img.IsArray(), "Atlas json parse failed: \"images\" scope is not an array in \"%s\"!\n");
				for (const auto& sub : img.GetArray()) {
					const char* name = sub["n"].GetString();
					auto clipRect = Math::IRectangle{
						Math::IVec2(sub["x"].GetInt(), sub["y"].GetInt()),
						Math::IVec2(sub["w"].GetInt(), sub["h"].GetInt()),
					};
					atlas.m_textures[name] = MTexture(mTexture, name, clipRect);
				}
			}
			break;
		}
	}

private:
	STL::List<Texture2D> m_sources;

	STL::HashMap<StringID,MTexture> m_textures;
	STL::HashMap<StringID,STL::List<MTexture>> m_orderedTexturesCache;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_ATLAS_H_