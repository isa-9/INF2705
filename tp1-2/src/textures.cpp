#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture2D::Texture2D(const char* path, GLenum wrapMode)
{
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data == NULL)
        std::cout << "Error loading texture \"" << path << "\": " << stbi_failure_reason() << std::endl;

    // TODO: Chargement	de la texture.
	
    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    // TODO: Supprimer la mémoire de l'objet
}

void Texture2D::enableMipmap()
{
    // TODO: Activer le mipmap. N'oublier pas de modifier les paramètres de texture.
}

void Texture2D::use()
{
    // TODO: Utilise la texture
}

void Texture2D::unuse()
{
    // TODO: N'utilise aucune texture
}


TextureCubeMap::TextureCubeMap(const char** pathes)
{
    const size_t N_TEXTURES = 6;
    unsigned char* datas[N_TEXTURES];
    int widths[N_TEXTURES];
    int heights[N_TEXTURES];
    int nChannels[N_TEXTURES];
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < 6; i++)
    {
        datas[i] = stbi_load(pathes[i], &widths[i], &heights[i], &nChannels[i], 0);
        if (datas[i] == NULL)
            std::cout << "Error loading texture \"" << pathes[i] << "\": " << stbi_failure_reason() << std::endl;
    }

    
	// TODO: Chargement des textures du cubemap.
	

    for (unsigned int i = 0; i < 6; i++)
    {
        stbi_image_free(datas[i]);
    }
    

}

TextureCubeMap::~TextureCubeMap()
{
    // TODO: Supprimer la mémoire de l'objet
}

void TextureCubeMap::use()
{
    // TODO: Utilise la texture du cubemap
}
