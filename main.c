#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Autor: Marcelo Robert Santos
CIC250 - Cálculo numérico para computação
Professor: Carlos Minoru Tamaki
Última modificação: 05/07/2021
*/

/*
recebe os valores iniciais de x e f(x)
encontrar os Ls
encontrar g
*/

int main(void){
	int n, i, j, k;
	double *x, *fx, **L, *g, xr, fxr;
	//a matriz L guarda em cada linha a equação L_i, sendo cada coluna j o expoente n-j, o elemento L[i][j] guarda o coeficiente de x^(n-j) em L_i. Nesse sentido, é possível separar os coeficientes por linha e por expoente.

	//recebe os valores iniciais
	printf("Dê o valor de n (a quantidade de pontos de amostra):\n");
	scanf("%d", &n);

	//aloca as variáveis
	x = (double*) malloc(n*sizeof(double)); //valores iniciais
	fx = (double*) malloc(n*sizeof(double)); //valores iniciais
	g = (double*) calloc(n, sizeof(double)); //valores finais
	L = (double**) calloc(n, sizeof(double*)); //matriz dos coeficientes
	for(i=0; i<n; i++){
		L[i] = (double*) calloc(n, sizeof(double));
	}

	//recebe os valores
	printf("\nDê os valores iniciais de x e f(x), modelo:\nx1 f(x1)\nx2 f(x2)\n...\n\n");
	for(i=0; i<n; i++){
		scanf("%lf", &x[i]);
		scanf("%lf", &fx[i]);
	}

	printf("\nDê o valor de x para se calcular f(x) ao final do programa:\n");
	scanf("%lf", &xr);

	//resolve com lagrange
	//"Note que não configura no numerador o termo (x – xk) e no denominador o termo (xk – xk)."
	
	//IMPORTANTE:
	//desmembrando os termos da multiplicação, podemos refazê-la par a par, sendo que nesse caso, cada coeficiente a_i de x^i é multiplicado pelo próximo termo na sequência e somado ao coeficiente de x^{i-1}, isso para todo termo. É possível fazer tudo de uma vez só, deixando a divisão depois de se calcular o numerador, bastando dividir no final todos os termos pelo denominador

	/*imagine (x + x0)(x + x1)(x + x2) ... (x + xn-2)(x + xn-1)(x + xn)
	Vamos observar cada par de parênteses e agrupá-los em um único parênteses, a ideia é fazer (a)*(b)*(c) passar para ((a)*(b))(c) com todos os pares.
	Com os termos x + xi teremos que os pares serão agrupados da seguinte maneira:
	(x + x0) * (x + x1) =
	(x^2 + x*(x1 + x0) + x0x1)
	seja (x1 + x0) = p e x0x1 = q,
	(x^2 + x*p + q).
	(x^2 + x*p + q) * (x + x2) =
	(x^3 + x^2(x2 + p) + x(px2 + q) + q*x2)
	Aqui já é possível ver o que acontece, cada coeficiente de x^i é multiplicado pelo novo termo e somado com o próximo, incluindo, por exemplo, o coeficiente de x^3, pois antes era 0 e agora será 0*x2 + 1 = 1. É essa a ideia
	*/

	//essa operação de "multiplica pelo próximo termo e soma com o coeficiente seguinte" é feita (n-1) vezes para cada L, pois o termo x_k é ignorado
	double denominador;

	for(i=0; i<n; i++){//estamos lidando com "termos" e não "valores", como precisamos ir somando os termos, mas a fórmula diz que precisamos subtrair os valores, então basta mutiplicar todos os valores por -1 e somá-los depois
		if(x[i] != 0) //evita problemas de -0
			x[i] *= -1;
	}
	
	for(i=0; i<n; i++){ //inicializa os valores para x^0, porque senão a matriz ficaria zerada sempre
		L[i][n-1] = 1;
	}
	for(i=0; i<n; i++){ //calcula L_i
		denominador = 1;
		for(j=0; j<n; j++){ //serão feitas n operações tirando quando i == j
			if(j != i){
				for(k=0; k<n; k++){ //para cada operação, todos os valores são alterados
					if(k != n-1){ //o último valor não soma à nada, o if aqui é para evitar a soma com lixo de memória
						L[i][k] = L[i][k] * x[j] + L[i][k+1];
					}
					else {
						L[i][k] = L[i][k] * x[j];
					}
				}
				denominador *= ((-1)*(x[i]-x[j])); //calcula-se o denominador, como se subtrai só o segundo termo, é necessário voltar para o formato original para subtrair, (-1) * (x[i] - x[j]) == -x[i] + x[j], como os x estão trocados de sinal, isso é o mesmo que x[i] - x[j]
			}
		}
		//depois de calcular os valores, dividi-los pelo denominador
		for(j=0; j<n; j++){
			L[i][j] /= denominador;
		}
	}

	//daqui sai a matriz L, basta então somar os termos f(x)*L para obter o final
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			g[j] += fx[i]*L[i][j];
		}
	}

	//opcionalmente, pode-se voltar o vetor x para seus valores originais
	for(i=0; i<n; i++){
		if(x[i] != 0)
			x[i] *= (-1);
	}

	//imprime o resultado
	printf("\n--------------------\nResultado da função:\n");
	for(i=0; i<n; i++){
		if(i==n-1){
			if(g[i] >= 0)
				printf("+"); //para deixar mais fáicl de entender
			printf("%lf ", g[i]);
		} else {
			if(g[i] >= 0)
				printf("+");
			printf("%lfx^%d ", g[i], (n-i-1));
		}
	}

	printf("\n\n");
	for(i=0; i<n; i++){
		printf("L_%d: ", i);
		for(j=0; j<n; j++){
			printf("%lf ", L[i][j]);
		}
		printf("\n");
	}

	printf("\n\nResultado para x = %lf,\n", xr);
	fxr = 0;
	for(i=0; i<n; i++){
		fxr += g[i]*pow(xr, (n-i-1));
	}
	printf("%lf\n", fxr);

}