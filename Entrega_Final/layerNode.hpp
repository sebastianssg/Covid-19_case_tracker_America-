#ifndef _LAYERNODE_HPP
#define _LAYERNODE_HPP
#include <iostream>
#include <vector>
#include <string>
#include <set>

#include <cctype> //para poner en  mayusculas

//librerias para poder trabajar con el archivo de tipo texto
#include <fstream>
#include <stdio.h>


/*
***************************************************
ACA COMIENZA EL MAPA QUE SE USA PARA EL GENERO
************************************************
*/



const int INITIAL_SIZE = 5;

struct nodeGender
{
  //El value que corresponde al genero
  bool value; //Si es true es mujer, si es false es hombre
  //El key que es el id unico
  int key;
  //El next indicando el siguiente en el bucket
  nodeGender* next;
};


class GenderMap
{
private:
  //Como es parecido a un mapa se usa una tabla hash 
  nodeGender** table;
  int table_size;
  int count;

  //Se encarga de volver a acomodar a todos los eleemntos
  void reHash();
  //La funcion hash que se tiene
  int hash_fun(int key);
  //para buscar en el bucket
  nodeGender* search_bucket(int i, int key);
public:
  //Constructor y destructor de la clase
  GenderMap();
  ~GenderMap();

  //Metodos base
  int size();
  
  bool empty();

  //Para insetar un elemento dado un id unico y un valor que es el genero
  void insert(int pKey, bool value);
  //Para ver como se van organizando se incluye el display
  void display();
  //Con el fin de borrarlo al final
  void clear();
 
  std::vector<int> generarLista(bool pGenero);//Genera un vector que contiene los nombres que contienen cierta propiedad es decir el genero
  //Metodo para poder ir probando los demas metodos
  void test();
  
};


//No retorna nada y no tiene parametro es probar metodos privados
void GenderMap::test()
{
  std::cout<<"Test"<<std::endl;
}

//Requiere una variable de tipo booleano donde true es mujer y false es hombre y retorna un vector con los IDs de estos
std::vector<int> GenderMap::generarLista(bool pGenero)
{
  //Recordar si es true es mujer si es hombre es false

  std::vector<int> retorno;
  nodeGender* cursor = new nodeGender;
  std::cout<<"generando lista generos.."<<std::endl;
  //Se recorre tabla hash como el set
  for(int i = 0; i <table_size; i++)
    {
      cursor = table[i];
      while(cursor != nullptr)
	{
	  if(cursor->value == pGenero)
	    {
	      
	      retorno.push_back(cursor->key);
	    }
	  cursor = cursor->next;
	}
    }

  return retorno;
  
}


void GenderMap::reHash(){
  int old_table_size = table_size;
  nodeGender **old_table = table;
  table_size = 2*table_size;
  table = new nodeGender*[table_size];
  for(int i = 0; i < table_size;++i) table[i] = nullptr;
  count = 0;
  //Un cursor para ir recorriendolo
  nodeGender *cursor;
  //Se crea la nueva tabla
  
  
  for(int i = 0; i < old_table_size; ++i){
      cursor = old_table[i];
      while(old_table[i] != nullptr){
	  insert(cursor->key, cursor->value);
	  cursor = cursor->next;
	  delete old_table[i];
	  old_table[i] = cursor;
	}
    }
  delete[] old_table;

}

//Destructor
GenderMap::~GenderMap(){
  clear();
  delete[] table;
}

//Forma recursiva de borrar un bucket tiene como parametro el nodo de la tabla y no retorna nada
void clear_bucket(nodeGender* pNodo){
  nodeGender* temp;
  
  if(pNodo != nullptr){
      temp = pNodo->next;
      delete pNodo;
      
      clear_bucket(temp);
    }
  
}

//Wrapper para clear_bucket con cada uno de los elementos de table_size
void GenderMap::clear(){
  for(int i = 0; i < table_size; ++i){
      clear_bucket(table[i]);
      table[i] = nullptr;
    }

}



//Constructor
GenderMap::GenderMap(){
  
  table_size = INITIAL_SIZE;
  table = new nodeGender*[INITIAL_SIZE];
  for(int i = 0; i < table_size; i++){ 
      table[i] = nullptr;
    }
  count = 0;
}

//Retorna el numero de elementos, no requiere parametros de entrada
int GenderMap::size(){
  return count;
}

//Retorna si esta vacio, no requiere parametros de entrada
bool GenderMap::empty(){
  return count == 0;
}

//Funcion hash que organiza donde deberian ir los elementos
int GenderMap::hash_fun(int key){
  return key % table_size;
  
}
//Insert a la estructura, requiere la llave que es el id unico y genero, no retorna

void GenderMap::insert(int pKey, bool value){
  int entra = hash_fun(pKey);
  nodeGender* esta = search_bucket(entra, pKey);
  
  if(esta == nullptr && count < (2*table_size)){
      nodeGender*temporal = new nodeGender;
      temporal->value = value;
      temporal->key = pKey;
      //el next ahora es el primero del tabl
      temporal->next = table[entra];
      //lo introduzco al inicio del bucket
      table[entra]= temporal;
      count++;
    }
  else if(esta == nullptr && count == (2* table_size))
    {
      //hay que hacer el rehash
      reHash();
      insert(pKey, value);
    }
  else
    {
      //Remplaza
      
      esta->value=value;
    }
}

//Metodo que retorna el apuntador a nodeGender en que bucket se encuentra el elemento en teoria

nodeGender* GenderMap::search_bucket(int i, int key){

  nodeGender* elemento = new nodeGender;
  if(table[i] != nullptr){
      elemento->key = table[i]->key;
      elemento->value = table[i]->value;
      elemento->next = table[i]->next;
      
      while(elemento != nullptr){
	  
	  if(elemento->key == key)
	    return elemento;
	  elemento = elemento->next;
	  
	}
    }


  return nullptr;
}

//Se utiliza para ver como esta quedando la estructura
void GenderMap::display(){
  std::cout<<count<<" elements:\n";
  nodeGender* cursor;
  for(int i = 0; i <table_size; ++i){
      cursor = table[i];
      while(cursor != nullptr)
	{
	  std::cout<<"("<<cursor->key<< ","<<cursor->value<<")";
	  cursor =cursor->next;
	}
      std::cout<<std::endl;
    }
}


/*
 *
 *ACA COMIENZA LA ESTRUCTURA QUE CONTIENE LA INFORMACION DE LA EDAD EN UN BINARY TREE
 *
 */

//Corresponde al margen de error que puede haber entre los rangos de edad, por ejemplo si se introduce 10 como edad minima pero el minimo elemento es 15 entonces va viendo si se encuentra y así hasta que llegue al valor minimo
int NEXT = 20;

//Nodo de localizacion para el arbol
struct nodeLoc
{
  
  int idUnico;
  int edad;
  //Izquierda
  nodeLoc* left;
  //Derecha que es mayor
  nodeLoc*  right;

  //El padre
  nodeLoc* parent;
  
};


class BinLoc{

private:
  //El primer elemento que se introduce
  nodeLoc* root;
  //Funcion que muetra los atributos de un node
  void displayNode(nodeLoc* pNode, int count);
  //Funcion que permite insertar un nodo dado un idUnico, edad, el padre y el nodo base
  void insertNode(nodeLoc* &pNode, int pKey, int pIdUnico, nodeLoc* pParent);
  //Funcion para destruir recursivamente todos los elementos de la estructura
  void destroyRecursive(nodeLoc* pNode);
  //Encuentra el maximo en el arbol (hablando de edad)
  nodeLoc* maximum(nodeLoc *t);
  //Encuentra el minimo en el arbol (hablando de edad)
  nodeLoc* minimum(nodeLoc *t);
  //Encuentra un nodo, dada la edad que es el mismo idUnico y el nodo desde donde debe comenzar a buscar
  nodeLoc* findNode(nodeLoc* pNode, int pEdad);
  //Almacena la cantidad de elementos que hay en el arbol
  int size;
	
public:
  //Constructor inicializa los atributos
  BinLoc(){root = nullptr;}
  //Destructor recursivo
  ~BinLoc(){destroyRecursive(root);}
  //Wrapper para insertar un elemento con la propiedad de los arboles binarios, aunque permite elementos repetidos es decir el key no es unico pero su valor si lo es
  void insert(int pKey, int pIdUnico){insertNode(root, pKey, pIdUnico, nullptr);}
  //Muestra en la terminal como queda el arbol
  void display();
  //Encuentra el predecesor dado un nodo
  nodeLoc* predecessor(nodeLoc *pNode);
  //Encuentra el sucesor dado un nodo
  nodeLoc* successor(nodeLoc *pNode);
  //Wrapper para el metodo de find_node pero lo busca desde el root
  nodeLoc* find(int pEdad){return findNode(root, pEdad);}
  //Revisa si contiene cierta edad
  bool contains(int pEdad);//este elemento existe en la estructura
  //Busca el siguiente elemento que se encuentre en el arbol con la edad mas parecida dentro del rango del valor next
  int searchNext(int pEdad);
  //Busca el anterior elemento que se encuentre en el arbol dentro del rango de next
  int searchLast(int pEdad);
  //Retorna la cantidad de elementos que hay en el arbol
  int tamano();
  //Metodo que dado un rango es decir un desde y un hasta retorna los elementos del arbol que se encuentran dentro del rango incluyente[pDesde, pHasta] siedo int enteros
  std::vector<int> rango(int pDesde, int pHasta);
  //Metodo para probar metodos privados de la estructura
  void test();
  
};

int BinLoc::searchNext(int pEdad){
  //En caso de que el elemento no exista, se usara esto con el fin de encontrar el elemento mas cercano en un umbral de NEXT
  int tempEdad = pEdad;
  int contador = 0;
  while(contador < NEXT && !contains(tempEdad))
    {
      tempEdad++;
      contador++;
      
    }

  if(contains(tempEdad))
    return tempEdad;
  else
    return -1;
}

int BinLoc::searchLast(int pEdad)
{
  //En caso de que el elemento no exista, se usara esto con el fin de encontrar el elemento mas cercano en un umbral de NEXT
  int tempEdad = pEdad;
  int contador = 0;
  while(contador < NEXT && !contains(tempEdad))
    {
      tempEdad--;
      contador++;
      
    }

  if(contains(tempEdad))
    return tempEdad;
  else
    return -1;
}


bool BinLoc::contains(int pEdad)
{
  if(find(pEdad) == nullptr)
    {
      return false;
    }
  return true;
}
std::vector<int> BinLoc::rango(int pDesde, int pHasta){
  if(pDesde != -1 && pHasta != -1)
    {
      std::vector<int> retorno;
      if(contains(pDesde) && contains(pHasta))
	{
	  nodeLoc* temp = find(pDesde);
	  
	  nodeLoc* tempDos = find(pHasta);
	  //Los numeros mayores
	 
	  while(temp != nullptr)
	    {
	      if(temp->idUnico == tempDos->idUnico)
		{
		  //Aca es donde deja de ver si hay mas con el mismo valor del predecesor 
		  retorno.push_back(temp->idUnico);
		  break;
		}
	      else
		{
		  retorno.push_back(temp->idUnico);
		  temp = successor(temp);
		  if(tempDos->idUnico == temp->idUnico)
		    {
		      while(temp->edad == tempDos->edad)
			{
			  retorno.push_back(temp->idUnico);
			  temp = successor(temp);
			}
		      break;
		    }

		  
		}
	    }


	  return retorno;
	  
	}
      else
	{

	 return rango(searchNext(pDesde), searchLast(pHasta));
	   
	}
    }
  

}





void BinLoc::test(){
  
  std::vector<int> prueba= rango(0,10);

  for(unsigned int i =0; i < prueba.size(); i++){
      std::cout<<prueba[i]<<" ";
    }
  std::cout<<std::endl;
  
}


nodeLoc* BinLoc::minimum(nodeLoc *t)
{  

  if(t->left == nullptr){
      return t;
    }
  else
    {
      minimum(t->left);
    }
}


nodeLoc* BinLoc::maximum(nodeLoc *t){

  if(t->right == nullptr)
    {
      return t;
    }
  else
    {
      maximum(t->right);
    }
}

int BinLoc::tamano(){
  return size;
}

void BinLoc::insertNode(nodeLoc *&pNode, int pKey, int pIdUnico, nodeLoc *pParent){
  
  if(pNode == nullptr){
      pNode = new nodeLoc;
      pNode->edad = pKey;
      pNode->left = nullptr;
      pNode->right = nullptr;
      pNode->parent = pParent;
      pNode->idUnico = pIdUnico;
      size++;
    }
  else
    {
     
	  if(pKey < pNode->edad){
	      insertNode(pNode->left, pKey, pIdUnico , pNode);
	    }
	  else
	    {
	      insertNode(pNode->right, pKey, pIdUnico, pNode);
	    }
	
    }

  

}

void BinLoc::destroyRecursive(nodeLoc *pNode)
{
  if(pNode != nullptr)
    {
      destroyRecursive(pNode->left);
      destroyRecursive(pNode->right);
      delete pNode;
    }
}


void BinLoc::displayNode(nodeLoc *pNode, int count)
{
  //Recursion
  count++;
  if(pNode != nullptr)
    {

      displayNode(pNode->left, count);	
      std::cout<< "("<<count -1<< ")"<<pNode->edad<<" id["<<pNode->idUnico <<"] " ;
      displayNode(pNode->right, count);
	
    }
}


void BinLoc::display()
{
  //Wraper para no acceder al root desde afuera
  int contador = 0;
  displayNode(root, contador);
  std::cout<<std::endl;
}

nodeLoc* BinLoc::successor(nodeLoc *pNode)
{

  
  if(pNode->right != nullptr)
    {
      return minimum(pNode->right);
    }
  else
    {

      nodeLoc* tempB = new nodeLoc;
      while(((pNode->parent)->right == pNode) && (pNode->parent) != nullptr)
	{
	  pNode = pNode->parent;
	  if(root == pNode)
	    {
	      break;
	    }
	  
	}

      return pNode->parent;
    }

}

nodeLoc* BinLoc::predecessor(nodeLoc* pNode)
{
  //Continua mientras siga siendo hijo izquierdo de su padre
  if(pNode->left != nullptr)
    {
      return maximum(pNode->left);
    }
  else
    {
      while(((pNode->parent)->left == pNode) && (pNode->parent) != nullptr)
	{
	  pNode = pNode->parent;
	}

      return pNode->parent;
    }
}

nodeLoc* BinLoc::findNode(nodeLoc *pNode, int pEdad)
{
  if(pNode == nullptr) return nullptr;
  if(pNode->edad == pEdad) return pNode;
  if(pNode->edad < pEdad)
    return findNode(pNode->right, pEdad);
  else
    return findNode(pNode->left, pEdad);
}


/*
 *
 *ACA COMIENZA LA CLASE QUE CREA UN SET PARA CADA DEPARTAMENTO Y LOS GUARDA EN UN VECTOR
 *
 */


class DptoSet{
private:

  //Contiene cada uno de los sets que se crearon
  std::vector<std::set<int>> elementos;
  
public:
  DptoSet();

  //Se introduce una llave que es el idUnico y el departametno en formato "amazonas"
  void insert(int pKey, std::string value);
  //Muestra los elementos que hay en un set dado
  void displaySet(int pNumero);
  //Muestra todos los elementos que hay en el todos los set
  void display();
  //Genera un vector dado un numero de de que departamento se esta hablando
  std::vector<int> generarLista(int valor);
  //Genera una union entre los ids de las personas que se encuentren en departamentos dados en formato("Amazonas,Bogota,Cundinamarca,Antioquia")
  std::vector<int> unionDepto(std::string pString); 
  //Metodo para ir probando los metodos privados de la clase
  void test();
};




DptoSet::DptoSet(){
// El numero comentado es que se usa a la hora de generar el archivo de texto
// Se tienen los 32 departamentos y Bogota, para un total de 33 set
  std::set<int> antioquia; // 05 == 0
  std::set<int> atlantico {}; // 08 ==1
  std::set<int> bogota {}; // 11 == 2
  std::set<int> bolivar {}; // 13 == 3
  std::set<int> boyaca {}; // 15 == 4
  std::set<int> caldas {}; // 17 == 5
  std::set<int> caqueta {}; // 18 == 6
  std::set<int> cauca {}; // 19 == 7 
  std::set<int> cesar {}; // 20== 8
  std::set<int> cordova {}; // 23
  std::set<int> cundinamarca {}; // 25
  std::set<int> choco {}; // 27
  std::set<int> huila {}; // 41
  std::set<int> la_guajira {}; // 44
  std::set<int> magdalena {}; //47
  std::set<int> meta {}; // 50
  std::set<int> narino {}; // 52
  std::set<int> norte_de_santander {}; // 54
  std::set<int> quindio {}; // 63
  std::set<int> risaralda {}; // 66
  std::set<int> santander {}; // 68
  std::set<int> sucre {}; // 70
  std::set<int> tolima {}; // 73
  std::set<int> valle_del_cauca {}; // 76
  std::set<int> arauca {}; // 81
  std::set<int> casanare {}; // 85
  std::set<int> putumayo {}; // 86
  std::set<int> san_andres {}; // 88
  std::set<int> amazonas {}; // 91
  std::set<int> guainia {}; // 94
  std::set<int> guaviare {}; // 95
  std::set<int> vaupes {}; //  97
  std::set<int> vichada; //  99
  
  std::vector<std::set<int>> prueba{antioquia, atlantico, bogota, bolivar, boyaca, caldas, caqueta, cauca, cesar, cordova, cundinamarca, choco, huila, la_guajira, magdalena, meta, narino, norte_de_santander, quindio, risaralda, santander, sucre, tolima, valle_del_cauca, arauca, casanare, putumayo, san_andres, amazonas, guainia, vaupes, vichada};
  elementos = prueba;
 
}


void DptoSet::test()
{
  std::vector<int> prueba = generarLista(8);
  for(unsigned int i = 0; i < prueba.size(); i++)
    {
      std::cout<<prueba[i]<<" ";
    }
  std::cout<<std::endl;


  unionDepto("Amazonas,Putumayo");
}


std::vector<int> DptoSet::generarLista(int numero)
{

  std::vector<int> retorno{};
  int contador = 0;
  
  int lista[33] = {5,8,11,13,15,17,18,19,20,23,25,27,41,44,47,50,52,54,63,66,68,70,73,76,81,85,86,88,91,94,95,97,99};
  //std::cout<<"el valor que entro: "<<numero <<std::endl;
  for(int i = 0; i < 33; i++)
    {
      
      if(numero == lista[i])
	{
	  //Si numero es igual al de la lista

	  if(elementos[i].size() != 0)
	    {
	      std::set<int>::iterator it;

	      for(it = elementos[i].begin(); it !=  elementos[i].end(); it++)
		{
		  retorno.push_back(*it);
		  
		}
	    }

	  break;
	}

    }

  return retorno;

  
}


std::string mayus(std::string pCadena)
{

  std::string retorno = "";
  for(int i = 0; i < pCadena.length(); i++)
    {
      retorno += std::toupper(pCadena[i]);
    }

  return retorno;
}
std::vector<int> DptoSet::unionDepto(std::string pString)
{
  //Dada una string hace una interseccion entre los departamentos dados
  std::vector<std::string> numeros;
  int count1 = 0;
  int count2 = 0;
  pString+= ",";
  for(int i = 0; i < pString.length(); i++)
    {
      
      if(pString.substr(i,1) == ",")
	{
	  //Encuentra una separacion
	  int count2 = pString.find(",", count1);
	  numeros.push_back(mayus(pString.substr(count1,(count2-count1))));
	  count1 = count2+1;
	  
	}
    }
  
  if(numeros.size() == 0){
      numeros.push_back(pString);
    }
  

  //Por cada uno de los elemntos que se tiene hay que asignarle primero que todo un numero y hay que generar una lista

  std::vector<std::vector<int>> vectorPadre;
  for (unsigned int i = 0; i < numeros.size(); i++)
    {
      //Se recorren todos los elementos
      if(numeros[i] == "ANTIOQUIA"){
	  vectorPadre.push_back(generarLista(5));
	}
      else if(numeros[i] == "ATLANTICO")
	{
	  vectorPadre.push_back(generarLista(8));
	}
      else if(numeros[i] == "BOGOTA")
	{
	  vectorPadre.push_back(generarLista(11));
	}
      else if(numeros[i] == "BOLIVAR")
	{
	  vectorPadre.push_back(generarLista(13));
	}
      else if(numeros[i] == "BOYACA")
	{
	  vectorPadre.push_back(generarLista(15));
	}
      else if(numeros[i] == "CALDAS")
	{
	  vectorPadre.push_back(generarLista(17));
	}
      else if(numeros[i] == "CAQUETA")
	{
	  vectorPadre.push_back(generarLista(18));
	}
      else if(numeros[i] == "CAUCA")
	{
	  vectorPadre.push_back(generarLista(19));
	}
      else if(numeros[i] == "CESAR")
	{
	  vectorPadre.push_back(generarLista(20));
	}
      else if(numeros[i] == "CORDOVA")
	{
	  vectorPadre.push_back(generarLista(23));
	}
      else if(numeros[i] == "CUNDINAMARCA")
	{
	  vectorPadre.push_back(generarLista(25));
	}
      else if(numeros[i] == "CHOCO")
	{
	  vectorPadre.push_back(generarLista(27));
	}
      else if(numeros[i] == "HUILA")
	{
	  vectorPadre.push_back(generarLista(41));
	}
      else if(numeros[i] == "LA GUAJIRA")
	{
	  vectorPadre.push_back(generarLista(44));
	}
      else if(numeros[i] == "MAGDALENA")
	{
	  vectorPadre.push_back(generarLista(47));
	}
      else if(numeros[i] == "META")
	{
	  vectorPadre.push_back(generarLista(50));
	}
      else if(numeros[i] == "NARINO")
	{
	  vectorPadre.push_back(generarLista(52));
	}
      else if(numeros[i] == "NORTE DE SANTANDER")
	{
	  vectorPadre.push_back(generarLista(54));
	}
      else if(numeros[i] == "QUINDIO")
	{
	  vectorPadre.push_back(generarLista(63));
	}
      else if(numeros[i] == "RISARALDA")
	{
	  vectorPadre.push_back(generarLista(66));
	}
      else if(numeros[i] == "SANTANDER")
	{
	  vectorPadre.push_back(generarLista(68));
	}
      else if(numeros[i] == "SUCRE")
	{
	  vectorPadre.push_back(generarLista(70));
	}
      else if(numeros[i] == "TOLIMA")
	{
	  vectorPadre.push_back(generarLista(73));
	}
      else if(numeros[i] == "VALLE DEL CAUCA")
	{
	  vectorPadre.push_back(generarLista(76));
	}
      else if(numeros[i] == "ARAUCA")
	{
	  vectorPadre.push_back(generarLista(81));
	}
      else if(numeros[i] == "CASANARE")
	{
	  vectorPadre.push_back(generarLista(85));
	}
      else if(numeros[i] == "PUTUMAYO")
	{
	  vectorPadre.push_back(generarLista(86));
	}
      else if(numeros[i] == "SAN ANDRES")
	{
	  vectorPadre.push_back(generarLista(88));
	}
      else if(numeros[i] == "AMAZONAS")
	{
	  vectorPadre.push_back(generarLista(91));
	}
      else if(numeros[i] == "GUAINIA")
	{
	  vectorPadre.push_back(generarLista(94));
	}
      else if(numeros[i] == "GUAVIARE")
	{
	  vectorPadre.push_back(generarLista(95));
	}
      else if(numeros[i] == "VAUPES")
	{
	  vectorPadre.push_back(generarLista(97));
	}
      else if(numeros[i] == "VICHADA")
	{
	  vectorPadre.push_back(generarLista(99));
	}
      else
	{
	  std::cout<<"Error: "<<numeros[i]<<" no existe"<<std::endl;
	}
    }

  /

  //Se crea el vector que contenga todos los ids
  std::vector<int> retorno{};
  
  for(int i =0; i< vectorPadre.size();i++)
    {
      for(int e = 0; e<vectorPadre[i].size();e++)
	{
	  retorno.push_back((vectorPadre[i])[e]);
	}
    }
  

  
  return retorno;
}

void DptoSet::insert(int pKey, std::string value)
{
  //Debe bucar que value corresponde para esto hay que compararlos con las que existan
  //5,8,11,13,15,17,18,19,20,23,25,27,41,44,47,50,52,54,63,66,68,70,73,76,81,85,86,88,91,94,95,97,99
  int casos = std::stoi(value);
  int lista[33] = {5,8,11,13,15,17,18,19,20,23,25,27,41,44,47,50,52,54,63,66,68,70,73,76,81,85,86,88,91,94,95,97,99};
  for(int i = 0; i < 33;i++){
      if(casos == lista[i]){
	  (elementos[i]).insert(pKey);
	  
	}
      
    }
}

void DptoSet::displaySet(int pNumero){

  std::set<int>::iterator it;
  
  for(it = (elementos[pNumero]).begin(); it != elementos[pNumero].end(); it++)
    {
      std::cout<<*it<<" ";
    }

  std::cout<<std::endl;
}

void DptoSet::display(){
  
  for(int i = 0; i < elementos.size();i++)
    {
      if(elementos[i].size() != 0)
	{
	  std::cout<<"Elementos("<<i<<"): ";
	  displaySet(i);
	}
    }
}



/*
ESTRUCTURA GENERAL QUE CONTIENE A TODOS LOS ELEMENTOS
*/


struct nodeMaster{
  int idUnico;
  int age;
  bool gender;
  std::string location;
};


class BDCovid
{
private:
  std::vector<nodeMaster*> tabla ;//Es la general que contendra todo
  
  GenderMap genderMap;//Contine los generos
  BinLoc binaryTree;//Contirne las edades
  DptoSet depSet;//Contene las locaciones

public:
  //Constructor
  BDCovid();
  //Metodo que retorna un vector luego de filtrar los parametros, genero ,rango  de edad(desde, hasta), y una string de locacion ("Amazonas,Bogota")
  std::vector<int> busqueda(bool pGenero,int pDesde, int pHasta, std::string pLocation);
  //Genera los archivos de tipo texto que luego python utiliza para poder hacer un display de la informacion dados los mismos parametro que busqueda
  void generarArchivoVis(bool pGenero,int pDesde, int pHasta, std::string pLocation);
};

BDCovid::BDCovid()
{
  //Comienza a leer todos los valores que hay en el datasetfinal.txt
  std::ifstream ifs("datasetFinal.txt");
  std::string line = "";
  if(ifs.good())
    {
      while(!ifs.eof())
	{
	  std::getline(ifs, line);

	  if(line != "")
	    {
	      //Se extrae linea y se guarda en line
	      int cont1 = 0;
	      //bool para cada uno de los atributos
	      bool id = true;
	      bool gender = true;
	      bool age = true;
	      bool loc = true;
	      nodeMaster* temp = new nodeMaster;
	      
	      for(int i = 0; i < line.length(); i++)
		{
		  
		  //Se recorre la linea
		  if(line[i] == ',')
		    {
		      //Se separa el espacio en memoria
		      if(id)
			{
			  
			  int tempCont = line.find(",");
  
			  temp->idUnico = std::stoi(line.substr(cont1,tempCont));
			  id = false;
			  cont1 = i;
			}
		      else if(age)
			{
			  int tam = line.find(",", cont1+1);
			  int pAge = std::stoi(line.substr(cont1+1, tam-1));
			  temp->age = pAge;
			  //Se introduce al arbol
			  binaryTree.insert(pAge, temp->idUnico);
			  age = false;
			  cont1 = i;
			  
			}
		      else if(gender)
			{
			  
			  if(line.substr(cont1+1,1) == "F")
			    {
			      temp->gender = true;
			      
			    }
			  else if(line.substr(cont1+1,1) == "M")
			    {
			      temp->gender = false;
			    }

			  genderMap.insert(temp->idUnico, temp->gender);
			  gender = false;
			  cont1 = i;
			}
		      else if(loc)
			{
			  int tam = line.find(",",cont1+1);
	     
			  int tam2 = line.find(",", tam);
			  
			  temp->location = line.substr(tam2+1, 2);
			  loc = false;
			  depSet.insert(temp->idUnico, temp->location);
			
			}
		    }
		
		}
	      tabla.push_back(temp);
	    }
	      
	}
      
    }
  
  ifs.close();

}

int binary_search(int x, const std::vector<int>&v)
{
  int left = 0, right = v.size()-1;
  while(left <= right)
    {
      int mid = (right+left)/2;
      if(v[mid]== x)
	return mid;
      else
	{
	  if(x> v[mid])
	    {
	      left = mid+1;
	    }
	  else
	    {
	      right = mid-1;
	    }
	}
    }
  return -1;
}



void swap(int i, int j, std::vector<int>& v){
  int temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}
void bubble_sort(std::vector<int> &v)
{
  bool swap_used = true;
  while(swap_used)
    {
      swap_used = false;
      for(int i = 0; i < v.size()-1;++i)
	{
	  if(v[i]> v[i+1])
	    {
	      swap(i, i+1, v);
	      swap_used = true;
	    }
	}
    }
}


std::vector<int> BDCovid::busqueda(bool pGenero,int pDesde, int pHasta, std::string pLocation){
  std::vector<std::vector<int>> retorno;
  std::vector<int> retornoFinal;
  std::cout<<"Antes de los generos"<<std::endl;
  retorno.push_back(genderMap.generarLista(pGenero));
  std::cout<<"Ya se tienen los del genero: "<<retorno[0].size()<<std::endl;
  retorno.push_back(binaryTree.rango(pDesde, pHasta));
  std::cout<<"Ya se tienen los de edad: "<<retorno[1].size()<<std::endl;
  retorno.push_back(depSet.unionDepto(pLocation));
  std::cout<<"Ya se tienen los de locacion: "<<retorno[2].size()<<std::endl;
  
  //Un elemento con id unico dado puede pasar si y solo si esta en las tres listas para eso se va comparar de a pares

  std::vector<int> parcial;
  for(int i = 0; i < retorno[1].size(); i++)
    {
      
      int valor = binary_search(retorno[1][i], retorno[2]);
      
      if(valor != -1)
	{
	  parcial.push_back(retorno[2][valor]);
	}
    }
	
  //Se hace el primer filtro

  bubble_sort(retorno[0]);
  
  for(int i = 0; i < parcial.size(); i++)
    {
      int valor = binary_search(parcial[i], retorno[0]);
      if(valor != -1)
	{
	  retornoFinal.push_back(retorno[0][valor]);
	}
    }
  
  std::cout<<"Hay: "<<retornoFinal.size()<<" personas que satisfacen los parametros de: "<< tabla.size()<<std::endl;

 
  return retornoFinal;
  
}

void BDCovid::generarArchivoVis(bool pGenero,int pDesde, int pHasta, std::string pLocation){

  std::remove("vis.txt");
  //Revisa si el archivo contiene un valor
  std::vector<int> resultado = busqueda(pGenero,pDesde, pHasta, pLocation);
  std::ofstream ofs("vis.txt",std::ios::app);

  bubble_sort(resultado);
  
  
  if(ofs.good())
    {
      
      for(int i = 0; i <tabla.size(); i++)
	{
	  
	  int elemento = binary_search(tabla[i]->idUnico,resultado);
	  if(elemento != -1)
	    {
	      ofs<<tabla[i]->location<<"\n";
	    }
	  
	}
      
    }

  ofs.close();
  
}

#endif
