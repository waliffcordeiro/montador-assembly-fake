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

using namespace std;

const vector<string> split(const string& frase, const char& delimitador1, const char& delimitador2);

string toUpperCase(string str);

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
    TabelaSimbolos(string simbolo, string valor);
    ~TabelaSimbolos();
    
    // Atributos
    string simbolo;
    string valor;

    //Setters e Getters
    void setSimbolo(string simbolo);
    string getSimbolo();

    void setValor(string valor);
    string getValor();

    void set(string simbolo, string valor);

};
/*******************************************************************************/





#endif /* CLASSES_H */