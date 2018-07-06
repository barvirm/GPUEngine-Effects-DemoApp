//
// Created by makku on 16.1.18.
//

#include <glsg/TextureFactory.h>
#include <glsg/EnumToGL.h>
#include <geGL/Texture.h>


/**
 * Creates texture (2D) from image with mip-maps and GL_LINEAR_MIPMAP_LINEAR (min), GL_LINEAR (mag) and GL_RGBA8 as internal format.
 * The texture is initialized with the data from image.
 * \param img Input image
 * \param context Context to create texture with.
 * \return Newly created texture.
 */
std::shared_ptr<ge::gl::Texture> ge::glsg::DefaultTextureFactory::create(ge::sg::MaterialImageComponent *img, std::shared_ptr<ge::gl::Context> &context) {
    int w = (int) img->image->getWidth();
    int h = (int) img->image->getHeight();
    int l = 0;
    while (h > 0 || w > 0) {
        h >>= 1;
        w >>= 1;
        ++l;
    }
    std::shared_ptr<ge::gl::Texture> tex(std::make_shared<ge::gl::Texture>(context->getFunctionTable(), GL_TEXTURE_2D, GL_RGBA8, l, (GLsizei) img->image->getWidth(), (GLsizei) img->image->getHeight()));
    //copying data might not be needed
    context->glTextureSubImage2DEXT(tex->getId(), GL_TEXTURE_2D, 0, 0, 0, (GLsizei) img->image->getWidth(), (GLsizei) img->image->getHeight(), ge::glsg::translateEnum(img->image->getFormat()), ge::glsg::translateEnum(img->image->getDataType()), img->image->getBits());
    context->glGenerateTextureMipmap(tex->getId());
    context->glTextureParameteri(tex->getId(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    context->glTextureParameteri(tex->getId(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return tex;
}
