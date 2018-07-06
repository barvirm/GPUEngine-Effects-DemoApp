#include <sg/CubeMapTexture.h>
#include <geSG/Image.h>
#include <geGL/Texture.h>
#include <glsg/EnumToGL.h>
#include <QtImageLoader.h>
#include <algorithm>


void msg::CubeMapTexture::initTexture() {
    if (!images.empty()) {
        texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA8, 1, images[0]->getWidth(), images[0]->getHeight());
        texture->bind(0);
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, images[0]->getWidth(), images[0]->getHeight(), ge::glsg::translateEnum(images[0]->getFormat()), ge::glsg::translateEnum(images[0]->getDataType()), images[0]->getBits());
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, images[1]->getWidth(), images[1]->getHeight(), ge::glsg::translateEnum(images[1]->getFormat()), ge::glsg::translateEnum(images[1]->getDataType()), images[1]->getBits());
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, images[2]->getWidth(), images[2]->getHeight(), ge::glsg::translateEnum(images[2]->getFormat()), ge::glsg::translateEnum(images[2]->getDataType()), images[2]->getBits());
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, images[3]->getWidth(), images[3]->getHeight(), ge::glsg::translateEnum(images[3]->getFormat()), ge::glsg::translateEnum(images[3]->getDataType()), images[3]->getBits());
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, images[4]->getWidth(), images[4]->getHeight(), ge::glsg::translateEnum(images[4]->getFormat()), ge::glsg::translateEnum(images[4]->getDataType()), images[4]->getBits());
        gl->glTextureSubImage2DEXT(texture->getId(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, images[5]->getWidth(), images[5]->getHeight(), ge::glsg::translateEnum(images[5]->getFormat()), ge::glsg::translateEnum(images[5]->getDataType()), images[5]->getBits());
    }
}

ge::sg::Image *msg::QtImageLoaderWrapper::loadImage(const std::string &file) const {
    QtImage *image(QtImageLoader::loadImage(file.c_str()));
    return image;
}

std::string msg::CubeMapTextureFactory::dir;

std::shared_ptr<msg::CubeMapTexture> msg::CubeMapTextureFactory::create(const std::string &px, const std::string &nx, const std::string &py, const std::string &ny, const std::string &pz, const std::string &nz, const msg::ImageLoader &imageLoader) {
    std::shared_ptr<msg::CubeMapTexture> cubeMap = std::make_shared<msg::CubeMapTexture>();
    ge::sg::Image *image = nullptr;

    std::vector<std::string> names{px,nx,py,ny,pz,nz};
    for(const std::string &name : names) {
        image = imageLoader.loadImage(dir+name);
        if (image) {
            cubeMap->images.emplace_back(std::shared_ptr<ge::sg::Image>(image));
        }
        else {
            return nullptr;
        }
    }

    return cubeMap;
}
