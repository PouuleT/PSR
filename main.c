#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

void afficherListe(chain liste)
{
    mail *tmp = liste;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        /* On affiche */
        printf("%d ", tmp->el.pid);
        /* On avance d'une case */
        tmp = tmp->next;
    }
}

void print_packet(packet *p) {
  printf("Packet :\n");
  printf("\tpid : %d\n",p->pid);
  printf("\tfid : %d\n",p->fid);
  printf("\tsource : %d\n",p->source);
  printf("\tdestination : %d\n",p->destination);
  printf("\tdepart : %f\n",p->departure);  
  printf("\tlast_time : %f\n",p->last_time);  
  printf("\tsize : %d\n",p->size);
  printf("\tpos : %d\n",p->pos);
  printf("\n");
}

void copy(packet* el, packet* old, int i){ // On copie old dans el
  if(i) {
    el->source = old->source;
    el->destination = old->destination;
    el->pid = old->pid;
    el->fid = old->fid;
    el->departure = old->departure;
    el->last_time = old->last_time;
    el->size = old->size;
    el->pos = old->pos;
  }
  else {
      el->last_time = old->last_time;
    el->size = old->size;
    el->pos = old->pos;
    }
    
}

chain add_chain(chain l, packet* el) {
  mail* new_el = malloc(sizeof(mail));
  //printf("add_chain:On va rajouter au debut de la chaine le paquet : \n");
  //print_packet(el);
  copy(&(new_el->el), el,1);
  //printf("add_chain:On a copier le paquet, on affiche le nouveau \n");
  //print_packet(&(new_el->el));
//  new_el->el= el;
  new_el->next=l;
  //printf("add_chain:Tout s'est bien passer, on part\n");
  return new_el;
}

chain add_chain_end(chain liste, packet valeur)
{
  /* On crée un nouvel élément */
  mail* nouvelElement = malloc(sizeof(mail));
  /* On assigne la valeur au nouvel élément */
  nouvelElement->el = valeur;
  /* On ajoute en fin, donc aucun élément ne va suivre */
  nouvelElement->next = NULL;

  if(liste == NULL)
  {
      /* Si la liste est videé il suffit de renvoyer l'élément créé */
      return nouvelElement;
  }
  else
  {
    mail* temp=liste;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = nouvelElement;
    return liste;
  }
}

int is_empty(chain l) {
  if(l==NULL)
    return 1;
  else
    return 0;
}

int nombreElements(chain liste)
{
    /* Si la liste est vide, il y a 0 élément */
    if(liste == NULL)
        return 0;
 
    /* Sinon, il y a un élément (celui que l'on est en train de traiter)
    plus le nombre d'éléments contenus dans le reste de la liste */
    return nombreElements(liste->next)+1;
}

chain delElementEnTete(chain liste)
{
  if(liste != NULL)
  {
    /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
    l'élément en 2ème position */
    mail* aRenvoyer = liste->next;
    /* On libère le premier élément */
    free(liste);
    /* On retourne le nouveau début de la liste */
    return aRenvoyer;
  }
  else
  {
    return NULL;
  }
}

chain delElementEnFin(chain liste)
{
  /* Si la liste est vide, on retourne NULL */
  if(liste == NULL)
    return NULL;

  /* Si la liste contient un seul élément */
  if(liste->next == NULL)
  {
    /* On le libère et on retourne NULL (la liste est maintenant vide) */
    free(liste);
    return NULL;
  }

  /* Si la liste contient au moins deux éléments */
  mail* tmp = liste;
  mail* ptmp = liste;
  /* Tant qu'on n'est pas au dernier élément */
  while(tmp->next != NULL)
  {
    /* ptmp stock l'adresse de tmp */
    ptmp = tmp;
    /* On déplace tmp (mais ptmp garde l'ancienne valeur de tmp */
    tmp = tmp->next;
  }
  /* A la sortie de la boucle, tmp pointe sur le dernier élément, et ptmp sur
  l'avant-dernier. On indique que l'avant-dernier devient la fin de la liste
  et on supprime le dernier élément */
  ptmp->next = NULL;
  free(tmp);
  return liste;
}

chain delElement(chain liste, int valeur)
{
    /* Liste vide, il n'y a plus rien à supprimer */

  if(liste == NULL)
    return NULL;
 
    /* Si l'élément en cours de traitement doit être supprimé */
  if(liste->el.pid == valeur)
  {
    /* On le supprime en prenant soin de mémoriser 
    l'adresse de l'élément suivant */
    mail* tmp = liste->next;
    //printf("delElement - On test l'affichage de la liste\n");
    //afficherListe(liste);
    //printf("\n");
    //afficherListe(tmp);
    //printf("\n");
    //printf("On del l'el %d ... ",liste->el.pid);
    //printf("\nwooT\n");
    if(valeur!=4046)
      free(liste);
    //printf("Done\n");
    /* L'élément ayant été supprimé, la liste commencera à l'élément suivant
    pointant sur une liste qui ne contient plus aucun élément ayant la valeur recherchée */
    //chain delElementEnTete(chain liste)
    //tmp = delElementEnTete(liste);
    //printf("Pwet\n");
    tmp = delElement(tmp, valeur);
    //printf("YOLO\n");
    return tmp;
  }
  else
  {
    /* Si l'élement en cours de traitement ne doit pas être supprimé,
    alors la liste finale commencera par cet élément et suivra une liste ne contenant
    plus d'élément ayant la valeur recherchée */
    liste->next = delElement(liste->next, valeur);
    return liste;
  }
}

chain find_by_pid(chain liste, int valeur)
{
  mail *tmp=liste;
  /* Tant que l'on n'est pas au bout de la liste */
  while(tmp != NULL)
  {
    if(tmp->el.pid == valeur)
    {
      /* Si l'élément a la valeur recherchée, on renvoie son adresse */
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

chain find_by_packet(chain liste, packet valeur)
{
  mail *tmp=liste;
  /* Tant que l'on n'est pas au bout de la liste */
  while(tmp != NULL)
  {
    if(is_equal(tmp->el,valeur))
    {
      /* Si l'élément a la valeur recherchée, on renvoie son adresse */
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

int is_equal(packet p1, packet p2) {
  if(p1.pid == p2.pid)
    return 1;
  else
    return 0;
}

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

int new_packet(node *reseau, int code, int pid, int pos, int i) {
	static int nb_packets = 0;
	static int nb_emited=0;
	static int nb_traited=0;
	static int nb_recieved=0;
	static int nb_destroyed=0;
	
	if(i==2) {      // On actualise les informations
		if(code == 0) {// Paquet émis
			nb_emited++;
			reseau[pos-1].nb_emited++;
		}
		else if(code == 1) {  // Paquet arrivé dans un noeud intermédiaire
			nb_traited++;
			reseau[pos-1].nb_traited++;
		}
		else if(code == 3) { // Paquet arrivé à destination
			nb_recieved++;
			reseau[pos-1].nb_recieved++;
		}
		else if(code == 4) { // Destruction d'un paquet (file pleine)
			nb_destroyed++;
			reseau[pos-1].nb_destroyed++;		
	  }
		return 0;
	}
	else {// Sinon, on demande à récupérer les données
	  if (i==0)  // Paquet émis
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
}

void new_packet_lost(int pid) {
}

int max_fid(int fid) {
	static int nb_fid=0;
	if(nb_fid<fid)
		nb_fid=fid;
  return nb_fid;
}

float max_time(float t) {
	static float max=0.;
	if(max<t)
		max=t;
  return max;
}

int analyze_matrix(FILE* matrice, int m[50][50]) {
	char car[5];
	
	int i=0,j=0;
	int o,p;
	int end=0;
	int k=0;

  char c;

	if(matrice == NULL) {
		printf("Impossible d'ouvrir le fichier de matrice donne en argument\n");
		printf("Fin du programme\n");
		return -1;
	}
	else {
		printf("Analyse du fichier de matrice ... ");
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
		//printf("Le fichier comporte %d lignes\n", j);
		
		/*for(o=0;o<j;o++) {
		  for(p=0;p<j;p++) {
		    printf("%d ", m[p][o]);
		  }
		  printf("\n");
		}*/
	}
	printf("Done\n");
	return j;
}

void print_stats_lost_packets(node* reseau, int size, int total) {
  int i;
  for(i=0;i<size;i++) {
    printf("Nombre de packets perdus par le noeud N%d : %d ; soit %lf \%\n",i+1,reseau[i].nb_destroyed,(float)reseau[i].nb_destroyed/total*100 );
  }

}

void create_packet(packet *p_temp, float t, int code, int pid, int fid, int tos, int s, int d, int pos) {
  p_temp->source = s;
  p_temp->destination= d;
  p_temp->pid = pid;
  p_temp->fid = fid;
  p_temp->last_time=t;
  if(code == 0)
    p_temp->departure = t;
//  p_temp->size
  p_temp->pos = pos;
}

void analyze_and_do_packet(packet *p_temp, chain *temp, int code, log *stats) {
  chain temp_el = NULL;
  if(code == 0) {
    //Rajouter le paquet au début de la liste
    //printf("analyze_and_do_packet : c'est une creation ... ");
    //print_packet(p_temp);
    *temp = add_chain(*temp, p_temp);
    //printf("Done\n");
  }
  else if((code == 1) || (code == 2)) {
    //Mettre à jour les données du paquet qui existe déjà
    //printf("analyze_and_do_packet : c'est une modification %d, on cherche %d ... ", code, p_temp->pid);
    //print_packet(p_temp);
    temp_el = find_by_pid(*temp, p_temp->pid);
    if(temp_el==NULL) {
      printf("\nanalyse_and_do_packet : probleme, paquet en cours n'a pas ete cree\n");
      exit(-1);
    }
    else {
      //printf("Done\n");
      //printf("analyze_and_do_packet : c'est une modification : on modifie ... ");
      if((code==2)) {
        stats->total_wait = stats->total_wait + (p_temp->last_time - temp_el->el.last_time);
      }
      copy(&(temp_el->el), p_temp,0);
      //printf("Done\n");
    }
  }
  else if(code==3){
    //printf("analyze_and_do_packet : paquet %d arrive a destination, c'est une suppression\n",p_temp->pid);
    //Supprimer le paquet car il est arrivé à destination
    //Mettre à jour les statistiques
    //printf("On test l'affichage de la liste\n");
    //afficherListe(*temp);
    //printf("\n");
    //printf("id de l'el : %d\n", p_temp->pid);
    chain temp_el = find_by_pid(*temp,p_temp->pid);
    //printf("Date de depart : %f\n",test->el.departure);
    //printf("last_time : %f\n",p_temp->last_time);
    //printf("delai d'acheminement : %f\n",p_temp->last_time-p_temp->departure);
    //pause();
    stats->total_delay=stats->total_delay+(p_temp->last_time)-temp_el->el.departure;
    *temp = delElement(*temp, p_temp->pid);
  }
  else if(code==4) {
    //printf("analyze_and_do_packet : paquet %d destroy, c'est une suppression ... ",p_temp->pid);
    //Supprimer le paquet car il est detruit
    *temp= delElement(*temp, p_temp->pid);
    //printf("Done\n");
  }
  else {
      printf("analyse_and_do_packet : probleme, code : %d n'existe pas\n", code);
      exit(-1);
  }
}

void print_stat_flux(flux *flow) {
  printf("Flux fid = %d entre N%d et N%d\n", flow->fid, flow->source, flow->destination);
  printf("\tNombre de paquets emis : %d\n",flow->nb_packets);
  printf("\tNombre de paquets recus : %d\n",flow->nb_recieved);
  printf("\tNombre de paquets perdus : %d\n",flow->nb_destroyed);
  printf("\tTaux de perte : %f\%\n",(float)flow->nb_destroyed/flow->nb_packets*100);
  printf("\tTemps cumule passe dans les files : %fs\n",flow->packet_wait);
  printf("\tDuree de vie : %fs\n", flow->end-flow->start);
}

int main(int argc, char *argv[]) {
	FILE* fichier = fopen(argv[1],"r");
	FILE* matrice = fopen(argv[2],"r");

	float t, last_t, packet_wait=0;
	int code, pid, fid, tos, s, d, pos, bif,i=0, size;
	int nb_fid, nb_node, last_pos=0;
	node *reseau;
	int m[50][50];

  node test = {10,0,0,0,0,0};

  chain temp = NULL;
  packet p_temp = {0};
  log stats;
  
	nb_node = analyze_matrix(matrice,m);
	
	reseau = calloc(nb_node, sizeof(node));
	
  if(argc==3) {
	  if(fichier != NULL) {
		  //printf("On commence l'analyse du fichier\n");
		  while(fscanf(fichier, "%f %d ", &t, &code)==2) { // Tant que le fscanf recupere 2 valeurs, on continue de parser
			  if(code==4) {
				  fscanf(fichier, "%d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &s, &d, &pos);
			  }
			  else {
				  fscanf(fichier, "%d %d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &bif, &s, &d, &pos);
			  }
			  create_packet(&p_temp,t, code, pid, fid, tos, s, d, pos);   // On met les valeurs dans p_temp
			  //printf("On a cree le paquet p_temp : \n");
			  //print_packet(&p_temp);
			  analyze_and_do_packet(&p_temp, &temp, code, &stats);
			  new_packet(reseau, code, pid, pos, 2);
			  nb_fid = max_fid(fid);
			  //printf("Nombre d'elements : %d\n",nombreElements(temp));
			  //afficherListe(temp);
			  //printf("\n");
			  /*if(i==1000) {
			    printf("Manual break\n");
			    break;
			    }*/
			  i++;
		  }
		  if(feof(fichier)) {
			  printf("Fin de l'analyse du fichier\n\n");
        printf("Statistiques observées : \n");
			  printf("Nombre de flux : %d\n", nb_fid+1);
			  printf("Nombre de paquets emis : %d\n", new_packet(NULL,0,0,0,0));
			  printf("Nombre de paquets transmis : %d\n", new_packet(NULL,0,0,0,1));
			  printf("Nombre de paquets recus : %d\n", new_packet(NULL,0,0,0,3));
			  printf("Nombre de paquets perdus : %d\n", new_packet(NULL,0,0,0,4));
			  printf("Taux de perte : %f\%\n", (float)new_packet(NULL,0,0,0,4)/new_packet(NULL,0,0,0,0)*100);
			  printf("Nombre de sauts moyen : %f\n", (float)new_packet(NULL,0,0,0,1)/new_packet(NULL,0,0,0,0));
			  printf("Nombre de paquets moyen par flux : %f\n", (float)new_packet(NULL,0,0,0,0)/(nb_fid+1));
			  printf("Delai moyen de bout en bout : %fs\n", (float)stats.total_delay/new_packet(NULL,0,0,0,3));
        printf("Temps total cumulé d'attente : %fs\n",stats.total_wait);
        printf("Temps moyen d'attente par files : %fs\n", (float)stats.total_wait/(new_packet(NULL,0,0,0,1)+new_packet(NULL,0,0,0,0)));
        printf("Temps moyen d'attente par paquets : %fs\n", (float)stats.total_wait/(new_packet(NULL,0,0,0,3)+new_packet(NULL,0,0,0,4)));
			  print_stats_lost_packets(reseau, nb_node, new_packet(NULL,0,0,0,4));
			
		  }
		  else
			  printf("Probleme de lecture\n");

		  fclose(fichier);
	  }
	  else {
		  printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		  printf("Fin du programme\n");
	  }
  }
  else if(argc == 5) {
    if(atoi(argv[3])==0) {
      int the_pid = atoi(argv[4]);
      printf("\nAnalyse d'un paquet\n");
/*      printf("\n%d %d %d\n\n",m[0][3],m[3][0],m[0][19]);
      printf("%d %d %d\n",m[23][19],m[19][1],m[1][16]);
      printf("%d %d %d\n",m[22][18],m[18][0],m[0][15]);
      printf("%d %d %d\n",m[19][23],m[1][19],m[16][1]);
      printf("%d %d %d\n",m[18][22],m[0][18],m[15][0]);*/
	    if(fichier != NULL) {
		    //printf("On commence l'analyse du fichier\n");
		    while(fscanf(fichier, "%f %d ", &t, &code)==2) { // Tant que le fscanf recupere 2 valeurs, on continue de parser
			    if(code==4) {
				    fscanf(fichier, "%d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &s, &d, &pos);
			    }
			    else {
				    fscanf(fichier, "%d %d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &bif, &s, &d, &pos);
			    }
			    if(pid==the_pid) {
			      create_packet(&p_temp,t, code, pid, fid, tos, s, d, pos);   // On met les valeurs dans p_temp
			      if(code==0) {
			        printf("-t=%f\tPaquet %d, depart : N%d, destination : N%d\n",t,the_pid,s,d);
			        last_t = t;
			        last_pos=pos;
			      }
			      else if(code == 1) {
			        printf("-t=%f\tArrive a N%d (Temps d'acheminement : %f)\n",t,pos,t-last_t);
			        //printf("\nDebit entre %d et %d = %d\n",last_pos, pos,m[last_pos-1][pos-1]);
			        last_pos = pos;
			        last_t = t;
			      }
			      else if(code == 2) {
			        printf("-t=%f\tPart de N%d a destination de N%d apres avoir passe %fs en attente\n", t,last_pos, pos, t-last_t);
			        packet_wait = packet_wait + t-last_t;
			      }
			      else if(code == 3) {
			        printf("-t=%f\tArrive a destination au noeud N%d\n",t,pos);
			        printf("\tTemps total passe en attente : %f\n",packet_wait);
			        printf("\tTemps total : %f\n",t-p_temp.departure);
			      }
			      else if(code == 4) {
			      }
			      else {
			        printf("Probleme, code de paquet invalide\n");
			        exit(-1);
			      }
		      }
		    }
		    if(feof(fichier)) {
			    printf("Fin\n");// de l'analyse du fichier\n\n");
			  }
			}
	    else {
		    printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		    printf("Fin du programme\n");
	    }
    }
    else if(atoi(argv[3])==1) {
      printf("\nAnalyse d'un flux ... ");
      int the_fid = atoi(argv[4]);
      int first=1;
      flux flow={0.};
      
      flow.fid = the_fid;
      
	    if(fichier != NULL) {
		    //printf("On commence l'analyse du fichier\n");
		    while(fscanf(fichier, "%f %d ", &t, &code)==2) { // Tant que le fscanf recupere 2 valeurs, on continue de parser
			    if(code==4) {
				    fscanf(fichier, "%d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &s, &d, &pos);
			    }
			    else {
				    fscanf(fichier, "%d %d %d %d N%d N%d N%d\n", &pid, &fid, &tos, &bif, &s, &d, &pos);
			    }
			    if(fid==the_fid) {
			      create_packet(&p_temp,t, code, pid, fid, tos, s, d, pos);   // On met les valeurs dans p_temp
			      
/*typedef struct flux{
  int nb_packets;
	int nb_emited;
	int nb_traited;
	int nb_recieved;
	int nb_destroyed;
	float start;
	float end;
	float avg_delay;
}flux;*/
			      
			      if(code==0) {
			        //printf("-t=%f\tFlux %d, depart : N%d, destination : N%d\n",t,the_fid,s,d);
			        last_t = t;
			        //last_pos=pos;
			        if(first){
			          flow.source = s;
			          flow.destination = d;
  			        flow.start = t;
  			        first = 0;
  			        }
			        flow.nb_packets++;
			      }
			      else if(code == 1) {
			        /*printf("-t=%f\tArrive a N%d (Temps d'acheminement : %f)\n",t,pos,t-last_t);
			        printf("\nDebit entre %d et %d = %d\n",last_pos, pos,m[last_pos-1][pos-1]);
			        last_pos = pos;*/
			        last_t = t;
			        flow.nb_traited++;
			      }
			      else if(code == 2) {
			        /*printf("-t=%f\tPart de N%d a destination de N%d apres avoir passe %fs en attente\n", t,last_pos, pos, t-last_t);
			        */
			        flow.packet_wait = packet_wait + t-last_t;
			      }
			      else if(code == 3) {
			        /*printf("-t=%f\tArrive a destination au noeud N%d\n",t,pos);
			        printf("\tTemps total passe en attente : %f\n",packet_wait);
			        printf("\tTemps total : %f\n",t-p_temp.departure);*/
			        flow.nb_recieved++;
  		        flow.end = max_time(t);
			      }
			      else if(code == 4) {
			        flow.nb_destroyed++;
			      }
			      else {
			        printf("Probleme, code de paquet invalide\n");
			        exit(-1);
			      }
		      }
		    }
		    if(feof(fichier)) {
			    printf("Fin\n");// de l'analyse du fichier\n\n");
			  }
			  //printf("Done\n");
			  print_stat_flux(&flow);
			}
	    else {
		    printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		    printf("Fin du programme\n");
	    }
    }
    else {
      printf("\nCette fonction n'existe pas\n");
    }
  }
  else if(argc==6) {
    printf("\nDebut de l'analyse des flux entre deux noeuds\n");
    printf("Fonction pas encore en place\n");
  }
  else {
    printf("Mauvais appel de la fonction\n");
    printf("Syntaxe pour les statistiques globales :\n");
    printf("\tprog nomdufichiersource nomdufichiermap\n");
    printf("Syntaxe pour les statistiques particulières :\n");
    printf("Pour les statistiques d'un paquet :\n");
    printf("\tprog nomdufichiersource nomdufichiermap 0 pid\n");
    printf("Pour les statistiques d'un flux :\n");
    printf("\tprog nomdufichiersource nomdufichiermap 1 fid\n");
    printf("Pour les statistiques entre deux noeuds :\n");
    printf("\tprog nomdufichiersource nomdufichiermap 2 idnoeud1 idnoeud2\n");
    printf("\n\n");
  }
  
  printf("Fin du programme\n");
  
  //free(reseau);
  
	return 0;		
	
}
