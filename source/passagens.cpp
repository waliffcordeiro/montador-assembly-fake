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
                tokens[0].pop_back(); // Retirando dois pontos
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

bool segunda_passagem(string file, vector<TabelaSimbolos> tabelaSimbolos) {
    
    ifstream entrada; // ifstream -> leitura de arquivo

    string linha, token, sessao;
    vector<string> sessoes;

    vector<LinhaObjeto> linhaObj;

    int contador_linha = 1;
    bool erro = false;
    
    // Pegando o nome do arquivo sem a extensão e o path
    string nome_arquivo = file.substr(file.find_last_of("/") + 1);
    nome_arquivo = nome_arquivo.substr(0, nome_arquivo.find_last_of('.'));

    entrada.open(file);
    if(entrada.is_open()) {
        while(getline(entrada, linha)) {
            vector<string> tokens;
            tokens = split(linha, ' ', '\t');

            string isSessao = toUpperCase(tokens[0]);
            if(isSessao == "SECTION") {
                sessao = toUpperCase(tokens[1]);
                sessoes.push_back(sessao);
                contador_linha++;
                continue;
            } else { // Se não é section inicia-se com label, instrução ou diretiva
                if (validaLabel(tokens[0])) { // Iniciada em label
                    tokens.erase(tokens.begin()); // Retirando Label   
                }
                string comando = toUpperCase(tokens[0]);
                if(tabelaInstrucoes.find(comando) != tabelaInstrucoes.end()) {
                    // Verifica se é instrução
                    int tamanho = tabelaTamanhos[comando];
                    int qtd_argumentos = tamanho - 1;
                    if(tamanho == (tokens.size())) {
                        // Substraindo a label
                        // É instrução e o tamanho é válido, verificar argumentos
                        if (qtd_argumentos == 0) {

                        } else if(validaInstrucao(tokens, qtd_argumentos)) {
                            // Instrução válida!
                            int codigo = tabelaInstrucoes[comando];
                            int argumento1 = -1, argumento2 = -1;
                            /************ Consultando a tabela de símbolos ************/
                            if(qtd_argumentos == 1) {
                                for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                        if(simbolo.getSimbolo() == tokens[1]) {
                                            argumento1 = simbolo.getPosicao();
                                            break;
                                        }
                                }
                            } else if(qtd_argumentos == 2) {
                                if(comando == "COPY") {
                                    tokens[1].pop_back();
                                }
                                for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                    if(simbolo.getSimbolo() == tokens[1]) {
                                        argumento1 = simbolo.getPosicao();
                                    }
                                    if(simbolo.getSimbolo() == tokens[2]) {
                                        argumento2 = simbolo.getPosicao();
                                    }
                                }
                                continue;
                            }
                            /*********************************************************/
                            LinhaObjeto *linhaobjeto = new LinhaObjeto(codigo, argumento1, argumento2);
                            linhaObj.push_back(*linhaobjeto);
                        } else {
                            erro = true;
                            // Erro
                            contador_linha++;
                            continue;
                        }
                    } else {
                        erro = true;
                        cout << "Erro. Na linha " << contador_linha << " a instrução" <<
                        comando << " possui menos operadores que o necessário";
                        contador_linha++;
                        continue;
                    }
                } else if (tabelaTamanhos.find(comando) != tabelaTamanhos.end()) {
                    // Verifica se é diretiva
                    if(sessao == "TEXT") {
                        erro = true;
                        cout << "Erro. A diretiva " << comando << "está na Section Text";
                        contador_linha++;
                        continue;
                    } else {
                        // Tratar diretivas
                    }
                } else {
                    // Não é comando nem diretiva, conferir sintaxe
                }

            }
        }
        if(!linhaObj.empty()) { 
            for(LinhaObjeto &linha : linhaObj) {
                linha.imprimir();
            }
        }

    } else {
        cout << "Não foi possível abrir o arquivo: "<< file <<endl; 
    }
}