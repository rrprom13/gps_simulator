#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct node {
	int *kid, nr;
	double *lg;
} node;

void print(int *v, int nr) //functia de afisare a rutei de parcurgere spre destinatie
{
	int i;
	for (i = nr - 1 ; i >= 0 ; i--)
		if (i == 0)
			printf("%d\n",v[i]);
		else
			printf("%d ",v[i]);
}

void way(int dest, int **path, int N, int *v, int *nr) //retinerea path-ului spre destinatie
{
	if (dest == 0)
		return;
	if (dest != N-1) {
		if (*nr > 0)
			v = realloc (v,(*nr + 1) * sizeof(int));
		v[(*nr)++] = dest - 1;
	}
	int i;
	for (i = 1 ; i <= path[dest][0] ; i++)
		way(path[dest][i],path,N,v,nr);
}

int mindist(int *visit, double *dist, int N) //returneaza nodul vecin cu distanta minima
{
	int i, position;
	double min = INT_MAX;
	for (i = 0 ; i < N ; i++)
		if (visit[i] == 0 && dist[i] <= min) {
			min = dist[i];
			position = i;
		}
	return position;
}

double distance(node **root, int x, int y, int *ok) //distanta dintre x si y
{
	int i;
	double val = 0;
	for (i = 0 ; i < root[x]->nr ; i++)
		if (root[x]->kid[i] == y) {
			val = root[x]->lg[i];
			*ok = 1;
			break;
		} 
	return val;
}

//functia de construire a vectorului de distante fata de origine, plecand din aceasta
void dijkstra(node **root, int *visit, double *dist, int **path, int *v, int N, int *nr, int node)
{
	dist[node] = 0;
	int i, j, min, ok;
	double x;
	for (i = 0 ; i < N ; i++) {
		min = mindist(visit,dist,N);
		visit[min] = 1;
		for (j = 0 ; j < N ; j++)
			if (visit[j] == 0) {
				ok = 0;
				x = distance(root,min,j,&ok);
				if (ok == 1 && dist[min] != INT_MAX) 
					if (dist[min] + x < dist[j]) {
						dist[j] = dist[min] + x;
						path[j][0] = 1;
						path[j][1] = min;//variabila ce retine nodul predecesor nodului curent
					}
					else 
						if (dist[min] + x == dist[j]) {
							path[j][0]++;
							path[j][path[j][0]] = min;
						}
			}
	}
	way(N-1,path,N,v,nr);
}

int readtwo(node **root, int N, int *origins, int *destination, int *number_orig, int *number_dest, int *ultimate, int task)
{
	//origins retine nodul precedent originii
	//destination retine nodul precedent destinatiei
	//number_orig retine a cata legatura din nodul precedent originii duce spre aceasta
	//number_dest retine a cata legatura din nodul precedent destinatiei duce spre aceasta
	int i, j, orig_plecare, orig_sosire, dest_plecare, dest_sosire;
	double orig_distanta,dest_distanta;
	if (task == 1)
		scanf("%d %d %lf %d %d %lf",&orig_plecare,&orig_sosire,&orig_distanta,&dest_plecare,&dest_sosire,&dest_distanta);	
	else {
		scanf("%d %d %lf",&dest_plecare,&dest_sosire,&dest_distanta);
		scanf("%d %d %lf",&orig_plecare,&orig_sosire,&orig_distanta);
	}
	if (orig_plecare >= N - 2 || orig_sosire >= N - 2 || dest_plecare >= N - 2 || dest_sosire >= N - 2) {
		printf("adresa invalida\n");
		return 0;
	}
	root[N-1]->kid = (int*) malloc (sizeof(int));
	root[N-1]->lg = (double*) malloc (sizeof(double));
	root[N-1]->kid[0] = dest_sosire + 1;
	root[N-1]->nr = 1;
	root[0]->kid = (int*) malloc (sizeof(int));
	root[0]->lg = (double*) malloc (sizeof(double));
	root[0]->nr = 1;

	if (orig_plecare == dest_plecare && orig_sosire == dest_sosire) {//originea si destinatia se gasesc pe aceeasi latura
		if (orig_distanta == dest_distanta) {//originea si destinatia sunt identice
			*ultimate = 0;
			return 0;
		}			
		if (orig_distanta > dest_distanta) {//originea se afla dupa destinatie
			for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++)
				if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
					if (orig_distanta > root[orig_plecare + 1]->lg[i]) {//distanta prea mare
						printf("adresa invalida\n");
						return 0;
					}
					if (dest_distanta > root[orig_plecare + 1]->lg[i]) {//distanta prea mare
						printf("adresa invalida\n");
						return 0;
					}		
					root[orig_plecare + 1]->kid[i] = N - 1;
					root[N-1]->kid[0] = 0;
					root[0]->kid[0] = orig_sosire + 1;	
					double x = root[orig_plecare + 1]->lg[i];
					root[orig_plecare + 1]->lg[i] = dest_distanta;
					root[N-1]->lg[0] = orig_distanta - dest_distanta;
					root[0]->lg[0] = x - orig_distanta;
					*origins = N - 1;
					*destination = orig_plecare + 1;
					*number_orig = 0;
					*number_dest = i;
					return 1;
				}
		}
		if (orig_distanta < dest_distanta) {//destinatia se afla dupa origine
			for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++)
				if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
					if (orig_distanta > root[orig_plecare + 1]->lg[i]) {//distanta prea mare
						printf("adresa invalida\n");
						return 0;
					}
					if (dest_distanta > root[orig_plecare + 1]->lg[i]) {//distanta prea mare
						printf("adresa invalida\n");
						return 0;
					}		
					root[orig_plecare + 1]->kid[i] = 0;
					root[0]->kid[0] = N - 1;
					root[N-1]->kid[0] = orig_sosire + 1;	
					double x = root[orig_plecare + 1]->lg[i];
					root[orig_plecare + 1]->lg[i] = orig_distanta;
					root[0]->lg[0] = dest_distanta - orig_distanta;
					root[N-1]->lg[0] = x - dest_distanta;
					*origins = orig_plecare + 1;
					*destination = 0;
					*number_orig = i;
					*number_dest = 0;
					return 1;
				}
		}	
	}
	else {
		for (i = 0 ; i < root[dest_plecare + 1]->nr ; i++) //inseram destinatia
			if (root[dest_plecare + 1]->kid[i] == dest_sosire + 1) {
				if (dest_distanta > root[dest_plecare + 1]->lg[i]) {//distanta prea mare
					printf("adresa invalida\n");
					return 0;
				}
				root[dest_plecare + 1]->kid[i] = N - 1;
				root[N-1]->kid[0] = dest_sosire + 1;
				double x = root[dest_plecare + 1]->lg[i];
				root[dest_plecare + 1]->lg[i] = dest_distanta;
				root[N-1]->lg[0] = x - dest_distanta;
				*number_dest = i;
				*destination = dest_plecare + 1;
				break;
			}
		for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++) //inseram originea
			if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
				if (orig_distanta > root[orig_plecare + 1]->lg[i]) {//distanta prea mare
					printf("adresa invalida\n");
					return 0;
				}		
				root[orig_plecare + 1]->kid[i] = 0;
				root[0]->kid[0] = orig_sosire + 1;
				double x = root[orig_plecare + 1]->lg[i];
				root[orig_plecare + 1]->lg[i] = orig_distanta;
				root[0]->lg[0] = x - orig_distanta;
				*number_orig = i;
				*origins = orig_plecare + 1;
				return 1;
			}
	}
}

int readone(node **root, int N, int *origins, int *destination, int *number_orig, int *number_dest, int *ultimate)
{
	//origins retine nodul precedent originii
	//destination retine nodul precedent destinatiei
	//number_orig retine a cata legatura din nodul precedent originii duce spre aceasta
	//number_dest retine a cata legatura din nodul precedent destinatiei duce spre aceasta
	int i, j, orig_plecare, orig_sosire;
	double orig_dist;
	scanf("%d %d %lf",&orig_plecare,&orig_sosire,&orig_dist);
	if (orig_plecare >= N - 2 || orig_sosire >= N - 2) {
		printf("adresa invalida\n");
		return 0;
	}
	if (root[0]->kid[0] == N - 1) { //vechea origine se afla pe aceeasi linie cu destinatia, inaintea ei
		if (*origins == orig_plecare + 1) //punctele anteriore originilor coincid
			if (root[N-1]->kid[0] == orig_sosire + 1) { //originea se insereaza pe aceeasi linie ca si anterioara
				if (orig_dist > root[*origins]->lg[*number_orig] + root[0]->lg[0] + root[N-1]->lg[0]) {//distanta prea mare
					printf("adresa invalida\n");
					return 0;
				}
				if (orig_dist == root[*origins]->lg[*number_orig] + root[0]->lg[0]) {
					*ultimate = 0;
					return 0;
				}
				if (orig_dist < root[*origins]->lg[*number_orig] + root[0]->lg[0]) {
					double x = root[*origins]->lg[*number_orig];
					root[*origins]->lg[*number_orig] = orig_dist;
					root[0]->lg[0] = x + root[0]->lg[0] - orig_dist;
					return 1;
				}
				if (orig_dist > root[*origins]->lg[*number_orig] + root[0]->lg[0]) {
					root[*origins]->kid[*number_orig] = N - 1;
					root[N-1]->kid[0] = 0;
					root[0]->kid[0] = orig_sosire + 1;
					double x = root[*origins]->lg[*number_orig];
					root[*origins]->lg[*number_orig] = x + root[0]->lg[0];
					root[0]->lg[0] = root[*origins]->lg[*number_orig] + root[N-1]->lg[0] - orig_dist;
					root[N-1]->lg[0] = orig_dist - root[*origins]->lg[*number_orig];
					*origins = N - 1;
					*destination = orig_plecare + 1;
					*number_dest = *number_orig;
					*number_orig = 0;
					return 1;
				}
			}
			else { //originea se introduce pe alta linie
				root[*origins]->kid[*number_orig] = N - 1;
				root[*origins]->lg[*number_orig] = root[*origins]->lg[*number_orig] + root[0]->lg[0];
				*destination = *origins;
				*number_dest = *number_orig;
				for (i = 0 ; i < root[*origins]->nr ; i++)
					if (root[*origins]->kid[i] == orig_sosire + 1) {
						if (orig_dist > root[*origins]->lg[i]) { //distanta prea mare
							printf("adresa invalida\n");
							return 0;
						}
						root[*origins]->kid[i] = 0;
						root[0]->kid[0] = orig_sosire + 1;
						root[0]->lg[0] = root[*origins]->lg[i] - orig_dist;
						root[*origins]->lg[i] = orig_dist;
						*number_orig = i;	
						return 1;
					}
			}
		else //punctele anteriore originilor nu coincid
			for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++)
				if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
					if (orig_dist > root[orig_plecare + 1]->lg[i]) {
						printf("adresa invalida\n");
						return 0;
					}
					root[*origins]->kid[*number_orig] = N - 1;
					root[*origins]->lg[*number_orig] = root[*origins]->lg[*number_orig] + root[0]->lg[0];		
					*destination = *origins;
					*number_dest = *number_orig;
					root[orig_plecare + 1]->kid[i] = 0;
					root[0]->kid[0] = orig_sosire + 1;
					root[0]->lg[0] = root[orig_plecare + 1]->lg[i] - orig_dist;
					root[orig_plecare + 1]->lg[i] = orig_dist;
					*origins = orig_plecare + 1;
					*number_orig = i;
					return 1;			
				}
	}
	if (root[N-1]->kid[0] == 0) { //vechea origine se afla pe aceeasi linie cu destinatia, dupa ea
		if (*destination == orig_plecare + 1)
			if (root[0]->kid[0] == orig_sosire + 1) { //noua origine se insereaza pe aceeasi linie
				if (orig_dist > root[*origins]->lg[*number_orig] + root[0]->lg[0] + root[N-1]->lg[0]) {//distanta prea mare
					printf("adresa invalida\n");
					return 0;
				}
				if (orig_dist == root[*origins]->lg[*number_orig]) {
					*ultimate = 0;
					return 0;
				}
				if (orig_dist > root[*origins]->lg[*number_orig]) {
					double x = root[0]->lg[0];
					root[0]->lg[0] = root[*origins]->lg[*number_orig] + root[N-1]->lg[0] + x - orig_dist;
					root[N-1]->lg[0] = root[N-1]->lg[0] + x - root[0]->lg[0];
					return 1;
				}
				if (orig_dist < root[*origins]->lg[*number_orig]) {
					root[*origins]->kid[*number_orig] = 0;
					root[N-1]->kid[0] = orig_sosire + 1;
					root[0]->kid[0] = N - 1;	
					double x = root[*origins]->lg[*number_orig];
					root[N-1]->lg[0] = root[N-1]->lg[0] + root[0]->lg[0];
					root[*origins]->lg[*number_orig] = orig_dist;
					root[0]->lg[0] = x - orig_dist;
					*origins = orig_plecare + 1;
					*destination = 0;
					*number_orig = *number_dest;
					*number_dest = 0;
					return 1;
				}
			}
			else { //originea se introduce pe alta linie
				root[N-1]->kid[0] = root[0]->kid[0];
				root[N-1]->lg[0] = root[N-1]->lg[0] + root[0]->lg[0];
				for (i = 0 ; i < root[*origins]->nr ; i++)
					if (root[*origins]->kid[i] == orig_sosire + 1) {
						if (orig_dist > root[*origins]->lg[i]) { //dont be sure of that!!!
							printf("adresa invalida\n");
							return 0;
						}
						root[*origins]->kid[i] = 0;
						root[0]->kid[0] = orig_sosire + 1;
						root[0]->lg[0] = root[*origins]->lg[i] - orig_dist;
						root[*origins]->lg[i] = orig_dist;
						*number_orig = i;	
						return 1;
					}
			}
		else //punctele anteriore originilor nu coincid
			for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++)
				if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
					if (orig_dist > root[orig_plecare + 1]->lg[i]) {
						printf("adresa invalida\n");
						return 0;
					}
					root[N-1]->kid[0] = root[0]->kid[0];
					root[N-1]->lg[0] = root[N-1]->lg[0] + root[0]->lg[0];
					root[orig_plecare + 1]->kid[i] = 0;
					root[0]->kid[0] = orig_sosire + 1;
					root[0]->lg[0] = root[orig_plecare + 1]->lg[i] - orig_dist;
					root[orig_plecare + 1]->lg[i] = orig_dist;
					*origins = orig_plecare + 1;
					*number_orig = i;
					return 1;			
				}
	}
	if (*destination == orig_plecare + 1 && root[N-1]->kid[0] == orig_sosire + 1) {//destinatia se afla pe aceeasi linie cu noua origine
		if (orig_dist > root[*destination]->lg[*number_dest] + root[N-1]->lg[0]) {//distanta prea mare
			printf("adresa invalida\n");
			return 0;
		}
		if (orig_dist == root[*destination]->lg[*number_dest]) {
			*ultimate = 0;
			return 0;
		}
		if (orig_dist < root[*destination]->lg[*number_dest]) {
			root[*destination]->kid[*number_dest] = 0;
			root[0]->kid[0] = N - 1;
			root[0]->lg[0] = root[*destination]->lg[*number_dest] - orig_dist;
			root[*destination]->lg[*number_dest] = orig_dist;
			*origins = *destination;
			*number_orig = *number_dest;
			*destination = 0;
			*number_dest = 0;
			return 1;
		}	
		if (orig_dist > root[*destination]->lg[*number_dest]) {
			root[N-1]->kid[0] = 0;
			root[0]->kid[0] = orig_sosire + 1;
			root[0]->lg[0] = root[*destination]->lg[*number_dest] + root[N-1]->lg[0] - orig_dist;
			root[N-1]->lg[0] = root[N-1]->lg[0] - root[0]->lg[0];
			*origins = N - 1;
			*number_orig = 0;
			return 1;
		}	
	}
	if (*origins == orig_plecare + 1 && root[0]->kid[0] == orig_sosire + 1) { //punctele de plecare ale originilor coincid
		if (orig_dist > root[*origins]->lg[*number_orig] + root[0]->lg[0]) {//distanta prea mare
			printf("adresa invalida\n");
			return 0;
		}
		root[0]->lg[0] = root[*origins]->lg[*number_orig] + root[0]->lg[0] - orig_dist;
		root[*origins]->lg[*number_orig] = orig_dist;
		return 1; 
	}
	else { //punctele de plecare ale originilor nu coincid
		for (i = 0 ; i < root[orig_plecare + 1]->nr ; i++)
			if (root[orig_plecare + 1]->kid[i] == orig_sosire + 1) {
				if (orig_dist > root[orig_plecare + 1]->lg[i]) { //distanta prea mare
					printf("adresa invalida\n");
					return 0;
				}
				root[*origins]->kid[*number_orig] = root[0]->kid[0];
				root[*origins]->lg[*number_orig] = root[*origins]->lg[*number_orig] + root[0]->lg[0];
				root[orig_plecare + 1]->kid[i] = 0;
				root[0]->kid[0] = orig_sosire + 1;
				root[0]->lg[0] = root[orig_plecare + 1]->lg[i] - orig_dist;
				root[orig_plecare + 1]->lg[i] = orig_dist;
				*origins = orig_plecare + 1;
				*number_orig = i;
				return 1;
			}
	}
}

void gps(node **root, int *v, int N, int nr, int number_dest)//functia pt task-ul 2
{
	int i, j;
	double x;
	for (i = nr ; i >= 0 ; i--) {
		if (i == nr) 
			x = root[0]->lg[0];
		if (i == 0)
			for (j = 0 ; j < root[v[0] + 1]->nr ; j++)
				if (root[v[0] + 1]->kid[j] == N - 1) {
					x = root[v[0] + 1]->lg[j];
					break;
				}
		if (i < nr && i > 0)
			for (j = 0 ; j < root[v[i] + 1]->nr ; j++)
				if (root[v[i] + 1]->kid[j] == v[i - 1] + 1) {
					x = root[v[i] + 1]->lg[j];
					break;
				}
		printf("mergeti inainte %.3lf km\n",x);
		if (i == 0)
			printf("ati ajuns la destinatie\n");
		if (i == 1)
			printf("virati catre %d\n",root[N-1]->kid[0] - 1);
		if (i > 1)
			printf("virati catre %d\n",v[i - 2]);
	}	
}

//functia verifica daca noul punct origine se afla pe traseul calculat anterior
int verify(node **root, int *v, int N, int nr, int origins, int checker_orig, int destination, int checker_dest)
{
	if (root[0]->kid[0] == N - 1)
		return 1;
	if (root[N-1]->kid[0] == 0) {
		int a = root[N-1]->lg[0] * 1000;
		if (a > 25)
			return 0;
		return 1;
	}
	if (root[0]->kid[0] == v[nr - 1] + 1)
		if (checker_orig == origins && checker_dest == destination)
			return 1;
	int i;
	for (i = nr - 1 ; i >= 0 ; i--) {
		if (v[i] == origins - 1)
			if (v[i-1] == destination - 1)
				return 1;
			else
				return 0;
	}
	return 0;
}

//functia cauta urmatorul nod de pe traseu
int search(node **root, int *v, int N, int nr)
{
	if (root[0]->kid[0] - 1 == v[0])
		return root[N-1]->kid[0] - 1;
	int i;
	for (i = nr - 1 ; i >= 0 ; i--)
		if (v[i] == root[0]->kid[0] - 1)
			return v[i - 1];
}

//functia pt task-ul 3
void interactive(node **root, int N)
{
	int i, j, k, nr = 0, ok = 1, count = 0, origins = -1, destination = -1, number_orig = -1, number_dest = -1, checker_orig = -1, checker_dest = -1, config = 1, dest = -1, ultimate = 1;
	int *v, **path, *visit, round;
	double *dist;
	while (1) {
		if (count == 0) {
			ok = readtwo(root,N,&origins,&destination,&number_orig,&number_dest,&ultimate,2);
			if (ok == 0)
				break;
			if (ultimate == 0) {
				printf("ati ajuns la destinatie\n");
				break;
			}
			config = 0;
		}
		else {
			ok = readone(root,N,&origins,&destination,&number_orig,&number_dest,&ultimate);
			if (ok == 0)
				break;
			if (ultimate == 0) {
				printf("ati ajuns la destinatie\n");
				break;
			}
		}
		dest = root[0]->kid[0];
		if (count == 0) {
			v = (int*) calloc (1, sizeof(int));
			visit = (int*) calloc (N, sizeof(int));
			dist = (double*) malloc (N * sizeof(double));
			path = (int**) malloc (N * sizeof(int*));
			for (i = 0 ; i < N ; i++) {
				dist[i] = INT_MAX;
				path[i] = (int*) malloc (N * sizeof(int));
				path[i][0] = 0;
			}
		}
		if (count && verify(root,v,N,nr,origins,checker_orig,dest,checker_dest) == 0) {
			printf("reconfigurare traseu\n");
			config = 0;
			nr = 0;
			v = (int*) calloc (1, sizeof(int));
			visit = (int*) calloc (N, sizeof(int));
			dist = (double*) malloc (N * sizeof(double));
			path = (int**) malloc (N * sizeof(int*));
			for (i = 0 ; i < N ; i++) {
				dist[i] = INT_MAX;
				path[i] = (int*) malloc (N * sizeof(int));
				path[i][0] = 0;
			}
			dijkstra(root,visit,dist,path,v,N,&nr,0);
			if (dist[N-1] == INT_MAX) {
				printf("traseu imposibil\n");
				break;
			}
		}
		if (count == 0) {
			nr = 0;
			dijkstra(root,visit,dist,path,v,N,&nr,0);
			if (dist[N-1] == INT_MAX) {
				printf("traseu imposibil\n");
				break;
			}
		}
		checker_orig = origins;
		checker_dest = dest;
		round = (int)(float)(root[0]->lg[0] * 1000);
			if (root[0]->kid[0] == N - 1)
				if (round < 500)
					if (round > 25)
						printf("in %d metri ati ajuns la destinatie\n",round);
					else {
						printf("ati ajuns la destinatie\n");
						break;
					}
				else
					printf("mergeti inainte\n");
			else
				if (root[N-1]->kid[0] == 0) {
					int a = root[N-1]->lg[0] * 1000;
					if (a <= 25) {
						printf("ati ajuns la destinatie\n");
						break;;
					}
					else
						if (round < 500)
							printf("in %d metri virati catre %d\n",round,search(root,v,N,nr));
						else
							printf("mergeti inainte\n");
				}
				else
					if (round < 500)
						printf("in %d metri virati catre %d\n",round,search(root,v,N,nr));
					else
						printf("mergeti inainte\n");
		if (count == 0)//count retine numarul citirii curente
			count++;
	}
}

int main(int args, char **argv)
{
	char c = argv[1][0];
	node **root = NULL;
	int i, j, N, x, *v, **path, *visit, nr = 0, origins = -1, destination = -1, number_orig = -1, number_dest = -1, ultimate = 1;
	double *dist;
	FILE *f = fopen(argv[2],"r");
	fscanf(f,"%d",&N);
	N = N + 2;
	root = (node**) malloc (N * sizeof(node*));
	v = (int*) calloc (1, sizeof(int));
	visit = (int*) calloc (N, sizeof(int));
	dist = (double*) malloc (N * sizeof(double));
	path = (int**) malloc (N * sizeof(int*));
	for (i = 0 ; i < N ; i++) {
		root[i] = (node*) malloc (sizeof(node));
		dist[i] = INT_MAX;
		path[i] = (int*) malloc (N * sizeof(int));
		path[i][0] = 0;
		if (i > 0 && i < N - 1) {
			fscanf(f,"%d",&(root[i]->nr));
			root[i]->kid = (int*) malloc (root[i]->nr * sizeof(int));
			root[i]->lg = (double*) malloc (root[i]->nr * sizeof(double));
			for (j = 0 ; j < root[i]->nr ; j++) {	
				fscanf(f,"%lf",&(root[i]->lg[j]));
				fscanf(f,"%d",&x);
				root[i]->kid[j] = x + 1;
			}
		}
	}
	fclose(f);

	switch(c) {
		case '1':
				if (readtwo(root,N,&origins,&destination,&number_orig,&number_dest,&ultimate,1) == 1) {
					dijkstra(root,visit,dist,path,v,N,&nr,0);
					if (dist[N-1] == INT_MAX)
						printf("traseu imposibil\n");
					else {
						printf("%.3lf ",dist[N-1]);
						print(v,nr);
					}
				}
				break;
		case '2':
				if (readtwo(root,N,&origins,&destination,&number_orig,&number_dest,&ultimate,1) == 1) {
					dijkstra(root,visit,dist,path,v,N,&nr,0);
					gps(root,v,N,nr,number_dest);
				}
			 	break;
		case '3':
				interactive(root,N);
				break;
	}
	return 0;
}
