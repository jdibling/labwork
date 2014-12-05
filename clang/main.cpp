#include <cstdlib>
#include <map>
#include <string>

typedef std::map <std::string, size_t> NameMap;

int main()
{
  NameMap nameMap;
  size_t i = 1234;
  const std::string name = "Foo";
  nameMap[name] = i;
}
