#include "../include/classes.hpp"

/*********************************** Maps ***********************************/
unordered_map<string, int> tabelaInstrucoes = {
    {"ADD",     1}, 
    {"SUB",     2}, 
    {"MULT",    3}, 
    {"DIV",     4}, 
    {"JMP",     5}, 
    {"JMPN",    6}, 
    {"JMPP",    7}, 
    {"JMPZ",    8}, 
    {"COPY",    9}, 
    {"LOAD",    10},
    {"STORE",   11},
    {"INPUT",   12},
    {"OUTPUT",  13},
    {"STOP",    14}
};

unordered_map<string, int> tabelaTamanhos = {
    {"ADD",     2}, 
    {"SUB",     2}, 
    {"MULT",    2}, 
    {"DIV",     2}, 
    {"JMP",     2}, 
    {"JMPN",    2}, 
    {"JMPP",    2}, 
    {"JMPZ",    2}, 
    {"COPY",    3}, 
    {"LOAD",    2},
    {"STORE",   2},
    {"INPUT",   2},
    {"OUTPUT",  2},
    {"STOP",    1},
    {"CONST",   1},
    {"SPACE",   1},
    {"SECTION", 0}
};
/*****************************************************************************/

/********************************** Funções **********************************/

bool isInteger(const std::string &s) {
    return std::regex_match(s, std::regex("-?[0-9]{0,10}"));
}

bool validaInstrucao(vector<string> tokens, int tamanho) {
    string comando = toUpperCase(tokens[1]);
    if (tamanho == 1) {
        return validaWord(tokens[1]);
    } else if (tamanho == 2) {
        return validaCopy(tokens[1]) && validaWord(tokens[2]);
    } 
    return false;
}
bool validaLabel (string label) {
    return label.size() <= 50 && regex_match(label, regex("^[a-zA-Z_$][a-zA-Z_$0-9]*[:]"));
}
bool validaWord (string word) {
    return word.size() <= 50 && regex_match(word, regex("^[a-zA-Z_$][a-zA-Z_$0-9]*"));
}
bool validaCopy (string argumento) {
    return argumento.size() <= 50 && regex_match(argumento, regex("^[a-zA-Z_$][a-zA-Z_$0-9]*[,]"));
}

const vector<string> split(const string& frase, const char& delimitador1, const char& delimitador2) {
	string buff{""};
	vector<string> palavras;
	
	for(auto letra:frase)
	{
		if(letra != delimitador1 && letra != delimitador2) {
            buff+=letra; 
        }
        else if((letra == delimitador1 || letra == delimitador2) && buff != "") { 
            palavras.push_back(buff); buff = ""; 
        }
	}
	if(buff != "") palavras.push_back(buff);	
	return palavras;
}

string toUpperCase(string str) {
    transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}
/*******************************************************************************/

/************************************ Linha ************************************/

Linha::Linha(string rotulo, string comando, string operador1, string operador2) { // Construtor
    this->rotulo = rotulo;
    this->comando = comando;
    this->operador1 = operador1;
    this->operador2 = operador2;
}

Linha::~Linha() { // Destrutor

}

// ----- Setters -----
void Linha::setRotulo(string rotulo) {
    this->rotulo = rotulo;
}

void Linha::setComando(string comando) {
    this->comando = comando;
}

void Linha::setOperador1(string operador1) {
    this->operador1 = operador1;
}

void Linha::setOperador2(string operador2) {
    this->operador2 = operador2;
}

void Linha::setSessao(string sessao) {
    this->sessao = sessao;
}

void Linha::set(string rotulo, string comando, string operador1, string operador2) {
    this->rotulo = rotulo;
    this->comando = comando;
    this->operador1 = operador1;
    this->operador2 = operador2;
}
// -------------------

// ----- Getters -----

string Linha::getRotulo() {
    return this->rotulo;
}
string Linha::getComando() {
    return this->comando;
}
string Linha::getOperador1() {
    return this->operador1;
}
string Linha::getOperador2() {
    return this->operador2;
}
string Linha::getSessao() {
    return this->sessao;
}
// -------------------

void Linha::imprimir() {
    if(this->rotulo != "") {
        cout<<this->rotulo<<" ";
    }
    if(this->comando != "") {
        cout<<this->comando<<" ";
    }
    if(this->operador1 != "") {
        cout<<this->operador1<<" ";
    }
    if(this->operador2 != "") {
        cout<<this->operador2<<" ";
    }
    if(this->sessao != "") {
        cout<<this->sessao<<" ";
    }
    cout<<endl;  
}

/*******************************************************************************/

/************************************* EQU *************************************/

EQU::EQU(string nome, string valor) { // Construtor
    this->nome = nome;
    this->valor = valor;
}

EQU::~EQU() { // Destrutor

}

// ----- Setters -----
void EQU::setNome(string nome) {
    this->nome = nome;
}

void EQU::setValor(string valor) {
    this->valor = valor;
}

void EQU::set(string nome, string valor) {
    this->nome = nome;
    this->valor = valor;
}
// -------------------

// ----- Getters -----

string EQU::getNome() {
    return this->nome;
}
string EQU::getValor() {
    return this->valor;
}

// -------------------

void EQU::imprimir() {
    if(this->nome != "") {
        cout<<this->nome<<" ";
    }
    if(this->valor != "") {
        cout<<this->valor<<" ";
    }
    cout<<endl;
}

bool EQU::EQU_unico(vector<EQU> EQUs) {
    vector<EQU>::iterator it_equ;
    for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
        if(this->nome == it_equ->nome) {
            cout << this->nome << " foi definido mais de uma vez, verifique se foi um erro. Para o pre-processamento foi utilizado o primeiro valor informado."<< endl;
            return false;
        }
    }
    return true;
}

/*******************************************************************************/

/***************************** Tabela de Símbolos ******************************/
TabelaSimbolos::TabelaSimbolos(string simbolo, int posicao) { // Construtor
    this->simbolo = simbolo;
    this->posicao = posicao;
}

TabelaSimbolos::~TabelaSimbolos() { // Destrutor

}

// ----- Setters -----
void TabelaSimbolos::setSimbolo(string simbolo) {
    this->simbolo = simbolo;
}

void TabelaSimbolos::setPosicao(int posicao) {
    this->posicao = posicao;
}

void TabelaSimbolos::set(string simbolo, int posicao) {
    this->simbolo = simbolo;
    this->posicao = posicao;
}
// -------------------

// ----- Getters -----

string TabelaSimbolos::getSimbolo() {
    return this->simbolo;
}
int TabelaSimbolos::getPosicao() {
    return this->posicao;
}

// -------------------

void TabelaSimbolos::imprimir() {
    if(this->simbolo != "") {
        cout<<this->simbolo<<" ";
    }
    cout<<this->posicao<<" ";
    cout<<endl;  
}

/*******************************************************************************/

/***************************** Tabela de Símbolos ******************************/
LinhaObjeto::LinhaObjeto(int codigo, int simbolo1, int simbolo2) { // Construtor
    this->codigo = codigo;
    this->simbolo1 = simbolo1;
    this->simbolo2 = simbolo2;
}

LinhaObjeto::~LinhaObjeto() { // Destrutor

}

// ----- Setters -----

void LinhaObjeto::setCodigo(int codigo) {
    this->codigo = codigo;
}

void LinhaObjeto::setSimbolo1(int simbolo1) {
    this->simbolo1 = simbolo1;
}

void LinhaObjeto::setSimbolo2(int simbolo2) {
    this->simbolo2 = simbolo2;
}

void LinhaObjeto::set(int codigo, int simbolo1, int simbolo2) {
    this->codigo = codigo;
    this->simbolo1 = simbolo1;
    this->simbolo2 = simbolo2;
}
// -------------------

// ----- Getters -----
int LinhaObjeto::getCodigo() {
    return this->codigo;
}
int LinhaObjeto::getSimbolo1() {
    return this->simbolo1;
}
int LinhaObjeto::getSimbolo2() {
    return this->simbolo2;
}

// -------------------

void LinhaObjeto::imprimir() {
    if(this->codigo != -1) {
        cout<<this->codigo<<" ";
    }
    if(this->simbolo1 != -1) {
        cout<<this->simbolo1<<" ";
    }
    if(this->simbolo2 != -1) {
        cout<<this->simbolo2<<" ";
    }
    cout<<endl;
}

/*******************************************************************************/