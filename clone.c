#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <pomiar_czasu.h>

int zmienna_globalna=0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku( void* argument )
{
  zmienna_globalna++;

  int wynik; 
  wynik=execv("./program",NULL); 
  if(wynik==-1) 
    printf("Proces potomny nie wykonal programu\n");

  return 0;
}

int main()
{

  void *stos;
  pid_t pid;
  int i; 
  inicjuj_czas(); 
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  for(i=0;i<1000;i++){

    pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, NULL );
	waitpid(pid, NULL, __WCLONE);
  }
	
  free( stos );
  drukuj_czas();
  printf("%d\n" , zmienna_globalna);
}

