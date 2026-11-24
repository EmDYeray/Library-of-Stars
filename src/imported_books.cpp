#include "imported_books.hpp"
#include "locale.hpp"

namespace biblioteca {
const std::array<ImportedBook,importedChildren+importedAdults>& importedBooks(){
    if(isEnglish())return importedBooksEnglish();
    using V=Illustration;
    static constexpr std::array<ImportedBook,importedChildren+importedAdults> books{{
        {{"El pequeño salto de una gran águila","Luna","Eagle aterriza y dos amigos pisan la Luna.","#615c8c",{{
            {"El Águila",
             "Desde la Tierra, una nave llamada Eagle viajaba hacia la Luna. Su nombre significa Águila en inglés. Tenía cuatro patas doradas y parecía una araña. Adentro iban Neil Armstrong y Buzz Aldrin, que esperaban poder bajar.",
             "Mientras ellos se preparaban, Michael Collins seguía dando vueltas alrededor de la Luna en la nave principal. Desde arriba esperaba el momento de volver a reunirse con sus compañeros. Al Águila todavía le faltaba posar las patas en el suelo."},
            {"Polvo gris",
             "El 20 de julio de 1969, el Águila encendió el motor para bajar despacio. Debajo se veía una llanura gris llamada Mar de la Tranquilidad. Allí las patas tocaron el polvo y la nave se quedó quieta.",
             "Neil habló por radio para avisar a la Tierra. El viaje había llegado hasta un lugar al que nadie había bajado antes. Por la ventanilla podían ver un suelo gris, lleno de cráteres, y pronto podrían caminar sobre él."},
            {"El primer paso",
             "Neil abrió la escotilla y bajó por la escalera. Cuando puso una bota en el polvo, dejó la primera huella de una persona en la Luna. Buzz bajó poco después. Con unos pasos, los dos empezaron a explorar aquel lugar tan silencioso.",
             "Como en la Luna pesaban menos, podían moverse a saltitos. Tomaron fotos, recogieron piedras en cajas y dejaron aparatos movidos por la luz del Sol. La Tierra recibiría noticias de aquellos aparatos después de que el Águila se fuera."},
            {"De vuelta a casa",
             "Después de pasar allí casi un día, Neil y Buzz regresaron por la escalera. Encendieron el motor de la cabina y despegaron. Abajo quedaron las cuatro patas doradas, firmes sobre el mismo polvo donde habían dejado sus huellas.",
             "En el espacio, Michael los esperaba en la nave principal. Los tres volvieron a la Tierra con las cajas de piedras. Allí podrían abrirlas y estudiar de cerca un pedacito del lugar donde había aterrizado el Águila."}
        }}},V::Eagle},
        {{"La aventura del valiente Intrepid","Luna","Pete y Alan visitan un robot que ya vivía en la Luna.","#715249",{{
            {"Otra visita",
             "En noviembre de 1969, Pete Conrad y Alan Bean viajaron a la Luna en una nave llamada Intrepid. Llevaba cuatro patas largas en la parte de abajo y una cabina arriba. Allí iban los dos astronautas durante el descenso.",
             "El Intrepid bajó en el Océano de las Tormentas. Aquel océano era una llanura de polvo y piedras. Las patas sostuvieron la nave como las de una mesa. La parte de arriba les serviría para regresar."},
            {"La caja de ciencia",
             "En el primer paseo, Pete y Alan abrieron la caja de experimentos ALSEP. Sacaron sus aparatos y los instalaron en el polvo gris. Uno de ellos podía sentir cuando el suelo temblaba y enviar el aviso a la Tierra.",
             "Al apuntar la cámara de televisión al Sol, la luz dañó la cámara. Los astronautas siguieron su paseo. Les quedaban otras tareas y un segundo recorrido para visitar a un robot que había llegado antes que ellos."},
            {"Un robot dormido",
             "En el segundo paseo caminaron hasta Surveyor 3, un robot que llevaba años en la Luna. Se acercaron a él y desmontaron algunas piezas. Querían llevarlas a la Tierra para estudiar cómo habían cambiado allí.",
             "También recogieron piedras y usaron una lámina para atrapar partículas que venían del Sol. Cuando volvieron al Intrepid, llevaban materiales del suelo lunar y pequeñas piezas que habían pasado años expuestas en aquel lugar."},
            {"Un golpe en la Luna",
             "Para partir, Pete y Alan encendieron el motor de la cabina. Las patas se quedaron en la Luna. Cuando los dos ya estaban en la nave de regreso, soltaron la cabina vacía para que bajara de nuevo.",
             "Al caer, la cabina hizo temblar el suelo. El aparato de la primera caminata registró el golpe y el equipo de la Tierra recibió la noticia. Desde lejos pudieron saber que su caja de ciencia seguía trabajando."}
        }}},V::Intrepid},
        {{"El secreto del Halcón Dorado","Luna","Un coche plegable, un martillo y una pluma viajan en Falcon.","#5c7086",{{
            {"Un halcón sin alas",
             "Falcon quiere decir Halcón. Esta nave no tenía alas, pero tenía cuatro patas doradas y zapatos redondos para apoyarse en el polvo lunar. En julio de 1971 viajaron en ella los astronautas David Scott y James Irwin.",
             "Además de sus herramientas, llevaban un coche eléctrico que se doblaba para caber en la nave. Guardaron también una estación científica. Al llegar, tendrían que sacar las dos cosas y prepararlas en el suelo."},
            {"Montañas de la Luna",
             "Falcon bajó al pie de los montes Apeninos de la Luna. Al abrirse la compuerta, David y James desplegaron su coche plegable. Con las ruedas en el suelo, por fin podían salir a recorrer el paisaje.",
             "Viajaron entre cráteres y recogieron rocas. Antes de irse, dejaron un aparato para estudiar temblores. El coche les permitió recorrer más terreno, y la estación seguiría enviando noticias después de la visita."},
            {"El martillo y la pluma",
             "David levantó un martillo con una mano y una pluma con la otra. Delante de la cámara, abrió las dos manos a la vez. Los que vieran la grabación podrían comprobar cuál tocaba primero el polvo.",
             "En la Tierra, el aire frena mucho a la pluma. En la Luna casi no hay aire. La pluma y el martillo cayeron juntos y tocaron el suelo al mismo tiempo."},
            {"La despedida",
             "Al despedirse de la Luna, David y James encendieron el motor de la cabina de Falcon. El coche, las cuatro patas y la estación se quedaron junto a las huellas que habían hecho los astronautas.",
             "Durante años, la estación siguió enviando noticias a la Tierra. Mucho después, otras naves fotografiaron el lugar desde arriba. Incluso desde el espacio podían verse marcas de aquella visita."}
        }}},V::Falcon},
        {{"Kepler y K2: en busca de nuevos mundos","Estrellas","Un telescopio busca planetas mirando la luz de las estrellas.","#424d86",{{
            {"Mirar con paciencia",
             "En 2009 enviaron al espacio un telescopio llamado Kepler. Su cámara podía mirar miles de estrellas a la vez. Los científicos le habían encargado buscar mundos que giraran alrededor de estrellas muy lejanas.",
             "Kepler guardaba cuánto brillaba cada estrella. A veces una perdía un poco de luz y luego volvía a brillar como antes. De todos los puntitos del cielo, esos cambios tan pequeños eran los que buscaba."},
            {"Un planeta escondido",
             "Imagina que alguien pasa delante de una lámpara. Durante un instante ves un poco menos de luz. Algo parecido pasa cuando un planeta cruza delante de su estrella desde donde lo mira Kepler.",
             "Si la luz baja a intervalos parecidos, puede haber un planeta dando vueltas. Kepler anotaba esas bajadas para que el equipo pudiera estudiarlas. Así encontró pistas de mundos que estaban demasiado lejos para fotografiarlos directamente."},
            {"La segunda aventura",
             "Dos ruedas que ayudaban a Kepler a apuntar dejaron de funcionar. Sin ellas le costaba quedarse mirando al mismo sitio. Los científicos buscaron una forma de volver a equilibrar el telescopio.",
             "Con las ruedas que quedaban y el empujón de la luz del Sol, Kepler pudo mirar otras partes del cielo. Esa segunda etapa se llamó K2. Cada cierto tiempo le tocaba apuntar a un grupo nuevo de estrellas."},
            {"Lo que quedó",
             "Kepler y K2 reunieron medidas de muchos mundos. Algunos eran grandes y otros pequeños. También vieron que alrededor de ciertas estrellas daba vueltas más de un planeta, como pasa en nuestro sistema solar.",
             "En 2018 se acabó el combustible de Kepler. Sus medidas quedaron guardadas. Aún pueden servir para hacer preguntas nuevas, como qué mundos tienen un tamaño parecido al de la Tierra y cuáles merecen mirarse otra vez."}
        }}},V::Kepler},
        {{"Pioneer 10: el pequeño viajero","Júpiter","Una nave cruza el cinturón de asteroides y envía imágenes de Júpiter.","#8a553e",{{
            {"Una nave con antena",
             "Pioneer 10 salió de la Tierra en 1972. Llevaba cables, instrumentos y un gran plato que era su antena. Por ella podría contarle a su equipo lo que viera al viajar hacia los planetas más lejanos.",
             "Poco a poco la Tierra quedó reducida a un punto azul. Delante había una región llena de asteroides y, más lejos, Júpiter. La nave tendría que mandar sus noticias desde distancias cada vez mayores."},
            {"Rocas en el camino",
             "Pioneer se internó en el cinturón de asteroides. Allí muchas rocas de distintos tamaños giran alrededor del Sol. El equipo quería averiguar si la nave podía cruzar aquella región con sus instrumentos en buen estado.",
             "La nave pasó entre rocas muy separadas. Siguió avanzando y pudo mandar noticias a la Tierra. Al otro lado del cinturón la esperaba un mundo mucho más grande que cualquiera de aquellas piedras."},
            {"Júpiter de cerca",
             "En diciembre de 1973, Pioneer pasó cerca de Júpiter. Vio bandas de nubes de distintos colores y una mancha roja enorme. Desde cerca pudo mirar detalles que se perdían cuando observaban el planeta desde la Tierra.",
             "Pioneer tomó imágenes y reunió medidas durante el encuentro. Después las mandó usando su gran antena. Mientras las personas estudiaban las fotos, el pequeño viajero seguía alejándose hacia una parte más oscura del espacio."},
            {"La voz lejana",
             "Pasaron los años y la antena de la Tierra cada vez oía más débil a Pioneer. Al final, sus mensajes dejaron de llegar. La nave siguió viajando después de que terminara el contacto.",
             "Con ella quedó una placa de metal. Unos dibujos cuentan de dónde vino y cómo es la gente que la hizo. Aunque su voz se apagó, la placa sigue siendo una forma de contar su historia."}
        }}},V::Pioneer},
        {{"Pioneer 10: el viajero que soñaba","Júpiter","Un relato sobre la placa que lleva un recuerdo de la Tierra.","#7b6952",{{
            {"Un punto azul",
             "En este cuento, Pioneer mira hacia atrás mientras viaja. Los océanos, las montañas y los bosques se vuelven un único punto azul. Su destino está adelante, mucho más allá de Marte.",
             "El pequeño viajero atraviesa una región llena de asteroides, rocas que giran alrededor del Sol. Cuando sale de allí, aún lleva sus instrumentos y su antena listos para el encuentro con Júpiter."},
            {"Una ventana a Júpiter",
             "Júpiter ocupa cada vez más espacio delante de la nave. Al pasar cerca, Pioneer toma imágenes de sus franjas y su gran tormenta roja. También reúne medidas para quienes esperan sus noticias.",
             "En la Tierra, cada imagen abre una pequeña ventana al planeta gigante. Pioneer y su equipo todavía pueden comunicarse. Sin embargo, la nave ya se está preparando para seguir por un camino que la alejará durante años."},
            {"Una carta de metal",
             "Antes de salir, el equipo había colocado en Pioneer una placa dorada. En ella dejó dibujos de dos personas y pistas sobre el lugar de donde salió la nave. Está pensada para viajar con ella.",
             "Con el tiempo, las señales llegan más débiles y en 2003 se recibe la última. Los dibujos siguen allí. En el cuento, alguien podría encontrarse algún día con aquella carta y preguntarse quién la envió."},
            {"Un viaje sin fin",
             "Pioneer sigue adelante con su placa. Si alguien la encontrara, tal vez intentaría leer sus líneas. Para quienes la enviaron, lleva un recuerdo de la Tierra, el planeta que desde lejos parecía una lucecita azul.",
             "Mientras la nave se aleja, otras personas siguen mirando el cielo. Las imágenes que envió ya les han mostrado cómo era Júpiter de cerca. El viaje dejó preguntas para quienes quisieran continuar la exploración."}
        }}},V::Pioneer},
        {{"El robot que escuchó a Marte","Marte","Un robot escucha los temblores para descubrir cómo es Marte por dentro.","#995143",{{
            {"Robotito llega a Marte",
             "Marte era rojo como una manzana y parecía guardar silencio. Para escuchar lo que ocurría bajo su suelo, construyeron un robot llamado InSight. En el cuento le diremos Robotito. Viajó meses en una nave con patas.",
             "En 2018 bajó despacito, como una mariposa. Cuando estuvo firme, abrió sus paneles para recibir Sol y estiró un brazo. Con él puso sobre el suelo un aparato redondo que usaría para escuchar."},
            {"La oreja de Robotito",
             "El aparato se llama sismómetro. Es como una oreja para el suelo. Puede sentir temblores tan pequeños que se te escaparían si pusieras una mano sobre la tierra.",
             "Robotito se quedó quieto. Registró temblores, incluso cuando una roca del espacio chocó contra Marte. Mandó las señales a la Tierra. Allí se veían como líneas con subidas y bajadas, parecidas a latidos."},
            {"Capas escondidas",
             "Los temblores viajan dentro de Marte. Su movimiento cambia cuando pasan de una capa a otra. Al mirar las señales, el equipo pudo hacerse una idea de lo que había bajo el suelo.",
             "Era como pegar la oreja a una pared para averiguar qué hay del otro lado. Gracias a aquellas pistas pudieron imaginar cómo estaban apiladas las capas de Marte, como las de un pastel."},
            {"Un último mensaje",
             "El polvo se posó sobre Robotito como una cobija naranja. Sus paneles recibían cada vez menos luz y en 2022 dejó de mandar mensajes. Para entonces, los científicos ya habían guardado las señales que escuchó.",
             "Tiempo después volvieron a abrir sus libretas y encontraron pistas de grandes trozos de roca escondidos dentro de Marte. Robotito seguía quieto bajo el polvo. Sus señales aún ayudaban a hacer nuevas preguntas."}
        }}},V::Insight},
        {{"Los gemelos del espacio","Marte","Dos robots con ruedas buscan piedras que guarden pistas de agua.","#805345",{{
            {"Dos robots iguales",
             "Spirit y Opportunity se parecían como dos gemelos. Cada uno tenía seis ruedas, un brazo largo y ojos de cámara para acercarse a las piedras. Eran más o menos del tamaño de un carrito de supermercado.",
             "Salieron en naves distintas hacia Marte. Cuando llegaron, ambos llevaban la misma pregunta. ¿Habría corrido agua alguna vez por aquel mundo lleno de polvo rojo? Para responderla, tendrían que mirar el suelo muy despacio."},
            {"Dos lugares distintos",
             "Spirit abrió un paracaídas y rebotó en bolsas de aire, como un balón. Al quedarse quieto, abrió sus paneles y miró el cráter Gusev. Tal vez allí había existido un lago mucho tiempo atrás.",
             "Opportunity bajó al otro lado de Marte, en una llanura. Encontró bolitas parecidas a arándanos entre las piedras. Desde dos lugares distintos, los gemelos empezaron a enviar fotos del suelo a la Tierra."},
            {"Pistas de agua",
             "Opportunity usó su brazo para mirar las bolitas muy de cerca. Estaban hechas de un mineral. Juntas con otras piedras, dejaban pistas de que allí había habido agua hace muchísimo tiempo.",
             "Spirit encontró otras pistas en las rocas de su cráter. En la Tierra juntaron las noticias de los dos. Las piedras, igual que un cuaderno viejo, contaban algo de cómo había sido Marte antes."},
            {"Huellas que quedan",
             "Spirit quedó atrapado en la arena. Opportunity siguió recorriendo Marte durante años, hasta que una tormenta enorme cubrió el cielo. Sus paneles ya no podían recoger la luz que necesitaba para mandar mensajes.",
             "Los dos quedaron quietos en Marte, cubiertos de polvo rojo. Otros robots, como Perseverance, llegaron después. Sus equipos pudieron aprender de las fotos y de todo lo que los gemelos habían descubierto sobre las piedras."}
        }}},V::Rovers},
        {{"1969-059C: crónica del Águila","Historia de misión","Dos etapas, muestras de contingencia y el programa EASEP.","#555c67",{{
            {"Un módulo en dos etapas",
             "El Eagle de Apolo 11 separaba dos funciones. La etapa inferior llevaba el motor de descenso y las patas, que distribuían la carga al tocar el suelo. La superior protegía a Armstrong y Aldrin durante la estancia y tenía su propio motor de ascenso. Collins permaneció en órbita en el módulo de mando.\nEsta división evitaba volver a levantar el equipo que solo se necesitaba para aterrizar.",
             "El 20 de julio de 1969 el Eagle se posó en el Mar de la Tranquilidad. La maniobra final exigió ajustar el punto de contacto para evitar terreno irregular. Aterrizar significaba llevar el empuje y la orientación a condiciones compatibles con el contacto de las patas.\nLa comunicación entre la nave, la órbita y la estación terrestre permitió fijar la hora y el estado del vehículo."},
            {"Una muestra antes que nada",
             "Armstrong recogió primero una muestra de contingencia. La razón era operativa: una salida prematura todavía podía dejar a la misión con una pequeña cantidad de material lunar. Después, con Aldrin, el trabajo se amplió a la fotografía, el muestreo y el registro del entorno.\nConviene distinguir una muestra escogida por urgencia de una colección recogida con tiempo y contexto.",
             "La expedición trajo unos 21.5 kg de roca y suelo. El valor científico dependía de poder relacionar piezas, fotografías y lugares de recogida. Una piedra aislada puede estudiarse en el laboratorio, pero su posición ayuda a reconstruir cómo llegó al sitio.\nEn el suelo quedaron también equipos de observación y un reflector para experimentos de distancia."},
            {"Un día para el EASEP",
             "El EASEP agrupó experimentos tempranos de superficie. Entre ellos había un sismómetro pasivo que registraba vibraciones. El instrumento debía distinguir movimientos del suelo de perturbaciones asociadas a la nave y a su propio entorno. Los paneles solares condicionaban cuándo podía trabajar.\nUna serie temporal que se interrumpe durante la noche lunar tiene huecos que cuentan al interpretar los datos.",
             "El registro se vio limitado por la alimentación y por las condiciones térmicas. Cuando un equipo pierde energía, la ausencia de mediciones no demuestra que cesaran los fenómenos que medía. El diseño de otras expediciones buscó aumentar la continuidad de los instrumentos.\nLa toma de datos es una parte del experimento. El calendario de funcionamiento y las fallas también se documentan."},
            {"Ascenso y encuentro",
             "Tras casi veintidós horas sobre la superficie, la cabina empleó la etapa inferior como base de lanzamiento. El motor de ascenso la llevó a una órbita desde la que podía encontrarse con Collins. Era una segunda maniobra independiente del alunizaje. Un vehículo que había sido capaz de descender aún tenía que demostrar que podía salir y reunirse con el módulo de mando.",
             "Armstrong y Aldrin trasladaron las muestras y el material de registro a la nave que regresaría a la Tierra. El Eagle vacío quedó separado. La etapa inferior permaneció en el lugar de aterrizaje.\nEl reparto de funciones se ve con claridad al final: una mitad se quedó en el suelo, otra hizo el viaje de vuelta a órbita y el módulo de mando completó el regreso."}
        }}},V::Eagle},
        {{"1969-099C: Océano de las Tormentas","Historia de misión","Alunizaje de precisión, Surveyor 3 y un impacto registrado por ALSEP.","#715249",{{
            {"Un destino ya conocido",
             "El Intrepid se posó el 19 de noviembre de 1969 en Oceanus Procellarum. Conrad y Bean iban a trabajar cerca de Surveyor 3, una nave robótica que llevaba allí desde 1967. Esa cercanía convertía el alunizaje en una prueba de navegación y de planeación de terreno: la tripulación debía poder llegar al vehículo y volver a pie.",
             "Intrepid conservaba el diseño de dos etapas. El motor de descenso debía frenar y dirigir el módulo hasta quedar sostenido por cuatro patas. La cabina de ascenso quedaría acoplada durante los paseos.\nLa cercanía de Surveyor ofrecía un objetivo adicional al de tomar rocas: recuperar piezas expuestas al ambiente lunar y compararlas con su estado anterior."},
            {"Instrumentos y fallas",
             "La primera caminata incluyó la instalación del ALSEP, una estación científica con alimentación de larga duración. Entre sus instrumentos había un sismómetro y medidores del entorno. Los equipos se distribuyeron en el suelo y se conectaron a una estación central para enviar sus mediciones.\nLa instalación incluía una tarea menos visible que desplegar los instrumentos: elegir su ubicación y evitar que el tránsito afectara las lecturas.",
             "La cámara de televisión a color dejó de funcionar al apuntar directamente al Sol. Las observaciones siguieron por radio, fotografía y bitácora. Esta diferencia importa al revisar la misión: que se pierda el video en directo no implica que se pierda el registro de la caminata.\nUn sistema tiene más de una vía para conservar evidencia, pero cada vía guarda información distinta."},
            {"Las piezas de Surveyor",
             "En la segunda caminata los astronautas alcanzaron el cráter donde estaba Surveyor 3. Fotografiaron el sitio y recuperaron componentes para estudiar su exposición. También tomaron muestras del terreno y recolectaron partículas del viento solar con una lámina.\nCada pieza regresó con historia propia: dónde estaba, cuánto tiempo llevaba allí y qué superficies habían quedado al descubierto.",
             "El conjunto de rocas y suelo lunar devuelto por la misión fue de unos 34 kg. Las piezas de Surveyor respondían otra pregunta: qué había cambiado en materiales fabricados en la Tierra tras años de exposición.\nAmbos tipos de evidencia eran útiles, pero solo si las muestras artificiales y las geológicas conservaban un registro que permitiera distinguirlas."},
            {"Un impacto con hora conocida",
             "Después de reunirse con el módulo de mando, la tripulación separó la cabina de ascenso vacía. Esta fue dirigida a la superficie. El ALSEP registró las vibraciones del impacto. Al conocerse aproximadamente el sitio y la hora, el golpe ofrecía una referencia que no existe al medir un temblor de origen desconocido.",
             "La señal se prolongó mucho más que el golpe inicial. Parte de la energía se dispersó en el terreno antes de llegar al detector. Una manera de leer esos datos es separar el origen de la señal, el medio por el que se propaga y la respuesta del instrumento.\nCon esa distinción, el descenso de una pieza descartada se convirtió en una medición del subsuelo lunar."}
        }}},V::Intrepid},
        {{"Falcon: travesía por Hadley","Geología lunar","LRV, instrumentos del ALSEP y un experimento de caída libre.","#55636f",{{
            {"Al pie de los Apeninos",
             "Falcon descendió el 30 de julio de 1971 cerca de Hadley, en el borde del Mar de las Lluvias. David Scott y James Irwin tenían un objetivo geológico: recorrer terrenos diferentes, observar su continuidad y devolver muestras cuya procedencia pudiera reconstruirse. La cabina y la base de descenso cumplían funciones independientes.\nEl mapa previo servía para planear rutas, pero la luz baja y el relieve cambiaban la percepción de las distancias.",
             "La misión llevó el primer vehículo lunar LRV. Sus cuatro ruedas y su alimentación eléctrica extendieron el alcance de las caminatas. La distancia de cada recorrido seguía limitada por la posibilidad de regresar a pie si el vehículo fallaba.\nLa velocidad por sí sola no explica el valor del LRV. Poder cargar herramientas y muestras permitió elegir estaciones geológicas más separadas."},
            {"Entre cráteres y capas",
             "Durante las tres salidas recorrieron en conjunto alrededor de 28 km y recogieron unos 77 kg de muestras. Cerca de Hadley Rille podían comparar rocas de superficie con afloramientos y materiales proyectados por impactos. Las fotografías de contexto ayudaban a relacionar cada muestra con su estación.\nUn núcleo perforado aportaba una secuencia vertical, distinta de la información de una roca suelta.",
             "En la Luna el polvo se comportaba de forma distinta de un sendero en la Tierra. Sin aire denso, las partículas expulsadas por las ruedas no forman una nube suspendida del mismo modo. El vehículo levantaba arcos de polvo que volvían a caer.\nLa forma del rastro servía como indicio del movimiento, pero interpretar el suelo exigía combinarla con pendiente, velocidad y deslizamiento."},
            {"Un observatorio en el suelo",
             "El ALSEP incluyó instrumentos sismológicos y otros sensores del entorno. Su generador convertía en electricidad parte del calor de una fuente radiactiva para mantener las mediciones durante periodos sin Sol. La estación central recibía las señales y las enviaba a la Tierra.\nLa ubicación de los instrumentos y la resistencia de las conexiones afectaban la continuidad del registro.",
             "Scott soltó un martillo y una pluma al mismo tiempo ante la cámara. En el vacío lunar casi no hay resistencia del aire y ambos alcanzaron el suelo juntos. La demostración compara la aceleración bajo la misma gravedad y con condiciones iniciales parecidas.\nEn la Tierra, una pluma abierta cae de otro modo porque la resistencia del aire pesa mucho en su movimiento."},
            {"De la ruta al archivo",
             "El 2 de agosto la cabina de Falcon se separó de la base. Scott e Irwin se reunieron en órbita con Alfred Worden y trasladaron muestras y registros a la nave de regreso. El vehículo y la estación de experimentos se quedaron en la Luna.\nLa separación entre lo que volvía y lo que seguía funcionando allí prolongó el valor de la expedición.",
             "Las fotografías, las posiciones y las muestras permitieron volver a interpretar la geología de Hadley desde el laboratorio. Años después, otras imágenes orbitales registraron rastros de la expedición.\nEl legado de Falcon se puede leer en tres escalas: la muestra en una mesa, el camino entre estaciones y el paisaje regional al que todos esos datos pertenecen."}
        }}},V::Falcon},
        {{"Latidos del planeta rojo","Geofísica","Ondas sísmicas, capas internas y los límites de una estación en Marte.","#98553d",{{
            {"Una estación inmóvil",
             "InSight aterrizó en noviembre de 2018. A diferencia de un rover, su proyecto dependía de permanecer quieto y medir el mismo sitio durante mucho tiempo. Situó el sismómetro SEIS sobre el terreno, protegido por una cubierta contra el viento y los cambios térmicos. Eso permitía detectar movimientos pequeños sin atribuirlos de inmediato a una fuente interna.",
             "Una parte del trabajo era reconocer ruido ambiental. El viento podía mover el suelo o agitar la propia nave. Los cambios de temperatura afectaban al instrumento y sus alrededores.\nPara leer un registro, el equipo cruzaba el momento de la vibración con información del entorno y buscaba una forma de onda compatible con un temblor o un impacto."},
            {"La velocidad como pista",
             "Las ondas de compresión, llamadas P, y las de corte, llamadas S, viajan de manera distinta por los materiales. Su tiempo de llegada y los cambios de trayectoria aportan información sobre las capas cruzadas.\nCon una estación, distintas combinaciones de origen y estructura pueden dar señales parecidas. Por eso la interpretación se contrasta con más de un evento.",
             "Un impacto puede producir, además de la señal sísmica, un cráter visible en imágenes de órbita. Si se identifica ese cráter, la localización de la fuente queda mejor acotada.\nConocer dónde empezó la señal facilita interpretar qué parte de su retraso se debe a la distancia y cuál a los materiales del interior."},
            {"El experimento que no penetró",
             "El instrumento de flujo térmico pretendía medir el gradiente de temperatura bajo la superficie. Una sonda de penetración debía abrirse paso aprovechando el contacto con el terreno. En el sitio de InSight, la fricción y la consistencia del suelo no se comportaron como se necesitaba. La profundidad alcanzada no permitió la medición prevista.",
             "El resultado operativo y el sísmico pertenecen a instrumentos distintos. La dificultad de penetrar acotó cómo respondía ese suelo a la herramienta. Los sismos, por otro lado, ofrecieron evidencia de la estructura interna.\nSeparar resultados según su instrumento hace posible aprender de un experimento aunque otro de la misma misión no cumpla su objetivo."},
            {"Un modelo que puede cambiar",
             "Los registros permiten proponer espesores de corteza y modelos para el manto y las regiones profundas. Las interpretaciones se refinan a medida que cambian el conjunto de temblores y el modelo de propagación. Una medida derivada de ondas suele expresarse con un intervalo o junto a las hipótesis que la sostienen.",
             "El polvo redujo la producción de los paneles y la estación terminó dejando de transmitir. Los registros ya recibidos quedaron disponibles para nuevos análisis.\nAl volver a una señal conviene conservar la hora, la orientación y la calidad del dato. Son las condiciones que permiten comparar una interpretación antigua con una nueva."}
        }}},V::Insight},
        {{"Gemelos de hierro","Geología marciana","Hematita, arcillas y operación de dos vehículos con energía solar.","#825344",{{
            {"Dos lugares de medida",
             "Spirit y Opportunity llegaron a Marte en enero de 2004. Tenían diseños similares, pero buscaron evidencia en entornos diferentes: el cráter Gusev y Meridiani Planum. Un brazo permitía examinar afloramientos y retirar la capa superficial alterada. Las imágenes del entorno y las mediciones de composición daban contexto a la muestra elegida.",
             "Un rover trabaja a través de turnos y enlaces limitados. El equipo decide qué medir, verifica que la maniobra sea segura y manda una secuencia de instrucciones. El robot devuelve imágenes y datos cuando tiene contacto.\nPara interpretar una medición hay que saber si corresponde a polvo suelto, a la superficie de una roca o al material que quedó expuesto tras rasparla."},
            {"Los arándanos de Meridiani",
             "Opportunity encontró pequeñas esferas ricas en hematita, conocidas como «arándanos», junto con capas de roca que contenían sales. La forma de los depósitos y su composición respaldaron un pasado con agua líquida en ese entorno. Un solo mineral, sin ubicación ni comparación, no habría sustentado por sí mismo la misma interpretación.",
             "El origen de las esferas se puso en relación con el modo en que el agua pudo circular por el sedimento. Las sales sugerían episodios de evaporación. Otras observaciones posteriores ampliaron el conjunto de ambientes posibles.\nEl hallazgo respondía si había habido agua en determinadas regiones. Quedaban por acotar la duración, la acidez y las condiciones que sostenían esos ambientes."},
            {"La energía dicta el recorrido",
             "Los paneles solares de ambos robots perdían rendimiento al cubrirse de polvo. Algunas ráfagas limpiaban parte de la superficie y la energía disponible volvía a subir. En cambio, las tormentas extendidas podían reducir la luz que llegaba a las celdas durante muchos días. La orientación también importaba, sobre todo en invierno.",
             "Spirit quedó atrapado en arena y tuvo dificultad para orientar sus paneles. Opportunity pudo continuar hasta estudiar el borde del cráter Endeavour. Allí encontró evidencia de minerales de arcilla, útiles para comparar condiciones químicas con las de Meridiani.\nEl objetivo geológico y la ruta real dependieron siempre del terreno, la energía y el margen para corregir imprevistos."},
            {"Los datos sobreviven",
             "En 2018, una gran tormenta oscureció el cielo de Marte y dejó a Opportunity sin energía suficiente para sostener la comunicación. Sus años de trabajo ya habían cubierto decenas de kilómetros. Spirit también había superado con amplitud la duración inicial prevista.\nLa duración real de una misión depende de cómo envejecen sus baterías y componentes, pero también del terreno y los cambios de iluminación que encuentra durante la marcha.",
             "Las ruedas y los sistemas de ambos vehículos se estudiaron para preparar misiones posteriores. El aprendizaje geológico y el de ingeniería quedaron unidos: no es posible escoger buenas mediciones si no se puede llegar al terreno, observar con suficiente contexto y conservar energía para transmitir.\nLos gemelos dejaron un conjunto de observaciones que se sigue contrastando con nuevos recorridos y nuevas imágenes."}
        }}},V::Rovers}
    }};
    return books;
}
}
