# ifndef TAPE_SERIALIZER_FWD_INCLUDED
# define TAPE_SERIALIZER_FWD_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD
{
    struct empty_result {
        template <typename Ty_>
        empty_result(Ty_ const&) {} empty_result& operator *(){ return *this; }
    };

    template <typename T>
    struct tape_serializer;

    template <typename Stream>
    inline bool is_cout(Stream& stg)
    {
        __if_exists(std::cout)
        {
            return (&std::cout == reinterpret_cast<std::ostream*>(&stg));
        }

        return false;
    }

    template <typename Trait0, typename Serializer, typename Stream, typename... Args>
    inline void serialize__(Trait0, Stream& stg, Args...)
    {
# if defined CL_COMPILE_TIME_DEBUG_ENABLED
#   pragma message ("Empty serializer in: " __FUNCSIG__)
#   pragma message ("To implement serialize method plaese use:")

#   pragma message ("template <typename Serializer, typename Stream, typename... Args>")
#   pragma message ("inline void serialize__0"                              )
#   pragma message ("    (std::true_type"                                   )
#   pragma message ("     , Stream                             &s_out"      )
#   pragma message ("     , CppAD::vector<unsigned __int64>     user_iy"    )
#   pragma message ("     , CppAD::player<double>*              play"       )
#   pragma message ("     , double                             *taylor"     )
#   pragma message ("     , enum CppAD::OpCode                  op"         )
#   pragma message ("     , int                                 user_state" )
#   pragma message ("     , unsigned __int64                    i_op"       )
#   pragma message ("     , unsigned __int64                    q"          )
#   pragma message ("     , size_t                              user_m"     )
#   pragma message ("     , const size_t                        J"          )
#   pragma message ("     , const CppAD::addr_t                *arg"        )
#   pragma message ("     , size_t                              i_var"      )
#   pragma message ("     , Args&...                            args)"      )

# endif
    }

    enum serializer_type
    {
        no_info = 0, io_binary = 1 << 1, io_text = 1 << 2
    };

    template <typename Base>
    struct serializer_traits
    {
        typedef tape_serializer<Base> type;
    };

    template <typename Base, typename Stream, typename... Args>
    inline void serialize(Stream &stg, Args&... args)
    {
        typedef typename
            serializer_traits<Base>::type Serializer;

        serialize__<Serializer>(CppAD::is_implemented<Serializer>(), stg, args...);
    }

    namespace tapescript
    {
        template <typename Serializer, typename Stm>
        inline size_t io_type__(Stm& stm, std::false_type)
        {
            return 0;
        }

        template <typename Ty_>
        inline Ty_* get_ptr(Ty_& v) { return &v; }

        template <typename Ty_>
        inline Ty_* get_ptr(Ty_* v) { return v; }

        template <typename Serializer, typename Stm>
        inline size_t io_type__(Stm& stm, std::true_type)
        {
            typedef typename
                std::remove_pointer<Stm>::type type_non_pointer;
            return static_cast<Serializer*>(get_ptr(stm))->io_type();
            
        }

        template <typename Serializer>
        inline size_t io_type__(CppAD::empty_result& stm, std::true_type)
        {
            return 0;
        }

    }

    template <typename Base, typename Stm>
    inline bool is_io_binary(Stm& stm)
    {
        typedef typename
            serializer_traits<Base>::type Serializer;

        return tapescript::io_type__<Serializer>(stm
            , std::integral_constant<bool, CppAD::is_implemented<Serializer>::value 
                    && CppAD::is_io_typed<Serializer>::value>()) & serializer_type::io_binary;
    }

    template <typename Base, typename Stm>
    inline bool is_io_text(Stm& stm)
    {
        typedef typename
            serializer_traits<Base>::type Serializer;

        auto v = tapescript::io_type__<Serializer>(stm
                , std::integral_constant<bool, (bool)(CppAD::is_implemented<Serializer>::value 
                && CppAD::is_io_typed<Serializer>::value)>());

        return (v & serializer_type::io_text) != 0;
    }

    template <typename Se_, typename In_>
    inline Se_& cast(In_& s_out)
    {
        return static_cast<Se_&>(s_out);
    }

    template <typename Se_>
    inline Se_& cast(CppAD::empty_result& s_out)
    {
        typename 
            std::remove_reference<Se_ >::type *ptr = 0;

        return *ptr;
    }
}

# endif