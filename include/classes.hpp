#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <algorithm> 
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>

using namespace std;

/********************************* Funções *************************************/
const vector<string> split(const string& frase, const char& delimitador1, const char& delimitador2);

string toUpperCase(string str);

bool validaLabel (string label);
bool validaWord (string word);
bool validaCopy (string argumento);
/*******************************************************************************/

/*********************************** Maps **************************************/
extern unordered_map<string, int> tabelaInstrucoes;

extern unordered_map<string, int> tabelaTamanhos;
/*******************************************************************************/

/************************************ Linha ************************************/
class Linha {

    public:

    // Construtor e Destrutor
    Linha(string rotulo, string comando, string operador1, string operador2);
    ~Linha();
    
    // Atributos
    string rotulo;
    string comando;
    string operador1;
    string operador2;
    string sessao;

    //Setters e Getters
    void setRotulo(string rotulo);
    string getRotulo();

    void setComando(string comando);
    string getComando();

    void setOperador1(string operador1);
    string getOperador1();

    void setOperador2(string operador2);
    string getOperador2();

    void setSessao(string sessao);
    string getSessao();

    void set(string rotulo, string comando, string operador1, string operador2);

    void imprimir();
};
/*******************************************************************************/

/************************************* EQU *************************************/
class EQU {

    public:

    // Construtor e Destrutor
    EQU(string nome, string valor);
    ~EQU();
    
    // Atributos
    string nome;
    string valor;

    //Setters e Getters
    void setNome(string nome);
    string getNome();

    void setValor(string valor);
    string getValor();

    void set(string nome, string valor);

    void imprimir();

    bool EQU_unico(vector<EQU> EQUs);
};
/*******************************************************************************/

/****************************** Tabela de Símbolos *****************************/
class TabelaSimbolos {

    public:

    // Construtor e Destrutor
    TabelaSimbolos(string simbolo, int posicao);
    ~TabelaSimbolos();
    
    // Atributos
    string simbolo;
    int posicao;

    //Setters e Getters
    void setSimbolo(string simbolo);
    string getSimbolo();

    void setPosicao(int posicao);
    int getPosicao();

    void set(string simbolo, int posicao);

    void imprimir();

};
/*******************************************************************************/





#endif /* CLASSES_H */