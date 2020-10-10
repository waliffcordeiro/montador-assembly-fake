#include "../include/pre-processamento.hpp"

void pre_processamento(string file) {
    
    vector<string> tokens;
    vector<string>::iterator it;

    vector<Linha> linhas;
    vector<Linha>::iterator it_linha;

    vector<EQU> EQUs;
    vector<EQU>::iterator it_equ;

    // Variáveis de leitura
    ifstream entrada; // ifstream -> leitura de arquivo
    string linha, token, isEQU;

    string nome_arquivo = file.substr(file.find_last_of("/") + 1);
    nome_arquivo = nome_arquivo.substr(0, nome_arquivo.find_last_of('.'));

    entrada.open(file);
    if(entrada.is_open()) {
        // Pegando linha a linha
        while(getline(entrada, linha)) {
            // Separa todos os tokens em um vector de string retirando espaços e tabulações
            tokens = split(linha, ' ', '\t');

            if(tokens.size() == 1) { // Só um token. É comando, label ou IF 
                if (tokens[0].front() == ';') { // Ignorando comentário
                    continue;
                }
                else if(tokens[0].back() == ':') { // Se o último caracter é : então é label ou EQU    
                    
                    string elemento = tokens[0]; // Salva a label ou EQU
                    string label = tokens[0];

                    // Pegamos a linha seguinte
                    if (getline(entrada, linha)) { // Evitar ler o fim do arquivo
                        tokens = split(linha, ' ', '\t'); // Retira espaços e tabulações

                        // Verificar se é EQU
                        isEQU = toUpperCase(tokens[0]);

                        if(isEQU == "EQU") { // Criar objeto EQU
                            elemento.pop_back(); // Retirando o : para salvar o nome do EQU
                            EQU *equ = new EQU(elemento, "");
                            if(equ->EQU_unico(EQUs)) {
                                if (tokens.size() == 2) {
                                    // Garantindo que tem os 2 tokens (EQU e valor)
                                    equ->setValor(tokens[1]);
                                    EQUs.push_back(*equ);
                                }
                                else { // Verificando quantidade de argumentos no EQU
                                    cout << "Erro sintático. O EQU referente ao " << elemento << " foi definido com quantidade não permitida de parâmetros. Utilize apenas um parâmetro."<<endl;
                                    continue;
                                }
                            }
                        }
                        else { // Se não for EQU, criar objeto Linha
                            Linha *linhaObj = new Linha(label, "", "", "");

                            int contador = 0;
                            for(it=tokens.begin(); it != tokens.end(); it++) { // Iterando a linha complementar à label
                                string word = (string)*it;
                                if(word[0] == ';') { // Ignorando comentários
                                    break;
                                }
                                if(contador == 0) { // O primeiro elemendo da linha seguinte à label é comando
                                    linhaObj->setComando(*it);
                                } else if (contador == 1) {
                                    // O segundo elemendo da linha seguinte à label é o primeiro operador
                                    linhaObj->setOperador1(*it);
                                    // Verificando se é EQU
                                    for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                                        if(it_equ->nome == *it) {
                                            linhaObj->setOperador1(it_equ->valor);
                                        }
                                    }
                                } else if (contador == 2) {
                                    // Se tiver 3 argumentos
                                    // O terceiro elemendo da linha seguinte à label é o segundo operador
                                    linhaObj->setOperador2(*it);
                                    // Verificando se é EQU
                                    for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                                        if(it_equ->nome == *it) {
                                            linhaObj->setOperador2(it_equ->valor);
                                        }
                                    }
                                }
                            }
                            contador++;
                            linhas.push_back(*linhaObj);                
                        } 
                    }               
                }
                else { // Não é label nem EQU, então é comando sem argumento
                    Linha *linhaObj = new Linha("", tokens[0], "", "");
                    linhas.push_back(*linhaObj);
                }
            }
            else { // Linha comum 

                // Verificar se é EQU
                isEQU = toUpperCase(tokens[1]);
                if(isEQU == "EQU" && tokens.size() != 3) {
                    tokens[0].pop_back();
                    string elemento = tokens[0];
                    cout << "Erro sintático. O EQU referente ao " << elemento << " foi definido com quantidade não permitida de parâmetros. Utilize apenas um parâmetro."<<endl;
                    continue;
                }

                Linha *linhaObj = new Linha("", "", "", "");
                string isElemento;
                isEQU = toUpperCase(tokens[1]);
                isElemento = toUpperCase(tokens[0]);
                if (tokens.size() == 2) { // Section ou IFs
                    if(isElemento == "SECTION") {
                        string section = tokens[0]+" "+tokens[1];
                        linhaObj->set("", section, "", "");
                    } else if (isElemento == "IF") {
                        int qtd_EQUs = 1;
                        for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                            // Verificando "chave/valor" do EQU
                            if(it_equ->nome == tokens[1]) {
                                if (it_equ->valor == "0") {
                                    // Se o valor é False, ignora a linha (lê e volta pro início do código para uma nova leitura)
                                    getline(entrada, linha);
                                    break;
                                }
                                else {
                                    // Senão, faz a leitura da próxima linha, voltando ao início do código
                                    break;
                                }
                            } else if (qtd_EQUs == EQUs.size()) {
                                // Iterou todo o vector e não encontrou o elemento
                                cout << "Erro semântico. " << tokens[1] << " foi passado como argumento de um IF mas não foi definido."<< endl;
                                // Ignorando a linha seguinte
                                getline(entrada, linha);
                                break;
                            }
                            qtd_EQUs++;
                        }
                        continue;
                    } else { // Se não for section, contém comando e um operador
                        linhaObj->set("", tokens[0], tokens[1], "");
                        for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                            // Verificando se o argumento é EQU
                            if(it_equ->nome == tokens[1]) {
                                linhaObj->set("", tokens[0], it_equ->valor, "");
                            }
                        }
                    }
                } else if (tokens[1] == "EQU") { // Salvando EQU na mesma linha que a label
                    if(tokens.size() != 3) { // Verificando quantidade de argumentos do EQU
                        string elemento = tokens[0];
                        elemento.pop_back();
                        cout << "Erro sintático. O EQU referente ao " << elemento << " foi definido com quantidade não permitida de parâmetros. Utilize apenas um parâmetro."<<endl;
                        continue;
                    }
                    tokens[0].pop_back(); // Retirando o : para salvar o nome do EQU
                    EQU *equ = new EQU(tokens[0], tokens[2]);
                    if(equ->EQU_unico(EQUs)) {
                        EQUs.push_back(*equ);
                    }
                } else { // Linha comum com 3 ou 4 componentes
                    
                    linhaObj->set("", "", "", "");
                    
                    int contador = 0;
                    for(it=tokens.begin(); it != tokens.end(); it++) {
                        string word = (string)*it;
                        if(word[0] == ';') { // Ignorando os comentários
                            break;
                        }
                        if(contador == 0) {
                            linhaObj->setRotulo(*it);
                        } else if (contador == 1) {
                            linhaObj->setComando(*it);
                        } else if (contador == 2) {
                            // Atribui o argumento ao operador1
                            linhaObj->setOperador1(*it);
                            for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                                // Se o argumento for EQU, redefine
                                if(it_equ->nome == *it) {
                                    linhaObj->setOperador1(it_equ->valor);
                                }
                            }
                        } else if (contador == 3) {
                            // Atribui o argumento ao operador2
                            linhaObj->setOperador2(*it);
                            for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                                // Se o argumento for EQU, redefine
                                if(it_equ->nome == *it) {
                                    linhaObj->setOperador2(it_equ->valor);
                                }
                            }
                        }
                        
                        contador++;
                    }         
                }
                linhas.push_back(*linhaObj);
            }
        }
        // Salvando no Arquivo
        ofstream saida(nome_arquivo+".pre"); // ofstream -> registro de arquivo, sai do programa e salva em arquivo
        for(it_linha=linhas.begin(); it_linha != linhas.end(); it_linha++) {
            Linha linha_refatorada = (Linha)*it_linha;
            if(it_linha->rotulo != "") {
                saida<<it_linha->rotulo<<" ";
            }
            if(it_linha->comando != "") {
                saida<<it_linha->comando<<" ";
            }
            if(it_linha->operador1 != "") {
                saida<<it_linha->operador1<<" ";
            }
            if(it_linha->operador2 != "") {
                saida<<it_linha->operador2<<" ";
            }
            if(it_linha->rotulo != "" || it_linha->comando != "" || it_linha->operador1 != "" || it_linha->operador2 != "") {
                // Se tiver pelo menos um elemento, printa a quebra de linha
                saida<<endl; 
            }
        }
        entrada.close();
        saida.close();
    } else {
        cout << "Não foi possível abrir o arquivo: "<< file <<endl; 
    }
}