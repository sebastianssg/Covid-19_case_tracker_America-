#ifndef _ESTRUCTURAPERSONAS_HPP
#define _ESTRUCTURAPERSONAS_HPP
#include <string>
#include <iostream>
//cajitas dentro de la estructura es decir la persona
struct person
{
  //edad
  int age;
  //genero
  bool gender;
  //locacion
  std::string location;
  // idUnico
  int id;
  //apuntador al siguiente por izquieda
  person* left;
  //apuntador al soguiente por derecha
  person* right;
  //apuntador al padre
  person* parent;
};


class EstructuraPersonas
{

private:
  //tenemos la raiz
  person *root;
  //para mostrar una perosna
  void display_person(person* pPerson, int count);
  //insertar una persona
  void insert_person(person *&pPersona, int pId, std::string pLocation, bool pGender, int pAge, person*p);
  
  
  //no accedemos nunca desde afuera
  //node desde donde comenzamos a destruir recursivamente
  
  void destroy_recursive(person *pPerson);
  
  person* find_person(person *pPerson, int pId);
  //Minimo y maximo
  person* minimum(person *t);
  person* maximum(person *t);
  
public:
  EstructuraPersonas(){root = nullptr;}
  ~EstructuraPersonas(){destroy_recursive(root);} //destructor 
  //crea un nodo, y derecho e izquiedo apunta a nullptr
  
  void display();
  bool empty();
  /*--------------*/
  void test(int val);

  //
  void insert(int pIdU, bool pGenderU, int pAgeU, std::string pLocationU){insert_person(root,pIdU, pLocationU, pGenderU,  pAgeU, nullptr);}
  person* find(int pId){return find_person(root, pId);}
  //Predecesor y sucesor
  person *predecessor(person *pPerson); //le doy un nodo y me deberia retorna el predecesor de este
  person *successor(person *pPerson);

  //Removiendo Nodos
  person* remove_person(person *node, int k);

};
#endif
