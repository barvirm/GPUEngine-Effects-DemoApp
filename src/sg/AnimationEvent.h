#pragma once 


namespace msg {
    class AnimationCallback {
        public:
            virtual void operator()() = 0;
    };

    template<class UnaryFunction>
    class AnimationEvent : public AnimationCallback {
        public:
            AnimationEvent(UnaryFunction func) : f(func) {};
            virtual void operator()() override { 
                if (!called) { 
                    f();
                    called = true;
                }
            }
        protected:
            UnaryFunction f;
            bool called = false;
    };
}
