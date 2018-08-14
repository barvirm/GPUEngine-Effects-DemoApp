#pragma once 


namespace msg {
    class AnimationCallback {
        public:
            virtual void operator()() = 0;
    };

    template<class UnaryFunction>
    class AnimationEvent : public AnimationCallback {
        public:
            virtual void operator()() override { 
                if (!called) { 
                    f();
                    called = true;
                }
            }
            AnimationEvent(UnaryFunction func) : f(func) {};
        protected:
            UnaryFunction f;
            bool called = false;
    };

    class AnimationEventFactory {
        public:
        template<typename T>
        static auto create(T t) -> std::shared_ptr<AnimationCallback> {
            return std::make_shared<AnimationEvent<T>>(t);
        }
    };
}
