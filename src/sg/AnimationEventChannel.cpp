#include <sg/AnimationEventChannel.h>
#include <iostream>
#include <algorithm>
#include <chrono>

void msg::AnimationEventChannel::update(const ge::core::time_point &t) {
    double currentTime = ge::core::TPtoFP(t);
    auto cmp = [](auto &p, const double &tg)->bool{return (tg) >= p.getT();};
    auto it = std::lower_bound(KF.begin(), KF.end(), currentTime, cmp);

    if ( it != KF.begin() ) {
        auto shared_functor = (it-1)->getValue();
        (*shared_functor)();
    }
    
}

ge::core::time_unit msg::AnimationEventChannel::getDuration() const {
    ge::core::time_point end = KF.empty() ? ge::core::time_point() : KF.back().getTime();
    return std::chrono::duration_cast<ge::core::time_unit>(end.time_since_epoch());
}
