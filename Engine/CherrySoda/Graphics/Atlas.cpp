#include <CherrySoda/Graphics/Atlas.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Json.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/XML.h>

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace cherrysoda {

namespace {

// Little-endian binary reader for the CrunchBinary and Packer formats
class AtlasBinaryReader
{
public:
	explicit AtlasBinaryReader(const String& filename) : m_stream(filename, std::ios::binary) {}

	bool IsOpen() const { return m_stream.is_open(); }
	bool IsAtEnd() { return m_stream.peek() == std::char_traits<char>::eof(); }

	type::Int16 ReadInt16()
	{
		char bytes[2];
		m_stream.read(bytes, sizeof(bytes));
		return static_cast<type::Int16>(static_cast<type::UInt16>(static_cast<unsigned char>(bytes[0])) |
										(static_cast<type::UInt16>(static_cast<unsigned char>(bytes[1])) << 8));
	}

	type::Int32 ReadInt32()
	{
		char bytes[4];
		m_stream.read(bytes, sizeof(bytes));
		return static_cast<type::Int32>(static_cast<type::UInt32>(static_cast<unsigned char>(bytes[0])) |
										(static_cast<type::UInt32>(static_cast<unsigned char>(bytes[1])) << 8) |
										(static_cast<type::UInt32>(static_cast<unsigned char>(bytes[2])) << 16) |
										(static_cast<type::UInt32>(static_cast<unsigned char>(bytes[3])) << 24));
	}

	String ReadNullTerminatedString()
	{
		String str;
		char c = 0;
		while (m_stream.read(&c, 1) && c != 0) {
			str += c;
		}
		return str;
	}

	// .NET BinaryWriter-style string: 7-bit encoded length prefix + bytes
	String ReadPrefixedString()
	{
		type::UInt32 length = 0;
		type::UInt32 shift = 0;
		char b = 0;
		do {
			m_stream.read(&b, 1);
			length |= static_cast<type::UInt32>(static_cast<unsigned char>(b) & 0x7f) << shift;
			shift += 7;
		} while (static_cast<unsigned char>(b) & 0x80);

		String str(length, '\0');
		if (length > 0) {
			m_stream.read(str.data(), length);
		}
		return str;
	}

private:
	std::ifstream m_stream;
};

// A whole-texture MTexture with an explicit draw offset and size, matching
// Monocle's MTexture(texture, offset, width, height) constructor
MTexture MakeWholeTexture(const Texture2D& texture, const char* name, const Math::Vec2& drawOffset, int width,
						  int height)
{
	MTexture whole(texture);
	return MTexture(
		whole, name,
		Math::IRectangle{.m_coord = Math::IVec2(0, 0), .m_size = Math::IVec2(texture.Width(), texture.Height())},
		drawOffset, width, height);
}

} // namespace

Atlas::~Atlas()
{
	for (auto& tex2D : m_sources) {
		Graphics::DestroyTexture(tex2D.GetHandle());
	}
}

const MTexture& Atlas::GetOrDefault(const StringID& id, const MTexture& defaultTexture) const
{
	// Returning the caller's default by reference is the point of this API
	if (id.IsEmpty() || !Has(id)) {
		return defaultTexture; // NOLINT(bugprone-return-const-ref-from-parameter)
	}
	return m_textures.at(id);
}

const STL::Vector<MTexture>& Atlas::GetAtlasSubtextures(const String& key) const
{
	if (!STL::ContainsKey(m_orderedTexturesCache, key)) {
		STL::Vector<MTexture> list;
		for (int index = 0;; ++index) {
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

MTexture Atlas::GetAtlasSubtextureAt(const String& key, int index) const
{
	STL::Vector<MTexture> list;
	if (STL::TryGetValue(m_orderedTexturesCache, key, list)) {
		return list[index];
	}

	return GetAtlasSubtextureFromAtlasAt(key, index);
}

const MTexture& Atlas::GetAtlasSubtextureFromCacheAt(const StringID& key, int index) const
{
	return m_orderedTexturesCache.at(key)[index];
}

MTexture Atlas::GetAtlasSubtextureFromAtlasAt(const String& key, int index) const
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

void Atlas::ReadAtlasData(Atlas* atlas, const String& path, AtlasDataFormat format /* = AtlasDataFormat::CrunchJson*/)
{
	switch (format) {
	case AtlasDataFormat::TexturePacker_Sparrow: {
		xml::xml_document doc;
		XMLUtil::ReadXMLFile(doc, path);
		xml::xml_node at = doc.child("TextureAtlas");
		CHERRYSODA_ASSERT_FORMAT(at != nullptr, "Atlas xml parse failed: no \"TextureAtlas\" node in \"%s\"!\n",
								 path.c_str());

		String texturePath = StringUtil::Path_GetDirectoryName(path) + at.attribute("imagePath").as_string("");
		auto texture = Texture2D::FromFile(texturePath);
		auto mTexture = MTexture(texture);
		STL::Add(atlas->m_sources, texture);

		for (xml::xml_node sub = at.child("SubTexture"); sub; sub = sub.next_sibling("SubTexture")) {
			const char* name = sub.attribute("name").as_string("");
			auto clipRect = Math::IRectangle{
				.m_coord = Math::IVec2(sub.attribute("x").as_int(0), sub.attribute("y").as_int(0)),
				.m_size = Math::IVec2(sub.attribute("width").as_int(0), sub.attribute("height").as_int(0)),
			};
			if (sub.attribute("frameX")) {
				atlas->m_textures[name] =
					MTexture(mTexture, name, clipRect,
							 Math::Vec2(-sub.attribute("frameX").as_int(0), -sub.attribute("frameY").as_int(0)),
							 sub.attribute("frameWidth").as_int(0), sub.attribute("frameHeight").as_int(0));
			}
			else {
				atlas->m_textures[name] = MTexture(mTexture, name, clipRect);
			}
		}
		break;
	}
	case AtlasDataFormat::CrunchJson: {
		json::Document doc;
		JsonUtil::ReadJsonFile(doc, path);
		const auto& at = doc["textures"];
		CHERRYSODA_ASSERT_FORMAT(
			at.IsArray(), "Atlas json parse failed: \"textures\" scope is not an array in \"%s\"!\n", path.c_str());
		for (const auto& tex : at.GetArray()) {
			String texturePath = StringUtil::Path_GetDirectoryName(path) + tex["name"].GetString() + ".png";
			auto texture = Texture2D::FromFile(texturePath);
			auto mTexture = MTexture(texture);
			STL::Add(atlas->m_sources, texture);
			const auto& img = tex["images"];
			CHERRYSODA_ASSERT_FORMAT(
				img.IsArray(), "Atlas json parse failed: \"images\" scope is not an array in \"%s\"!\n", path.c_str());
			for (const auto& sub : img.GetArray()) {
				const char* name = sub["n"].GetString();
				auto clipRect = Math::IRectangle{
					.m_coord = Math::IVec2(sub["x"].GetInt(), sub["y"].GetInt()),
					.m_size = Math::IVec2(sub["w"].GetInt(), sub["h"].GetInt()),
				};
				if (sub.HasMember("fx")) {
					atlas->m_textures[name] =
						MTexture(mTexture, name, clipRect, Math::Vec2(-sub["fx"].GetInt(), -sub["fy"].GetInt()),
								 sub["fw"].GetInt(), sub["fh"].GetInt());
				}
				else {
					atlas->m_textures[name] = MTexture(mTexture, name, clipRect);
				}
			}
		}
		break;
	}
	case AtlasDataFormat::CrunchXml: {
		xml::xml_document doc;
		XMLUtil::ReadXMLFile(doc, path);
		xml::xml_node at = doc.child("atlas");
		CHERRYSODA_ASSERT_FORMAT(at != nullptr, "Atlas xml parse failed: no \"atlas\" node in \"%s\"!\n", path.c_str());

		for (xml::xml_node tex = at.child("texture"); tex; tex = tex.next_sibling("texture")) {
			String texturePath = StringUtil::Path_GetDirectoryName(path) + tex.attribute("n").as_string("") + ".png";
			auto texture = Texture2D::FromFile(texturePath);
			auto mTexture = MTexture(texture);
			STL::Add(atlas->m_sources, texture);

			for (xml::xml_node sub = tex.first_child(); sub; sub = sub.next_sibling()) {
				const char* name = sub.attribute("n").as_string("");
				auto clipRect = Math::IRectangle{
					.m_coord = Math::IVec2(sub.attribute("x").as_int(0), sub.attribute("y").as_int(0)),
					.m_size = Math::IVec2(sub.attribute("w").as_int(0), sub.attribute("h").as_int(0)),
				};
				if (sub.attribute("fx")) {
					atlas->m_textures[name] =
						MTexture(mTexture, name, clipRect,
								 Math::Vec2(-sub.attribute("fx").as_int(0), -sub.attribute("fy").as_int(0)),
								 sub.attribute("fw").as_int(0), sub.attribute("fh").as_int(0));
				}
				else {
					atlas->m_textures[name] = MTexture(mTexture, name, clipRect);
				}
			}
		}
		break;
	}
	case AtlasDataFormat::CrunchBinary: {
		AtlasBinaryReader reader(path);
		CHERRYSODA_ASSERT_FORMAT(reader.IsOpen(), "Atlas binary not found: \"%s\"!\n", path.c_str());

		int textures = reader.ReadInt16();
		for (int i = 0; i < textures; ++i) {
			String textureName = reader.ReadNullTerminatedString();
			String texturePath = StringUtil::Path_GetDirectoryName(path) + textureName + ".png";
			auto texture = Texture2D::FromFile(texturePath);
			STL::Add(atlas->m_sources, texture);

			auto mTexture = MTexture(texture);
			int subtextures = reader.ReadInt16();
			for (int j = 0; j < subtextures; ++j) {
				String name = reader.ReadNullTerminatedString();
				auto clipRect = Math::IRectangle{
					.m_coord = Math::IVec2(reader.ReadInt16(), reader.ReadInt16()),
					.m_size = Math::IVec2(reader.ReadInt16(), reader.ReadInt16()),
				};
				Math::Vec2 drawOffset(-reader.ReadInt16(), -reader.ReadInt16());
				int width = reader.ReadInt16();
				int height = reader.ReadInt16();

				atlas->m_textures[name.c_str()] = MTexture(mTexture, name.c_str(), clipRect, drawOffset, width, height);
			}
		}
		break;
	}
	case AtlasDataFormat::CrunchBinaryNoAtlas: {
		AtlasBinaryReader reader(path + ".bin");
		CHERRYSODA_ASSERT_FORMAT(reader.IsOpen(), "Atlas binary not found: \"%s.bin\"!\n", path.c_str());

		int folders = reader.ReadInt16();
		for (int i = 0; i < folders; ++i) {
			String folderName = reader.ReadNullTerminatedString();
			String folderPath = StringUtil::Path_GetDirectoryName(path) + folderName;

			int subtextures = reader.ReadInt16();
			for (int j = 0; j < subtextures; ++j) {
				String name = reader.ReadNullTerminatedString();
				reader.ReadInt16(); // x
				reader.ReadInt16(); // y
				reader.ReadInt16(); // w
				reader.ReadInt16(); // h
				Math::Vec2 drawOffset(-reader.ReadInt16(), -reader.ReadInt16());
				int width = reader.ReadInt16();
				int height = reader.ReadInt16();

				auto texture = Texture2D::FromFile(folderPath + name + ".png");
				STL::Add(atlas->m_sources, texture);
				atlas->m_textures[name.c_str()] = MakeWholeTexture(texture, name.c_str(), drawOffset, width, height);
			}
		}
		break;
	}
	case AtlasDataFormat::CrunchJsonOrBinary: {
		if (Calc::FileExists(path + ".bin")) {
			ReadAtlasData(atlas, path + ".bin", AtlasDataFormat::CrunchBinary);
		}
		else {
			ReadAtlasData(atlas, path + ".json", AtlasDataFormat::CrunchJson);
		}
		break;
	}
	case AtlasDataFormat::CrunchXmlOrBinary: {
		if (Calc::FileExists(path + ".bin")) {
			ReadAtlasData(atlas, path + ".bin", AtlasDataFormat::CrunchBinary);
		}
		else {
			ReadAtlasData(atlas, path + ".xml", AtlasDataFormat::CrunchXml);
		}
		break;
	}
	case AtlasDataFormat::Packer: {
		AtlasBinaryReader reader(path + ".meta");
		CHERRYSODA_ASSERT_FORMAT(reader.IsOpen(), "Atlas meta not found: \"%s.meta\"!\n", path.c_str());

		reader.ReadInt32();          // version
		reader.ReadPrefixedString(); // args
		reader.ReadInt32();          // hash

		int textures = reader.ReadInt16();
		for (int i = 0; i < textures; ++i) {
			String textureName = reader.ReadPrefixedString();
			String texturePath = StringUtil::Path_GetDirectoryName(path) + textureName + ".data";
			auto texture = Texture2D::FromFile(texturePath);
			STL::Add(atlas->m_sources, texture);

			auto mTexture = MTexture(texture);
			int subtextures = reader.ReadInt16();
			for (int j = 0; j < subtextures; ++j) {
				String name = reader.ReadPrefixedString();
				std::ranges::replace(name, '\\', '/');
				auto clipRect = Math::IRectangle{
					.m_coord = Math::IVec2(reader.ReadInt16(), reader.ReadInt16()),
					.m_size = Math::IVec2(reader.ReadInt16(), reader.ReadInt16()),
				};
				Math::Vec2 drawOffset(-reader.ReadInt16(), -reader.ReadInt16());
				int width = reader.ReadInt16();
				int height = reader.ReadInt16();

				atlas->m_textures[name.c_str()] = MTexture(mTexture, name.c_str(), clipRect, drawOffset, width, height);
			}
		}
		break;
	}
	case AtlasDataFormat::PackerNoAtlas: {
		AtlasBinaryReader reader(path + ".meta");
		CHERRYSODA_ASSERT_FORMAT(reader.IsOpen(), "Atlas meta not found: \"%s.meta\"!\n", path.c_str());

		reader.ReadInt32();          // version
		reader.ReadPrefixedString(); // args
		reader.ReadInt32();          // hash

		int folders = reader.ReadInt16();
		for (int i = 0; i < folders; ++i) {
			String folderName = reader.ReadPrefixedString();
			String folderPath = StringUtil::Path_GetDirectoryName(path) + folderName;

			int subtextures = reader.ReadInt16();
			for (int j = 0; j < subtextures; ++j) {
				String name = reader.ReadPrefixedString();
				std::ranges::replace(name, '\\', '/');
				reader.ReadInt16(); // x
				reader.ReadInt16(); // y
				reader.ReadInt16(); // w
				reader.ReadInt16(); // h
				Math::Vec2 drawOffset(-reader.ReadInt16(), -reader.ReadInt16());
				int width = reader.ReadInt16();
				int height = reader.ReadInt16();

				auto texture = Texture2D::FromFile(folderPath + name + ".data");
				STL::Add(atlas->m_sources, texture);
				atlas->m_textures[name.c_str()] = MakeWholeTexture(texture, name.c_str(), drawOffset, width, height);
			}
		}
		break;
	}
	default:
		CHERRYSODA_ASSERT(false, "Atlas data format unsupported for now!\n");
		break;
	}
}

Atlas* Atlas::FromMultiAtlas(const String& rootPath, const STL::Vector<String>& dataPath, AtlasDataFormat format)
{
	auto* atlas = new Atlas;
	for (const auto& data : dataPath) {
		String dataPathFull = rootPath;
		dataPathFull += '/';
		dataPathFull += data;
		ReadAtlasData(atlas, dataPathFull, format);
	}
	return atlas;
}

Atlas* Atlas::FromMultiAtlas(const String& rootPath, const String& filename, AtlasDataFormat format)
{
	auto* atlas = new Atlas;

	int index = 0;
	while (true) {
		String dataPath = CHERRYSODA_FORMAT("%s/%s%d.xml", rootPath.c_str(), filename.c_str(), index);
		if (!Calc::FileExists(dataPath)) {
			break;
		}
		ReadAtlasData(atlas, dataPath, format);
		++index;
	}

	return atlas;
}

Atlas* Atlas::FromDirectory(const String& path)
{
	auto* atlas = new Atlas;

	std::error_code ec;
	auto iterator = std::filesystem::recursive_directory_iterator(path, ec);
	if (ec) {
		CHERRYSODA_LOG(CHERRYSODA_FORMAT("Atlas::FromDirectory: cannot read directory \"%s\"\n", path.c_str()));
		return atlas;
	}

	for (const auto& entry : iterator) {
		if (ec || !entry.is_regular_file(ec)) {
			continue;
		}
		if (entry.path().extension() != ".png") {
			continue;
		}

		String relative = entry.path().lexically_relative(path).generic_string();
		String key = relative.substr(0, relative.size() - 4);

		auto texture = Texture2D::FromFile(entry.path().generic_string());
		STL::Add(atlas->m_sources, texture);
		atlas->m_textures[key.c_str()] = MTexture(texture);
	}

	return atlas;
}

} // namespace cherrysoda
