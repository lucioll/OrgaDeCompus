## Instrucciones para ejecutar el TP0 en NetBSD

* Ir a la carpeta del gxemul por medio de una terminal nueva.
* Ejecutar "./xgxemul -e 3max -d netbsd-pmax.img".
* Ingresar con el usuario root y pass orga6620.
* Abrir una nueva terminal y poner "sudo ifconfig lo:0 172.20.0.1"
* En la terminal con el gxemul ejecutar "ssh -R 2222:127.0.0.1:22 USER@172.20.0.1" donde dice USER corresponde a su nombre de usuario de linux. Pedira confirmarlo con la clave de ese usuario.
* En una terminal nueva escribiremos "ssh -p 2222 root@127.0.0.1" nos pedira la pass que es "orga6620".
* Si aparece algo como "Terminal type?" apretar ctrl + c.
* Ahora necesitamos copiar el tp a NETBSD para eso en otra terminal nueva de linux hacemos:
	 scp -P2222 -r /home/lucio/Documentos/Compus/_tp0-c1-2018/OrgaDeCompus/TP0 root@127.0.0.1:/home/  (Nos volvera a pedir la contraseña "orga6620")
 
* Una vez terminado el proceso de envio de archivos, en la terminal que tenemos el tunel a NETBSD, podemos ejecutar comandos como cd o ls para ir hasta donde esta el tp, en este caso home/TP0 y alli para compilar el tp se realiza:

     * gcc -c -ggdb -Wall main.c
     * gcc -c -ggdb -Wall contador_carac_palab_lineas.c
     * gcc main.o contador_carac_palab_lineas.o -o tp

* Ahora para correr el tp solo hacemos ./tp "comandos"

---

* Solucion al problema que aparece luego de escribir "ssh -R 2222:127.0.0.1:22 USER@172.20.0.1":
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
Someone could be eavesdropping on you right now (man-in-the-middle attack)!
It is also possible that the RSA host key has just been changed.
The fingerprint for the RSA key sent by the remote host is
52:c2:0a:e6:e5:16:75:2a:20:7d:87:68:62:21:b4:10.
Please contact your system administrator.
Add correct host key in /root/.ssh/known_hosts to get rid of this message.
Offending key in /root/.ssh/known_hosts:1
RSA host key for 172.20.0.1 has changed and you have requested strict checking.
Host key verification failed.
* Escribir en la consola con el gxemul corriendo cd ~/.ssh/
* Ejecutar el comando rm known_hosts
* Luego de ese paso ya se puede volver a intentar el comando "ssh -R 2222:127.0.0.1:22 USER@172.20.0.1" nos pedira confirmar algo y le daremos "yes".

