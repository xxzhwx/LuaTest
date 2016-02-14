#include <stdio.h>
#include "World.h"

World::World()
{
  //printf("World Ctor.");
}

World::~World()
{
  //printf("World Dtor");
}

int World::add(int a, int b)
{
  return a + b;
}

void World::setVal(int val)
{
  _val = val;
}

int World::getVal()
{
  return _val;
}
