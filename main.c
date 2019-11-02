#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int x;
	int y;
}point;

typedef struct {
	point point1;
	point point2;
	float distance;
}pair;

float findDistance(point* noktalar, int i, int j) {
	return sqrt( pow(noktalar[i].x- noktalar[j].x, 2) + pow(noktalar[i].y- noktalar[j].y, 2));
}

pair findMinimum(point* noktalar, int p, int q) {
	int i, j;
	pair a;
	float dist;
	a.distance = findDistance(noktalar, p, p + 1);
	a.point1 = noktalar[p];
	a.point2 = noktalar[p + 1];
	for (i = p + 1; i < q; i++) {
		for (j = i + 1; j < q + 1; j++) {
			dist = findDistance(noktalar, i, j);
			if (dist < a.distance) {
				a.distance = dist;
				a.point1 = noktalar[i];
				a.point2 = noktalar[j];
			}
		}
	}
	return a;
}

void swap(int i, int j, point* noktalar) {
    int tmp = noktalar[i].x;
    noktalar[i].x = noktalar[j].x;
    noktalar[j].x =  tmp;
	
	tmp = noktalar[i].y;
    noktalar[i].y = noktalar[j].y;
    noktalar[j].y =  tmp;
}

int partition_x(point* noktalar, int left, int right) {
    int pivot, i, j;
    pivot = noktalar[left].x;
    i = left ; // do-while yapisi oldugu icin 
    j = right + 1; //normalde pivot +1 ile right arasi gezer
    do {
        do {
            i++;
        } while ( ( i <= j ) && ( noktalar[i].x <= pivot ) );
        
        do {
            j--;
        } while ( ( i <= j ) && ( noktalar[j].x > pivot ) );
        swap (i, j, noktalar);
    } while (i < j);
    swap (i, j, noktalar);
    swap(left, j, noktalar);
    return j;
}

int partition_y(point* noktalar, int left, int right) {
    int pivot, i, j;
    pivot = noktalar[left].y;
    i = left ; // do-while yapisi oldugu icin 
    j = right + 1; //normalde pivot +1 ile right arasi gezer
    do {
        do {
            i++;
        } while ( ( i <= j ) && ( noktalar[i].y <= pivot ) );
        
        do {
            j--;
        } while ( ( i <= j ) && ( noktalar[j].y > pivot ) );
        swap (i, j, noktalar);
    } while (i < j);
    swap (i, j, noktalar);
    swap(left, j, noktalar);
    return j;
}

void quickSort(point* noktalar, int left, int right, char tip) {
	int s;
	if ( left < right) {
		if (tip == 'x')
			s = partition_x(noktalar, left, right);
		else if (tip == 'y')
			s = partition_y(noktalar, left, right);
		quickSort(noktalar, left, s-1, tip);
		quickSort(noktalar, s + 1, right, tip);
	}
}

void medianAreaBoundary(point* noktalar, int p, int q, pair min, int *start, int *end) {
	int i;
	i = p;
	while ((i != q/2) && (noktalar[i].x < noktalar[q/2].x - min.distance)) { 
		i++;
	}
	*start = i;
	if (*start !=  q/2)
		*start = i--;
	
	i = q/2 + 1;
	while ((i != q) && (noktalar[i].x < noktalar[q/2].x + min.distance)) {
		i++;
	}
	*end = i;
	if (*end != q)
		*end = --i;
}

pair medianArea(point* noktalar, int p, int q, pair min) {
	int i, j, k, start, end;
	float dist;
	pair local = min;
	
	medianAreaBoundary(noktalar, p, q, min, &start, &end);
	
	point* medyanCevresi = (point*) malloc((end-start+1)*sizeof(point));
	
	j = 0;
	for(i = start; i< end + 1; i++) {
		medyanCevresi[j].x = noktalar[i].x;
		medyanCevresi[j].y = noktalar[i].y;
		j++;
	}
	
	quickSort(medyanCevresi, 0, j-1, 'y');
	
	printf("-------Y'ye gore sirali medyan cevresi noktalari-------\n");
	for(i = 0; i < end-start+1; i++) {
		printf("%d %d \n", medyanCevresi[i].x, medyanCevresi[i].y);
	}printf("\n");
	
	for (i = 0; i < j - 1; i++) {
		k = i + 1;
		while(( k < j ) && (medyanCevresi[k].y <= medyanCevresi[i].y + local.distance)) {
				dist = findDistance(medyanCevresi, k, i);
				if (dist < local.distance) {
					local.distance = dist;
					local.point1 = medyanCevresi[k];
					local.point2 = medyanCevresi[i];
				}
			k++;
		}
	}
	return local;
}

pair findClosestPair (point* noktalar, int p, int q) {
	pair min;
	if (q - p > 2) {
		int median = p + (q-p)/2;
			
		pair A = findClosestPair(noktalar, p, median);
		
		printf("Dallanma indis araligi -> Noktalar[%d: %d]\n", p,  median);	
		printf("Dallanmanin closest pairi: (%d, %d) - (%d, %d), distance : %f \n", A.point1.x, A.point1.y, A.point2.x, A.point2.y, A.distance);
		printf("\n");
		pair B = findClosestPair(noktalar, median + 1, q);
		
		printf("Dallanma indis araligi -> Noktalar[%d: %d] \n", median + 1,  q);
		printf("Dallanmanin closest pairi: (%d, %d) - (%d, %d), distance : %f \n", B.point1.x, B.point1.y, B.point2.x, B.point2.y, B.distance);
		printf("\n");
		if (A.distance < B.distance)
			min = A;
		else
			min = B;
		
		A = medianArea(noktalar, p, q, min);
		printf("Medyan cevresinden bulunan pair: (%d, %d) - (%d, %d), distance : %f \n", A.point1.x, A.point1.y, A.point2.x, A.point2.y, A.distance);
		printf("\n");
		return A;
	}
	else 
		return findMinimum(noktalar, p, q);
}

void tokenizer(char line[], point* noktalar, int i) {
	int j = 0, k = 0, flag = 1;
	char num[10];
	if (line[j] == '-') {
		flag = -1;
		j++;
	}
	
	while(line[j] != ' ') {
		num[k] = line[j];
		j++; k++;
	} num[k] = '\0';
	noktalar[i].x = flag*atoi(num);
	num[0] = '\0';
	
	flag = 1;
	j++;
	if (line[j] == '-') {
		flag = -1;
		j++;
	}
	k = 0;
	while((line[j] != '\n') && (line[j] != ' ')) {
		num[k] = line[j];
		j++; k++;
	} num[k] = '\0';
	noktalar[i].y = flag*atoi(num);
}


int main(int argc, char **argv) {
	int n = 20, i = 0;
	FILE *f1;
	char line[100];
	point* noktalar = (point*) malloc(n* sizeof(point));
	f1 = fopen("D:\\Input4.txt", "r");
	
	if (f1 == NULL){
		printf("File could not opened!!\n");
	}
	fseek(f1, 0, SEEK_SET);
	
	while((fgets(line, 100, f1) != NULL) && (line[0] != '\n')) {
		if (i == n) {
			noktalar = (point*) realloc(noktalar, sizeof(point)*5);
			n += 5;
		}
		//printf("line: %s", line);
		tokenizer(line, noktalar, i);
		i++;
	}
	n = i;
	printf("-------Noktalar------- \n");
	for(i = 0; i < n; i++) {
		printf("%d %d\n", noktalar[i].x, noktalar[i].y);
	}printf("\n");
	quickSort(noktalar, 0, n-1, 'x');
	
	printf("-------X'e gore sirali noktalar------- \n");
	for(i = 0; i < n; i++) {
		printf("%d %d\n", noktalar[i].x, noktalar[i].y);
	}printf("\n");
	
	pair closestPair = findClosestPair(noktalar, 0, n-1);
	
	printf("-------Closest Pair------- \n");
	printf("Nokta1.x = %d, Nokta1.y = %d \n", closestPair.point1.x, closestPair.point1.y);
	printf("Nokta2.x = %d, Nokta2.y = %d \n", closestPair.point2.x, closestPair.point2.y);
	printf("Minimum distance: %f \n", closestPair.distance);
	printf("\n");
	return 0;
}
