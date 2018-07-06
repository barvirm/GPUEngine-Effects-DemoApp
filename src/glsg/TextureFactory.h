#pragma once 

#include <memory>

namespace ge {
    namespace gl {
        class Texture;
        class Context;
    }

    namespace sg {
        class MaterialImageComponent;
    }

}


namespace ge {
    namespace glsg {
        /**
         * Base class for texture factory.
         */
        class TextureFactory {
        public:
            /**
             * This function is called with active OpenGL context when the GLSceneProcessor processes
             * MaterialImageComponent.
             */
            virtual std::shared_ptr<ge::gl::Texture> create(ge::sg::MaterialImageComponent *img, std::shared_ptr<ge::gl::Context> &context) = 0;
        };

        /**
         * Used when no custom factory is provided by user. It will generate RGBA8 texture with mipmaps and set min filter to
         * GL_LINEAR_MIPMAP_LINEAR and mag filter to GL_LINEAR
         */
        class DefaultTextureFactory : public TextureFactory {
            std::shared_ptr<ge::gl::Texture> create(ge::sg::MaterialImageComponent *img, std::shared_ptr<ge::gl::Context> &context) override;
        };
    }
}
