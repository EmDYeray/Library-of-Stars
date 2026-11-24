#include "stories.hpp"
#include "locale.hpp"
#include <array>

namespace biblioteca {
namespace {
using F=Figure;
using Story=std::array<Chapter,8>;
constexpr std::array<Story,7> stories{{
    Story{{
        {"La primera imagen","El observatorio despertó sobre el Pacífico. Su cámara distinguió una columna de humo en el borde de la sierra.",
         "Inés pidió otra pasada. Si el humo seguía unido al suelo, el incendio había empezado antes del amanecer.",F::Earth},
        {"Dos imágenes del fuego","El satélite repetidor recibió dos fotografías que parecían idénticas. Inés encendió el canal térmico para compararlas.",
         "El resplandor se había movido tres kilómetros cuesta arriba. El viento llevaba el fuego hacia el pueblo.",F::Satellite},
        {"La sombra de la nube","La nube cubrió el valle en la siguiente pasada. El equipo buscó una medición anterior que sirviera de referencia.",
         "La sonda aportó una banda infrarroja. Bajo la nube apareció un frente caliente que seguía el lecho seco del río.",F::Probe},
        {"Minutos de contacto","La estación de montaña solo tenía siete minutos antes de perder el satélite detrás del horizonte.",
         "Inés envió primero el mapa ligero. Los bomberos pudieron verlo mientras siguieron llegando los datos completos.",F::Dish},
        {"La parte que faltaba","Un vehículo de inspección encontró el puente cortado por árboles. La ruta sugerida desde el espacio terminaba allí.",
         "El equipo cruzó el mapa con las imágenes del vehículo y abrió un camino por el otro lado del río.",F::EarthFire},
        {"La última entrega","Desde la estación orbital una tripulante tomó una foto oblicua que mostraba humo en la ladera que el mapa ocultaba.",
         "La imagen llegó a tiempo para mover los camiones. Al atardecer el camino nuevo seguía despejado.",F::Station},
        {"Medir lo perdido","Tres días después, Inés pidió el mismo encuadre de la primera mañana. La comparación mostró dónde acababa la cicatriz.",
         "En el archivo conservó ambas tomas. Los guardabosques podían recorrer, paso a paso, el avance del incendio.",F::EarthNight},
        {"Un valle distinto","Al año siguiente, el instrumento volvió a mirar el valle. La ladera todavía estaba oscura, pero el río ya traía agua.",
         "Inés agregó una capa al mapa. Donde antes había calculado velocidad del fuego, ahora anotó el regreso de las plantas.",F::Telescope}
    }},
    Story{{
        {"Las grietas azules","Mateo vio una línea oscura en el hielo de Europa. Cambiaba de espesor justo donde dos fracturas se encontraban.",
         "Marcó el cruce para una segunda pasada. Desde la sonda, el relieve parecía un río congelado.",F::Europa},
        {"La segunda pasada","La cámara perdió el encuadre al girar sobre la luna. Aún conservaba una franja estrecha de la primera imagen.",
         "Mateo comparó ambas tomas y halló la línea. La luz desde otro ángulo confirmaba que había una cresta.",F::Imager},
        {"Una antena en escucha","La antena recibió un mapa incompleto del terreno. Faltaba el lado de la fractura que quedaba en sombra.",
         "El equipo pidió primero la elevación de esa orilla. Necesitaban saber qué tan lejos estaba del plano de descenso.",F::EuropaDescent},
        {"Sombra sobre el hielo","La sonda bajó su ángulo de paso para seguir las dos crestas a la vez. Un punto brillante se movía con el Sol.",
         "Mateo descartó el reflejo y conservó el resto de la imagen. La fractura real seguía fija en los dos mapas.",F::Probe},
        {"El ensayo","Un vehículo en la Tierra probó la ruta sobre un modelo de hielo. La primera pendiente lo mandó hacia el borde.",
         "Cambiar la entrada unos grados hizo que las ruedas cruzaran por un tramo ancho. El ensayo quedó anotado.",F::Rover},
        {"El relevo","El satélite tomó una de las últimas transmisiones del día. La cresta terminaba cerca de una zona de roca expuesta.",
         "Mateo añadió esa referencia a la ruta. Podrían usarla incluso si el terreno brillante ocultaba el relieve.",F::Satellite},
        {"El mapa que viaja","El cohete que llevaría el siguiente equipo esperaba con el mapa y las imágenes de prueba cargados.",
         "La copia del mapa llevaba las dos rutas. Una seguía la grieta y la otra se apartaba si perdían el enlace.",F::Rocket},
        {"La franja de luz","Volvieron a ver Europa cuando el Sol alcanzó el borde de la fractura. El hielo devolvió una franja azul.",
         "Mateo guardó la toma junto a los planos. Desde el puente de la sonda se veía cómo se partía el hielo.",F::EuropaCrevasse}
    }},
    Story{{
        {"Me oyen, ¿verdad?","Aurora desplegó sus paneles al salir del eclipse. La corriente era correcta, pero la baliza llegaba con ocho segundos de retraso.",
         "Vera repitió la orden y anotó la hora. El retraso era demasiado regular para echarle la culpa al silencio del espacio.",F::Satellite},
        {"Una hora en la azotea","La antena principal estaba ocupada. Vera subió a la azotea y encendió una parabólica que llevaba meses sin usarse.",
         "La segunda estación recibió la misma demora. El reloj equivocado viajaba en el satélite.",F::Dish},
        {"Una foto a contraluz","El observatorio orbital le tomó una foto al equipo de canto. Una lámina suelta rozaba el sensor que buscaba las estrellas.",
         "Aurora corregía la orientación cada ocho segundos y pausaba su baliza al hacerlo. El atasco era mecánico.",F::Telescope},
        {"Una maniobra corta","Vera ensayó un giro breve con la réplica de la sonda. La luz rozó el sensor desde un lado que la lámina no cubría.",
         "Pidió el mismo giro en órbita. Por primera vez en seis horas, la baliza llegó sin retraso.",F::Probe},
        {"Una pieza entre manos","La tripulación de la estación recibió una copia de la lámina. Al vibrar, rozó el soporte tal como en la imagen.",
         "Un ingeniero dobló la pestaña en el banco de ensamble y repitió la prueba. Vera guardó la corrección.",F::OrbitalBuild},
        {"La revisión final","El nuevo vehículo subió con la pestaña reforzada y dos marcas que delataban cualquier flexión.",
         "La cámara de inspección tomó imágenes durante el despliegue. Las marcas conservaron su distancia.",F::Rocket},
        {"Mirar la salida del Sol","El sensor térmico siguió la pieza a través de dos amaneceres. El metal cambió de temperatura, pero conservó su forma.",
         "Aurora repitió la maniobra de prueba y recuperó su orientación en un solo intento.",F::Imager},
        {"Baliza de mañana","Vera dejó la estación automática al salir la primera luz. Desde el estacionamiento oyó el aviso de una pasada sin errores.",
         "Guardó el audio. La nueva pieza salió en la cápsula de servicio rumbo al puerto de la estación.",F::OrbitalDock}
    }},
    Story{{
        {"La primera rodada","Nadia vio las primeras marcas del rover sobre Marte. La luz baja pintaba de naranja la cresta frente a las ruedas.",
         "El rover se detuvo ante una grieta. En la siguiente jornada intentaría cruzar por una franja más plana.",F::Mars},
        {"El desvío","El observador orbital mostró que el terreno caía en escalones detrás del borde. La cámara del rover solo alcanzaba a ver el primero.",
         "Nadia trazó un rodeo por terreno firme. Había una zona abierta que también mantenía la antena a la vista del cielo.",F::Imager},
        {"Ruedas en la ladera","El rover avanzó corto y levantó una nube de polvo fino. Una rueda resbaló al pasar sobre una roca oscura.",
         "Nadia lo hizo retroceder. La segunda tentativa bordeó la roca y dejó seis huellas nítidas en la cuesta.",F::Mars},
        {"La ventana de radio","El repetidor pasó por encima del cráter con pocos minutos de enlace. El rover preparó primero las fotos más pequeñas.",
         "Una de ellas mostraba capas claras en la pared. La siguiente orden cambió el destino al pie del estrato.",F::Satellite},
        {"Una roca distinta","El vehículo extendió la cámara cerca del suelo. Sobre el polvo rojo apareció una roca con bandas delgadas y claras.",
         "El taladro tomó la muestra al pie del estrato. Nadia guardó su posición junto a la foto del barranco.",F::MarsQuarry},
        {"El polvo se acerca","Una franja oscura cubrió el cielo al mediodía. Nadia puso al equipo a descansar en un terreno sin pendiente.",
         "Cuando el polvo se asentó, la cámara reconoció la cresta. Las ruedas seguían apuntando hacia el regreso.",F::MarsStorm},
        {"La voz de la Tierra","Con un enlace breve, la antena de la Tierra recuperó las imágenes que el rover había conservado desde la tormenta.",
         "La última foto mostró un camino intacto. Podían salir sin tocar las marcas que guiaban el retorno.",F::Dish},
        {"La ruta en rojo","Al amanecer, el rover volvió a la parte alta. Se detuvo donde la cresta cortaba el cielo sobre el cráter.",
         "Nadia dejó la ruta completa en el archivo. Las huellas eran el registro más directo de lo que habían encontrado.",F::MarsSummit}
    }},
    Story{{
        {"Una sombra sobre la Luna","Iris seguía el descenso hacia la Luna cuando apareció un fragmento en la ruta prevista para el relevo.",
         "El módulo aguardó en órbita. Una segunda toma mostró que el objeto se acercaba a su trayectoria.",F::Moon},
        {"Tamaño desconocido","La sonda de seguimiento midió el fragmento desde dos ángulos. Era pequeño, brillante y giraba demasiado rápido.",
         "Iris calculó una zona de incertidumbre mayor. Prefería apartar la nave a tiempo que adivinar el tamaño.",F::Probe},
        {"Esperar el aviso","Una antena recibió la orden corregida minutos antes del cambio. Desde el valle llamaron para confirmar el paquete.",
         "La nave acusó el recibo. Iris dejó en el mapa la trayectoria anterior para comparar la separación.",F::Dish},
        {"La cámara del turno nocturno","El observatorio captó al fragmento detrás de una luz de la estación. Una pequeña esquina brilló tres veces.",
         "El destello se repitió en el ciclo siguiente. Pudieron estimar cómo giraba y hacia dónde se alejaba.",F::Imager},
        {"Corregir rumbo","La estación encendió sus motores durante unos segundos. No hizo falta cambiar el horario del siguiente atraque.",
         "Iris esperó las lecturas. El fragmento pasaría detrás del laboratorio con espacio de sobra.",F::Station},
        {"La tripulación de relevo","El vehículo de lanzamiento recibió la nueva trayectoria. Su tripulación encontró a la estación exactamente donde esperaba.",
         "La maniobra terminó con el depósito casi intacto. Pudieron conservar el combustible para la vuelta.",F::MoonCapsule},
        {"Volver a medir","El satélite repetidor entregó mediciones de dos observatorios. Juntas mostraban que el fragmento ya no cruzaría la ruta.",
         "Iris quitó la alerta, pero conservó la curva con todos los puntos que habían llevado a la decisión.",F::Satellite},
        {"El descenso al amanecer","Al despejarse la trayectoria, el módulo bajó al borde soleado del cráter. Una de sus patas tocó antes que las otras.",
         "Iris recibió la primera foto junto al puesto polar. Se veían la cúpula, las huellas y las dos antenas listas.",F::MoonOutpost}
    }},
    Story{{
        {"La entrada a los anillos","Leo recibió un pulso de la sonda antes de su encuentro con Saturno. El parte de navegación venía incompleto.",
         "Sobre el disco de Saturno, la sonda vio una sombra fina de los anillos. Siguió tomando datos durante el contacto.",F::Saturn},
        {"La cara del planeta","La sonda pasó junto a Saturno con la cámara encendida. Tenía menos de un minuto para ajustar el enfoque.",
         "El disparador respondió al segundo intento. El borde iluminado entró completo en el cuadro.",F::Probe},
        {"El archivo partido","Un repetidor recibió una copia de las primeras medidas. Un fallo de memoria había partido el archivo en dos.",
         "Leo cotejó los tiempos. Podía ensamblar las lecturas sin inventar los intervalos perdidos.",F::Satellite},
        {"Cuando se fue el Sol","La nave quedó en sombra al otro lado del planeta. Leo quitó de la cola todos los comandos que podían esperar.",
         "La maniobra de salida consumió menos energía de lo previsto. La cámara conservó una última exposición.",F::Station},
        {"Una chispa en la órbita","El telescopio de seguimiento vio el destello de la nave al salir del eclipse. Era un punto a un lado del anillo.",
         "La fotografía confirmó la trayectoria. La sonda seguía con la antena orientada hacia casa.",F::Telescope},
        {"Una columna de hielo","La imagen de detalle llegó en varias piezas. Una línea clara se curvaba detrás del borde iluminado.",
         "Leo señaló la curva cerca de una de las lunas. Era la primera pista que tenían del origen del hielo disperso.",F::SaturnMoons},
        {"La copia de seguridad","Un próximo cohete llevaría una cámara con más memoria. Leo enseñó al equipo cómo se había reconstruido la imagen.",
         "El diseño nuevo pondría una copia de la hora en cada fila. Si algo se rompía, seguirían sabiendo dónde mirar.",F::Rocket},
        {"Señal de vuelta","La antena terminó de recibir la fotografía cuando empezó a clarear. Los anillos aparecían dorados en un lado y oscuros en el otro.",
         "Leo mandó la foto con la hora del primer pulso. En un borde, la antena de la sonda cruzaba la sombra del anillo.",F::SaturnCrossing}
    }},
    Story{{
        {"La tormenta en la corona","El observatorio de Amalia detectó una erupción solar. Poco después se interrumpieron varios enlaces de radio.",
         "Amalia activó el protocolo de seguimiento. La actividad del Sol y el estado de los enlaces quedarían en el mismo informe.",F::Sun},
        {"Una antena prestada","En el extremo del valle, una escuela conservaba una parabólica en la azotea. Su cable estaba agrietado por el Sol.",
         "La maestra lo reemplazó y avisó por radio. A las seis esperaban el primer paquete del satélite.",F::Dish},
        {"El mensaje equivocado","Un sensor envió coordenadas de un puente que ya no existía. El mapa que usaba tenía cuatro años.",
         "Amalia pidió nuevas imágenes, marcó el puente borrado y cambió el punto de entrega al patio de la escuela.",F::Imager},
        {"Una caja con ruedas","El vehículo de reparto recorrió el patio con un módulo de comunicaciones a bordo. Su cámara leyó el código en la caja.",
         "El código coincidía con la lista de la maestra. Podían empezar a transmitir sin registrar otro equipo.",F::Rover},
        {"Dos países en la misma pasada","La estación orbital entregó el paquete durante una pasada sobre la frontera. La señal salió al mismo tiempo hacia dos valles.",
         "Los mensajes llegaron a las dos escuelas. Las maestras compararon la hora con la de la fulguración solar.",F::SolarFlare},
        {"Probar el relevo","Amalia pidió a la sonda de prueba que copiara el paquete si fallaba la estación principal.",
         "Apagaron el transmisor durante un minuto. Las escuelas siguieron oyendo el aviso completo.",F::Probe},
        {"El próximo lanzamiento","En la plataforma aguardaba un equipo de repuesto. Amalia corrigió el mapa y lo dejó junto al archivo de la misión.",
         "Cada estación llevaba una frecuencia extra para la siguiente crecida. Así habría dos caminos desde el primer día.",F::Rocket},
        {"La última nota","Cuando volvió la red terrestre, el observatorio tomó otra imagen del Sol. La zona de la erupción ya se veía tranquila.",
         "Amalia guardó la imagen en el archivo técnico. El escudo del observatorio seguía orientado hacia la corona. Amalia archivó la lectura junto al dibujo del valle.",F::SolarCorona}
    }}
}};
}

Destination destination(int book){return static_cast<Destination>(book%stories.size());}
const DestinationArt& destinationArt(int book){
    static constexpr DestinationArt art[]={
        {"Earth","#148dbd","#5ddabe","#1c3487"},
        {"Europa","#50d0ee","#dbf9ff","#333697"},
        {"Ground orbit","#366edb","#a8e9ff","#40348d"},
        {"Mars","#f05a2d","#ffc278","#78202e"},
        {"The Moon","#a7b9ed","#f6edee","#555a98"},
        {"Saturno","#efaa4b","#ffe4a0","#87407c"},
        {"The Sun","#ff8b21","#ffed91","#a12642"},
    };
    static constexpr DestinationArt artSpanish[]={
        {"Tierra","#148dbd","#5ddabe","#1c3487"},
        {"Europa","#50d0ee","#dbf9ff","#333697"},
        {"Órbita terrestre","#366edb","#a8e9ff","#40348d"},
        {"Marte","#f05a2d","#ffc278","#78202e"},
        {"La Luna","#a7b9ed","#f6edee","#555a98"},
        {"Saturno","#efaa4b","#ffe4a0","#87407c"},
        {"El Sol","#ff8b21","#ffed91","#a12642"},
    };
    return (isEnglish()?art:artSpanish)[static_cast<int>(destination(book))];
}
const Chapter& chapter(int book,int spread){return isEnglish()?chapterEnglish(book,spread):stories[book%stories.size()][spread];}
bool wordlessBinding(int book){return book%7==0 || book%7==4 || book%7==6;}
}
