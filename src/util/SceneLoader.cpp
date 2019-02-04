#include <SceneLoader.h>

#include <assimp/postprocess.h>
#include <AssimpModelLoader.h>
#include <QString>
#include <QFileInfo>
#include <QtImageLoader.h>
#include <QImage>

#include <geSG/Scene.h>
#include <geSG/DefaultImage.h>
#include <iostream>


std::shared_ptr<ge::sg::Scene> app::SceneLoader::loadScene(const char *modelFilePath) {
    QString modelFileName(modelFilePath);
    QFileInfo fileInfo(modelFileName);
    std::string modelPath(qUtf8Printable(fileInfo.canonicalPath() + "/"));
	std::cout << modelFileName.toUtf8().constData() << std::endl;
    std::shared_ptr<ge::sg::Scene> scene = std::shared_ptr<ge::sg::Scene>(AssimpModelLoader::loadScene(modelFileName.toUtf8().constData(), aiProcess_Triangulate | aiProcess_SortByPType  | aiProcess_FlipUVs));
    if (scene == nullptr) {
        std::cerr << "Fail to load scene" << std::endl;
    }
    loadImage(modelPath,scene);
    return scene;
}

void app::SceneLoader::loadImage(std::string &imageDir, std::shared_ptr<ge::sg::Scene> scene) {
    std::shared_ptr<ge::sg::DefaultImage> defaultImage(std::make_shared<ge::sg::DefaultImage>());
    for (const auto &model : scene->models) {
        for (const auto &material : model->materials) {
            for (auto &materialComponent : material->materialComponents) {
                if (materialComponent->getType() == ge::sg::MaterialComponent::ComponentType::IMAGE) {
                    auto *materialImage = dynamic_cast<ge::sg::MaterialImageComponent *>(materialComponent.get());
                    std::string textFile(imageDir + materialImage->filePath);
                    std::shared_ptr<QtImage> image(QtImageLoader::loadImage(textFile.c_str()));
                    if (image == nullptr) {
                        std::cerr << "Image fail loading, try replace with default image" << std::endl;
                        materialImage->image = defaultImage;
                    } else {
                        materialImage->image = image;
                    }
                }
            }
        }
    }
}
