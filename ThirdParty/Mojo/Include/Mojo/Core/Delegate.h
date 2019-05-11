#pragma once

inline namespace Mojo
{
    template <typename T>
    struct Delegate;

    template <typename R, typename ...Args>
    struct Delegate<R(Args...)>
    {
        using Stub = R(*)(void*, Args...);

        static R StubFunction(void* target, Args... args)
        {
            using Function = R(*)(Args...);
            return ((Function)target)(args...);
        };

        template <typename T, R(T::*method)(Args...)>
        static R StubMethod(void* target, Args... args)
        {
            return (((T*)target)->*method)(args...);
        };

        template <typename T, R(T::*method)(Args...) const>
        static R StubConstMethod(void* target, Args... args)
        {
            return (((const T*)target)->*method)(args...);
        };

        Stub  _stub;
        void* _target;

        Delegate(void)
            : _stub(0)
            , _target(0)
        {
        }

        Delegate(R(*function)(Args...))
        {
            this->Bind(function);
        }

        void Bind(R(*function)(Args...))
        {
            this->_stub   = &StubFunction;
            this->_target = function;
        }

        template <typename T, R(T::*method)(Args...)>
        void Bind(T* object)
        {
            this->_stub   = &StubMethod<T, method>;
            this->_target = object;
        }

        template <typename T, R(T::*method)(Args...) const>
        void Bind(const T* object)
        {
            this->_stub   = &StubConstMethod<T, method>;
            this->_caller = (void*)object;
        }

        R Invoke(Args... args)
        {
            return _stub(_target, args...);
        }

        R operator()(Args... args)
        {
            return this->Invoke(args...);
        }

        operator bool(void) const
        {
            return _stub != 0 && _target != 0;
        }

        friend bool operator==(const Delegate& lhs, const Delegate& rhs)
        {
            return lhs._stub == rhs._stub && lhs._target == rhs._target;
        }

        friend bool operator!=(const Delegate& lhs, const Delegate& rhs)
        {
            return lhs._stub == rhs._stub && lhs._target == rhs._target;
        }
    };
}