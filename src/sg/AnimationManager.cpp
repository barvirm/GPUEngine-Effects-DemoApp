#include <sg/AnimationManager.h>

void msg::AnimationManager::removeAnimation(std::shared_ptr<ge::sg::Animation> &animation) {
    playlist.remove(animation);
}
