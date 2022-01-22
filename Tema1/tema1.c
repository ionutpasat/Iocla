#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

/*functie de initializare a unui nou director*/
struct Dir *getNewDir(char *name)
{
	struct Dir *Dir;
	Dir = (struct Dir *)malloc(sizeof(struct Dir));
	Dir->parent = NULL;
	Dir->head_children_dirs = NULL;
	Dir->head_children_files = NULL;
	Dir->name = (char *)malloc(strlen(name) + 1);
	strcpy(Dir->name, name);
	Dir->next = NULL;
	return Dir;
}

/*functie de initializare a unui fisier*/
struct File *getNewFile(char *name)
{
	struct File *File;
	File = (struct File *)malloc(sizeof(struct File));
	File->parent = NULL;
	File->name = (char *)malloc(strlen(name) + 1);
	strcpy(File->name, name);
	File->next = NULL;
	return File;
}

/*functie de eliberare a fisierelor*/
void free_files(File *file)
{
	if (file == NULL)
		return;
	free_files(file->next);
	free(file->name);
	free(file);
}

/*functie de eliberare a directoarelor*/
void free_dirs(Dir *parent)
{
	if (parent == NULL)
		return;
	free_dirs(parent->next);
	free_dirs(parent->head_children_dirs);
	free_files(parent->head_children_files);
	free(parent->name);
	free(parent);
}

void touch(Dir *parent, char *name)
{
	Dir *curr_dir = parent->head_children_dirs;
	File *curr_file = parent->head_children_files;

	/*verific daca exista deja un fisier cu acelasi nume*/
	while (curr_file)
	{
		if (strcmp(curr_file->name, name) == 0)
		{
			printf("File already exists\n");
			return;
		}
		curr_file = curr_file->next;
	}
	/*verific si daca nu cumva exista un director cu acelasi nume*/
	while (curr_dir)
	{
		if (strcmp(curr_dir->name, name) == 0)
		{
			printf("Directory with this name already exists\n");
			return;
		}
		curr_dir = curr_dir->next;
	}

	/*daca se trece de ambele verificari adaug fisierul
	  la inceput sau in continuare depinzand de caz*/
	File *new_file = getNewFile(name);
	new_file->parent = parent;
	if (parent->head_children_files == NULL)
	{
		parent->head_children_files = new_file;
	}
	else
	{
		File *temp = parent->head_children_files;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_file;
	}
}

void mkdir(Dir *parent, char *name)
{
	Dir *curr_dir = parent->head_children_dirs;
	File *curr_file = parent->head_children_files;

	/*aceleasi verificari si implementare precum la "touch"*/
	while (curr_dir)
	{
		if (strcmp(curr_dir->name, name) == 0)
		{
			printf("Directory already exists\n");
			return;
		}
		curr_dir = curr_dir->next;
	}
	while (curr_file)
	{
		if (strcmp(curr_file->name, name) == 0)
		{
			printf("File with this name already exists\n");
			return;
		}
		curr_file = curr_file->next;
	}

	Dir *new_dir = getNewDir(name);
	new_dir->parent = parent;
	if (parent->head_children_dirs == NULL)
	{
		parent->head_children_dirs = new_dir;
	}
	else
	{
		Dir *temp = parent->head_children_dirs;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_dir;
	}
}

void ls(Dir *parent)
{
	Dir *curr_dir = parent->head_children_dirs;
	File *curr_file = parent->head_children_files;
	
	/*parcurg listele de directoare/fisiere si le afisez in ordine*/
	while (curr_dir != NULL)
	{
		printf("%s\n", curr_dir->name);
		curr_dir = curr_dir->next;
	}
	while (curr_file != NULL)
	{
		printf("%s\n", curr_file->name);
		curr_file = curr_file->next;
	}
}

void rm(Dir *parent, char *name)
{
	/*daca directorul in care caut nu are fisiere returnez eroare*/
	if (parent->head_children_files == NULL)
	{
		printf("Could not find the file\n");
		return;
	}

	/*daca are il caut in lista si exista 3 cazuri de stergere: */
	File *curr_file = parent->head_children_files;
	while (curr_file != NULL)
	{	
		if (strcmp(curr_file->name, name) == 0)
		{	/*fisierul este primul din director => schimb legaturile
			  si eliberez memoria celui de sters*/
			if (curr_file == parent->head_children_files)
			{
				parent->head_children_files = curr_file->next;
				free(curr_file->name);
				free(curr_file);
				return;
			}
			/*fisierul este ultimul din director => schimb legaturile cu penultimul
			  si eliberez memoria celui de sters*/
			if (curr_file->next = NULL)
			{
				File *aux = parent->head_children_files;
				while (aux->next != curr_file)
				{
					aux = aux->next;
				}
				aux->next = NULL;
				free(aux->name);
				free(aux);
				return;
			}
			/*fisierul e la mijloc => refac legaturile si eliberez memoria celui de sters*/
			File *aux = parent->head_children_files;
			while (aux->next != curr_file)
			{
				aux = aux->next;
			}
			aux->next = curr_file->next;
			free(curr_file->name);
			free(curr_file);
			return;
		}
		curr_file = curr_file->next;
	}
	printf("Could not find the file\n");
}

void rmdir(Dir *parent, char *name)
{
	/*aceeasi implementare ca la "rm" doar ca eliberez
	  directorul gasit cu tot continutul acestuia*/
	if (parent->head_children_dirs == NULL)
	{
		printf("Could not find the dir\n");
		return;
	}

	Dir *del_dir = parent->head_children_dirs;
	while (del_dir != NULL)
	{
		if (strcmp(del_dir->name, name) == 0)
		{
			if (del_dir == parent->head_children_dirs)
			{
				parent->head_children_dirs = del_dir->next;
				if (del_dir->head_children_dirs != NULL)
					free_dirs(del_dir->head_children_dirs);
				if (del_dir->head_children_files != NULL)
					free_files(del_dir->head_children_files);
				free(del_dir->name);
				free(del_dir);
				return;
			}
			if (del_dir->next == NULL)
			{
				Dir *aux = parent->head_children_dirs;
				while (aux->next != del_dir)
					aux = aux->next;
				aux->next = NULL;
				if (del_dir->head_children_dirs != NULL)
					free_dirs(del_dir->head_children_dirs);
				if (del_dir->head_children_files != NULL)
					free_files(del_dir->head_children_files);
				free(del_dir->name);
				free(del_dir);
				return;
			}
			Dir *aux = parent->head_children_dirs;
			while (aux->next != del_dir)
				aux = aux->next;
			aux->next = del_dir->next;
			if (del_dir->head_children_dirs != NULL)
				free_dirs(del_dir->head_children_dirs);
			if (del_dir->head_children_files != NULL)
				free_files(del_dir->head_children_files);
			free(del_dir->name);
			free(del_dir);
			return;
		}
		del_dir = del_dir->next;
	}
	printf("Could not find the dir\n");
}

void cd(Dir **target, char *name)
{
	/*daca argumentul este ".." iar directorul curent e home nu fac nimic*/
	if (strcmp(name, "..") == 0)
	{
		if (strcmp((*target)->name, "home") == 0)
			return;
		else
		{
			(*target) = (*target)->parent;
			return;
		}
	}
	/*parcurg lista de dir si schimb directorul curent*/
	Dir *curr_dir = (*target)->head_children_dirs;
	while (curr_dir != NULL)
	{
		if (strcmp(curr_dir->name, name) == 0)
		{
			(*target) = curr_dir;
			return;
		}
		curr_dir = curr_dir->next;
	}
	/*daca se ajunge aici nu s a gasit niciun director*/
	printf("No directories found!\n");
}

char *pwd(Dir *target)
{
	/*initializez un string in care voi construi calea*/
	char *path = malloc(strlen(target->name) + 2);

	/*adaug /numele_directorului_curent in sir*/
	strcpy(path, "/");
	strcat(path, target->name);
	Dir *curr_dir = target->parent;
	while (curr_dir != NULL)
	{
		int length = strlen(path);
		/*aloc si realoc memorie pt un string, terminatorul lui de sir si "/*/
		char *aux_str = malloc(strlen(curr_dir->name) + length + 2);
		path = realloc(path, strlen(curr_dir->name) + length + 2);
		/*ma folosesc de stringul auxiliar poate a concatena pe rand cate un director
		  la stringul final*/
		strcpy(aux_str, "/");
		strcat(aux_str, curr_dir->name);
		strcat(aux_str, path);
		strcpy(path, aux_str);
		free(aux_str);
		curr_dir = curr_dir->parent;
	}
	return path;
}

/*eliberez memoria si inchei aplicatia*/
void stop(Dir *target)
{
	free_dirs(target);
	exit(0);
}

void tree(Dir *target, int level)
{
	/*pentru fiecare director ce intra in functie afisez spatiile date
	  de level apoi afisez directorul*/
	if (target == NULL)
		return;
	int l = level;
	while (l)
	{
		printf("    ");
		l--;
	}
	printf("%s\n", target->name);
	/*apoi merg mai adanc in ierarhia de directoare*/
	tree(target->head_children_dirs, level + 1);
	/*abia ca mai apoi sa afisez fisierele
	  dupa ce se ajunge la null si se intoarce*/
	File *curr_file = target->parent->head_children_files;
	while (curr_file)
	{
		int l = level;
		while (l)
		{
			printf("    ");
			l--;
		}
		printf("%s\n", curr_file->name);
		curr_file = curr_file->next;
	}
	/*iar la final merg mai departe in lista de directoare initiala*/
	tree(target->next, level);
}

void mv(Dir *parent, char *oldname, char *newname)
{
	int ok = 0;
	Dir *curr_dir = parent->head_children_dirs;
	File *curr_file = parent->head_children_files;
	Dir *aux_dir = NULL;
	File *aux_file = NULL;

	/* verific daca oldname nu se gaseste in lista de directoare
	  sau daca newname nu exista deja si afiseaz un mesaj corespunzator*/
	while (curr_dir)
	{
		if (strcmp(curr_dir->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		if (strcmp(curr_dir->name, oldname) != 0 && curr_dir->next == NULL && ok == 0)
		{
			printf("File/Director not found\n");
			return;
		}
		if (strcmp(curr_dir->name, oldname) == 0)
		{
			/*in caz ca oldaname exista si e director pastrez un pointer*/
			aux_dir = curr_dir;
			ok = 1;
		}
		curr_dir = curr_dir->next;
	}
	while (curr_file)
	{
		if (strcmp(curr_file->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		if (strcmp(curr_file->name, oldname) != 0 && curr_file->next == NULL && ok == 0)
		{
			printf("File/Director not found\n");
			return;
		}
		if (strcmp(curr_file->name, oldname) == 0)
		{
			/*in caz ca oldaname exista si e fisier pastrez un pointer*/
			aux_file = curr_file;
			ok = 1;
		}
		curr_file = curr_file->next;
	}

	/*daca instanta de inlocuit este fisier il sterg si refac un
	  nou fisier folosinf functiile deja implementata*/
	if (aux_file != NULL)
	{
		rm(parent, oldname);
		touch(parent, newname);
		return;
	}

	/*daca e director avem 3 cazuri: */
	if (aux_dir != NULL)
	{
		Dir *temp = parent->head_children_dirs;
		/*directorul este capul listei => refac legaturile, fac un director nou
		  caruia ii atribui legaturile cu headul fisierelor si al directoarelor
		  directorului de sters apoi ii eliberez memoria*/
		if (aux_dir = parent->head_children_dirs)
		{
			parent->head_children_dirs = aux_dir->next;
			mkdir(parent, newname);
			while (temp->next != NULL)
				temp = temp->next;
			temp->head_children_dirs = aux_dir->head_children_dirs;
			temp->head_children_files = aux_dir->head_children_files;
			free(aux_dir->name);
			free(aux_dir);
			return;
		}
		/*daca directorul se afla la finalul listei doar il redenumesc*/
		if (aux_dir->next = NULL)
		{
			aux_dir->name = realloc(aux_dir->name, strlen(newname) + 1);
			strcpy(aux_dir->name, newname);
			return;
		}
		/*daca directorul se afla undeva la mijloc refac legaturile, fac un 
		director nou caruia ii atribui legaturile cu headul fisierelor si al 
		directoarelor directorului de sters apoi ii eliberez memoria */
		while (temp->next != aux_dir)
			temp = temp->next;
		temp->next = aux_dir->next;
		mkdir(parent, newname);
		while (temp->next != NULL)
			temp = temp->next;
		temp->head_children_dirs = aux_dir->head_children_dirs;
		temp->head_children_files = aux_dir->head_children_files;
		free(aux_dir->name);
		free(aux_dir);
	}
	return;
}

/*functie care sparge o linie in comanda + parametri*/
void read(Dir **parent, char *str)
{
	char *comanda = strsep(&str, " ");
	if(strncmp(comanda, "mkdir", 5) == 0){
		char *param = strsep(&str, " ");
		mkdir(*parent, param);
		return;
	}
	if(strncmp(comanda, "touch", 5) == 0){
		char *param = strsep(&str, " ");
		touch(*parent, param);
		return;
	}
	if(strncmp(comanda, "ls", 2) == 0){
		ls(*parent);
		return;
	}
	if(strncmp(comanda, "rmdir", 5) == 0){
		char *param = strsep(&str, " ");
		rmdir(*parent, param);
		return;
	}
	if(strncmp(comanda, "rm", 2) == 0){
		char *param = strsep(&str, " ");
		rm(*parent, param);
		return;
	}
	if(strncmp(comanda, "cd", 2) == 0){
		char *param = strsep(&str, " ");
		cd(parent, param);
		return;
	}
	if(strncmp(comanda, "pwd", 3) == 0){
		char *s = pwd(*parent);
		printf("%s\n", s);
		free(s);
		return;
	}
	if(strncmp(comanda, "tree", 4) == 0){
		tree((*parent)->head_children_dirs, 0);
		return;
	}
	if(strncmp(comanda, "mv", 2) == 0){
		char *param1 = strsep(&str, " ");
		char *param2 = strsep(&str, " ");
		mv(*parent, param1, param2);
		return;
	}
}

int main()
{
	/*initializez directorul home*/
	Dir *home = getNewDir("home");
	Dir *curr_dir = home;
	char *text, *token1, *token2, *ptr;
	text = (char*)malloc(MAX_INPUT_LINE_SIZE + 1);

	do
	{
		/*citesc cate o linie de la stdin si o trimit la "read"*/
		fgets(text, MAX_INPUT_LINE_SIZE + 1, stdin);
		text[strlen(text) - 1] = '\0';
		read(&curr_dir, text);

	} while (strcmp(text, "stop") != 0);

	/*eliberez textul si inchid aplicatia*/
	free(text);
	stop(home);

	return 0;
}