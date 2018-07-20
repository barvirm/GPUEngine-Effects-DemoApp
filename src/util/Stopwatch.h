#pragma once

#include <chrono>
#include <type_traits>


namespace app {
    namespace util {

        template<
            typename T,
            typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr, 
            class _Clock = std::chrono::high_resolution_clock 
        >
        class Stopwatch {

            public:
                Stopwatch() = default;
                ~Stopwatch() = default;

                void start() { _startTP = clock::now(); }
                void pause() { _pauseTP = clock::now(); _pause = true; }
                void resume() { _pause = false; _startTP +=  clock::now() - _pauseTP; }
                void reset() { start(); }

                T getTime() {
                    return _pause ? _currentTime : ( _currentTime = std::chrono::duration<T>(clock::now() - _startTP).count() );
                }

                protected:
                    using clock = _Clock;
                    std::chrono::time_point<clock> _startTP;
                    std::chrono::time_point<clock> _pauseTP;
                    T _currentTime;
                    bool _pause = false;
        };
    }
}
#undef TEMPLATE_LIMIT_FLOATING_P
