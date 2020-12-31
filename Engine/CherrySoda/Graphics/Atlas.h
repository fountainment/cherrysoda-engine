#ifndef _CHERRYSODA_GRAPHICS_ATLAS_H_
#define _CHERRYSODA_GRAPHICS_ATLAS_H_

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

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

	Atlas() = default;
	~Atlas();

	const STL::Vector<MTexture> GetAtlasSubtextures(const String& key);
	const MTexture GetAtlasSubtextureAt(const String& key, int index);
	static Atlas* FromAtlas(const String& path, AtlasDataFormat format = AtlasDataFormat::CrunchJson)
	{
		Atlas* atlas = new Atlas;
		ReadAtlasData(atlas, path, format);
		return atlas;
	}

	static void ReadAtlasData(Atlas* atlas, const String& path, AtlasDataFormat format);

private:
	const MTexture GetAtlasSubtextureFromCacheAt(const String& key, int index);
	const MTexture GetAtlasSubtextureFromAtlasAt(const String& key, int index);

	STL::List<Texture2D> m_sources;

	STL::HashMap<StringID,MTexture> m_textures;
	STL::HashMap<StringID,STL::Vector<MTexture>> m_orderedTexturesCache;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_ATLAS_H_
