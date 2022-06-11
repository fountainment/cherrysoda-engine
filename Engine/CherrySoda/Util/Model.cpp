#include <CherrySoda/Util/Model.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include <cstring>

namespace cherrysoda {

Model Model::FromGltf(const String& gltfFile)
{
	Model ret;
	cgltf_options options;
	std::memset(&options, 0, sizeof(cgltf_options));
	cgltf_data* data = nullptr;
	cgltf_result result = cgltf_parse_file(&options, gltfFile.c_str(), &data);
	auto gltfDir = StringUtil::Path_GetDirectoryName(gltfFile);
	CHERRYSODA_DEBUG_FORMAT("Loading gltf model: \"%s\"\n", gltfFile.c_str());
	if (result == cgltf_result_success) {
		result = cgltf_load_buffers(&options, data, gltfFile.c_str());
		if (result == cgltf_result_success) result = cgltf_validate(data);
		if (result == cgltf_result_success) {
			CHERRYSODA_DEBUG_FORMAT("    Meshes: %u\n", (unsigned)data->meshes_count);
			CHERRYSODA_DEBUG_FORMAT("    Textures: %u\n", (unsigned)data->textures_count);
			STL::Vector<Graphics::TextureHandle> textures;
			STL::Reserve(textures, data->textures_count);
			for (int i = 0; i < static_cast<int>(data->textures_count); ++i) {
				CHERRYSODA_DEBUG_FORMAT("        uri %d: %s\n", i, data->textures[i].image->uri);
				STL::Add(textures, Graphics::CreateTexture(gltfDir + String(data->textures[i].image->uri)));
			}

			for (int m = 0; m < static_cast<int>(data->meshes_count); ++m) {
				for (int n = 0; n < static_cast<int>(data->meshes[m].primitives_count); ++n) {
					auto& primitive = data->meshes[m].primitives[n];
					Graphics::MeshInfo mesh;
					if (primitive.material->has_pbr_metallic_roughness) {
						auto baseColorTexture = primitive.material->pbr_metallic_roughness.base_color_texture.texture;
						auto metallicRoughnessTexture = primitive.material->pbr_metallic_roughness.metallic_roughness_texture.texture;
						if (baseColorTexture) {
							mesh.baseColorTexture = textures[baseColorTexture - data->textures];
						}
						if (metallicRoughnessTexture) {
							mesh.metallicRoughnessTexture = textures[metallicRoughnessTexture - data->textures];
						}
					}
					auto normalTexture = primitive.material->normal_texture.texture;
					if (normalTexture) {
						mesh.normalTexture = textures[normalTexture - data->textures];
					}
					cgltf_accessor* positionAccessor = nullptr;
					cgltf_accessor* normalAccessor = nullptr;
					cgltf_accessor* texcoord0Accessor = nullptr;
					cgltf_accessor* colorAccessor = nullptr;
					for (size_t i = 0; i < primitive.attributes_count; ++i) {
						auto& attribute = primitive.attributes[i];
						if (attribute.index == 0) {
							switch (attribute.type) {
								case cgltf_attribute_type_position:
									positionAccessor = attribute.data;
									break;
								case cgltf_attribute_type_normal:
									normalAccessor = attribute.data;
									break;
								case cgltf_attribute_type_texcoord:
									texcoord0Accessor = attribute.data;
									break;
								case cgltf_attribute_type_color:
									colorAccessor = attribute.data;
									break;
								default:
									break;
							}
						}
					}
					if (positionAccessor) {
						for (int i = 0; i < static_cast<int>(positionAccessor->count); ++i) {
							Math::Vec3 position;
							cgltf_accessor_read_float(positionAccessor, i, reinterpret_cast<float*>(&position), 3);
							STL::Add(mesh.vertices, Graphics::VertexInfo{ position, Vec4_One, Vec3_ZUp, Vec2_Zero });
						}
					}
					if (colorAccessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == colorAccessor->count, "Color attribute count incorrect!\n");
						for (int i = 0; i < static_cast<int>(colorAccessor->count); ++i) {
							cgltf_accessor_read_float(colorAccessor, i, reinterpret_cast<float*>(&(mesh.vertices[i].color)), 4);
						}
					}
					if (normalAccessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == normalAccessor->count, "Normal attribute count incorrect!\n");
						for (int i = 0; i < static_cast<int>(normalAccessor->count); ++i) {
							cgltf_accessor_read_float(normalAccessor, i, reinterpret_cast<float*>(&(mesh.vertices[i].normal)), 3);
						}
					}
					if (texcoord0Accessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == texcoord0Accessor->count, "Texcoord0 attribute count incorrect!\n");
						for (int i = 0; i < static_cast<int>(texcoord0Accessor->count); ++i) {
							cgltf_accessor_read_float(texcoord0Accessor, i, reinterpret_cast<float*>(&(mesh.vertices[i].texcoord0)), 2);
						}
					}
					cgltf_accessor* indexAccessor = primitive.indices;
					if (indexAccessor) {
						for (int i = 0; i < static_cast<int>(indexAccessor->count); ++i) {
							cgltf_size index = cgltf_accessor_read_index(indexAccessor, i);
							CHERRYSODA_ASSERT(index == static_cast<cgltf_size>(static_cast<type::UInt16>(index)), "Index out of UInt16 range!\n");
							STL::Add(mesh.indices, static_cast<type::UInt16>(index));
						}
					}

					ret.AddMesh(mesh);
				}
			}
		}
		cgltf_free(data);
	}
	CHERRYSODA_DEBUG_FORMAT("    Result: %u\n", result);
	return ret;
}

} // namespace cherrysoda
