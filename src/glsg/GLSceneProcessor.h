#pragma once

#include <memory>
#include <glsg/TextureFactory.h>

namespace ge {
    namespace gl {
        class Context;
    }

    namespace sg {
        class Scene;
        class Mesh;
    }

    namespace glsg {
        class GLScene;
    }
}

namespace ge {
    namespace glsg {
        /**
         * Class that creates and initializes the GLScene class. It creates ge::gl::Buffer for every AttributeDescriptor
         * And Texture for every MaterialImageComponent. OpenGL context needs to be <b>active</b> before you
         * call process(). You can also supply a custom TextureFactory object to supervise texture and sampler
         * creation.
         */
        class GLSceneProcessor {
        public:
            GLSceneProcessor(std::shared_ptr<ge::gl::Context> context = nullptr);

            static std::shared_ptr<ge::glsg::GLScene> processScene(std::shared_ptr<ge::sg::Scene> scene, std::shared_ptr<ge::gl::Context> context,
                                                                   std::shared_ptr<TextureFactory> textureFactory = std::shared_ptr<DefaultTextureFactory>(
                                                                           std::make_shared<DefaultTextureFactory>()));


            std::shared_ptr<TextureFactory> textureFactory;

        private:
            static void processMeshes(std::shared_ptr<ge::sg::Scene> &scene, std::shared_ptr<ge::glsg::GLScene> &glscene, std::shared_ptr<ge::gl::Context> context);

            //static void createGLAttribs(ge::sg::Mesh* mesh, std::shared_ptr<ge::glsg::GLScene>& glscene);
            static void createTextures(std::shared_ptr<sg::Scene> scene, std::shared_ptr<GLScene> glscene, std::shared_ptr<ge::gl::Context> context,
                                       std::shared_ptr<TextureFactory> textureFactory);
        };
    }

}