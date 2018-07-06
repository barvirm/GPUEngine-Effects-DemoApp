#pragma once

#include <vector>
#include <memory>

namespace ge {
    namespace gl {
        class Texture;
        class Context;
    }
    namespace sg {
        class Image;
    }
}

namespace msg {

    class ImageLoader {
        public:
            virtual ge::sg::Image *loadImage(const std::string &file) const = 0;
    };

    class QtImageLoaderWrapper : public ImageLoader {
        ge::sg::Image *loadImage(const std::string &file) const override;
    };

    class CubeMapTexture {
    public:
        CubeMapTexture() = default;
        std::vector<std::shared_ptr<ge::sg::Image>> images;
        std::shared_ptr<ge::gl::Texture> texture;
        std::shared_ptr<ge::gl::Context> gl;

        void initTexture();
    };

    class CubeMapTextureFactory {
    public:
        static std::shared_ptr<CubeMapTexture> create(const std::string &px, const std::string &nx, const std::string &py, const std::string &ny, const std::string &pz, const std::string &nz, const msg::ImageLoader &imageLoader);
        static std::string dir;

    };
}

