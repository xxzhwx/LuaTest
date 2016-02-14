#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LuaUtils.h"

static void error(lua_State *L, const char *fmt, ...)
{
  va_list vl;
  va_start(vl, fmt);
  vfprintf(stderr, fmt, vl);
  va_end(vl);

  lua_close(L);
  exit(EXIT_FAILURE);
}

/**
 * 1.无须检查func是否为一个函数，因为lua_pcall会发现这类错误
 * 2.由于要压入任意数量的参数，因此必须确保栈中具有足够的空间
 * 3.由于可能会返回字符串，因此该函数不能将结果弹出栈，调用者
     必须在使用完字符串结果（或将字符串复制到其他缓冲）后弹出所有结果
 */
void call_lua_func(lua_State *L, const char *func, const char *sig, ...)
{
  va_list vl;
  va_start(vl, sig);

  // 压入函数
  lua_getglobal(L, func);

  // 压入参数
  int narg;
  for (narg = 0; *sig; ++narg)
  {
    luaL_checkstack(L, 1, "too many arguments");

    switch (*sig++)
    {
    case 'd':
      lua_pushnumber(L, va_arg(vl, double));
      break;
    case 'i':
      lua_pushinteger(L, va_arg(vl, int));
      break;
    case 's':
      lua_pushstring(L, va_arg(vl, char *));
      break;
    case '>':
      goto endargs;
    default:
      error(L, "invalid option (%c)", *(sig - 1));
    }
  }

endargs:
  int nres = strlen(sig);

  if (lua_pcall(L, narg, nres, 0) != 0)
    error(L, "error calling '%s': %s", func, lua_tostring(L, -1));

  // 获取结果
  nres = -nres; //第一个结果的栈索引
  while (*sig)
  {
    switch (*sig++)
    {
    case 'd':
      if (!lua_isnumber(L, nres))
        error(L, "wrong result type");
      *va_arg(vl, double *) = lua_tonumber(L, nres);
      break;
    case 'i':
      if (!lua_isnumber(L, nres))
        error(L, "wrong result type");
      *va_arg(vl, int *) = (int)lua_tointeger(L, nres);
      break;
    case 's':
      if (!lua_isstring(L, nres))
        error(L, "wrong result type");
      *va_arg(vl, const char **) = lua_tostring(L, nres);
      break;
    default:
      error(L, "invalid option (%c)", *(sig - 1));
    }
    ++nres;
  }

  va_end(vl);
}

void dump_stack(lua_State *L)
{
  // 从下往上遍历栈
  int top = lua_gettop(L);
  for (int i = 1; i <= top; ++i)
  {
    int t = lua_type(L, i);
    switch (t)
    {
    case LUA_TSTRING:
      {
        printf("'%s'", lua_tostring(L, i));
        break;
      }
    case LUA_TBOOLEAN:
      {
        printf(lua_toboolean(L, i) ? "true" : "false");
        break;
      }
    case LUA_TNUMBER:
      {
        printf("%g", lua_tonumber(L, i));
        break;
      }
    default:
      {
        printf("%s", lua_typename(L, t));
        break;
      }
    }
    printf("  ");
  }
  printf("\n");
}

void clear_stack(lua_State *L)
{
  lua_settop(L, 0);
}
