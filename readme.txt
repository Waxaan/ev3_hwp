Anleitung Git aufsetzen + Docker nutzen

##############
	Git
##############

-Auf https://github.com/ einen Account erstellen und Email verifizieren
-Mir Link zum Account schicken, damit ich euch freischalten kann


Installieren von GIT aus dem apt-Repository
>sudo apt install git-all 

+++Optional+++
	Zum signieren von commits
	>git config --global user.[name/email] "name/email"

Verzeichnis lokal klonen
>git clone https://github.com/Waxaan/ev3_hwp

Auf den Dev-branch wechseln 
>git checkout dev

Sicherstellen, dass alles aktuell ist
>git pull

DATEI ERSTELLEN/ÄNDERN

Datei zum commit hinzufügen
>git add [.] oder
>git add DATEI[N]

Commit bündeln und mit Message versehen
>git commit -m"[Was geändert wurde]"

Pushen
>git push


##############
 Build-chain
##############

komfortabel mit VSCode(mit ev3dev extension) [hier $] und einem Terminal-Fenster [hier >]

$scp path/to/main.c robot@ev3dev.local:/home/robot/ev3_hwp/
$password = maker

>ssh robot@ev3dev.local
>password = maker
>cd /home/robot/ev3_hwp/
>gcc main.c -lev3dev-c -o main && ./main







##############
	Docker
##############
Was ist Docker? 
NIEMAND WILL DOCKER!


Eine virtuelle Programmierumgebung, in der alle benötigten Librarys, compiler, etc vorliegen
und nichts per Hand nachinstalliert/aktuellsiert werden muss!


Installiere Docker vom apt-Repository
>sudo apt install docker.io


+++Optional+++
	Spart, falls $USER root rechte hat, das sudo vor eingabe von docker im terminal

	Füge die Gruppe "docker" hinzu
	sudo groupadd docker 

	Gib Gruppe docker $USER Rechte / oder root 
	sudo usermod -aG docker $USER

Starten des Standart hello-world von docker
>sudo docker run hello-world

Pull die ev3dev-Umgebung bzw das ev3dev Docker-image
>sudo docker pull ev3dev/debian-jessie-cross

Umbennen des Docker-images zu ev3cc
>sudo docker tag ev3dev/debian-jessie-cross ev3cc

	Ausgabe aller Verfügbaren docker images 
	>sudo docker images
	(sollte "hello-world, ev3cc, ev3dev/debian-jessie-cross" ausgeben)

Ausführen des Docker-images im Userverzeichnis
>docker run --rm -it -v /home/[user]/[path zum git]/:/src -w /src ev3cc

>arm-linux-gnueabi-gcc -o [output] [input].c



	BSPW: erstelle hello-world.c 
	>touch hello-world.c
	>nano hello-world.c 
		#include <stdio.h>

		int main(int argc, const char *argv[])
		{
		    printf("Hello World!\n");

		    return 0;
		}
	>CTRL-X , J, ENTER


+++++++++++++++++++++++++++++++++++++++
Nützliche Links
+++++++++++++++++++++++++++++++++++++++

Installation von docker 
	https://www.ev3dev.org/docs/tutorials/using-docker-to-cross-compile/

Datenüvertragung zum ev3
	https://www.ev3dev.org/docs/networking/

C-API von ev3dev
	https://github.com/in4lio/ev3dev-c