
class World
{
public:
  World();
  virtual ~World();

public:
  int add(int a, int b);
  void setVal(int val);
  int getVal();

private:
  int _val;
};
