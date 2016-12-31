#ifndef __STL_CONFIG_H
# define __STL_CONFIG_H

# define __U_STL_BEGIN_NAMESPACE // ustd{
# define __U_STL_END_NAMESPACE // }

#define __STL_TEMPLATE_NULL template<>

#define __U_STL_DEFAULT_ALLOCATOR(T) ualloc

//# ifdef __STL_USE_EXCEPTIONS
#   define __U_STL_TRY try
#   define __U_STL_CATCH_ALL catch(...)
#   define __U_STL_THROW(x) throw x
#   define __U_STL_RETHROW throw
#   define __U_STL_NOTHROW throw()
#   define __U_STL_UNWIND(action) catch(...) { action; throw; }

#   define __U_MEMBER_TEMPLATES

#endif