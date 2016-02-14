#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

#include "lua.hpp"

/**
 * @param sig - һ�����������������ͺͽ�����͵��ַ��������У���'d'��ʾ˫���ȸ�������
                ��'i'��ʾ��������'s'��ʾ�ַ���������'>'��ʾ���������ķָ���
 */
extern void call_lua_func(lua_State *L, const char *func, const char *sig, ...);

extern void dump_stack(lua_State *L);

extern void clear_stack(lua_State *L);

#endif
