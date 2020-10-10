#include "../include/classes.hpp"
#include "../include/pre-processamento.hpp"
#include "../include/passagens.hpp"

// Waliff Cordeiro Bandeira - 17/0115810

using namespace std;

void pre_processamento(string file);

int main(int argc, char **argv) {

    string argumento;

    vector<TabelaSimbolos> tabelaSimbolos;
    vector<TabelaSimbolos>::iterator it;

    if(argc == 3) {
        // argv[1] contém a flag -p ou -o (verificar na hora de gerar a saída)
        argumento = (string)argv[1];

        if(argumento == "-p") {
            pre_processamento(argv[2]);
        } else if(argumento == "-o"){

            tabelaSimbolos = primeira_passagem(argv[2]);
            segunda_passagem(argv[2], tabelaSimbolos);                   
            
            /**** Para visualizar a tabela de símbolos ****/
            // for(it=tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++) {
            //     it->imprimir();
            // }
            /**********************************************/
        }
        else {
            cout<<"Argumento inválido, utilize -p para o pre-processamento e -o para montar"<<endl;
        }
    } else {
        cout<<"O executável ./montador deve ser chamador seguido de um argumento e o nome do arquivo."<<endl;
        cout<<"Utilize -p para o pre-processamento e -o para montar"<<endl;
        cout<<"Para o argumento -p utilize um arquivo .asm e para o argumento -o utilize um arquivo .PRE"<<endl;
    } 
    
    return 0;
}