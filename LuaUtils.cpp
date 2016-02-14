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
 * 1.������func�Ƿ�Ϊһ����������Ϊlua_pcall�ᷢ���������
 * 2.����Ҫѹ�����������Ĳ�������˱���ȷ��ջ�о����㹻�Ŀռ�
 * 3.���ڿ��ܻ᷵���ַ�������˸ú������ܽ��������ջ��������
     ������ʹ�����ַ�����������ַ������Ƶ��������壩�󵯳����н��
 */
void call_lua_func(lua_State *L, const char *func, const char *sig, ...)
{
  va_list vl;
  va_start(vl, sig);

  // ѹ�뺯��
  lua_getglobal(L, func);

  // ѹ�����
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

  // ��ȡ���
  nres = -nres; //��һ�������ջ����
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
  // �������ϱ���ջ
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
