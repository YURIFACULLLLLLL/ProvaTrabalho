 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numero;
    char titular[50];
    float saldo;
} Conta;

int buscarConta(int numero) {
    FILE *f = fopen("contas.dat", "rb");
    Conta c;
    if (!f) return 0;

    while (fread(&c, sizeof(Conta), 1, f)) {
        if (c.numero == numero) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

Conta lerConta(int numero) {
    FILE *f = fopen("contas.dat", "rb");
    Conta c;

    while (fread(&c, sizeof(Conta), 1, f)) {
        if (c.numero == numero) {
            fclose(f);
            return c;
        }
    }

    fclose(f);
    c.numero = -1;
    return c;
}

void salvarConta(Conta c) {
    FILE *f = fopen("contas.dat", "rb+");
    Conta temp;

    while (fread(&temp, sizeof(Conta), 1, f)) {
        if (temp.numero == c.numero) {
            fseek(f, -sizeof(Conta), SEEK_CUR);
            fwrite(&c, sizeof(Conta), 1, f);
            break;
        }
    }

    fclose(f);
}

void criarConta() {
    FILE *f = fopen("contas.dat", "ab+");
    Conta nova;

    printf("Nome do titular: ");
    fgets(nova.titular, 50, stdin);
    nova.titular[strcspn(nova.titular, "\n")] = 0;

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    nova.numero = tamanho / sizeof(Conta) + 1;
    nova.saldo = 0;

    fwrite(&nova, sizeof(Conta), 1, f);
    fclose(f);

    printf("Conta criada! Número: %d\n", nova.numero);
}

void extrato() {
    int num;
    printf("Número da conta: ");
    scanf("%d", &num);

    if (!buscarConta(num)) {
        printf("Conta não encontrada!\n");
        return;
    }

    Conta c = lerConta(num);

    printf("\nTitular: %s\n", c.titular);
    printf("Conta: %d\n", c.numero);
    printf("Saldo: R$ %.2f\n", c.saldo);
}

void deposito() {
    int num;
    float v;

    printf("Número da conta: ");
    scanf("%d", &num);

    if (!buscarConta(num)) {
        printf("Conta não encontrada!\n");
        return;
    }

    printf("Valor: ");
    scanf("%f", &v);

    if (v <= 0) {
        printf("Valor inválido!\n");
        return;
    }

    Conta c = lerConta(num);
    c.saldo += v;
    salvarConta(c);

    printf("Depósito realizado!\n");
}

void saque() {
    int num;
    float v;

    printf("Número da conta: ");
    scanf("%d", &num);

    if (!buscarConta(num)) {
        printf("Conta não encontrada!\n");
        return;
    }

    printf("Valor: ");
    scanf("%f", &v);

    Conta c = lerConta(num);

    if (v <= 0 || v > c.saldo) {
        printf("Valor inválido ou saldo insuficiente!\n");
        return;
    }

    c.saldo -= v;
    salvarConta(c);

    printf("Saque realizado!\n");
}

void transferencia() {
    int orig, dest;
    float v;

    printf("Conta origem: ");
    scanf("%d", &orig);
    printf("Conta destino: ");
    scanf("%d", &dest);
    printf("Valor: ");
    scanf("%f", &v);

    if (!buscarConta(orig) || !buscarConta(dest)) {
        printf("Conta não encontrada!\n");
        return;
    }

    Conta c1 = lerConta(orig);
    Conta c2 = lerConta(dest);

    if (v <= 0 || v > c1.saldo) {
        printf("Valor inválido ou saldo insuficiente!\n");
        return;
    }

    c1.saldo -= v;
    c2.saldo += v;

    salvarConta(c1);
    salvarConta(c2);

    printf("Transferência concluída!\n");
}

void listarContas() {
    char senha[20];
    printf("Senha admin: ");
    scanf("%s", senha);

    if (strcmp(senha, "1234") != 0) {
        printf("Senha incorreta!\n");
        return;
    }

    FILE *f = fopen("contas.dat", "rb");
    if (!f) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    Conta c;
    printf("\n--- CONTAS ---\n");

    while (fread(&c, sizeof(Conta), 1, f)) {
        printf("Conta %d | Titular: %s | Saldo: R$ %.2f\n",
               c.numero, c.titular, c.saldo);
    }

    fclose(f);
}

int main() {
    int opc;

    do {
        printf("\n1. Criar Conta\n");
        printf("2. Extrato\n");
        printf("3. Depósito\n");
        printf("4. Saque\n");
        printf("5. Transferência\n");
        printf("6. Listar Contas\n");
        printf("7. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1: criarConta(); break;
            case 2: extrato(); break;
            case 3: deposito(); break;
            case 4: saque(); break;
            case 5: transferencia(); break;
            case 6: listarContas(); break;
            case 7: break;
            default: printf("Inválido!\n");
        }

    } while (opc != 7);

    return 0;
}
