# Prótesis-actuada-con-control-miográfico
Creación de un brazo mecatrónico actuado mediante señales EMG adaptado con Machine Learning.

Los documentos que se incluyen aquí pertenecen al desarrollo de una prótesis de un brazo mecatrónico actuado mediante señales EMG. Se ha utilizado la API MyoBridge, y en base a ésta, se han utilizado sus detecciones de POSE por defecto incluídas en la misma. Pero también, se ha realizado el mismo código obteniendo la lectura de la pose a partir de las detecciones al aplicar un algoritmo de Machine Learnin.


El código que utiliza la detección de POSE por defecto está en la carpeta "Código Arduino con POSE" y el código que utiliza Machine Learning está en "Código Arduino con ML".


En la carpeta "Dataset" podemos encontrar el fichero "EMGCierreRelajacion.csv", que contiene los datos con los que ha sido entrenado el algoritmo Machine Learning.


La carpeta "Obtener ClasificadorCierre_h" está el fichero que ha sido ejecutado en Google Colab y que genera el fichero "ClasificadorCierre.h" que es utilizado en el código con Machine Learning y contiene la información del algoritmo entrenado.


Por último, la carpeta "Diseños 3D", contiene todos los diseños utilizados en la creación de la prótesis en formato .STL.

