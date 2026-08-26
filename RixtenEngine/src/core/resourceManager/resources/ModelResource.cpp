#include "core/resourceManager/resources/ModelResource.h"

#include <stdint.h>

#include "core/memory/MemoryArena.h"
#include "utils/logger.h"


AttributeType ModelLoaderGLTF::getAttributeType(cgltf_component_type type) {
    switch (type) {
        case cgltf_component_type_r_8: return byte_type;
        case cgltf_component_type_r_8u: return unsigned_byte_type;
        case cgltf_component_type_r_16: return short_type;
        case cgltf_component_type_r_16u: return unsigned_short_type;
        case cgltf_component_type_r_32u: return unsigned_int_type;
        case cgltf_component_type_r_32f: return float_type;
        case cgltf_component_type_invalid: return invalid_type;
        case cgltf_component_type_max_enum: return max_enum_type;
        default: return invalid_type;
    }
}

uint32_t ModelLoaderGLTF::componentSize(cgltf_component_type type) {
    switch (type) {
        case cgltf_component_type_r_8: return sizeof(int8_t);
        case cgltf_component_type_r_8u: return sizeof(uint8_t);
        case cgltf_component_type_r_16: return sizeof(int16_t);
        case cgltf_component_type_r_16u: return sizeof(uint16_t);
        case cgltf_component_type_r_32u: return sizeof(uint32_t);
        case cgltf_component_type_r_32f: return sizeof(float);
        default: return 0;
    }
}

ModelLoaderGLTF::ModelLoaderGLTF(ResourceManager* mngr_) : 
    mngr(mngr_), arena(MemoryArena::GetInstance()) {
}

ModelResource ModelLoaderGLTF::load(const char* path) {
    cgltf_options options{};
    cgltf_data* data = nullptr;
    
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if(result != cgltf_result_success){LOG_ERROR("Can't load model! Error while parse, file: ", path); return {};}; 
    
    result = cgltf_load_buffers(&options, data, path);
    if(result != cgltf_result_success){LOG_ERROR("Can't load model! Error while load buffers, file: ", path); return {};};

    ModelResource model{data->meshes_count};
    LOG_DEBUG(data->meshes_count);
    for(int i = 0; i < data->meshes_count; i++) {
        cgltf_mesh gmesh = data->meshes[i];
        
        for(int j = 0; j < gmesh.primitives_count; j++) {
            cgltf_primitive& primitive = gmesh.primitives[j];
            model.subMeshes.emplace_back(loadSubMesh(primitive));
        }
    }
    if (data) cgltf_free(data);
    return model;
}

SubModelResource ModelLoaderGLTF::loadSubMesh(cgltf_primitive& primitive) {
    // VertexLayout
    VertexLayout vl{};
    uint32_t stride = 0;

    if(primitive.attributes_count > 8) LOG_WARN("Can't load all attribute model! Attribute count more than 8!");
    vl.attributeCount = (primitive.attributes_count > 8) ? 8 : primitive.attributes_count;
    for(int i = 0; i < vl.attributeCount; i++) {
        cgltf_accessor* acc = primitive.attributes[i].data;
        cgltf_attribute& attr = primitive.attributes[i];

        LOG_DEBUG(
            "gltf attribute ",
            i,
            " type=", static_cast<int>(attr.type),
            " count=", cgltf_num_components(attr.data->type));

        Attribute& attribute = vl.attributes[i];
        attribute.offset = stride;
        attribute.type = getAttributeType(acc->component_type);
        attribute.count = (uint8_t)cgltf_num_components(acc->type);
        attribute.normalize = acc->normalized;

        stride += attribute.count * componentSize(acc->component_type);
    }
    vl.stride = stride;

    ResourceIndex vertexLayoutIndex = 0;
    if (mngr->getPool<VertexLayout>()->size() == 0)
        vertexLayoutIndex = mngr->createResource<VertexLayout>(vl);
    else {
        for (int i = 0; i < mngr->getPool<VertexLayout>()->size(); i++) {
            VertexLayout otherVL = mngr->getPool<VertexLayout>()->operator[](i);
            if (otherVL == vl) {
                vertexLayoutIndex = i;  
                break;
            } else if (i == mngr->getPool<VertexLayout>()->size() - 1) {
                vertexLayoutIndex = mngr->createResource<VertexLayout>(vl);
                break;
            }
        }
    }
    for (int j = 0; j < vl.attributeCount; j++) {
        cgltf_accessor* acc = primitive.attributes[j].data;

        float values[4] = {};

        cgltf_accessor_read_float(
            acc,
            0,
            values,
            cgltf_num_components(acc->type));

        LOG_DEBUG(
            "SOURCE attr=", j,
            " values=(",
            values[0], ", ",
            values[1], ", ",
            values[2],
            ")");
    }

    VertexLayout& vll = mngr->getResource<VertexLayout>(vertexLayoutIndex);
    for(int i = 0; i < vll.attributeCount; i++) {
        Attribute attr = vll.attributes[i];
        LOG_DEBUG("attrib ", i, " size=", attr.count, " stride=", vl.stride, " offset=", attr.offset);
    }

    // indices 
    uint32_t indexCount = primitive.indices ? (uint32_t)primitive.indices->count : 0;
    size_t indices = 0;

    if (indexCount > 0) {
        indices = arena.allocate(indexCount * sizeof(uint32_t), alignof(uint32_t));
        uint32_t* idxPtr = reinterpret_cast<uint32_t*>(arena.getPtr(indices));
        for (int i = 0; i < indexCount; i++) {
            idxPtr[i] = (uint32_t)cgltf_accessor_read_index(primitive.indices, i);
        }
    }

    uint32_t vertexCount = (uint32_t)primitive.attributes[0].data->count;

    size_t vertexData = arena.allocate(vertexCount * stride, alignof(float));
    //float* v = (float*)arena.getPtr(vertexData);

    for(int i = 0; i < vertexCount; i++) {
        // LOG_DEBUG(
        //     "VERTEX ", i,
        //     " pos=(",
        //     v[i * 8 + 0], ", ",
        //     v[i * 8 + 1], ", ",
        //     v[i * 8 + 2],
        //     ") normal=(",
        //     v[i * 8 + 3], ", ",
        //     v[i * 8 + 4], ", ",
        //     v[i * 8 + 5],
        //     ") uv=(",
        //     v[i * 8 + 6], ", ",
        //     v[i * 8 + 7],
        //     ")");

        uint32_t attrOffset = 0;
        for(int j = 0; j < vl.attributeCount; j++) {
            cgltf_accessor* acc = primitive.attributes[j].data;
            
            uint32_t compSize = componentSize(acc->component_type);
            uint32_t numSize = (uint32_t)cgltf_num_components(acc->type);

            if (!acc->buffer_view || !acc->buffer_view->buffer || !acc->buffer_view->buffer->data) {
                LOG_ERROR("Invalid buffer view or buffer data");
                return SubModelResource{};
            }
            uint8_t* base = (uint8_t*)acc->buffer_view->buffer->data + acc->buffer_view->offset + acc->offset;
            size_t accStride = acc->stride ? acc->stride : compSize * numSize;

            uint8_t* src = base + i * accStride;
            uint8_t* dst = (uint8_t*)arena.getPtr(vertexData) + i * stride + attrOffset;
            memcpy(dst, src, compSize * numSize);

            attrOffset += compSize * numSize;
        }
    }

    // for (uint32_t i = 0; i < 5 && i < vertexCount; ++i) {
    //     float* v =
    //         reinterpret_cast<float*>(arena.getPtr(vertexData) + i * stride);

    //     LOG_DEBUG(
    //         "FINAL VERTEX ", i,
    //         " pos=(",
    //         v[0], ", ", v[1], ", ",
    //         v[2], ") normal=(",
    //         v[3], ", ", v[4], ", ",
    //         v[5], ") uv=(",
    //         v[6], ", ", v[7], ")");
    // }

    ResourceIndex mesh = mngr->createResource<MeshResource>(
        vertexLayoutIndex, 
        vertexData, 
        indices, 
        vertexCount, 
        indexCount
    );

    
    return SubModelResource{mesh, 0};
}
