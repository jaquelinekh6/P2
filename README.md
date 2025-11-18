PAV - P2: detección de actividad vocal (VAD)
============================================

Jaqueline Khalioulline &
Oscar Pelagio 
----------------------------------------------

Esta práctica se distribuye a través del repositorio GitHub [Práctica 2](https://github.com/albino-pav/P2),
y una parte de su gestión se realizará mediante esta web de trabajo colaborativo.  Al contrario que Git,
GitHub se gestiona completamente desde un entorno gráfico bastante intuitivo. Además, está razonablemente
documentado, tanto internamente, mediante sus [Guías de GitHub](https://guides.github.com/), como
externamente, mediante infinidad de tutoriales, guías y vídeos disponibles gratuitamente en internet.


Inicialización del repositorio de la práctica.
----------------------------------------------

Para cargar los ficheros en su ordenador personal debe seguir los pasos siguientes:

*	Abra una cuenta GitHub para gestionar esta y el resto de prácticas del curso.
*	Cree un repositorio GitHub con el contenido inicial de la práctica (sólo debe hacerlo uno de los
	integrantes del grupo de laboratorio, cuya página GitHub actuará de repositorio central del grupo):
	-	Acceda la página de la [Práctica 2](https://github.com/albino-pav/P2).
	-	En la parte superior derecha encontrará el botón **`Fork`**. Apriételo y, después de unos segundos,
		se creará en su cuenta GitHub un proyecto con el mismo nombre (**P2**). Si ya tuviera uno con ese 
		nombre, se utilizará el nombre **P2-1**, y así sucesivamente.
*	Habilite al resto de miembros del grupo como *colaboradores* del proyecto; de este modo, podrán
	subir sus modificaciones al repositorio central:
	-	En la página principal del repositorio, en la pestaña **:gear:`Settings`**, escoja la opción 
		**Collaborators** y añada a su compañero de prácticas.
	-	Éste recibirá un email solicitándole confirmación. Una vez confirmado, tanto él como el
		propietario podrán gestionar el repositorio, por ejemplo: crear ramas en él o subir las
		modificaciones de su directorio local de trabajo al repositorio GitHub.
*	En la página principal del repositorio, localice el botón **Branch: master** y úselo para crear
	una rama nueva con los primeros apellidos de los integrantes del equipo de prácticas separados por
	guion (**fulano-mengano**).
*	Todos los miembros del grupo deben realizar su copia local en su ordenador personal.
	-	Copie la dirección de su copia del repositorio apretando en el botón **Clone or download**.
		Asegúrese de usar *Clone with HTTPS*.
	-	Abra una sesión de Bash en su ordenador personal y vaya al directorio **PAV**. Desde ahí, ejecute:

		```.sh
		git clone dirección-del-fork-de-la-práctica
		```

	-	Vaya al directorio de la práctica `cd P2`.

	-	Cambie a la rama **fulano-mengano** con la orden:

		```.sh
		git checkout fulano-mengano
		```

*	A partir de este momento, todos los miembros del grupo de prácticas pueden trabajar en su directorio
	local del modo habitual, usando el repositorio remoto en GitHub como repositorio central para el trabajo colaborativo
	de los distintos miembros del grupo de prácticas o como copia de seguridad.
	-	Puede *confirmar* versiones del proyecto en su directorio local con las órdenes siguientes:

		```.sh
		git add .
		git commit -m "Mensaje del commit"
		```

	-	Las versiones confirmadas, y sólo ellas, se almacenan en el repositorio y pueden ser accedidas en cualquier momento.

*	Para interactuar con el contenido remoto en GitHub es necesario que los cambios en el directorio local estén confirmados.

	-	Puede comprobar si el directorio está *limpio* (es decir, si la versión actual está confirmada) usando el comando
		`git status`.

	-	La versión actual del directorio local se sube al repositorio remoto con la orden:

		```.sh
		git push
		```

		*	Si el repositorio remoto contiene cambios no presentes en el directorio local, `git` puede negarse
			a subir el nuevo contenido.

			-	En ese caso, lo primero que deberemos hacer es incorporar los cambios presentes en el repositorio
				GitHub con la orden `git pull`.

			-	Es posible que, al hacer el `git pull` aparezcan *conflictos*; es decir, ficheros que se han modificado
				tanto en el directorio local como en el repositorio GitHub y que `git` no sabe cómo combinar.

			-	Los conflictos aparecen marcados con cadenas del estilo `>>>>`, `<<<<` y `====`. Los ficheros correspondientes
				deben ser editados para decidir qué versión preferimos conservar. Un editor avanzado, del estilo de Microsoft
				Visual Studio Code, puede resultar muy útil para localizar los conflictos y resolverlos.

			-	Tras resolver los conflictos, se ha de confirmar los cambios con `git commit` y ya estaremos en condiciones
				de subir la nueva versión a GitHub con el comando `git push`.


	-	Para bajar al directorio local el contenido del repositorio GitHub hay que ejecutar la orden:

		```.sh
		git pull
		```
	
		*	Si el repositorio local contiene cambios no presentes en el directorio remoto, `git` puede negarse a bajar
			el contenido de este último.

			-	La resolución de los posibles conflictos se realiza como se explica más arriba para
				la subida del contenido local con el comando `git push`.



*	Al final de la práctica, la rama **fulano-mengano** del repositorio GitHub servirá para remitir la
	práctica para su evaluación utilizando el mecanismo *pull request*.
	-	Vaya a la página principal de la copia del repositorio y asegúrese de estar en la rama
		**fulano-mengano**.
	-	Pulse en el botón **New pull request**, y siga las instrucciones de GitHub.


Entrega de la práctica.
-----------------------

Responda, en este mismo documento (README.md), los ejercicios indicados a continuación. Este documento es
un fichero de texto escrito con un formato denominado _**markdown**_. La principal característica de este
formato es que, manteniendo la legibilidad cuando se visualiza con herramientas en modo texto (`more`,
`less`, editores varios, ...), permite amplias posibilidades de visualización con formato en una amplia
gama de aplicaciones; muy notablemente, **GitHub**, **Doxygen** y **Facebook** (ciertamente, :eyes:).

En GitHub. cuando existe un fichero denominado README.md en el directorio raíz de un repositorio, se
interpreta y muestra al entrar en el repositorio.

Debe redactar las respuestas a los ejercicios usando Markdown. Puede encontrar información acerca de su
sintáxis en la página web [Sintaxis de Markdown](https://daringfireball.net/projects/markdown/syntax).
También puede consultar el documento adjunto [MARKDOWN.md](MARKDOWN.md), en el que se enumeran los
elementos más relevantes para completar la redacción de esta práctica.

Recuerde realizar el *pull request* una vez completada la práctica.

Ejercicios
----------

### Etiquetado manual de los segmentos de voz y silencio

- Etiquete manualmente los segmentos de voz y silencio del fichero grabado al efecto. Inserte, a 
  continuación, una captura de `wavesurfer` en la que se vea con claridad la señal temporal, el contorno de
  potencia y la tasa de cruces por cero, junto con el etiquetado manual de los segmentos.

![alt text](<img/captura_wavesurfer.png>)

>| Inicio   | Fin     | Estado |
>|------------|-------------|--------|
>| 0.0000000  | 0.5547286   | S      |
>| 0.5547286  | 1.1950439   | V      |
>| 1.1950439  | 1.3535378   | S      |
>| 1.3535378  | 2.1523470   | V      |
>| 2.1523470  | 2.5454118   | S      |
>| 2.5454118  | 3.6295100   | V      |
>| 3.6295100  | 3.9908760   | S      |
>| 3.9908760  | 5.8420846   | V      |



- A la vista de la gráfica, indique qué valores considera adecuados para las magnitudes siguientes:

	* Incremento del nivel potencia en dB, respecto al nivel correspondiente al silencio inicial, para estar seguros de que un segmento de señal se corresponde con voz.

		>Como vemos en la imagen, necesitamos un incremento de **20 dBs** para asegurarnos que hay un cambio a voz. 


	* Duración mínima razonable de los segmentos de voz y silencio.

		>Vemos que son segmentos de **200 ms** como mínimo. 


	* ¿Es capaz de sacar alguna conclusión a partir de la evolución de la tasa de cruces por cero?

		
		>La tasa de cruces por cero **disminuye cuando estamos hablando**, ya que la mayoria del tiempo emitimos sonidos sonoros los cuales podemos considerar ciertamente periódicos 
		>
		>**El ruido blanco**, sin embargo, **tiene una tasa de cruces por 0 muy elevado**, ya que es una señal muy aleaotoria. Sin embargo, no nos es de gran ayuda dado que los sonidos como la **"s" o la "f" también tienen una tasa muy alta**, y en este caso nos podria llevar a error. 
		>
		>Sería quizás un segundo discriminante interesante, en caso de que hubiese dudas clasificando únicamente partiendo de la potencia de la señal.

		


### Desarrollo del detector de actividad vocal

- Complete el código de los ficheros de la práctica para implementar un 	detector de actividad vocal en
  tiempo real tan exacto como sea posible. Tome como objetivo la maximización de la puntuación-F `TOTAL`.

  
  
  >Para maximizar la puntuación F TOTAL, se ha implementado un **FSA con estados transitorios** (ST_POSIBLE_V, ST_POSIBLE_S) y se ha realizado un barrido de parámetros para optimizar las constantes de potencia y tiempo.Los parámetros óptimos encontrados son: 
  >
	>| Constante | Valor |
	>|----------|----------|
	>| Alpha 1    | 10.25   |
	>| Alpha 2    | 0.75   |
	>| N_POSIBLES    | 2   |
	>| MIN_SEGMNENT_FRAMES    | 10   |
  


- Inserte una gráfica en la que se vea con claridad la señal temporal, el etiquetado manual y la detección
  automática conseguida para el fichero grabado al efecto. 

  ![alt text](<img/figura 2.png>)
	
	>La el primer .lab es el que ha generado nuestro codigo y el segundo es el ground truth. 




- Explique, si existen. las discrepancias entre el etiquetado manual y la detección automática.
	
	>Nuestro audio en concreto no tiene mucho espacio de silencio, son dos frases muy seguidas, y como hemos ajustado el código a un dataset con silencios más largos, en nuestro audio particular no funciona demasiado bien. 
	>
	>Sí que vemos por ejemplo que detecta correctamente el inicio de la frase. 
	

- Evalúe los resultados sobre la base de datos `db.v4` con el script `vad_evaluation.pl` e inserte a 
  continuación las tasas de sensibilidad (*recall*) y precisión para el conjunto de la base de datos (sólo
  el resumen).

  	>**Summary:**
	>
	>Recall V: 469.88/495.55 94.82%
	>
	>Precision V:469.88/528.21 88.96%   
	>
	>F-score V (2)  : 93.59%
	>
	>Recall S:262.84/321.17 81.84%   
	>
	>Precision S:262.84/288.51 91.10%   
	>
	>F-score S (1/2): 89.09%
	>
	>**-> TOTAL: 91.309%**
	>
	>Vemos que para esta base de datos hemos conseguido un resultado más que correcto. 


### Trabajos de ampliación

#### Cancelación del ruido en los segmentos de silencio

- Si ha desarrollado el algoritmo para la cancelación de los segmentos de silencio, inserte una gráfica en
  la que se vea con claridad la señal antes y después de la cancelación (puede que `wavesurfer` no sea la
  mejor opción para esto, ya que no es capaz de visualizar varias señales al mismo tiempo).

#### Gestión de las opciones del programa usando `docopt_c`

- Si ha usado `docopt_c` para realizar la gestión de las opciones y argumentos del programa `vad`, inserte
  una captura de pantalla en la que se vea el mensaje de ayuda del programa.


### Contribuciones adicionales y/o comentarios acerca de la práctica

- Indique a continuación si ha realizado algún tipo de aportación suplementaria (algoritmos de detección o 
  parámetros alternativos, etc.).

	>La contribución principal fue la **optimización exhaustiva** de los de potencia y tiempo del detector VAD:
	>
	>- Se determinó el valor óptimo de **$\alpha_1 = 10.25$ y $\alpha_2 = 0.75$** para los umbrales de detección.
	>
	>- Se validó que el uso de $N_{POSIBLES}=2$ y $MIN_{SEGMENT\_FRAMES}=10$ maximiza el balance entre sensibilidad y estabilidad.
	


- Si lo desea, puede realizar también algún comentario acerca de la realización de la práctica que
	considere de interés de cara a su evaluación.

	>La implementación del Autómata Finito de Estados (FSA) con estados transitorios (ST_POSIBLE_V/S) **ha demostrado ser considerable para la robustez del VAD.** 


### Antes de entregar la práctica

Recuerde comprobar que el repositorio cuenta con los códigos correctos y en condiciones de ser 
correctamente compilados con la orden `meson bin; ninja -C bin`. El programa generado (`bin/vad`) será
el usado, sin más opciones, para realizar la evaluación *ciega* del sistema.
