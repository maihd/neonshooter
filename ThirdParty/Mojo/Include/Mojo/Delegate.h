#pragma once

inline namespace Mojo
{
    template <typename T>
    struct Delegate;

    template <typename R, typename ...Args>
    struct Delegate<R(Args...)>
    {
        using Stub = R(*)(void*, void*, Args...);

        static R StubFunction(void*, void* callee, Args... args)
        {
            using Function = R(*)(Args...);
            return ((Function)callee)(args...);
        };

        template <typename T>
        static R StubMethod(void* caller, void* callee, Args... args)
        {
            union
            {
                void* callee;
                R(T::*method)(Args...);
            } convert;
            convert.callee = callee;

            return (((T*)caller)->*convert.method)(args...);
        };

        Stub  _stub;
        void* _caller;
        void* _callee;

        Delegate(void)
            : _stub(NULL)
            , _caller(NULL)
            , _callee(NULL)
        {
        }

        Delegate(R(*function)(Args...))
        {
            this->Bind(function);
        }

        template <typename T>
        Delegate(T* object, R(T::*method)(Args...))
        {
            this->Bind(object, method);
        }

        template <typename T>
        Delegate(const T* object, R(T::*method)(Args...) const)
        {
            this->Bind(object, method);
        }

        void Bind(R(*function)(Args...))
        {
            this->_stub   = &StubFunction;
            this->_caller = NULL;
            this->_callee = function;
        }

        template <typename T>
        void Bind(T* object, R(T::*method)(Args...))
        {
            union
            {
                void* callee;
                R(T::*method)(Args...);
            } convert;
            convert.method = method;

            this->_stub   = &StubMethod<T>;
            this->_caller = object;
            this->_callee = convert.callee;
        }

        template <typename T>
        void Bind(const T* object, R(T::*method)(Args...) const)
        {
            union
            {
                void* callee;
                R(T::*method)(Args...) const;
            } convert;
            convert.method = method;

            this->_stub   = &StubMethod<T>;
            this->_caller = (void*)object;
            this->_callee = convert.callee;
        }

        R Invoke(Args... args)
        {
            return _stub(_caller, _callee, args...);
        }

        R operator()(Args... args)
        {
            return this->Invoke(args...);
        }

        operator bool(void) const
        {
            return _callee != NULL;
        }

        friend bool operator==(const Delegate& lhs, const Delegate& rhs)
        {
            return lhs._callee == rhs._callee && lhs._caller == rhs._caller;
        }

        friend bool operator!=(const Delegate& lhs, const Delegate& rhs)
        {
            return lhs._callee != rhs._callee || lhs._caller != rhs._caller;
        }
    };

    template <typename R, typename ...Args>
    Delegate<R(Args...)> BindDelegate(R(*function)(Args...))
    {
        return Delegate<R(Args...)>(function);
    }

    template <typename T, typename R, typename ...Args>
    Delegate<R(Args...)> BindDelegate(T* object, R(T::*method)(Args...))
    {
        return Delegate<R(Args...)>(object, method);
    }

    template <typename T, typename R, typename ...Args>
    Delegate<R(Args...)> BindDelegate(const T* object, R(T::*method)(Args...) const)
    {
        return Delegate<R(Args...)>(object, method);
    }
}