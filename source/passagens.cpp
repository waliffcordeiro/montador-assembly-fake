#include "../include/passagens.hpp"

vector<TabelaSimbolos> primeira_passagem(string file) {
    
    ifstream entrada;

    vector<string> tokens;
    vector<string>::iterator it_tokens;

    vector<TabelaSimbolos> tabelaSimbolos;
    vector<TabelaSimbolos>::iterator it;

    string linha, token, comando;

    int contador_posicao = 0, contador_linha = 1;

    entrada.open(file);
    if(entrada.is_open()) {
        // Pegando linha a linha
        while(getline(entrada, linha)) {
            tokens = split(linha, ' ', '\t');
            
            // Verificando se o primeiro token é uma label  
            if(tokens[0].back() == ':') { 
                // Verificar se a label já foi definida
                for(it=tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++) {
                    if(tokens[0] == it->getSimbolo()) { // Se foi definida, deu erro
                        cout<<"Erro: "<< tokens[0] << " redefinido na linha " << contador_linha << endl;
                        break;
                    }
                }
                // Se não foi definida, definir
                TabelaSimbolos *tabela = new TabelaSimbolos(tokens[0], contador_posicao);
                tabelaSimbolos.push_back(*tabela);
                
                // Salvando o comando ou diretiva referente à label
                comando = toUpperCase(tokens[1]);

            } else { // É um comando ou diretiva
                comando = toUpperCase(tokens[0]);
            }
            if(tabelaInstrucoes.find(comando) != tabelaInstrucoes.end()) {
            // Verificando se é uma instrução
            int tamanho = tabelaTamanhos[comando];
            contador_posicao += tamanho;
            } else if (tabelaTamanhos.find(comando) != tabelaTamanhos.end()) {
                // Verificando se é diretiva ou section
                int tamanho = tabelaTamanhos[comando];
                contador_posicao += tamanho;
            }
            else { // Se não é instrução nem diretiva
                cout << "Operação inválida: "<< comando << " na linha " << contador_linha << endl;
            }
        contador_linha++;
        }
        return tabelaSimbolos;
    } else {
        cout << "Não foi possível abrir o arquivo: "<< file <<endl;
        return tabelaSimbolos;
    }
}

void segunda_passagem(string file, vector<TabelaSimbolos>) {
    
    ifstream entrada; // ifstream -> leitura de arquivo

    string linha, token;
    vector<string> tokens;
    
    // Pegando o nome do arquivo sem a extensão e o path
    string nome_arquivo = file.substr(file.find_last_of("/") + 1);
    nome_arquivo = nome_arquivo.substr(0, nome_arquivo.find_last_of('.'));

    entrada.open(file);
    if(entrada.is_open()) {
        while(getline(entrada, linha)) {
            tokens = split(linha, ' ', '\t');
        }

    } else {
        cout << "Não foi possível abrir o arquivo: "<< file <<endl; 
    }
}