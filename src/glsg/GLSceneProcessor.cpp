#include <glsg/GLSceneProcessor.h>

#include <glsg/GLScene.h>
#include <glsg/EnumToGL.h>
#include <geSG/Scene.h>
#include <geSG/AttributeDescriptor.h>
#include <geSG/Image.h>
#include <geGL/Buffer.h>
#include <geSG/DefaultImage.h>
#include <geGL/Texture.h>

ge::glsg::GLSceneProcessor::GLSceneProcessor(std::shared_ptr<ge::gl::Context> context)
        : textureFactory(std::make_shared<DefaultTextureFactory>()) {
}

/**
 * Processes given scene and creates apropriate geGL objects in given context.
 * \param scene Scene you want to process
 * \param context OpenGL context to use
 * \param textureFactory Factory that is used to create ge::gl::Texture from image. You can suplly your own or use the DefaultTextureFactory.
 *                       If you dont suplly this argument the DefaultTextureFactory is used.
 * \return Returns the GLScene class that holds the dictionaries to map geSG objects to created geGL objects.
 */
std::shared_ptr<ge::glsg::GLScene>
ge::glsg::GLSceneProcessor::processScene(std::shared_ptr<ge::sg::Scene> scene, std::shared_ptr<ge::gl::Context> context, std::shared_ptr<ge::glsg::TextureFactory> textureFactory) {
    std::shared_ptr<ge::glsg::GLScene> glscene(std::make_shared<GLScene>());
    processMeshes(scene, glscene, context);
    createTextures(scene, glscene, context, textureFactory);
    glscene->scene = scene;
    return glscene;
}

/**
 * Processes mesh thus creating collection of GLAttribs that maps AttributeDescriptor onto Buffer.
 * \param scene
 * \param glscene
 * \param context
 */
void ge::glsg::GLSceneProcessor::processMeshes(std::shared_ptr<ge::sg::Scene> &scene, std::shared_ptr<ge::glsg::GLScene> &glscene, std::shared_ptr<ge::gl::Context> context) {
    for (const auto &model : scene->models) {
        for (const auto &mesh : model->meshes) {
            glscene->GLMeshes[mesh.get()] = ge::glsg::GLScene::GLAttribArray();
            for (auto attribute : mesh->attributes) {

                std::shared_ptr<ge::gl::Buffer> buff(std::make_shared<ge::gl::Buffer>(context->getFunctionTable(), attribute->size, (char *) (attribute->data.get()) + attribute->offset));
                glscene->GLMeshes[mesh.get()].emplace_back(attribute, buff);

            }
        }
    }
}

/**
 * Creates a geGL Texture for every geSG Image found in scene materials. For the texture setup and creation it uses a TextureFactory.
 * \param scene
 * \param glscene
 * \param context
 * \param textureFactory
 */
void ge::glsg::GLSceneProcessor::createTextures(std::shared_ptr<ge::sg::Scene> scene, std::shared_ptr<ge::glsg::GLScene> glscene, std::shared_ptr<ge::gl::Context> context,
                                      std::shared_ptr<ge::glsg::TextureFactory> textureFactory) {
    for (const auto &model : scene->models) {
        for (const auto &material : model->materials) {
            //continue if we already processed this material
            for (const auto &materialComponent : material->materialComponents) {
                if (materialComponent->getType() == ge::sg::MaterialComponent::ComponentType::IMAGE) {

                    auto *img = dynamic_cast<ge::sg::MaterialImageComponent *>(materialComponent.get());
                    //if the image of MaterialImageComponent hasn't been created
                    if (img && !img->image) {
                        img->image = std::make_shared<ge::sg::DefaultImage>();
                    }

                    glscene->textures[img] = textureFactory->create(img, context);
                }
            }
        }
    }
}


