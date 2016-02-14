#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

#include "lua.hpp"

/**
 * @param sig - 一个用于描述参数类型和结果类型的字符串，其中，用'd'表示双精度浮点数，
                用'i'表示整数，用's'表示字符串，而用'>'表示参数与结果的分隔符
 */
extern void call_lua_func(lua_State *L, const char *func, const char *sig, ...);

extern void dump_stack(lua_State *L);

extern void clear_stack(lua_State *L);

#endif
