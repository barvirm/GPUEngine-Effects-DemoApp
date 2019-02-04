#pragma once

#include <string>
#include <memory>

namespace ge {
    namespace sg {
        class Scene;
    }
}

namespace app {
    class SceneLoader {
        public:
            static std::shared_ptr<ge::sg::Scene> loadScene(const char *modelFilePath);
        private:
            static void loadImage(std::string &imageDir, std::shared_ptr<ge::sg::Scene> scene);
    };
}
