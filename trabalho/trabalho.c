#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Função para configurar a comunicação serial
HANDLE configurarSerial(const char *porta) {
    HANDLE hSerial;

    // Abre a porta serial
    hSerial = CreateFile(porta, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        // Verifica se houve erro ao abrir a porta serial
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("Porta serial nao encontrada.\n");
        }
        printf("Erro ao tentar abrir a porta serial.\n");
        return INVALID_HANDLE_VALUE;
    }

    // Configura os parâmetros da porta serial
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        // Verifica se houve erro ao obter os parâmetros da porta serial
        printf("Erro ao obter o estado da porta.\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Configuração dos parâmetros da porta serial
    dcbSerialParams.BaudRate = CBR_115200; // Velocidade em baud rate
    dcbSerialParams.ByteSize = 8; // Tamanho do byte
    dcbSerialParams.StopBits = ONESTOPBIT; // Um bit de stop
    dcbSerialParams.Parity = NOPARITY; // Sem paridade

    // Define os parâmetros na porta serial
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        // Verifica se houve erro ao definir os parâmetros da porta serial
        printf("Erro ao definir o estado da porta.\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Configura os timeouts da comunicação serial
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    // Define os timeouts na porta serial
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        // Verifica se houve erro ao definir os timeouts
        printf("Erro ao definir os timeouts.\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial; // Retorna o identificador da porta serial configurada
}

// Função para enviar comandos para o ESP32
void enviarComando(HANDLE hSerial, const char *comando) {
    DWORD bytes_written;

    // Escreve o comando na porta serial
    WriteFile(hSerial, comando, strlen(comando), &bytes_written, NULL);
    WriteFile(hSerial, "\n", 1, &bytes_written, NULL);  // Envia o caractere de nova linha
}

int main() {
    const char *porta = "COM4"; // Porta serial do ESP32, ajuste conforme necessário
    HANDLE hSerial = configurarSerial(porta); // Configura a porta serial

    if (hSerial == INVALID_HANDLE_VALUE) {
        return 1; // Encerra o programa se houver erro na configuração da porta serial
    }

    char opcao[10]; // Buffer para armazenar a opção do usuário

    while (1) {
        // Exibe o menu para o usuário
        printf("Menu:\n");
        printf("1 - Ligar LED\n");
        printf("2 - Desligar LED\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%s", opcao); // Lê a opção do usuário

        if (strcmp(opcao, "1") == 0) {
            enviarComando(hSerial, "on"); // Envia comando para ligar o LED
            printf("LED ligado.\n");
        } else if (strcmp(opcao, "2") == 0) {
            enviarComando(hSerial, "off"); // Envia comando para desligar o LED
            printf("LED desligado.\n");
        } else if (strcmp(opcao, "0") == 0) {
            CloseHandle(hSerial); // Fecha a porta serial
            printf("Programa encerrado.\n");
            return 0; // Encerra o programa
        } else {
            printf("Opcao invalida. Escolha novamente.\n"); // Opção inválida
        }

        Sleep(100); // Aguarda um pouco para garantir que o comando foi enviado
    }
}
