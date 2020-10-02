#include "../include/classes.hpp"

// ofstream -> registro de arquivo, sai do programa e salva em arquivo
// ifstream -> leitura de arquivo

// Waliff Cordeiro Bandeira - 17/0115810

using namespace std;

void pre_processamento(string file);

int main(int argc, char **argv) {

    string argumento;

    //linhaObj->imprimir();
    // saida.open("nome.txt", ios::app); //ios::app para não sobrescrever
    // saida << "inserção de texto";
    // saida.close();

    // argv[1] contém a flag -p ou -o (verificar na hora de gerar a saída)
    argumento = (string)argv[1];

    if(argc == 3) {
        if(argumento == "-p") {
            pre_processamento(argv[2]);
        } else if(argumento == "-o"){
            cout<<"-o";
        }
        else {
            cout<<"pre-processamento + montar";
        }
    }
    
    return 0;
}

void pre_processamento(string file) {
    
    vector<string> tokens;
    vector<string>::iterator it;

    vector<Linha> linhas;
    vector<Linha>::iterator it_linha;

    vector<EQU> EQUs;
    vector<EQU>::iterator it_equ;

    // Variáveis de leitura
    ifstream entrada;
    string linha, token;

    entrada.open(file);
    if(entrada.is_open()) {
        // Pegando linha a linha
        while(getline(entrada, linha)) {
            // Separa todos os tokens em um vector retirando espaços e tabulações
            tokens = split(linha, ' ', '\t');

            if(tokens.size() == 1) { // Só um token.É comando, label ou IF 
                if(tokens[0].back() == ':') { // Se o último caracter é : então é label ou EQU    
                    
                    string label = tokens[0]; // Salva a label

                    // Pegamos a linha seguinte
                    getline(entrada, linha);
                    tokens = split(linha, ' ', '\t');

                    // Verificar se é EQU
                    string isEQU;
                    isEQU = toUpperCase(tokens[0]);

                    if(isEQU == "EQU") { // Criar objeto EQU
                        label.pop_back(); // Retirando o : para salvar o nome do EQU
                        EQU *equ = new EQU(label, "");
                        if(equ->EQU_unico(EQUs)) {
                            if (tokens.size() == 2) {
                                // Garantindo que tem os 2 tokens (EQU e valor)
                                equ->setValor(tokens[1]);
                                EQUs.push_back(*equ);
                            }
                        }
                    }
                    else { // Se não for EQU, criar objeto Linha
                        Linha *linhaObj = new Linha(label, "", "", "");

                        int contador = 0;
                        for(it=tokens.begin(); it != tokens.end(); it++) { // Iterando a linha complementar à label
                            string word = (string)*it;
                            if(word[0] != ';') {
                                if(contador == 0) {
                                    // O primeiro elemendo da linha seguinte à label é comando
                                    linhaObj->setComando(*it);
                                } else if (contador == 1) {
                                    // O segundo elemendo da linha seguinte à label é o primeiro operador
                                    linhaObj->setOperador1(*it);
                                } else if (contador == 2) {
                                    // Se tiver 3 argumentos
                                    // O terceiro elemendo da linha seguinte à label é o segundo operador
                                    linhaObj->setOperador2(*it);
                                }
                            }
                            contador++;
                    }
                    linhas.push_back(*linhaObj);                
                    }                
                }
                else { // Não é label nem EQU, então é comando sem argumento
                    Linha *linhaObj = new Linha("", tokens[0], "", "");
                    linhas.push_back(*linhaObj);
                }
            }
            else { // Linha comum 
                Linha *linhaObj = new Linha("", "", "", "");
                string isElemento, isEQU;
                isEQU = toUpperCase(tokens[1]);
                isElemento = toUpperCase(tokens[0]);
                if (tokens.size() == 2) { // Section
                    if(isElemento == "SECTION") {
                        string section = tokens[0]+" "+tokens[1];
                        linhaObj->set("", section, "", "");
                    } else if (isElemento == "IF") {
                        for(it_equ=EQUs.begin(); it_equ != EQUs.end(); it_equ++) {
                            // Verificando "chave/valor" do EQU
                            if(it_equ->nome == tokens[1] && it_equ->valor == "1") {
                                // Se o valor do IF é true, pega a próxima linha
                                break;
                            }
                            else {
                                // Senão despreza a próxima linha
                                getline(entrada, linha);
                                break;
                            }
                        }
                        continue;
                    } else { // Se não for section, contém comando e um operador
                        linhaObj->set("", tokens[0], tokens[1], "");
                    }
                } else if (tokens[1] == "EQU") { // Salvando EQU na mesma linha que a label
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
                        if(word[0] != ';') { // Ignorando os comentários
                            if(contador == 0) {
                                linhaObj->setRotulo(*it);
                            } else if (contador == 1) {
                                linhaObj->setComando(*it);
                            } else if (contador == 2) {
                                linhaObj->setOperador1(*it);
                            } else if (contador == 3) {
                                linhaObj->setOperador2(*it);
                            }
                        }
                        contador++;
                    }         
                }
                linhas.push_back(*linhaObj);
            }
        }
        // Salvando no Arquivo
        ofstream saida;
        saida.open("montador.PRE");
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
        cout << "Não foi possível abrir o arquivo"<<endl;   
    }
}

// Algoritmo da segunda passagem:
// Contador_posição = 0
// Contador_linha = 1
// Enquanto arquivo fonte não chegou ao fim, faça:
// Obtém uma linha do fonte
// Separa os elementos da linha: rótulo,operação,operandos,comentários
// Ignora o rótulo e os comentários
// Para cada operando que é símbolo
// Procura operando na TS
// Se não achou: Erro, símbolo indefinido
// Procura operação na tabela de instruções
// Se achou:
// contador_posição = contador_posição + tamanho da instrução
// Se número e tipo dos operandos está correto então
// gera código objeto conforme formato da instrução
// Senão: Erro, operando inválido
// Senão:
// Procura operação na tabela de diretivas
// Se achou:
// Chama subrotina que executa a diretiva
// Contador_posição = valor retornado pela subrotina
// Senão: Erro, operação não identificada
// Contador_linha = contador_linha + 1