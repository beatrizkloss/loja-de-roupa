#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX 100

// estrutura do item
typedef struct {
    int codigo;
    char nome[50];
    char marca[50];
    int quantidade;
    float preco;
} Item;

// estrutura da venda
typedef struct {
    Item item;
    float valorVenda;
    int ordem;
} Venda;

// estrutura do item vendido
typedef struct {
    char nome[50];
    int quantidade;
} ItemVendido;

// variáveis globais
Venda venda1[MAX];
ItemVendido itensVendidosPorNome[MAX];
int totalVendas = 0;
int totalItensVendidos = 0;

// código para calcular o valor da venda com o desconto de 10%
float calcularValorVenda(Item item) {
    float valor = item.quantidade * item.preco;
    if (item.quantidade >= 3) {
        valor *= 0.9; 
    }
    return valor;
}

// código para registrar uma venda
void registrarVenda() {
    if (totalVendas == MAX) {
        printf("Não é possível registrar mais vendas.\n");
        system("PAUSE");
        return;
    }

    Venda venda2;
    printf("Digite o código do item: ");
    scanf("%d", &venda2.item.codigo);
    getchar();  
    printf("Digite o nome do item: ");
    fgets(venda2.item.nome, 50, stdin);
    venda2.item.nome[strcspn(venda2.item.nome, "\n")] = 0;  
    printf("Digite a marca do item: ");
    fgets(venda2.item.marca, 50, stdin);
    venda2.item.marca[strcspn(venda2.item.marca, "\n")] = 0;  
    printf("Digite a quantidade de itens: ");
    scanf("%d", &venda2.item.quantidade);
    getchar();  
    printf("Digite o preço unitário do item: ");
    scanf("%f", &venda2.item.preco);
    getchar();  
	
	// Calcula o valor da venda
    venda2.valorVenda = calcularValorVenda(venda2.item);
    venda2.ordem = totalVendas;  
	
	// Atualiza a quantidade de itens vendidos por nome
    int i;
    for (i = 0; i < totalItensVendidos; i++) {
        if (strcmp(itensVendidosPorNome[i].nome, venda2.item.nome) == 0) {
            itensVendidosPorNome[i].quantidade += venda2.item.quantidade;
            break;
        }
    }
    if (i == totalItensVendidos) { 
        strcpy(itensVendidosPorNome[i].nome, venda2.item.nome);
        itensVendidosPorNome[i].quantidade = venda2.item.quantidade;
        totalItensVendidos++;
    }
 	
 	// Adiciona a venda ao registro
    venda1[totalVendas++] = venda2;

    // gravar os dados da venda no arquivo binário
    FILE *file = fopen("loja_roupa.dat", "ab");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        system("PAUSE");
        return;
    }

    fwrite(&venda2, sizeof(Venda), 1, file);
    fclose(file);
    printf("VENDA REGISTRADA COM SUCESSO!\n");
    system("PAUSE");
}

// código para ordenar as vendas em ordem decrescente 
void ordenarVendas() {
    for (int i = 0; i < totalVendas - 1; i++) {
        for (int j = 0; j < totalVendas - i - 1; j++) {
        	// Ordena com base no valor da venda e na ordem de registro
            if (venda1[j].valorVenda < venda1[j + 1].valorVenda ||
                (venda1[j].valorVenda == venda1[j + 1].valorVenda && venda1[j].ordem > venda1[j + 1].ordem)) {
                Venda temp = venda1[j];
                venda1[j] = venda1[j + 1];
                venda1[j + 1] = temp;
            }
        }
    }
}

// carregar os dados da vendas do arquivo
void carregarVendas() {
    FILE *file = fopen("loja_roupa.dat", "rb");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }

    Venda venda2;
    while (fread(&venda2, sizeof(Venda), 1, file)) {
        venda1[totalVendas++] = venda2;

		 // Atualiza a quantidade de itens vendidos por nome
        int i;
        for (i = 0; i < totalItensVendidos; i++) {
            if (strcmp(itensVendidosPorNome[i].nome, venda2.item.nome) == 0) {
                itensVendidosPorNome[i].quantidade += venda2.item.quantidade;
                break;
            }
        }
        if (i == totalItensVendidos) { 
            strcpy(itensVendidosPorNome[i].nome, venda2.item.nome);
            itensVendidosPorNome[i].quantidade = venda2.item.quantidade;
            totalItensVendidos++;
        }
    }

    fclose(file);
}

// código para gerar relatórios
void gerarRelatorios() {
    if (totalVendas == 0) {
        printf("Nenhuma venda foi registrada ainda.\n");
        system("PAUSE");
        system("CLS");
        return;
    }
    
    // calcula algumas métricas sobre as vendas
    int totalItens = 0;
    float faturamentoBruto = 0.0;
    int totalClientes = totalVendas;

    for(int i = 0; i < totalVendas; i++) {
        totalItens += venda1[i].item.quantidade;
        faturamentoBruto += venda1[i].valorVenda;
    }
    
    // encontra o item mais e menos vendido
	char itemMaisVendido[50];
    char itemMenosVendido[50];
    int maxQuantidade = 0;
    int minQuantidade = MAX;
    for (int i = 0; i < totalItensVendidos; i++) {
        if (itensVendidosPorNome[i].quantidade > maxQuantidade) {
            maxQuantidade = itensVendidosPorNome[i].quantidade;
            strcpy(itemMaisVendido, itensVendidosPorNome[i].nome);
        }
        if (itensVendidosPorNome[i].quantidade < minQuantidade) {
            minQuantidade = itensVendidosPorNome[i].quantidade;
            strcpy(itemMenosVendido, itensVendidosPorNome[i].nome);
        }
    }
    // Exibe as métricas e as informações de cada venda

    printf("*-----------------------------------------------------------------*\n");
    printf("|        *   *   *   *    MENU DE REGISTRO   *   *   *   *        |\n");
    printf("*-----------------------------------------------------------------*\n");
    printf("|   Quantidade total de itens vendidos no dia: %d                 \n", totalItens);
    printf("|   Faturamento bruto diário sob as vendas: %.2f                \n", faturamentoBruto);
    printf("|   Quantidade de clientes que realizaram compras: %d \n", totalClientes);
    printf("|   Item mais vendido: %s                                         \n", itemMaisVendido);
    printf("|   Item menos vendido: %s                                        \n", itemMenosVendido);
    printf("*\n");
    
    ordenarVendas();
    
    printf("\n| * * *  INFORMAÇÕES DE CADA VENDA * * * |\n");
    printf("*----------------------------------------*\n");
    for(int i = 0; i < totalVendas; i++) {
        printf("*--------------------------*\n");
        printf("|   Venda: %d              \n", i+1);
        printf("|   Código do item: %d     \n", venda1[i].item.codigo);
        printf("|   Nome do item: %s       \n", venda1[i].item.nome);
        printf("|   Marca do item: %s      \n", venda1[i].item.marca);
        printf("|   Quantidade vendida: %d \n", venda1[i].item.quantidade);
        printf("|   Preço unitário: %.2f   \n", venda1[i].item.preco);
        printf("|   Valor da venda: %.2f    \n\n", venda1[i].valorVenda);
        printf("*\n");
    }

    system("PAUSE");  
    system("CLS");
}

// função principal
int main(void) {
    
    system("CLS");
    
    setlocale(LC_ALL, "Portuguese");
    carregarVendas(); 
    int opcao;
    do {
        system("CLS");
        printf("+----------------------+\n");
        printf("|   BEM VINDO À LOJA   |\n");
        printf("+----------------------+\n");
        printf("|  [1] Registrar venda |\n");
        printf("|  [2] Gerar relatórios|\n");
        printf("|  [3] Finalizar       |\n");
        printf("+----------------------+\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        while ((getchar()) != '\n'); 

        if (opcao == 1) {
            registrarVenda();
        } else if (opcao == 2) {
            gerarRelatorios();
        } else if (opcao == 3) {
            printf("Finalizando o programa...\n");
        } else {
            printf("Opção inválida!\n");
        }
    } while (opcao != 3);
    system("PAUSE");
    return 0;
}

