#include <stdio.h>
#include <stdlib.h>

int is_new(int pid, int *packets) {
	int i=0;
	while(pid>=packets[i])
		i++;
	if(packets[i]!=pid) {
		
		return 1;
	}
	else
		return 0;

}

int new_packet(int code, int pid, int i) {
	static int nb_packets = 0;
	static int nb_emited=0;
	static int nb_traited=0;
	static int nb_recieved=0;
	static int nb_destroyed=0;
	
	if(i==2) {      // On actualise les informations
		if(code == 0) // Paquet émis
			nb_emited++;
		else if(code == 1)  // Paquet arrivé dans un noeud intermédiaire
			nb_traited++;
		else if(code == 3)  // Paquet arrivé à destination
			nb_recieved++;
		else if(code == 4)  // Destruction d'un paquet (file pleine)
			nb_destroyed++;		
		return 0;
	}
	// Sinon, on demande à récupérer les données
	else if (i==0)  // Paquet émis
		return nb_emited;
	else if (i==1)  // Paquet arrivé dans un noeud intermédiaire
		return nb_traited;
	else if (i==3)  // Paquet arrivé à destination
		return nb_recieved;
	else if (i==4)  // Destruction d'un paquet (file pleine)
		return nb_destroyed;
	else {
		printf("Probleme new_packet(), wrong i\n");
		return -1;
	}
}

void new_packet_lost(int pid) {
}

int max_fid(int fid) {
	static int nb_fid=0;
	if(nb_fid<fid)
		nb_fid=fid;
		return fid;
}

int analyze_matrix(FILE* matrice) {
	char car[5];
	
	int i=0,j=0;
	int o,p;
	int end=0;
	int k=0;
	int m[50][50];
  char c;

	if(matrice == NULL) {
		printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
		printf("Fin du programme\n");
		return -1;
	}
	else {
		printf("On commence l'analyse du fichier de matrice\n");
    do
 		{
      c = fgetc(matrice);
      if(c!=NULL) {
        if(c == '\n') {
          car[k]='\0';
          m[i][j] = atoi(car);
          //printf("%s devient : %d [%d ; %d]\n",car, m[i][j], i, j);

          i=0;
          j++;
          k=0;
        }
        else if(c == EOF) {
          end = 1;
          //printf("Fin du fichier\n");
        }
        else if(c == '\t') {
          car[k]='\0';
          m[i][j] = atoi(car);
          //printf("%s devient : %d [%d ; %d]\n",car, m[i][j], i, j);
          i++;
          k=0;
        }
        else {
          car[k] = c;
          k++;
        }
      }
      else {
	      printf("Probleme de parsing\n");
	      exit(-1);
      }
 	 	} while (!end);
		printf("Le fichier comporte %d lignes\n", j);
		
		for(o=0;o<j;o++) {
		  for(p=0;p<j;p++) {
		    printf("%d ", m[p][o]);
		  }
		  printf("\n");
		}
	}

}

int main(int argc, char *argv[]) {
	FILE* fichier = fopen(argv[1],"r");
	FILE* matrice = fopen(argv[2],"r");

	float t;
	int code, pid, fid, tos, s, d, pos, bif;
	int nb_fid;

	analyze_matrix(matrice);

	if(fichier != NULL) {
		printf("On commence l'analyse du fichier\n");
		while(fscanf(fichier, "%f %d ", &t, &code)==2) { // Tant que le fscanf recupere 2 valeurs, on continue de parser
			if(code==4) {
				fscanf(fichier, "%d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &s, &d, &pos);    
//				printf("%f %d %d %d %d N%d N%d N%d\n", t, code, pid, fid, tos, s, d, pos);

			}
			else {
				fscanf(fichier, "%d %d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &bif, &s, &d, &pos);
//				printf("%f %d %d %d %d %d N%d N%d N%d\n", t, code, pid, fid, tos, bif, s, d, pos);
			}
			new_packet(code, pid, 2);
			nb_fid = max_fid(fid);
		}
		if(feof(fichier)) {
			printf("Fin du fichier\n");
			printf("Nombre de flux : %d\n", nb_fid);
			printf("Nombre de paquets emis : %d\n", new_packet(0,0,0));
			printf("Nombre de paquets transmis : %d\n", new_packet(0,0,1));
			printf("Nombre de paquets recus : %d\n", new_packet(0,0,3));
			printf("Nombre de paquets perdus : %d\n", new_packet(0,0,4));
			
		}
		else
			printf("Probleme de lecture\n");

		fclose(fichier);
	}
	else {
		printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		printf("Fin du programme\n");
	}


	return 0;		
	
}
