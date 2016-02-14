#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "LuaUtils.h"
#include "World.h"

lua_State * L;

int main(int argc, char *argv[])
{
  L = luaL_newstate();
  luaL_openlibs(L);

  luaL_dofile(L, "D:\\work_\\code_cpp\\LuaTest\\scripts\\init.lua");

  dump_stack(L);

  lua_close(L);
  return 0;
}