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
            
            if (tokens.empty()) {
                // Não é pra acontecer esse caso pois foi tratado no pre-processamento
                contador_linha++;
                continue;
            }

            // Verificando se o primeiro token é uma label  
            if(tokens[0].back() == ':') { 
                // Verificar se a label já foi definida
                for(it=tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++) {
                    if(tokens[0] == it->getSimbolo()) { // Se foi definida, deu erro
                        cout<<"(Linha: " << contador_linha << ") Erro semântico. Símbolo "<< tokens[0] << " redefinido." << endl;
                        break;
                    }
                }
                // Se não foi definida, definir
                if(validaLabel(tokens[0])) { // Verificando se o formato da label é valido
                    tokens[0].pop_back(); // Retirando dois pontos
                    TabelaSimbolos *tabela = new TabelaSimbolos(tokens[0], contador_posicao);
                    tabelaSimbolos.push_back(*tabela);
                    
                }
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
                cout << "(Linha: " << contador_linha<< ") Erro sintático. Operação inválida: "<< comando << endl;
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

    int contador_linha = 1, qtd_labels;
    bool erro = false;
    
    // Pegando o nome do arquivo sem a extensão e o path
    string nome_arquivo = file.substr(file.find_last_of("/") + 1);
    nome_arquivo = nome_arquivo.substr(0, nome_arquivo.find_last_of('.'));

    entrada.open(file);
    if(entrada.is_open()) {
        while(getline(entrada, linha)) {
            vector<string> tokens;
            tokens = split(linha, ' ', '\t');
            if(tokens.empty()) { // Linha vazia
                // Não é pra acontecer esse caso pois foi tratado no pre-processamento
                contador_linha++;
                continue;
            }

            /* Verificando se tem mais de uma label na mesma linha */
            qtd_labels = 0;
            for(string &palavra : tokens) { 
                if (validaLabel(palavra)) {
                    qtd_labels++;
                }
            }
            /*******************************************************/
            
            if(qtd_labels > 1) { // Mais de uma label na mesma linha
                cout << "(Linha: " << contador_linha << 
                ") Erro sintático. Mais de uma label definida na linha." << endl;
                erro = true;
                contador_linha++;
                continue;
            }
            /******************************************************/

            string isSessao = toUpperCase(tokens[0]);
            if(isSessao == "SECTION") { // Pegando a Sessão
                if(tokens.size() == 2) {
                    sessao = toUpperCase(tokens[1]);
                    sessoes.push_back(sessao);
                } else if(tokens.size() == 1) { // Quantidades inválidas de argumento na SECTION
                    cout << "(Linha: "  << contador_linha << ") Erro sintático." << " A diretiva SECTION não possui argumento" << endl;
                } else {
                    cout << "(Linha: "  << contador_linha << ") Erro sintático." << " A diretiva SECTION possui quantidade inválida de argumentos" << endl;
                }
                contador_linha++;
                continue;
            } else { // Se não é section inicia-se com label, instrução ou outra diretiva
                if (validaLabel(tokens[0])) { // Iniciada em label
                    tokens.erase(tokens.begin()); // Retirando Label   
                } else if (tokens[0].back() == ':') {
                    if (!validaLabel(tokens[0])) { // Verificando se o token da label está com formato inválido
                        cout << "(Linha: "<< contador_linha <<") Erro léxico. A label " <<
                        tokens[0] << " está com formato inválido." << endl;
                        erro = true;
                        contador_linha++;
                        continue;
                    }
                }
                string comando = toUpperCase(tokens[0]);
                if(tabelaInstrucoes.find(comando) != tabelaInstrucoes.end()) { // Verifica se é instrução
                    if(sessao != "TEXT") { // Se é instrução e a sessão não é TEXT.
                        erro = true;
                        cout << "(Linha: " << contador_linha << ") Erro semântica. Instrução " << 
                        comando << " está na sessão inválida." << endl;
                    }
                    /* Inicializa as variáveis auxiliares para a linha objeto */
                    int tamanho = tabelaTamanhos[comando];
                    int argumento1 = -1, argumento2 = -1;
                    int qtd_argumentos = tamanho - 1, codigo;
                    /**********************************************************/
                    if(tamanho == (tokens.size())) { // É instrução de tamanho válido
                        codigo = tabelaInstrucoes[comando];
                        if (qtd_argumentos == 0) { // STOP
                            LinhaObjeto *linhaobjeto = new LinhaObjeto(codigo, argumento1, argumento2);
                            linhaObj.push_back(*linhaobjeto);
                            contador_linha++;
                            continue;
                            
                        } else if(validaInstrucao(tokens, qtd_argumentos)) {
                            // Instrução válida e possui argumentos
                            /************ Consultando a tabela de símbolos ************/
                            if(qtd_argumentos == 1) {
                                bool encontrouSimbolo = false;
                                for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                        if(simbolo.getSimbolo() == tokens[1]) {
                                            argumento1 = simbolo.getPosicao();
                                            encontrouSimbolo = true;
                                            break;
                                        }
                                }
                                if(!encontrouSimbolo) { // Não encontrou símbolo
                                    cout << "(Linha: " << contador_linha << 
                                    ") Erro semântico. O símbolo" << tokens[1] << 
                                    " não está definido na tabela de símbolos"<<endl;
                                    erro = true;
                                    contador_linha++;
                                    continue;
                                }
                            } else if(qtd_argumentos == 2) {
                                if(comando == "COPY") {
                                    tokens[1].pop_back();
                                }
                                bool encontrouSimbolo1 = false, encontrouSimbolo2 = false;
                                for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                    if(simbolo.getSimbolo() == tokens[1]) {
                                        encontrouSimbolo1 = true;
                                        argumento1 = simbolo.getPosicao();
                                    }
                                    if(simbolo.getSimbolo() == tokens[2]) {
                                        encontrouSimbolo2 = true;
                                        argumento2 = simbolo.getPosicao();
                                    }
                                }
                                /**** Não encontrou símbolo ****/
                                if(!encontrouSimbolo1) {
                                    cout << "(Linha: " << contador_linha << 
                                    ") Erro semântico. O simbolo do primeiro argumento " << tokens[1] << 
                                    " não está definido na tabela de símbolos"<<endl;
                                    if(!encontrouSimbolo2) {
                                        cout << "(Linha: " << contador_linha << 
                                        ") Erro semântico. O simbolo do segundo argumento " << tokens[2] << 
                                        " não está definido na tabela de símbolos"<<endl;
                                    }
                                    erro = true;
                                    contador_linha++;
                                    continue;
                                }
                                if(!encontrouSimbolo2) {
                                    cout << "(Linha: " << contador_linha << 
                                    ") Erro semântico. O simbolo do segundo argumento " << tokens[2] << 
                                    " não está definido na tabela de símbolos"<<endl;
                                    if(!encontrouSimbolo1) {
                                        cout << "(Linha: " << contador_linha << 
                                        ") Erro semântico. O simbolo do primeiro argumento " << tokens[1] << 
                                        " não está definido na tabela de símbolos"<<endl;
                                    }
                                    erro = true;
                                    contador_linha++;
                                    continue;
                                }
                            }
                            /*********************************************************/
                            /* Caso de sucesso - Cria Objeto */
                            LinhaObjeto *linhaobjeto = new LinhaObjeto(codigo, argumento1, argumento2);
                            linhaObj.push_back(*linhaobjeto);
                            contador_linha++;
                            continue;
                        } else { // Instrução mas não tem os argumentos compatíveis com a instrução
                            if(comando == "COPY") {
                                cout << "(Linha: " << contador_linha << ") Erro sintático. Formato de argumentos inválido."<< endl << 
                                "O COPY deve ter dois argumentos separados por uma vírgula e um espaço. Exemplo: COPY TAM1, TAM2" << endl;
                                
                                /******* Verificando os símbolos do COPY *******/
                                if(qtd_argumentos == 1) {
                                    bool encontrouSimbolo = false;
                                    for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                            if(simbolo.getSimbolo() == tokens[1]) {
                                                argumento1 = simbolo.getPosicao();
                                                encontrouSimbolo = true;
                                                break;
                                            }
                                    }
                                    if(!encontrouSimbolo) { // Não encontrou símbolo
                                        cout << "(Linha: " << contador_linha << 
                                        ") Erro semântico. O símbolo" << tokens[1] << 
                                        " não está definido na tabela de símbolos"<<endl;
                                    }
                                } else if(qtd_argumentos == 2) {
                                    bool encontrouSimbolo1 = false, encontrouSimbolo2 = false;
                                    for(TabelaSimbolos &simbolo : tabelaSimbolos) {
                                        if(simbolo.getSimbolo() == tokens[1]) {
                                            encontrouSimbolo1 = true;
                                            argumento1 = simbolo.getPosicao();
                                        }
                                        if(simbolo.getSimbolo() == tokens[2]) {
                                            encontrouSimbolo2 = true;
                                            argumento2 = simbolo.getPosicao();
                                        }
                                    }
                                    /**** Não encontrou símbolo ****/
                                    if(!encontrouSimbolo1) {
                                        cout << "(Linha: " << contador_linha << 
                                        ") Erro semântico. O simbolo do primeiro argumento " << tokens[1] << 
                                        " não está definido na tabela de símbolos"<<endl;
                                        if(!encontrouSimbolo2) {
                                            cout << "(Linha: " << contador_linha << 
                                            ") Erro semântico. O simbolo do segundo argumento " << tokens[2] << 
                                            " não está definido na tabela de símbolos"<<endl;
                                        }
                                    }
                                    else if(!encontrouSimbolo2) {
                                        cout << "(Linha: " << contador_linha << 
                                        ") Erro semântico. O simbolo do segundo argumento " << tokens[2] << 
                                        " não está definido na tabela de símbolos"<<endl;
                                        if(!encontrouSimbolo1) {
                                            cout << "(Linha: " << contador_linha << 
                                            ") Erro semântico. O simbolo do primeiro argumento " << tokens[1] << 
                                            " não está definido na tabela de símbolos"<<endl;
                                        }
                                    }
                                }
                                /**************************************/
                            } else {
                                cout << "(Linha: " << contador_linha << ") Erro sintático. Formato de argumentos inválido." << endl;
                            }
                            erro = true;
                            contador_linha++;
                            continue;
                        }
                    } else { // Instrução mas não possui quantidade de tokens válidos
                        erro = true;
                        cout << "(Linha: "  << contador_linha << ") Erro sintático." << " A instrução " <<
                        comando << " possui quantidade inválida de argumentos" << endl;
                        contador_linha++;
                        continue;
                    }
                } else if (tabelaTamanhos.find(comando) != tabelaTamanhos.end()) {
                    // Verifica se é diretiva
                    if(sessao != "DATA") { // Se é diretiva e não está no data
                        erro = true;
                        cout << "(Linha: " << contador_linha << ") Erro semântico. A diretiva " << 
                        comando << " não está na sessão correta." << endl;
                        contador_linha++;
                        continue;
                    } else { // Diretiva na Section correta
                        int codigo, argumento1 = -1, argumento2 = -1;
                        if(comando == "SPACE") {
                            codigo = 0;
                            LinhaObjeto *linhaobjeto = new LinhaObjeto(codigo, argumento1, argumento2);
                            linhaObj.push_back(*linhaobjeto);
                            contador_linha++;
                            continue;
                        } else if (comando == "CONST") {
                            if (tokens.size() == 2) {
                                codigo = -1;
                                if(isInteger(tokens[1])) {
                                    argumento1 = stoi(tokens[1]);
                                    LinhaObjeto *linhaobjeto = new LinhaObjeto(codigo, argumento1, argumento2);
                                    linhaObj.push_back(*linhaobjeto);
                                    contador_linha++;
                                    continue;
                                } else {
                                    cout << "(Linha: " << contador_linha << 
                                    ") Erro sintático. O argumento " << tokens[1] << " para o CONST é inválido" << endl;
                                    erro = true;
                                    contador_linha++;
                                    continue;
                                }
                            }
                        } else { 
                            cout << "(Linha: " << contador_linha << ") Erro léxico. Operação " << comando << " inválida" << endl; 
                            erro = true;
                            contador_linha++;
                            continue;  
                        }
                    }
                } else {
                    // Não é comando nem diretiva
                    cout << "(Linha: " << contador_linha << ") Erro léxico. Operação inválida: " << comando << endl;
                    contador_linha++;
                    erro = true;
                    continue;
                }

            }
        }
        /* Verificando se a Section Text e/ou Data foram definidas */
        bool existeText = false, existeData = false;
        if(!sessoes.empty()) {
            for(string &sect : sessoes) {
                if(sect == "TEXT") {
                    existeText = true;
                }
                else if(sect == "DATA") {
                    existeData = true;
                }
            }
        }
        if(!existeText) {
            cout << "(Linha: 1) Erro sintático. Não foi definida a Section Text." << endl;
            erro = true;
        }
        if(!existeData) {
            cout << "Erro sintático. Não foi definida a Section Data." << endl;
            erro = true;
        }
        /**********************************************************/
        
        /* Salvando no arquivo, se tiver algo para salvar e não houver erros */
        if(!linhaObj.empty() && !erro) { 
            ofstream saida(nome_arquivo+".obj"); // ofstream -> registro de arquivo, sai do programa e salva em arquivo
            for(LinhaObjeto &valor : linhaObj) {
                    if(valor.codigo != -1) {
                        saida<<valor.codigo<<" ";
                    }
                    if(valor.simbolo1 != -1) {
                        saida<<valor.simbolo1<<" ";
                    }
                    if(valor.simbolo2 != -1) {
                        saida<<valor.simbolo2<<" ";
                    }                    
            }
            saida.close();
        }
        entrada.close();

    } else {
        cout << "Não foi possível abrir o arquivo: "<< file <<endl; 
    }
}