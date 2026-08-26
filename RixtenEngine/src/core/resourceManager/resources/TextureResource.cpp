#include "core/resourceManager/resources/TextureResource.h"
#include "core/resourceManager/stb_image.h"
#include "utils/logger.h"

texture textureLoader::load(const char* path) {
    texture texture;
    // stbi call new out of global Rixten arena!
    texture.data = stbi_load(path, &texture.width, &texture.height, &texture.nrChannels, 0);
    if (!texture.data) LOG_ERROR("Failed to load texture");

    return texture;
}

void textureLoader::unload(texture& handle) {
    stbi_image_free(handle.data);
}
