#include <stdio.h>
#include <stdlib.h>

//declarando as funções usadas
int MDC(int a, int b);
int testarPrimo(int a);
int inverso(int a, int b);
int euler(int a);
long long exponencialModular (long long a, long long b, long long c);

int main(){

    //variaveis usadas
    int H, G, n, x, n1;
    int a = 0, P = 0, x1 = 0, q = 0, r = 0;

    //ler as variaveis do primeiro int
    printf("~~~ VERIFICAÇÃO MODULAR ~~~\n");
    printf("Iremos calcular: a^x mod n1\n");
    printf("Por favor insira os seguintes valores em ordem:\n");
    printf("H, G, n, x, n1\n");
    scanf("%d%d%d%d%d", &H, &G, &n, &x, &n1);
    
    //usar algoritmo de euclides no G e n para ver se sao primos
    //se nao forem, fale que nao e possivel
    printf("\nVerificando se G (%d) e n (%d) são coprimos...\n", G, n);
    printf("(Números coprimos são aqueles que possuem 1 como seu Máximo Divisor Comum!)\n");

    P = MDC(G, n);

    if (P != 1){
        printf("%d e %d não são coprimos entre si, o cálculo não e possível. Encerrando...\n", G, n);
        return 1;
    }

    printf("%d e %d são coprimos entre si, o cálculo é possivel. Continuando...\n\n", G, n);

    P = 0;

    //inverso de G em Zn
    //se nao tiver inverso em Zn, verificar equivalencia(?)
    printf("Calculando o inverso modular de G (%d)...\n", G);

    int inv = inverso(G, n);

    if(inv == -1){
        printf("O inverso modular de %d não existe em Z%d. Encerrando...\n", G, n);
        return 1;
    }

    printf("O inverso de %d em Z%d é %d.\n\n", G, n, inv);

    //divisao de H por G (mult H por inv) em Zn para achar a
    printf("Multiplicando H (%d) pelo inverso (%d)...\n", H, inv);
    printf("(Isso é o equivalente a dividir H por G em Zn!)");

    a = (H * inv) % n;

    printf("Resultado: a = %d\n\n", a);

    //MDC de a e n1 para ver se sao coprimos
    printf("Verificando se a (%d) e n1 (%d) são coprimos...\n", a, n1);

    P = MDC(a, n1);

    if (P != 1){
        printf("%d e %d não são coprimos entre si, o cálculo não e possível. Encerrando...\n", a, n1);
        return 1;
    }

    printf("%d e %d são coprimos entre si, o cálculo é possivel. Continuando...\n\n", a, n1);

    //ver se n1 e primo
    printf("Verificando se n1 (%d) é primo...\n", n1);
    int primo = testarPrimo(n1);

    //se n1 for primo, usar Fermat para definir x1 = n-1
    //se nao, usar Euler para definir x1 = φ(n)
    //nao usar bibliotecas prontas, usar apenas os fatores primos de n1
    if(primo == 0){
        printf("É primo! Asim, poderemos usar o Pequeno Teorema de Fermat para o cálculo:\n");
        x1 = n1 - 1;
        printf("x1 = n1 (%d) - 1 = %d\n\n", n1, x1);
    } else {
        printf("Não é primo! Assim, teremos que usar o Teorema de Euler para o cálculo.\n");
        printf("Para isso, temos que calcular φ(n1) (Totiente de Euler) usando os fatores primos de n1.\n");
        x1 = euler(n1);
        printf("x1 = φ(%d) = %d\n\n", n1, x1);
    }

    //usar o teorema da divisao para decompor o exponente x como
    // x = x1 * q + r
    printf("Decompondo a exponencial x, temos:\n");
    q = x / x1;
    r = x % x1;
    printf("%d = %d * %d + %d\n", x, x1, q, r);

    //reescrever a^x mod n1 como a^x ≡ (((a^n1)^q mod n1) * (a^r mod n1)) mod n1
    printf("Levando ao a^x mod n1, podemos reescrever como:\n");
    printf("%d^%d mod %d = (((%d^%d)^%d mod %d) * (%d^%d mod %d)) mod %d\n\n", a, x, n1, a, n1, q, n1, a, r, n1, n1);

    //calcule:
    //a^x1 mod n1 = x2
    //x2^q mod n1
    //a^r mod n1
    printf("Agora temos que calcular as potências modulares intermediárias:\n");
    long long x2 = exponencialModular(a, x1, n1);
    long long e1 = exponencialModular(x2, q, n1);
    long long e2 = exponencialModular(a, r, n1);
    printf("a^x1 mod n1 = %d^%d mod %d = %lld = x2\n", a, x1, n1, x2);
    printf("x2^q mod n1 = %lld^%d mod %d = %lld\n", x2, q, n1, e1);
    printf("a^r mod n1 = %d^%d mod %d = %lld\n\n", a, r, n1, e2);

    //combinar todos os resultados e imprimir o valor final da congruencia:
    // ((x2^q) * (a^r)) mod n1
    printf("Por fim, calculamos ((x2^q) * (a^r)) mod n1, e assim chegamos ao nosso resultado final:\n");
    long long resultadoFinal = (e1 * e2) % n1;
    printf("O resultado final é: %lld!\n", resultadoFinal);
    
    return 0;
}

int MDC(int a, int b){
    int r;

    while((a % b) > 0){
        r = a % b;
        a = b;
        b = r;
    }

    return b;
}

int testarPrimo(int a){
    int divisores = 0;

    for(int i = 2; i < a; i++){
        if(a % i == 0){
            divisores++;
            break;
        }
    }

    return divisores;
}

int inverso(int a, int b){
    if(MDC(a, b) > 1) return -1;
    
    for(int i = 0; i < b; i++){
        if(((a % b) * (i % b)) % b == 1){
            return i;
        }
    }
}

int euler(int a){
    int resultado = a;

    for (int i = 2; i * i <= a; i++){
        if(a % i == 0){
            while(a % i == 0) a /= i;
            resultado -= resultado / i;
        }
    }

    if(a > 1) resultado -= resultado / a;

    return resultado;
}

long long exponencialModular (long long a, long long b, long long c){
    long long resultado = 1;

    a = a % c;

    while(b > 0){
        if(b % 2 == 1){
            resultado = (resultado * a) % c;
        }
        a = (a * a) % c;
        b = b / 2;
    }

    return resultado;
}
