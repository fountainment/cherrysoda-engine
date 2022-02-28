#include <CherrySoda/Graphics/Atlas.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::Atlas;

using cherrysoda::Graphics;
using cherrysoda::JsonUtil;
using cherrysoda::Math;
using cherrysoda::MTexture;
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;

Atlas::~Atlas()
{
	for (auto& tex2D : m_sources) {
		Graphics::DestroyTexture(tex2D.GetHandle());
	}
}

const MTexture& Atlas::GetOrDefault(const StringID& id, const MTexture& defaultTexture) const
{
	if (id.IsEmpty() || !Has(id)) {
		return defaultTexture;
	}
	return m_textures.at(id);
}

const STL::Vector<MTexture>& Atlas::GetAtlasSubtextures(const String& key) const
{
	if (!STL::ContainsKey(m_orderedTexturesCache, key)) {
		STL::Vector<MTexture> list;
		for (int index = 0; ; ++index) {
			MTexture texture = GetAtlasSubtextureFromAtlasAt(key, index);
			if (texture.IsValid())
				STL::Add(list, texture);
			else
				break;
		}
		m_orderedTexturesCache[key] = list;
	}

	return m_orderedTexturesCache.at(key);
}

const MTexture Atlas::GetAtlasSubtextureAt(const String& key, int index) const
{
	STL::Vector<MTexture> list;
	if (STL::TryGetValue(m_orderedTexturesCache, key, list)) {
		return list[index];
	}
	else {
		return GetAtlasSubtextureFromAtlasAt(key, index);
	}
}

const MTexture& Atlas::GetAtlasSubtextureFromCacheAt(const StringID& key, int index) const
{
	return m_orderedTexturesCache.at(key)[index];
}

const MTexture Atlas::GetAtlasSubtextureFromAtlasAt(const String& key, int index) const
{
	if (index == 0 && STL::ContainsKey(m_textures, key)) {
		return m_textures.at(key);
	}

	char format[] = "%s%00d";
	int startLength = CHERRYSODA_FORMAT("%d", index).length();
	MTexture result;
	for (int keyLength = startLength; keyLength <= 4; ++keyLength) {
		format[4] = '0' + keyLength;
		if (STL::TryGetValue(m_textures, CHERRYSODA_FORMAT(format, key.c_str(), index), result)) {
			break;
		}
	}

	return result;
}

void Atlas::ReadAtlasData(Atlas* atlas, const String& path, AtlasDataFormat format/* = AtlasDataFormat::CrunchJson*/)
{
	switch (format) {
	case AtlasDataFormat::CrunchJson:
	{
		cherrysoda::json::Document doc;
		JsonUtil::ReadJsonFile(doc, path);
		const auto& at = doc["textures"];
		CHERRYSODA_ASSERT_FORMAT(at.IsArray(), "Atlas json parse failed: \"textures\" scope is not an array in \"%s\"!\n");
		for (const auto& tex : at.GetArray()) {
			String texturePath = StringUtil::Path_GetDirectoryName(path) + tex["name"].GetString() + ".png";
			auto texture = Texture2D::FromFile(texturePath);
			auto mTexture = MTexture(texture);
			STL::Add(atlas->m_sources, texture);
			const auto& img = tex["images"];
			CHERRYSODA_ASSERT_FORMAT(img.IsArray(), "Atlas json parse failed: \"images\" scope is not an array in \"%s\"!\n");
			for (const auto& sub : img.GetArray()) {
				const char* name = sub["n"].GetString();
				auto clipRect = Math::IRectangle{
					Math::IVec2(sub["x"].GetInt(), sub["y"].GetInt()),
					Math::IVec2(sub["w"].GetInt(), sub["h"].GetInt()),
				};
				if (sub.HasMember("fx")) {
					atlas->m_textures[name] = MTexture(mTexture, name, clipRect, Math::Vec2(-sub["fx"].GetInt(), -sub["fy"].GetInt()), sub["fw"].GetInt(), sub["fh"].GetInt());
				}
				else {
					atlas->m_textures[name] = MTexture(mTexture, name, clipRect);
				}
			}
		}
		break;
	}
	default:
		CHERRYSODA_ASSERT(false, "Atlas data format unsupported for now!\n");
		break;
	}
}
