#include "estructuraPersonas.hpp"
using namespace std;
int main()
{
  EstructuraPersonas prueba;
  prueba.insert(1,false, 35, "Bogota");
  prueba.insert(5,true, 43, "Arabia Saudita");
  prueba.insert(3,true, 37, "kathmandu");
  prueba.insert(2,true, 24, "Calarca");
  prueba.insert(4,true, 22, "Suesca");
  prueba.display();
  prueba.test(3);
  prueba.display();
  return 0;
}
