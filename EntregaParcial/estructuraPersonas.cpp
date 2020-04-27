#include "estructuraPersonas.hpp"

void EstructuraPersonas::insert_person(person *&pPersona, int pId, std::string pLocation, bool pGender, int pAge, person *p)
{

  if(pPersona == nullptr)
    {
      pPersona = new person;
      pPersona->id = pId;
      //atributos de una persona
      pPersona->location = pLocation;
      pPersona->gender = pGender;
      pPersona->age = pAge;
      //en el caso de que no tenga
      pPersona->left = nullptr;
      pPersona->right = nullptr;
      pPersona->parent = p;
    }
  else
    {
      if(pId != pPersona->id)
	{
	  if(pId < pPersona->id)
	    {
	      insert_person(pPersona->left, pId, pLocation, pGender, pAge, pPersona);
	    }
	  else
	    {
	      insert_person(pPersona->right, pId, pLocation, pGender, pAge, pPersona);
	    }
	}
    }

}

void EstructuraPersonas::display()
{
  //wraper para no acceder al root desde afuera
  int contador = 0;
  display_person(root, contador);
  std::cout<<std::endl;
}

void EstructuraPersonas::display_person(person *pPerson, int count)
{
  //recursion
  count++;
  if(pPerson != nullptr)
    {

      display_person(pPerson->left, count);	
      std::cout<< "("<<count -1<< ")"<<pPerson->id<<":"<<pPerson->location<<":"<<pPerson->gender<<":"<<pPerson->age<<" ";
      display_person(pPerson->right, count);
	
    }
}


void EstructuraPersonas::destroy_recursive(person *pPerson)
{
  //hasta que sea el root
  if(pPerson != nullptr)
    {
      destroy_recursive(pPerson->left);
      destroy_recursive(pPerson->right);
      delete pPerson;
    }
}

person *EstructuraPersonas::minimum(person*t)
{

  /*
  //mientras que el de la izquierda sea diferente de nullptr
  while(t->left != nullptr)
    //t ahora se va cambiando al nodo que tiene a la izquierda asi va moviendose
    t = t->left;

  return t;
  */
  

  if(t->left == nullptr)
    {
      return t;
    }
  else
    {
      minimum(t->left);
    }
}


person *EstructuraPersonas::maximum(person *t)
{
  //mismo principio de minimo pero ahora hacia la derecha con maximun
  /*
  while(t->right != nullptr)
    {
      t = t->right;
    }
  return t;
  */

  if(t->right == nullptr)
    {
      return t;
    }
  else
    {
      maximum(t->right);
    }
}

person * EstructuraPersonas::find_person(person* pPerson, int pId)
{
  if(pPerson == nullptr) return nullptr;
  if(pPerson->id == pId) return pPerson;
  if(pPerson->id < pId)
    return find_person(pPerson->right, pId);
  else
    return find_person(pPerson->left, pId);
}

bool EstructuraPersonas::empty()
{
  return root == nullptr;
}

//predecessor y sucessor
person *EstructuraPersonas::predecessor(person *pPerson)
{
  //continua mientras siga siendo hijo izquierdo de su padre
  if(pPerson->left != nullptr)
    {
      return maximum(pPerson->left);
    }
  else
    {
      while(((pPerson->parent)->left == pPerson) && (pPerson->parent) != nullptr)
	{
	  pPerson = pPerson->parent;
	}

      return pPerson->parent;
    }
}


person *EstructuraPersonas::successor(person *pPerson)
{
  if(pPerson->right != nullptr)
    {
      return minimum(pPerson->right);
    }
  else
    {
      while(((pPerson->parent)->right == pPerson) && (pPerson->parent) != nullptr)
	{
	  pPerson = pPerson->parent;
	}

      return pPerson->parent;
    }

}

person* EstructuraPersonas::remove_person(person* node, int k) {
  person* n = find_person(node, k);
  if(n != nullptr){
    person* p = n->parent;
    
    //Case 1: No children
    if(n->left == nullptr && n->right == nullptr){
      //
      std::cout<<"Case1\n";
      //
      if(p == nullptr){ //if node is root
	root = nullptr;  
      }else{
	if(n == p->left) //if n is left child
	  p->left = nullptr;
	else
	  p->right = nullptr;
      }
      delete n;
    }
    
    //Case 2: One child
    else if(n->left == nullptr || n->right == nullptr){
      //
      std::cout<<"Case2\n";
      //
      //;
      person* c;
      
      //std::cout<<"llegue hasta aca"<<std::endl;
      if(n == p->left)
	{
	  //std::cout<<"soy izquiedo"<<std::endl;
	  //if n is left child
	if(n->left != nullptr) //if child was left
	  {
	    c = n->left;
	    //std::cout<<"llegue hasta aca"<<std::endl;
	  }
	else
	  {
	    //if child was right
	    //std::cout<<"llegue hasta aca"<<std::endl;
	    c = n->right;
	  }
	if(p != nullptr) p->left = c;
      }
      else
	{ //if n is right child
	  //std::cout<<"soy derecho"<<std::endl;
	if(n->left != nullptr) //if child was left
	  c = n->left;
	else //if child was right
	  c = n->right;
	if(p != nullptr) p->right = c;
      }
      c->parent = p;
      delete n;


    }

    //Case 3: Two children
    else{
      //
      std::cout<<"Case3\n";
      //
      person* s = successor(n);
      int new_id = s->id;
      std::string new_loc  = s->location;
      int new_age = s->age;
      bool new_gender = s->gender;
      
      p = remove_person(s->parent, s->id);
      n->id = new_id;
      n->location = new_loc;
      n->age = new_age;
      n->gender = new_gender;
      
    }

    return p;
  }
  return nullptr;
}


void EstructuraPersonas::test(int val)
{
  remove_person(find_person(root, val), val); 
}

