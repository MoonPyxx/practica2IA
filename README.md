# Práctica 2 de *Inteligencia Artificial*, curso 2023/2024 SPIRIT!!



### 5. Enlazar tu repositorio personal con el de la asignatura
Aunque tu repositorio y el de la asignatura (recuerda que los conocemos por *origin* y *upstream* respectivamente) sean independientes, nos va a interesar que estén enlazados. De esta forma, podrás aplicar fácilmente sobre tu repositorio (*origin*) cualquier actualización que los profesores realicemos en *upstream*. Para enlazarlos, ejecuta lo siguiente dentro de la carpeta de tu repositorio:

`git remote add upstream git@github.com:ugr-ccia-IA/practica2.git`


### Actualizar tu repositorio con cambios realizados en el de la asignatura
Una vez tengas los repositiorios enlazados, lo único que debes hacer para aplicar posibles cambios en el repositorio de la asignatura en tu repositorio (cambios de *upstream* en *origin*) es: `git pull upstream main`

Hacer esto no sobreescribirá tus avances en la implementación de la práctica, puesto que tú no deberías haber modificado ninguna parte del código diferente a la que se indica en el guión.

Si quieres que esos cambios también se guarden en github, a continuación ejecuta: `git push origin main`


> Si quieres saber más sobre Git y GitHub, en Internet existen multitud de recursos, incluidos videos y tutoriales. Para realizar esta práctica sólo necesitas lo básico (hacer commits), pero hay muchas cosas más que se pueden hacer con estas herramientas (uso de ramas, gestión de conflictos, etc.) 
El propio GitHub pone a tu disposición un [breve curso](https://classroom.github.com/a/W33pQ3pa) (en inglés) para aprender lo básico.

