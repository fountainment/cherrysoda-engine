#ifndef _CHERRYSODA_GRAPHICS_ATLAS_H_
#define _CHERRYSODA_GRAPHICS_ATLAS_H_

#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Atlas
{
public:
	enum class AtlasDataFormat
	{
		TexturePacker_Sparrow,
		CrunchJson,
		CrunchXml,
		CrunchBinary,
		CrunchJsonOrBinary,
		CrunchXmlOrBinary,
		CrunchBinaryNoAtlas,
		Packer,
		PackerNoAtlas
	};

	Atlas() = default;
	~Atlas();

	inline const MTexture& operator[](const StringID& id) const { return Get(id); }

	inline bool Has(const StringID& id) const { return STL::ContainsKey(m_textures, id); }
	inline const MTexture& Get(const StringID& id) const { return m_textures.at(id); }
	const MTexture& GetOrDefault(const StringID& id, const MTexture& defaultTexture) const;

	const STL::Vector<MTexture>& GetAtlasSubtextures(const String& key) const;
	MTexture GetAtlasSubtextureAt(const String& key, int index) const;

	static Atlas* FromAtlas(const String& path, AtlasDataFormat format = AtlasDataFormat::CrunchJson)
	{
		Atlas* atlas = new Atlas;
		ReadAtlasData(atlas, path, format);
		return atlas;
	}
	static void ReadAtlasData(Atlas* atlas, const String& path, AtlasDataFormat format);

	// Loads each of the data files (relative to rootPath) into one atlas
	static Atlas* FromMultiAtlas(const String& rootPath, const STL::Vector<String>& dataPath, AtlasDataFormat format);
	// Loads rootPath/filename0.xml, filename1.xml, ... until one is missing
	static Atlas* FromMultiAtlas(const String& rootPath, const String& filename, AtlasDataFormat format);
	// Loads every png under the directory; keys are the extension-less
	// relative paths with '/' separators
	static Atlas* FromDirectory(const String& path);

private:
	const MTexture& GetAtlasSubtextureFromCacheAt(const StringID& key, int index) const;
	MTexture GetAtlasSubtextureFromAtlasAt(const String& key, int index) const;

	STL::List<Texture2D> m_sources;

	STL::HashMap<StringID, MTexture> m_textures;
	mutable STL::HashMap<StringID, STL::Vector<MTexture>> m_orderedTexturesCache;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_ATLAS_H_
