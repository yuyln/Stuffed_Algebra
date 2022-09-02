#ifndef ___
#define ___

#include <cstdlib>
#include <cstring>
#include <functional>
#include <cmath>
#include <cstdint>

#define ARRAY_NEW_SIZE_FACTOR (2 << 5)
#define MAX_VEC_DIMS (2 << 10)
#define SEPARATOR " "
#define COMPLEX_IMG "i"
#define COMPLEX_BET ""


#ifndef NO_ARRAY_ASSERTION
#include <cassert>
#define array_assert(cond) assert(cond)
#else
#define array_assert(cond)
#pragma message ("Removing Array assertions can improve performance, however, undefined behavior have HIGH probability of happening")
#endif

#ifndef NO_VEC_ASSERTION
#include <cassert>
#define vec_assert(cond) assert(cond)
#else
#define vec_assert(cond)
#pragma message ("Removing Vec assertions can improve performance, however, undefined behavior have HIGH probability of happening")
#endif

#ifndef NO_MATRIX_ASSERTION
#include <cassert>
#define matrix_assert(cond) assert(cond)
#else
#define matrix_assert(cond)
#pragma message ("Removing Matrix assertions can improve performance, however, undefined behavior have HIGH probability of happening")
#endif

//VEC
namespace my
{
    #ifdef VEC_DEBUG
    static std::size_t VEC_DEBUG_COUNT = 0;
    #endif

    template <typename E, typename T>
    class VecExpression
    {
    public:
        T operator[](std::size_t i) const { return static_cast<const E&>(*this)[i]; }
        T operator()(std::size_t i) const { return static_cast<const E&>(*this)(i); }
        std::size_t dim() const { return static_cast<const E&>(*this).dim(); }
    };

    template <std::size_t dims, typename T>
    class Vec : public VecExpression<Vec<dims, T>, T>
    {
    public:
        static_assert(dims * sizeof(T) < MAX_VEC_DIMS, "You're trying to use a my::Vec with "
                                                        "a resulting size greater than 2^10. "
                                                        "If you really need a my::Vec this big "
                                                        "consider changing MAX_VEC_DIMS, but "
                                                        "by doing that, you should know your risks. "
                                                        "Consider using my::Array instead.");

        Vec()
        {
            for (std::size_t i = 0; i < dims; ++i)
                data[i] = T();
            #ifdef VEC_DEBUG
            std::cout << "CLEAN CONSTRUCT\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif
        }

        template <typename... args>
        Vec(T item0, args... rest)
        {
            T new_data[] = {item0, rest...};
            #ifdef VEC_DEBUG
            std::cout << "VARIADIC TEMPLATE CONSTRUCT\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif
            static_assert(sizeof(new_data) / sizeof(T) == dims, "Number of arguments doesn't match with number of dimension for vector");
            for (std::size_t i = 0; i < dims; ++i)
                data[i] = new_data[i];
        }

        Vec(const Vec<dims, T> &o)
        {
            #ifdef VEC_DEBUG
            std::cout << "COPY CONSTRUCT\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif

            for (std::size_t i = 0; i < dims; ++i)
                data[i] = o.data[i];
        }

        Vec(Vec<dims, T> &&o)
        {
            #ifdef VEC_DEBUG
            std::cout << "MOVE CONSTRUCT\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif

            for (std::size_t i = 0; i < dims; ++i)
                data[i] = o.data[i];
        }
        
        template <typename K>
        Vec(const VecExpression<K, T>& expr)
        {
            #ifdef VEC_DEBUG
            std::cout << "COPY EXPR CONSTRUCT\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif
            vec_assert(dims == expr.dim());
            for (std::size_t i = 0; i < dims; ++i)
                data[i] = expr[i];
        }

        ~Vec() = default;

        Vec<dims, T>& operator=(const Vec<dims, T>& o)
        {
            #ifdef VEC_DEBUG
            std::cout << "COPY ASSIGN\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif

            for (std::size_t i = 0; i < dims; ++i)
                data[i] = o.data[i];
            return *this;
        }

        Vec<dims, T>& operator=(Vec<dims, T>&& o)
        {
            #ifdef VEC_DEBUG
            std::cout << "MOVE ASSIGN\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif

            for (std::size_t i = 0; i < dims; ++i)
                data[i] = o.data[i];
            return *this;
        }

        template <typename K>
        Vec<dims, T>& operator=(const VecExpression<K, T>& expr)
        {
            #ifdef VEC_DEBUG
            std::cout << "COPY EXPR ASSIGN\t" << "VEC DEBUG COUNT: " << VEC_DEBUG_COUNT++ << std::endl;
            #endif
            
            vec_assert(dims == expr.dim());
            for (std::size_t i = 0; i < dims; ++i)
                data[i] = expr[i];
            return *this;
        }

        //removed %dims
        const T& operator[] (std::size_t i) const { return data[i]; }
        const T& operator() (std::size_t i) const { return data[i]; }

        T& operator[] (std::size_t i) { return data[i]; }
        T& operator() (std::size_t i) { return data[i]; }

        std::size_t dim() const { return dims; }

        Vec<dims, T>& forEach(const std::function<T(std::size_t)> &func)
        {
            for (std::size_t i = 0; i < dims; ++i)
                data[i] = func(i);
        }

    private:
        T data[dims];
    };

    template <typename P1, typename P2, typename T>
    class VecSum : public VecExpression<VecSum<P1, P2, T>, T>
    {
    public:
        VecSum(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.dim() != p2.dim())
            {
                std::cerr << "Vecs with different dimensions: " << p1.dim() << " x " << p2.dim() << std::endl;
                exit(-1);
            }
        }
        T operator[] (std::size_t i) const { return p1[i] + p2[i]; }
        T operator() (std::size_t i) const { return p1[i] + p2[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    VecSum<P1, P2, T> operator+(const VecExpression<P1, T>& v1, const VecExpression<P2, T>& v2)
    {
        return VecSum<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
            );
    }

    template <typename P1, typename P2, typename T>
    class VecSub : public VecExpression<VecSub<P1, P2, T>, T>
    {
    public:
        VecSub(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.dim() != p2.dim())
            {
                std::cerr << "Vecs with different dimensions: " << p1.dim() << " x " << p2.dim() << std::endl;
                exit(-1);
            }
        }
        T operator[] (std::size_t i) const { return p1[i] - p2[i]; }
        T operator() (std::size_t i) const { return p1[i] - p2[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    VecSub<P1, P2, T> operator-(const VecExpression<P1, T>& v1, const VecExpression<P2, T>& v2)
    {
        return VecSub<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
            );
    }

    template <typename P1, typename P2, typename T>
    class VecMul : public VecExpression<VecMul<P1, P2, T>, T>
    {
    public:
        VecMul(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.dim() != p2.dim())
            {
                std::cerr << "Vecs with different dimensions: " << p1.dim() << " x " << p2.dim() << std::endl;
                exit(-1);
            }
        }
        T operator[] (std::size_t i) const { return p1[i] * p2[i]; }
        T operator() (std::size_t i) const { return p1[i] * p2[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    VecMul<P1, P2, T> operator*(const VecExpression<P1, T>& v1, const VecExpression<P2, T>& v2)
    {
        return VecMul<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
            );
    }

    template <typename P1, typename P2, typename T>
    class VecDiv : public VecExpression<VecDiv<P1, P2, T>, T>
    {
    public:
        VecDiv(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.dim() != p2.dim())
            {
                std::cerr << "Vecs with different dimensions: " << p1.dim() << " x " << p2.dim() << std::endl;
                exit(-1);
            }
        }        
        T operator[] (std::size_t i) const { return p1[i] / p2[i]; }
        T operator() (std::size_t i) const { return p1[i] / p2[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    VecDiv<P1, P2, T> operator/(const VecExpression<P1, T>& v1, const VecExpression<P2, T>& v2)
    {
        return VecDiv<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
            );
    }

    template <typename P1, typename T>
    class VecScaM : public VecExpression<VecScaM<P1, T>, T>
    {
    public:
        VecScaM(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[] (std::size_t i) const { return p1[i] * s; }
        T operator() (std::size_t i) const { return p1[i] * s; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    VecScaM<P1, T> operator*(const VecExpression<P1, T>& v1, const T& v2)
    {
        return VecScaM<P1, T>(
            *static_cast<const P1*>(&v1),
            v2
            );
    }

    template <typename P1, typename T>
    VecScaM<P1, T> operator*(const T& v2, const VecExpression<P1, T>& v1)
    {
        return VecScaM<P1, T>(
            *static_cast<const P1*>(&v1),
            v2
            );
    }

    template <typename P1, typename T>
    class VecScaDiv : public VecExpression<VecScaDiv<P1, T>, T>
    {
    public:
        VecScaDiv(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[] (std::size_t i) const { return p1[i] / s; }
        T operator() (std::size_t i) const { return p1[i] / s; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    VecScaDiv<P1, T> operator/(const VecExpression<P1, T>& v1, const T& v2)
    {
        return VecScaDiv<P1, T>(
            *static_cast<const P1*>(&v1),
            v2
            );
    }

    template <typename P1, typename T>
    class VecScaDivI : public VecExpression<VecScaDivI<P1, T>, T>
    {
    public:
        VecScaDivI(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[] (std::size_t i) const { return s / p1[i]; }
        T operator() (std::size_t i) const { return s / p1[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    VecScaDivI<P1, T> operator/(const T& v2, const VecExpression<P1, T>& v1)
    {
        return VecScaDivI<P1, T>(
            *static_cast<const P1*>(&v1),
            v2
            );
    }

    template <typename P1, typename T>
    class VecMinus : public VecExpression<VecMinus<P1, T>, T>
    {
    public:
        VecMinus(const P1& p1_): p1(p1_){ }
        T operator[] (std::size_t i) const { return -p1[i]; }
        T operator() (std::size_t i) const { return -p1[i]; }
        std::size_t dim() const { return p1.dim(); }

    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    VecMinus<P1, T> operator-(const VecExpression<P1, T>& v1)
    {
        return VecMinus<P1, T>(
            *static_cast<const P1*>(&v1)
            );
    }

    template <typename P1, typename P2, typename T>
    T operator^(const VecExpression<P1, T>& v1, const VecExpression<P2, T>& v2)
    {
        if (v1.dim() != v2.dim())
        {
            std::cerr << "Trying to dot product with vecs of different lengths: " << v1.dim() << " x " << v2.dim() << std::endl;
            exit(-1);
        }
        T ret = T();
        for (std::size_t i = 0; i < v1.dim(); ++i)
            ret = ret + v1[i] * v2[i];
        return ret;
    }

    template <typename E, typename T>
    std::ostream& operator<<(std::ostream& out, const VecExpression<E, T>& v)
    {
        for (std::size_t i = 0; i < v.dim() - 1; ++i)
            out << v[i] << SEPARATOR;
        std::size_t i = v.dim() - 1;
        out << v[i];
        return out;
    }

    using Vec2f = Vec<2, float>;
    using Vec2d = Vec<2, double>;
    using Vec2i = Vec<2, int>;

    using Vec3f = Vec<3, float>;
    using Vec3d = Vec<3, double>;
    using Vec3i = Vec<3, int>;
}

//ARRAY
namespace my
{
    #ifdef ARRAY_DEBUG
    static std::size_t ARRAY_DEBUG_COUNT = 0;
    #endif

    template <typename E, typename T>
    class ArrayExpression
    {
    public:
        std::size_t len() const { return static_cast<const E&>(*this).len(); }
        T operator[](std::size_t i) const { return static_cast<const E&>(*this)[i]; }
    };

    template <typename T>
    class Array : public ArrayExpression<Array<T>, T>
    {
    public:
        Array(): data(nullptr), sz_cached(ARRAY_NEW_SIZE_FACTOR), elems(0)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "CLEAN CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(ARRAY_NEW_SIZE_FACTOR, sizeof(T));
        }
        
        Array(std::size_t n): sz_cached(n), elems(n)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "SIZE CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            std::cout << "SIZEOF(T): " << sizeof(T) << " DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT << std::endl;
            #endif
            data = (T*)calloc(n, sizeof(T));
        }

        Array(std::size_t n, const std::function<T(std::size_t)> &func): sz_cached(n), elems(n)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "FUNCTION CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(n, sizeof(T));
            for (std::size_t i = 0; i < n; ++i)
                data[i] = func(i);
        }

        Array(const Array<T>& o): sz_cached(o.sz_cached), elems(o.elems)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "COPY CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(sz_cached, sizeof(T));
            for (std::size_t i = 0; i < elems; ++i)
                data[i] = o.data[i];
        }

        Array(Array<T>&& o): sz_cached(o.sz_cached), elems(o.elems)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "MOVE CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            data = o.data;
            o.data = nullptr;
        }

        template <typename E>
        Array(const ArrayExpression<E, T>& expr)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "EXPR COPY CONSTRUCT\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            sz_cached = expr.len();
            elems = expr.len();
            data = (T*)calloc(expr.len(), sizeof(T));
            for (std::size_t i = 0; i < expr.len(); ++i)
                data[i] = expr[i];
        }

        Array<T>& operator=(const Array<T>& o)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "COPY ASSING\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            elems = o.elems;
            sz_cached = o.sz_cached;
            if(data != nullptr)
                free(data);
            data = (T*)calloc(sz_cached, sizeof(T));
            for (std::size_t i = 0; i < elems; ++i)
                data[i] = o[i];
            return *this;
        }

        Array<T>& operator=(Array<T>&& o)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "MOVE ASSING\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            elems = o.elems;
            sz_cached = o.sz_cached;
            if (data != nullptr)
                free(data);
            data = o.data;
            o.data = nullptr;
            return *this;
        }

        template <typename E>
        Array<T>& operator=(const ArrayExpression<E, T>& expr)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "EXPR ASSING\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            elems = expr.len();
            sz_cached = elems;
            if (data != nullptr)
                free(data);
            data = (T*)calloc(elems, sizeof(T));
            for (std::size_t i = 0; i < elems; ++i)
                data[i] = expr[i];
            return *this;
        }

        ~Array()
        {
            #ifdef ARRAY_DEBUG
            std::cout << "ARRAY DESTRUCTOR\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            if (data != nullptr)
                free(data);
        }

        std::size_t len() const { return elems; }
        //removed %elems and array_assert(elems > 0)
        const T& operator[](std::size_t i) const{ return data[i]; }
        T& operator[](std::size_t i) { return data[i]; }
        Array<T>& insert(const T& obj)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "INSERT COPY\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            if (elems == sz_cached)
            {
                #ifdef ARRAY_DEBUG
                std::cout << "REALLOC\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
                #endif

                data = (T*)realloc((void*)data, sizeof(T) * (sz_cached + ARRAY_NEW_SIZE_FACTOR));
                array_assert(data != NULL);
                sz_cached += ARRAY_NEW_SIZE_FACTOR;
            }
            data[elems] = obj;
            ++elems;
            return *this;
        }
        
        Array<T>& insert(T&& obj)
        {
            #ifdef ARRAY_DEBUG
            std::cout << "INSERT MOVE\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
            #endif
            if (elems == sz_cached)
            {
                #ifdef ARRAY_DEBUG
                std::cout << "REALLOC\t" << "DEBUG ARRAY COUNT: " << ARRAY_DEBUG_COUNT++ << std::endl;
                #endif

                data = (T*)realloc((void*)data, sizeof(T) * (sz_cached + ARRAY_NEW_SIZE_FACTOR));
                array_assert(data != NULL);
                sz_cached += ARRAY_NEW_SIZE_FACTOR;
            }
            data[elems] = std::move(obj);
            ++elems;
            return *this;
        }

        const T& pop()
        {
            return data[elems-- - 1];
        }

        Array<T>& forEach(const std::function<T(std::size_t, T)> &func)
        {
            for (std::size_t i = 0; i < elems; ++i)
                data[i] = func(i, data[i]);
            return *this;
        }

    private:
        T *data;
        std::size_t sz_cached;
        std::size_t elems;
    };

    template <typename P1, typename P2, typename T>
    class ArraySum : public ArrayExpression<ArraySum<P1, P2, T>, T>
    {
    public:
        ArraySum(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.len() != p2.len())
            {
                std::cerr << "Arrays with different size: " << p1.len() << " x " << p2.len() << std::endl;
            }
        }
        T operator[](std::size_t i) const { return p1[i] + p2[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ArraySum<P1, P2, T> operator+(const ArrayExpression<P1, T>& v1, const ArrayExpression<P2, T>& v2)
    {
        return ArraySum<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
        );
    }

    template <typename P1, typename P2, typename T>
    class ArraySub : public ArrayExpression<ArraySub<P1, P2, T>, T>
    {
    public:
        ArraySub(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.len() != p2.len())
            {
                std::cerr << "Arrays with different size: " << p1.len() << " x " << p2.len() << std::endl;
            }
        }
        T operator[](std::size_t i) const { return p1[i] - p2[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ArraySub<P1, P2, T> operator-(const ArrayExpression<P1, T>& v1, const ArrayExpression<P2, T>& v2)
    {
        return ArraySub<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
        );
    }

    template <typename P1, typename P2, typename T>
    class ArrayMul : public ArrayExpression<ArrayMul<P1, P2, T>, T>
    {
    public:
        ArrayMul(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.len() != p2.len())
            {
                std::cerr << "Arrays with different size: " << p1.len() << " x " << p2.len() << std::endl;
            }
        }
        T operator[](std::size_t i) const { return p1[i] * p2[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ArrayMul<P1, P2, T> operator*(const ArrayExpression<P1, T>& v1, const ArrayExpression<P2, T>& v2)
    {
        return ArrayMul<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
        );
    }

    template <typename P1, typename P2, typename T>
    class ArrayDiv : public ArrayExpression<ArrayDiv<P1, P2, T>, T>
    {
    public:
        ArrayDiv(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.len() != p2.len())
            {
                std::cerr << "Arrays with different size: " << p1.len() << " x " << p2.len() << std::endl;
            }
        }
        T operator[](std::size_t i) const { return p1[i] / p2[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ArrayDiv<P1, P2, T> operator/(const ArrayExpression<P1, T>& v1, const ArrayExpression<P2, T>& v2)
    {
        return ArrayDiv<P1, P2, T>(
            *static_cast<const P1*>(&v1),
            *static_cast<const P2*>(&v2)
        );
    }

    template <typename P1, typename T>
    class ArraySca : public ArrayExpression<ArraySca<P1, T>, T>
    {
    public:
        ArraySca(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[](std::size_t i) const { return s * p1[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ArraySca<P1, T> operator*(const T& s, const ArrayExpression<P1, T>& v1)
    {
        return ArraySca<P1, T>(
            *static_cast<const P1*>(&v1), 
            s
        );
    }

    template <typename P1, typename T>
    ArraySca<P1, T> operator*(const ArrayExpression<P1, T>& v1, const T& s)
    {
        return ArraySca<P1, T>(
            *static_cast<const P1*>(&v1), 
            s
        );
    }

    template <typename P1, typename T>
    class ArrayScaD : public ArrayExpression<ArrayScaD<P1, T>, T>
    {
    public:
        ArrayScaD(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[](std::size_t i) const { return p1[i] / s; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ArrayScaD<P1, T> operator/(const ArrayExpression<P1, T>& v1, const T& s)
    {
        return ArrayScaD<P1, T>(
            *static_cast<const P1*>(&v1), 
            s
        );
    }

    template <typename P1, typename T>
    class ArrayScaI : public ArrayExpression<ArrayScaI<P1, T>, T>
    {
    public:
        ArrayScaI(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T operator[](std::size_t i) const { return s / p1[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ArrayScaI<P1, T> operator/(const T& s, const ArrayExpression<P1, T>& v1)
    {
        return ArrayScaI<P1, T>(
            *static_cast<const P1*>(&v1), 
            s
        );
    }

    template <typename P1, typename T>
    class ArrayNeg : public ArrayExpression<ArrayNeg<P1, T>, T>
    {
    public:
        ArrayNeg(const P1& p1_): p1(p1_) { }
        T operator[](std::size_t i) const { return -p1[i]; }
        std::size_t len() const { return p1.len(); }
    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    ArrayNeg<P1, T> operator-(const ArrayExpression<P1, T>& v1)
    {
        return ArrayNeg<P1, T>(
            *static_cast<const P1*>(&v1)
        );
    }

    template <typename P1, typename P2, typename T>
    T operator^(const ArrayExpression<P1, T>& p1, const ArrayExpression<P2, T>& p2)
    {
        if (p1.len() != p2.len())
        {
            std::cerr << "Trying to dot product with arrays of different lengths: " << p1.len() << " x " << p2.len() << std::endl;
            exit(-1);
        }

        T ret = T();
        for (std::size_t i = 0; i < p1.len(); ++i)
            ret = ret + p1[i] * p2[i];
        return ret;
    }

    template <typename E, typename T>
    std::ostream& operator<<(std::ostream& out, const ArrayExpression<E, T>& v)
    {
        if (v.len() == 0)
            return out;
        
        for (std::size_t i = 0; i < v.len() - 1; ++i)
            out << v[i] << SEPARATOR;

        std::size_t i = v.len() - 1;
        out << v[i];
        return out;
    }
}

//MATRIX
//in principle the matrix should not change its size by inserting rows and cols, just 
//by a expansion
//but i do have an urge to implement a resizing matrix, such as
//my::Array<my::Array<T>>
//one day, maybe

namespace my
{

    #ifdef MATRIX_DEBUG
    static std::size_t MATRIX_DEBUG_COUNT = 0;
    #endif

    template <typename E, typename T>
    class MatrixExpression
    {
    public:
        //T operator[] (std::size_t i) const { return static_cast<const E&>(*this)[i]; }
        T operator() (std::size_t row, std::size_t col) const { return static_cast<const E&>(*this)(row, col); }
        std::size_t rows() const { return static_cast<const E&>(*this).rows(); }
        std::size_t cols() const { return static_cast<const E&>(*this).cols(); }
    };

    template <typename T>
    class Matrix : public MatrixExpression<Matrix<T>, T>
    {
    public:
        Matrix(): m_rows(0), m_cols(0), data(nullptr)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "CLEAN CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
        }

        Matrix(std::size_t rows_, std::size_t cols_): m_rows(rows_), m_cols(cols_)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "ROWS AND COLS CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(m_rows * m_cols, sizeof(T));
        }

        Matrix(std::size_t rows_, std::size_t cols_, const std::function<T(std::size_t, std::size_t)> &func): 
        m_rows(rows_), m_cols(cols_)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "FUNC CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(m_rows * m_cols, sizeof(T));
            for (std::size_t i = 0; i < m_rows; ++i)
                for (std::size_t j = 0; j < m_cols; ++j)
                    (*this)(i, j) = func(i, j);
        }

        Matrix(const Matrix<T>& o): m_rows(o.m_rows), m_cols(o.m_cols)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "COPY CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(m_rows * m_cols, sizeof(T));
            for (std::size_t i = 0; i < m_rows * m_cols; ++i)
                data[i] = o.data[i];
        }

        Matrix(Matrix<T>&& o): m_rows(o.m_rows), m_cols(o.m_cols)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "MOVE CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            data = o.data;
            o.m_rows = o.m_cols = 0;
            o.data = nullptr;
        }

        template <typename E>
        Matrix(const MatrixExpression<E, T>& expr): m_rows(expr.rows()), m_cols(expr.cols())
        {
            #ifdef MATRIX_DEBUG
            std::cout << "EXPR COPY CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            data = (T*)calloc(m_rows * m_cols, sizeof(T));
            matrix_assert(data != NULL);
            for (std::size_t i = 0; i < m_rows; ++i)
                for (std::size_t j = 0; j < m_cols; ++j)
                    (*this)(i, j) = expr(i, j);
        }

        Matrix<T>& operator=(const Matrix<T>& o)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "COPY ASSIGN\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            if (data != nullptr)
                free(data);
            m_rows = o.m_rows;
            m_cols = o.m_cols;
            data = (T*)calloc(m_rows * m_cols, sizeof(T));
            for (std::size_t i = 0; i < m_rows * m_cols; ++i)
                data[i] = o.data[i];
            
            return *this;
        }

        Matrix<T>& operator=(Matrix<T>&& o)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "MOVE ASSIGN\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            m_rows = o.m_rows;
            m_cols = o.m_cols;
            data = o.data;
            o.m_rows = o.m_cols = 0;
            o.data = nullptr;
            return *this;
        }

        template <typename E>
        Matrix<T>& operator=(const MatrixExpression<E, T>& expr)
        {
            #ifdef MATRIX_DEBUG
            std::cout << "EXPR COPY CONSTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            m_rows = expr.rows();
            m_cols = expr.cols();

            if (data != nullptr)
                free(data);

            data = (T*)calloc(m_rows * m_cols, sizeof(T));
            matrix_assert(data != NULL);
            for (std::size_t i = 0; i < m_rows; ++i)
                for (std::size_t j = 0; j < m_cols; ++j)
                    (*this)(i, j) = expr(i, j);
            return *this;
        }

        ~Matrix()
        {
            #ifdef MATRIX_DEBUG
            std::cout << "MATRIX DESTRUCT\t" << "MATRIX DEBUG COUNT: " << MATRIX_DEBUG_COUNT++ << std::endl;
            #endif
            if (data != nullptr)
                free(data);
        }

        T& operator()(std::size_t row, std::size_t col) { return data[row * m_cols + col]; }
        const T& operator()(std::size_t row, std::size_t col) const { return data[row * m_cols + col]; }

        std::size_t rows() const { return m_rows; }
        std::size_t cols() const { return m_cols; }

        Matrix<T>& insertRow()
        {
            if (!m_cols)
                return *this;

            data = (T*)realloc((void*)data, sizeof(T) * (m_rows + 1) * m_cols);
            matrix_assert(data != NULL);
            ++m_rows;
            return *this;
        }

        Matrix<T>& insertCol()
        {
            if (!m_rows)
                return *this;
            
            expand(m_rows, m_cols + 1);
            return *this;
        }

        Matrix<T>& insertRow(const T* row_data)
        {
            insertRow();
            for (std::size_t j = 0; j < m_cols; ++j)
                (*this)(m_rows - 1, j) = row_data[j];
            
            return *this;
        }

        Matrix<T>& insertRow(const Array<T>& row_data)
        {
            if (m_cols == 0)
            {
                m_cols = row_data.len();
                data = (T*)calloc(m_cols, sizeof(T));
                matrix_assert(data != NULL);
            }

            matrix_assert(row_data.len() == m_cols);
            insertRow();
            for (std::size_t j = 0; j < m_cols; ++j)
                (*this)(m_rows - 1, j) = row_data[j];
            
            return *this;
        }

        template <std::size_t dim>
        Matrix<T>& insertRow(const Vec<dim, T>& row_data)
        {
            if (m_cols == 0)
            {
                m_cols = dim;
                data = (T*)calloc(m_cols, sizeof(T));
                matrix_assert(data != NULL);
            }


            matrix_assert(dim == m_cols);
            insertRow();
            for (std::size_t j = 0; j < m_cols; ++j)
                (*this)(m_rows - 1, j) = row_data[j];
            
            return *this;
        }

        Matrix<T>& insertCol(const T* col_data)
        {
            insertCol();
            for (std::size_t i = 0; i < m_rows; ++i)
                (*this)(i, m_cols - 1) = col_data[i];
            
            return *this;
        }

        Matrix<T>& insertCol(const Array<T>& col_data)
        {
            if (m_rows == 0)
            {
                m_rows = col_data.len();
                data = (T*)calloc(m_rows, sizeof(T));
                matrix_assert(data != NULL);
            }


            matrix_assert(col_data.len() == m_rows);
            insertCol();
            for (std::size_t i = 0; i < m_rows; ++i)
                (*this)(i, m_cols - 1) = col_data[i];
            
            return *this;
        }

        template <std::size_t dim>
        Matrix<T>& insertCol(const Vec<dim, T>& col_data)
        {
            if (m_rows == 0)
            {
                m_rows = dim;
                data = (T*)calloc(m_rows, sizeof(T));
                matrix_assert(data != NULL);
            }


            matrix_assert(dim == m_rows);
            insertCol();
            for (std::size_t i = 0; i < m_rows; ++i)
                (*this)(i, m_cols - 1) = col_data[i];
            
            return *this;
        }

        Matrix<T>& expand(std::size_t new_row, std::size_t new_col)
        {
            T *new_data = (T*)calloc(new_row * new_col, sizeof(T));
            matrix_assert(new_data != NULL);

            std::size_t min_row = new_row < m_rows? new_row: m_rows;
            std::size_t min_col = new_col < m_cols? new_col: m_cols;
            for (std::size_t i = 0; i < min_row; ++i)
                for (std::size_t j = 0; j < min_col; ++j)
                    new_data[i * new_col + j] = (*this)(i, j);

            m_rows = new_row;
            m_cols = new_col;

            if (data != nullptr)
                free(data);
            
            data = new_data;
            new_data = nullptr;
            return *this;
        }

        Matrix<T>& forEach(const std::function<T(std::size_t, std::size_t, T)> &func)
        {
            for (std::size_t i = 0; i < m_rows; ++i)
                for (std::size_t j = 0; j < m_cols; ++j)
                    (*this)(i, j) = func(i, j, (*this)(i, j));
            return *this;
        }

        T determinant()
        {
            matrix_assert(0 && "TODO: Implement.");
        }

    private:
        std::size_t m_rows, m_cols;
        T *data;
        //std::size_t rows_cached, cols_cached
    };

    template <typename P1, typename P2, typename T>
    class MatrixSum : public MatrixExpression<MatrixSum<P1, P2, T>, T>
    {
    public:
        MatrixSum(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (!(p1.rows() == p2.rows() && p1.cols() == p2.cols()))
            {
                std::cerr << "Trying to operate matrices with different dimensions (" << p1.rows() << "x" << p1.cols() << ")" <<
                " and (" << p2.rows() << "x" << p2.cols() << ")" << std::endl;
                exit(-1);
            }
        }

        T operator()(std::size_t row, std::size_t col) const
        {
            return p1(row, col) + 
                   p2(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixSum<P1, P2, T> operator+(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixSum<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename P2, typename T>
    class MatrixSub : public MatrixExpression<MatrixSub<P1, P2, T>, T>
    {
    public:
        MatrixSub(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (!(p1.rows() == p2.rows() && p1.cols() == p2.cols()))
            {
                std::cerr << "Trying to operate matrices with different dimensions (" << p1.rows() << "x" << p1.cols() << ")" <<
                " and (" << p2.rows() << "x" << p2.cols() << ")" << std::endl;
                exit(-1);
            }
        }

        T operator()(std::size_t row, std::size_t col) const
        {
            return p1(row, col) - 
                   p2(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixSub<P1, P2, T> operator-(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixSub<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename P2, typename T>
    class MatrixMultK : public MatrixExpression<MatrixMultK<P1, P2, T>, T>
    {
    public:
        MatrixMultK(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (!(p1.rows() == p2.rows() && p1.cols() == p2.cols()))
            {
                std::cerr << "Trying to operate matrices with different dimensions (" << p1.rows() << "x" << p1.cols() << ")" <<
                " and (" << p2.rows() << "x" << p2.cols() << ")" << std::endl;
                exit(-1);
            }
        }

        T operator()(std::size_t row, std::size_t col) const
        {
            return p1(row, col) * 
                   p2(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixMultK<P1, P2, T> operator&(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixMultK<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename P2, typename T>
    class MatrixDiv : public MatrixExpression<MatrixDiv<P1, P2, T>, T>
    {
    public:
        MatrixDiv(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (!(p1.rows() == p2.rows() && p1.cols() == p2.cols()))
            {
                std::cerr << "Trying to operate matrices with different dimensions (" << p1.rows() << "x" << p1.cols() << ")" <<
                " and (" << p2.rows() << "x" << p2.cols() << ")" << std::endl;
                exit(-1);
            }
        }

        T operator()(std::size_t row, std::size_t col) const
        {
            return p1(row, col) / 
                   p2(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixDiv<P1, P2, T> operator/(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixDiv<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename P2, typename T>
    class MatrixMult : public MatrixExpression<MatrixMult<P1, P2, T>, T>
    {
    public:
        MatrixMult(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_)
        {
            if (p1.cols() != p2.rows())
            {
                std::cerr << "Cols of Matrix A(" << p1.cols() <<  ") doesn't match rows of Matrix B(" << p2.rows() << ")" << std::endl;
                exit(-1);
            }
        }

        T operator()(std::size_t row, std::size_t col) const 
        {
            T ret = T();
            for (std::size_t i = 0; i < p1.cols(); ++i)
                ret = ret + p1(row, i) * p2(i, col);
            return ret;
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p2.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixMult<P1, P2, T> operator*(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixMult<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename P2, typename T>
    class MatrixTensor : public MatrixExpression<MatrixTensor<P1, P2, T>, T>
    {
    public:
        MatrixTensor(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_) { }

        T operator()(std::size_t row, std::size_t col) const
        {
            size_t iA = row / p2.rows();
            size_t jA = col / p2.cols();

            size_t iB = row % p2.rows();
            size_t jB = col % p2.cols();

            return p1(iA, jA) * p2(iB, jB);
        }

        std::size_t rows() const { return p1.rows() * p2.rows(); }
        std::size_t cols() const { return p1.cols() * p2.cols(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    MatrixTensor<P1, P2, T> operator%(const MatrixExpression<P1, T>& m1, const MatrixExpression<P2, T>& m2)
    {
        return MatrixTensor<P1, P2, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const P2*>(&m2)
        );
    }

    template <typename P1, typename T>
    class MatrixMultS : public MatrixExpression<MatrixMultS<P1, T>, T>
    {
    public:
        MatrixMultS(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return p1(row, col) * s;
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixMultS<P1, T> operator*(const MatrixExpression<P1, T>& m1, const T& s)
    {
        return MatrixMultS<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    MatrixMultS<P1, T> operator*(const T& s, const MatrixExpression<P1, T>& m1)
    {
        return MatrixMultS<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    class MatrixDivS : public MatrixExpression<MatrixDivS<P1, T>, T>
    {
    public:
        MatrixDivS(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return p1(row, col) / s;
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixDivS<P1, T> operator/(const MatrixExpression<P1, T>& m1, const T& s)
    {
        return MatrixDivS<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    class MatrixDivSI : public MatrixExpression<MatrixDivSI<P1, T>, T>
    {
    public:
        MatrixDivSI(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return s / p1(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixDivSI<P1, T> operator/(const T& s, const MatrixExpression<P1, T>& m1)
    {
        return MatrixDivSI<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    class MatrixT : public MatrixExpression<MatrixT<P1, T>, T>
    {
    public:
        MatrixT(const P1& p1_): p1(p1_){}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return p1(col, row);
        }

        std::size_t rows() const { return p1.cols(); }
        std::size_t cols() const { return p1.rows(); }
    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    MatrixT<P1, T> operator!(const MatrixExpression<P1, T>& m1)
    {
        return MatrixT<P1, T>(
            *static_cast<const P1*>(&m1)
        );
    }

    template <typename P1, typename T>
    class MatrixNeg : public MatrixExpression<MatrixNeg<P1, T>, T>
    {
    public:
        MatrixNeg(const P1& p1_): p1(p1_){}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return -p1(row, col);
        }

        std::size_t rows() const { return p1.cols(); }
        std::size_t cols() const { return p1.rows(); }
    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    MatrixNeg<P1, T> operator-(const MatrixExpression<P1, T>& m1)
    {
        return MatrixNeg<P1, T>(
            *static_cast<const P1*>(&m1)
        );
    }

    template <typename P1, typename T>
    class MatrixAddI : public MatrixExpression<MatrixAddI<P1, T>, T>
    {
    public:
        MatrixAddI(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return p1(row, col) + s * (row == col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixAddI<P1, T> operator+(const MatrixExpression<P1, T>& m1, const T& s)
    {
        return MatrixAddI<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    MatrixAddI<P1, T> operator+(const T& s, const MatrixExpression<P1, T>& m1)
    {
        return MatrixAddI<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    class MatrixSubI : public MatrixExpression<MatrixSubI<P1, T>, T>
    {
    public:
        MatrixSubI(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return p1(row, col) - s * (row == col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixSubI<P1, T> operator-(const MatrixExpression<P1, T>& m1, const T& s)
    {
        return MatrixSubI<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename P1, typename T>
    class MatrixSubII : public MatrixExpression<MatrixSubII<P1, T>, T>
    {
    public:
        MatrixSubII(const P1& p1_, const T& s_): p1(p1_), s(s_) {}

        T operator()(std::size_t row, std::size_t col) const 
        {
            return s * (row == col) - p1(row, col);
        }

        std::size_t rows() const { return p1.rows(); }
        std::size_t cols() const { return p1.cols(); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    MatrixSubII<P1, T> operator-(const T& s, const MatrixExpression<P1, T>& m1)
    {
        return MatrixSubII<P1, T>(
            *static_cast<const P1*>(&m1),
            *static_cast<const T*>(&s)
        );
    }

    template <typename E, typename T>
    std::ostream& operator<<(std::ostream& out, const MatrixExpression<E, T>& m)
    {
        if (m.rows() == 0 || m.cols() == 0)
            return out;
        
        for (std::size_t i = 0; i < m.rows() - 1; ++i)
        {
            for (std::size_t j = 0; j < m.cols() - 1; ++j)
            {
                out << m(i, j) << SEPARATOR;
            }
            std::size_t j = m.cols() - 1;
            out << m(i, j) << std::endl;
        }

        std::size_t i = m.rows() - 1;
        for (std::size_t j = 0; j < m.cols() - 1; ++j)
        {
            out << m(i, j) << SEPARATOR;
        }
        std::size_t j = m.cols() - 1;
        out << m(i, j);

        return out;
    }
}


//COMPLEX
namespace my
{
    template <typename E, typename T>
    class ComplexExpression
    {
    public:
        T r() const { return static_cast<const E&>(*this).r(); }
        T i() const { return static_cast<const E&>(*this).i(); }
    };

    template<typename T>
    class Complex : public ComplexExpression<Complex<T>, T>
    {
    public:
        Complex(): real(), imag() { }

        Complex(T real_part, T imag_part): real(real_part), imag(imag_part) { }

        Complex(const Complex<T>& other) = default;

        Complex(Complex<T>&& other) = default;

        template <typename E>
        Complex(const ComplexExpression<E, T>& expr): real(expr.r()), imag(expr.i()) { }

        ~Complex() = default;
        const T& r() const { return real; }
        const T& i() const { return imag; }
        T& r() { return real; }
        T& i() { return imag; }
    private:
        T real, imag;
    };

    template <typename P1, typename P2, typename T>
    class ComplexSum : public ComplexExpression<ComplexSum<P1, P2, T>, T>
    {
    public:
        ComplexSum(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_) { }
        T r() const { return p1.r() + p2.r(); }
        T i() const { return p1.i() + p2.i(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ComplexSum<P1, P2, T> operator+(const ComplexExpression<P1, T>& c1, const ComplexExpression<P2, T>& c2)
    {
        return ComplexSum<P1, P2, T> (
                                  *static_cast<const P1*>(&c1),
                                  *static_cast<const P2*>(&c2));
    }

    template <typename P1, typename P2, typename T>
    class ComplexSub : public ComplexExpression<ComplexSub<P1, P2, T>, T>
    {
    public:
        ComplexSub(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_) { }
        T r() const { return p1.r() - p2.r(); }
        T i() const { return p1.i() - p2.i(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ComplexSub<P1, P2, T> operator-(const ComplexExpression<P1, T>& c1, const ComplexExpression<P2, T>& c2)
    {
        return ComplexSub<P1, P2, T> (
                                  *static_cast<const P1*>(&c1),
                                  *static_cast<const P2*>(&c2));
    }

    template <typename P1, typename P2, typename T>
    class ComplexMult : public ComplexExpression<ComplexMult<P1, P2, T>, T>
    {
    public:
        ComplexMult(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_) { }
        T r() const { return p1.r() * p2.r() - p1.i() * p2.i(); }
        T i() const { return p1.r() * p2.i() + p1.i() * p2.r(); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ComplexMult<P1, P2, T> operator*(const ComplexExpression<P1, T>& c1, const ComplexExpression<P2, T>& c2)
    {
        return ComplexMult<P1, P2, T> (
                                       *static_cast<const P1*>(&c1),
                                       *static_cast<const P2*>(&c2));
    }

    template <typename P1, typename P2, typename T>
    class ComplexDiv : public ComplexExpression<ComplexDiv<P1, P2, T>, T>
    {
    public:
        ComplexDiv(const P1& p1_, const P2& p2_): p1(p1_), p2(p2_) { }
        T r() const { return (p1.r() * p2.r() + p1.i() * p2.i()) / (p2.r() * p2.r() + p2.i() * p2.i()); }
        T i() const { return (p1.i() * p2.r() - p1.r() * p2.i()) / (p2.r() * p2.r() + p2.i() * p2.i()); }
    private:
        const P1& p1;
        const P2& p2;
    };

    template <typename P1, typename P2, typename T>
    ComplexDiv<P1, P2, T> operator/(const ComplexExpression<P1, T>& c1, const ComplexExpression<P2, T>& c2)
    {
        return ComplexDiv<P1, P2, T> (
                                      *static_cast<const P1*>(&c1),
                                      *static_cast<const P2*>(&c2));
    }

    template <typename P1, typename T>
    class ComplexMultScal : public ComplexExpression<ComplexMultScal<P1, T>, T>
    {
    public:
        ComplexMultScal(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return p1.r() * s; }
        T i() const { return p1.i() * s; }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ComplexMultScal<P1, T> operator*(const ComplexExpression<P1, T>& c1, const T& c2)
    {
        return ComplexMultScal<P1, T> (
                                       *static_cast<const P1*>(&c1),
                                       c2);
    }

    template <typename P1, typename T>
    ComplexMultScal<P1, T> operator*(const T& c2, const ComplexExpression<P1, T>& c1)
    {
        return ComplexMultScal<P1, T> (
                                       *static_cast<const P1*>(&c1),
                                       c2);
    }

    template <typename P1, typename T>
    class ComplexDivScal : public ComplexExpression<ComplexDivScal<P1, T>, T>
    {
    public:
        ComplexDivScal(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return p1.r() / s; }
        T i() const { return p1.i() / s; }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ComplexDivScal<P1, T> operator/(const ComplexExpression<P1, T>& c1, const T& c2)
    {
        return ComplexDivScal<P1, T> (
                                      *static_cast<const P1*>(&c1),
                                      c2);
    }

    template <typename P1, typename T>
    class ComplexDivScalI : public ComplexExpression<ComplexDivScalI<P1, T>, T>
    {
    public:
        ComplexDivScalI(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return (s * p1.r()) / (p1.r() * p1.r() + p1.i() * p1.i()); }
        T i() const { return (-s * p1.i()) / (p1.r() * p1.r() + p1.i() * p1.i()); }
    private:
        const P1& p1;
        const T& s;
    };

    template <typename P1, typename T>
    ComplexDivScalI<P1, T> operator/(const T& c2, const ComplexExpression<P1, T>& c1)
    {
        return ComplexDivScalI<P1, T> (
                                       *static_cast<const P1*>(&c1),
                                       c2);
    }

    template <typename P1, typename T>
    class ComplexConjug : public ComplexExpression<ComplexConjug<P1, T>, T>
    {
    public:
        ComplexConjug(const P1& p1_): p1(p1_) { }
        T r() const { return p1.r(); }
        T i() const { return -p1.i(); }
    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    ComplexConjug<P1, T> operator!(const ComplexExpression<P1, T>& c1)
    {
        return ComplexConjug<P1, T> (*static_cast<const P1*>(&c1));
    }

    template <typename P1, typename T>
    class ComplexMinus : public ComplexExpression<ComplexMinus<P1, T>, T>
    {
    public:
        ComplexMinus(const P1& p1_): p1(p1_) { }
        T r() const { return -p1.r(); }
        T i() const { return -p1.i(); }
    private:
        const P1& p1;
    };

    template <typename P1, typename T>
    ComplexMinus<P1, T> operator-(const ComplexExpression<P1, T>& c1)
    {
        return ComplexMinus<P1, T> (*static_cast<const P1*>(&c1));
    }

    template <typename T>
    class ComplexExp : public ComplexExpression<ComplexExp<T>, T>
    {
    public:
        ComplexExp(const T& p1_): p1(p1_) { }
        T r() const { return cos(p1); }
        T i() const { return sin(p1); }
    private:
        const T& p1;
    };

    template <typename T>
    ComplexExp<T> complexExp(const T& theta)
    {
        return ComplexExp<T> (theta);
    }

    template <typename P1, typename T>
    class ComplexSumSca : public ComplexExpression<ComplexSumSca<P1, T>, T>
    {
    public:
        ComplexSumSca(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return p1.r() + s; }
        T i() const { return p1.i(); }
    private:
        const P1& p1;
        const T s;
    };

    template <typename E, typename T>
    ComplexSumSca<E, T> operator+(const ComplexExpression<E, T>& c, const T& s)
    {
        return ComplexSumSca<E, T>(*static_cast<const E*>(&c), s);
    }

    template <typename E, typename T>
    ComplexSumSca<E, T> operator+(const T& s, const ComplexExpression<E, T>& c)
    {
        return ComplexSumSca<E, T>(*static_cast<const E*>(&c), s);
    }

    template <typename P1, typename T>
    class ComplexSubSca : public ComplexExpression<ComplexSubSca<P1, T>, T>
    {
    public:
        ComplexSubSca(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return p1.r() - s; }
        T i() const { return p1.i(); }
    private:
        const P1& p1;
        const T s;
    };

    template <typename E, typename T>
    ComplexSubSca<E, T> operator-(const ComplexExpression<E, T>& c, const T& s)
    {
        return ComplexSubSca<E, T>(*static_cast<const E*>(&c), s);
    }

    template <typename P1, typename T>
    class ComplexSubScaI : public ComplexExpression<ComplexSubScaI<P1, T>, T>
    {
    public:
        ComplexSubScaI(const P1& p1_, const T& s_): p1(p1_), s(s_) { }
        T r() const { return s - p1.r(); }
        T i() const { return -p1.i(); }
    private:
        const P1& p1;
        const T s;
    };

    template <typename E, typename T>
    ComplexSubScaI<E, T> operator-(const T& s, const ComplexExpression<E, T>& c)
    {
        return ComplexSubScaI<E, T>(*static_cast<const E*>(&c), s);
    }

    template<typename E, typename T>
    std::ostream& operator<<(std::ostream& out, const ComplexExpression<E, T>& expr)
    {
        out << expr.r() << SEPARATOR << COMPLEX_BET << expr.i() << COMPLEX_IMG;
        return out;
    }
}

#endif