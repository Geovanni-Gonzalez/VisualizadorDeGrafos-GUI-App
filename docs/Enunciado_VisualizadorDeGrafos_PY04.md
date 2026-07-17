

Código: PY04
## Objetivo
Construir una aplicación que permita trabajar de forma gráfica con diferentes grafos para la
visualización de la información de estructuras distribuidas diversas.
## Implementación
Lineamientos para la construcción del sistema:
 El programa permite construir, editar, analizar, guardar y cargar estructuras tipo red
basadas en grafos, todo esto de forma gráfica, es decir con una interfaz basada en
ventanas y elementos accionables en pantalla, no por medio de la consola.
 Para construir, se debe permitir la creación de un nuevo grafo ya sea dirigido o no
dirigido, pesado o no pesado, al cual se le agregarán nuevos vértices y nuevas
conexiones.
 El grafo se puede editar mediante las mismas opciones anteriores, ya sea con el grafo
nuevo o uno recién cargado de archivo.
 El guardado y cargado de la estructura se hace mediante un archivo a disco duro con el
formato elegido por el grupo para guardar la información, este permite que la
estructura se almacene y posteriormente se pueda cargar nuevamente para trabajar en
otro momento.
 Las funciones de análisis permiten obtener la siguiente información:
- Generación de la matriz de caminos
- Búsqueda del camino más corto a partir de un solo vértice
- Todos los caminos más cortos entre cualquier par de vértices
- Construcción del árbol de expansión de coste mínimo
Todos los resultados deben representarse gráficamente en una forma que sea
gráficamente válida y no únicamente un homólogo de una impresión en pantalla en un
contenedor o campo de texto.
## Tecnología
El código fuente que se genere debe poder compilarse con la herramienta g++ para Linux Debian
10 o similares. Deben implementarse en todos los casos estructuras de datos dinámicas. El uso
de arreglos estáticos se limita únicamente a las matrices relacionadas con el análisis de la
estructura, las cuales deben ser temporales. Únicamente se permite el uso de librerías de
funciones estándar que no aporten estructuras de datos ya prefabricadas. En este proyecto se
permite el uso de las librerías y tipo de datos string.
Grupos de trabajo
Se debe desarrollar en grupos de 2 o 3 estudiantes. No se evaluará trabajo de grupos con otras
cantidades de miembros sin la justificación apropiada y previa aprobación del profesor. Cada

grupo debe indicar los miembros que lo componen a más tardar el martes 10 de mayo por
medio del espacio Proyecto 04 - Foro, para la configuración respectiva en la herramienta del
curso. Quienes no sean parte de un grupo reportado a esta fecha, serán asignados a un grupo de
forma aleatoria el día siguiente.
Datos administrativos
Este cuarto proyecto del semestre tiene un valor de 15%. La fecha límite es el 15 de junio, 2023,
a más tardar a las 5:00 PM hora del servidor. Si la fecha de entrega tiene una diferencia de x
horas con respecto a la fecha indicada, perderá 2x puntos del porcentaje base de calificación del
proyecto según lo indicado en los lineamientos del curso.
La documentación y código fuente debe encontrarse en el repositorio de código disponible en
https://git.mora.tk/ en un proyecto grupal, compartido al profesor, y a los estudiantes
asistentes del curso (Hansol Antay, @hros19 y Randall Corella @RandallC). En la asignación
actual en el sitio del curso debe agregar únicamente el enlace a este repositorio.
Serán válidos únicamente aquellas confirmaciones (commits) que se hagan anteriores a la fecha
y hora límite, a menos que el grupo indique que desea que se haga evalución de entrega tardía.
## Calificación
Se utilizará la rúbrica de evaluación de proyectos detallada en el documento correspondiente.
Tome en cuenta el documento con el estándar de programación adjunto para el rubro que
corresponde.
Cada grupo elegirá una hora de revisión dentro del espacio ofrecido por el profesor en donde
hará una demostración del producto generado, con particular atención a mostrar el
cumplimiento de los lineamientos de implementación en este enunciado.
Puntos adicionales
Se dará un máximo de 5 puntos porcentuales sobre la nota final del curso si se utiliza la librería
construida en el tercer proyecto de este curso para exportar el grafo construido. La puntuación
depende del criterio del profesor sobre la consecución del objetivo con la integración de la
librería.
## Recomendaciones
Algunos enlaces de interés para el tema de las interfaces gráficas de usuario en C/C++ para
## Linux:
 https://www.codeguru.com/cpp/cpp/creating-a-cc-gui-with-gtk.html
 https://www.gtkmm.org/en/
 https://doc.qt.io/qt-5/gettingstarted.html
¡Muchos éxitos con el trabajo! Empiecen con tiempo suficiente, analicen y diseñen la solución,
la programación se hace hasta tener claro el dominio del problema tanto como sea posible.