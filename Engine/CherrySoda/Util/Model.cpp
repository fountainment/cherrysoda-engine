#include <CherrySoda/Util/Model.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/STL.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include <cstring>

using cherrysoda::Model;

using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::STL;
using cherrysoda::String;


static size_t ComponentSize(cgltf_accessor *accessor)
{
	switch (accessor->component_type)
	{
		case cgltf_component_type_r_8:
		case cgltf_component_type_r_8u:
			return 1;

		case cgltf_component_type_r_16:
		case cgltf_component_type_r_16u:
			return 2;

		case cgltf_component_type_r_32u:
		case cgltf_component_type_r_32f:
			return 4;

		case cgltf_component_type_invalid:
			CHERRYSODA_ASSERT(0, "Asked for component_size of invalid component_type!\n");
		default:
			CHERRYSODA_ASSERT(0, "Asked for component_size of unknown component_type!\n");
	}
	return 0;
}

static size_t ComponentCount(cgltf_accessor *accessor)
{
	switch (accessor->type)
	{
		case cgltf_type_scalar:
			return 1;
		case cgltf_type_vec2:
			return 2;
		case cgltf_type_vec3:
			return 3;
		case cgltf_type_vec4:
		case cgltf_type_mat2:
			return 4;
		case cgltf_type_mat3:
			return 9;
		case cgltf_type_mat4:
			return 16;
		default:
			CHERRYSODA_ASSERT(0, "Asked for component_count of invalid component_type!\n");
	}
	return 0;
}

static size_t AccessorOffset(cgltf_accessor *accessor)
{
	return accessor->offset + accessor->buffer_view->offset;
}

static size_t AccessorStride(cgltf_accessor *accessor)
{
	return accessor->stride ? accessor->stride : ComponentSize(accessor) * ComponentCount(accessor);
}

Model Model::FromGltf(const String& gltfFile)
{
	Model ret;
	cgltf_options options;
	std::memset(&options, 0, sizeof(cgltf_options));
	cgltf_data* data = nullptr;
	cgltf_result result = cgltf_parse_file(&options, gltfFile.c_str(), &data);
	CHERRYSODA_DEBUG_FORMAT("Loading gltf model: \"%s\"\n", gltfFile.c_str());
	if (result == cgltf_result_success) {
		result = cgltf_load_buffers(&options, data, gltfFile.c_str());
		if (result == cgltf_result_success) result = cgltf_validate(data);
		if (result == cgltf_result_success) {
			CHERRYSODA_DEBUG_FORMAT("    Meshes: %u\n", (unsigned)data->meshes_count);
			CHERRYSODA_DEBUG_FORMAT("    Textures: %u\n", (unsigned)data->textures_count);
			for (int i = 0; i < data->textures_count; ++i) {
				CHERRYSODA_DEBUG_FORMAT("        uri %d: %s\n", i, data->textures[i].image->uri);
			}

			for (int m = 0; m < data->meshes_count; ++m) {
				for (int n = 0; n < data->meshes[m].primitives_count; ++n) {
					auto& primitive = data->meshes[m].primitives[n];
					Graphics::MeshInfo mesh;
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
						size_t offset = AccessorOffset(positionAccessor);
						size_t stride = AccessorStride(positionAccessor);
						char* data = reinterpret_cast<char*>(positionAccessor->buffer_view->buffer->data);
						for (int i = 0; i < positionAccessor->count; ++i) {
							auto position = reinterpret_cast<cherrysoda::Math::Vec3*>(&(data[offset + i * stride]));	
							STL::Add(mesh.vertices, Graphics::VertexInfo{ *position, Vec4_Zero, Vec3_ZUp, Vec2_Zero });
						}
					}
					if (colorAccessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == colorAccessor->count, "Color attribute count incorrect!\n");
						size_t offset = AccessorOffset(colorAccessor);
						size_t stride = AccessorStride(colorAccessor);
						char* data = reinterpret_cast<char*>(colorAccessor->buffer_view->buffer->data);
						for (int i = 0; i < colorAccessor->count; ++i) {
							auto color = reinterpret_cast<cherrysoda::Math::Vec4*>(&(data[offset + i * stride]));	
							mesh.vertices[i].color = *color;
						}
					}
					if (normalAccessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == normalAccessor->count, "Normal attribute count incorrect!\n");
						size_t offset = AccessorOffset(normalAccessor);
						size_t stride = AccessorStride(normalAccessor);
						char* data = reinterpret_cast<char*>(normalAccessor->buffer_view->buffer->data);
						for (int i = 0; i < normalAccessor->count; ++i) {
							auto normal = reinterpret_cast<cherrysoda::Math::Vec3*>(&(data[offset + i * stride]));	
							mesh.vertices[i].normal = *normal;
						}
					}
					if (texcoord0Accessor) {
						CHERRYSODA_ASSERT(STL::Count(mesh.vertices) == texcoord0Accessor->count, "Texcoord0 attribute count incorrect!\n");
						size_t offset = AccessorOffset(texcoord0Accessor);
						size_t stride = AccessorStride(texcoord0Accessor);
						char* data = reinterpret_cast<char*>(texcoord0Accessor->buffer_view->buffer->data);
						for (int i = 0; i < texcoord0Accessor->count; ++i) {
							auto texcoord0 = reinterpret_cast<cherrysoda::Math::Vec2*>(&(data[offset + i * stride]));	
							mesh.vertices[i].texcoord0 = *texcoord0;
						}
					}
					cgltf_accessor* indexAccessor = primitive.indices;
					if (indexAccessor) {
						for (int i = 0; i < indexAccessor->count; ++i) {
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
