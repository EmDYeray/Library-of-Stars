#include "child_text.hpp"
#include "locale.hpp"
#include "stories.hpp"

namespace biblioteca {
const AdultSpread& childSpread(int book,int spread){
    if(isEnglish())return childSpreadEnglish(book,spread);
    using Story=std::array<AdultSpread,spreads>;
    static constexpr std::array<Story,7> stories{{
        Story{{
            {"Humo sobre el valle","Inés vio humo cerca de una montaña. La foto venía de una cámara en el espacio.","Pidió otra foto. Quería saber si el humo salía del suelo."},
            {"Dos fotos","Las dos fotos parecían iguales. Inés usó una cámara que podía ver el calor.","El fuego subía por la montaña. El viento lo empujaba hacia el pueblo."},
            {"Debajo de la nube","Una nube tapó el valle. El equipo buscó fotos de antes.","La cámara de calor dio otra pista. Había fuego cerca del río seco."},
            {"Un mapa por radio","El satélite iba a ocultarse detrás de la montaña. Quedaba poco tiempo para mandar el mapa.","Inés mandó primero una copia pequeña. Los bomberos pudieron abrirla enseguida."},
            {"El puente cerrado","Un coche con cámara vio un árbol caído sobre el puente. Por allí no se podía pasar.","El equipo encontró otro camino al otro lado del río."},
            {"Otra vista","Desde una nave tomaron una foto de lado. Mostraba humo donde antes nadie lo había visto.","Los camiones pudieron cambiar de ruta. Al caer la tarde, el nuevo camino seguía libre."},
            {"La marca del fuego","Tres días después, Inés volvió a mirar la montaña. La tierra quemada se veía oscura.","Guardó las fotos del antes y el después. Así se veía hasta dónde había llegado el fuego."},
            {"El río de vuelta","Al año siguiente, el agua volvió al río. Algunas plantas ya estaban creciendo.","Inés dibujó los brotes en el mapa. Esta vez contaba cómo volvía la vida."}
        }},
        Story{{
            {"Un río de hielo","Mateo vio una raya oscura en Europa, una luna de Júpiter. Parecía un río congelado.","Marcó el lugar en el mapa. Quería mirar la grieta otra vez."},
            {"Dos miradas","La nave giró y la grieta salió de la foto. Por suerte, había quedado un pedacito en el borde.","Mateo juntó dos fotos. En la segunda, una sombra mostraba que el hielo se levantaba allí."},
            {"La orilla oscura","La antena solo recibió la mitad de la foto. La parte en sombra todavía no se veía.","El equipo pidió otra vista. Necesitaba saber si un robot podía bajar sin caer."},
            {"Un brillo que se mueve","Un punto de luz cambió de lugar cuando la nave giró. Parecía un espejo bajo el Sol.","Mateo comparó las fotos. El brillo se movió. La grieta siguió en el mismo sitio."},
            {"Hielo de prueba","En la Tierra hicieron un camino con hielo de prueba. El coche resbaló cerca del borde.","Lo intentaron por una parte más ancha. Esta vez las ruedas pasaron con cuidado."},
            {"Una piedra en el mapa","El satélite mandó una última foto. Al final de la grieta se veía una roca oscura.","Mateo la marcó. La roca podía ayudar a encontrar el camino."},
            {"Dos caminos","El cohete llevaría una copia del mapa al próximo equipo.","Había una ruta junto a la grieta y otra más lejos. Podían elegir al ver el hielo."},
            {"La franja azul","El Sol salió sobre el borde. El hielo brilló de azul.","Mateo guardó la foto. Ahora podía ver la grieta de una orilla a la otra."}
        }},
        Story{{
            {"¿Me oyen?","Aurora era un satélite. Cuando salió de la sombra, sus paneles volvieron a recibir luz.","Vera oyó un pitido ocho segundos tarde. Miró el reloj y volvió a probar."},
            {"En la azotea","Vera subió al techo. Allí había otra antena con forma de plato.","Oyó el mismo pitido tarde. Ya sabía que la primera antena no era la causa."},
            {"Una pieza floja","Otra nave tomó una foto de lado. Una pieza se movía junto a la cámara de Aurora.","La pieza tapaba las estrellas que Aurora usaba para saber hacia dónde miraba."},
            {"Un giro pequeño","En la Tierra, Vera giró una copia de Aurora. Encontró un lado por donde entraba la luz.","Le pidió a la nave que hiciera el mismo giro. ¡El pitido volvió a tiempo!"},
            {"La pieza en la mesa","Un ingeniero probó una copia de la pieza en su mesa. Al moverla, rozaba el borde.","Dobló la punta un poquito y volvió a probar. Esta vez quedó espacio entre las piezas."},
            {"Dos marcas","El cohete llevó una pieza nueva con dos rayas pintadas.","La cámara miró las rayas. Si se separaban, sabrían que la pieza se había doblado."},
            {"Sol y sombra","La pieza pasó del calor del Sol al frío de la sombra. Las rayas siguieron en su sitio.","Aurora pudo mirar las estrellas sin que nada tapara su cámara."},
            {"Un pitido a tiempo","Vera encendió la radio cuando salió el Sol. El pitido de Aurora llegó a su hora.","Lo guardó para acordarse de cómo una pieza tan pequeña pudo cambiar todo el viaje."}
        }},
        Story{{
            {"Primeras huellas","Nadia miró las huellas del robot con ruedas. El Sol pintaba las piedras de naranja.","El robot se paró junto a una grieta. Nadia buscaría un lugar más plano para pasar."},
            {"Dar la vuelta","Una foto desde arriba mostró escalones de roca que el robot no podía ver.","Nadia dibujó una vuelta por un terreno más firme."},
            {"Una rueda resbala","Al subir, una rueda se deslizó y levantó polvo. Debajo había una piedra oscura.","Nadia hizo retroceder al robot. En el segundo intento, rodeó la piedra."},
            {"Fotos por radio","Una nave pasó sobre el robot. Podía mandar fotos a la Tierra durante unos minutos.","Nadia recibió una foto con rayas claras en la roca. Quiso verla de cerca."},
            {"La piedra de rayas","El brazo del robot bajó su cámara hasta una piedra roja. Tenía rayas de colores.","Su taladro sacó un pedacito. Nadia guardó una foto del lugar donde lo encontró."},
            {"Una nube de polvo","El cielo se oscureció. Nadia dejó que el robot esperara en un lugar plano.","Cuando cayó el polvo, la cámara volvió a ver las huellas del camino."},
            {"La Tierra escucha","Una antena en la Tierra recibió las fotos que el robot había guardado.","En una foto, el camino de regreso seguía limpio."},
            {"La ruta roja","Al amanecer, el robot volvió a subir. Desde arriba se veía el borde de un gran hoyo.","Nadia guardó el mapa. Las huellas contaban por dónde había pasado su robot."}
        }},
        Story{{
            {"Un brillo junto a la Luna","Iris vio un pedacito de roca cerca del camino de una nave.","La nave esperó mientras Iris pedía otra foto. La roca se acercaba a su camino."},
            {"Una roca que gira","Una cámara miró la roca desde dos lados. Era pequeña, brillante y giraba muy rápido.","Iris prefirió apartar la nave. Era más seguro que adivinar el tamaño de la roca."},
            {"Una noticia por radio","Una antena envió el nuevo camino a la nave. Desde la Tierra esperaron su respuesta.","Cuando llegó la respuesta, Iris puso juntos el camino viejo y el nuevo."},
            {"Tres pestañeos","La cámara vio tres destellos. Era la luz del Sol sobre una esquina de la roca.","Al ver otro destello, el equipo pudo saber hacia dónde se movía."},
            {"Un empujón suave","La nave prendió sus motores durante unos segundos para apartarse un poco.","Iris miró las fotos nuevas. La roca pasaría por detrás, con espacio de sobra."},
            {"La nave amiga","Otra nave llegó con una tripulación nueva. Tenía el camino que Iris había dibujado.","Las dos naves se encontraron y todavía les quedaba combustible para volver."},
            {"La roca se aleja","Dos cámaras midieron dónde estaba la roca. Las dos vieron que ya se alejaba.","Iris quitó el aviso. Guardó las fotos para explicar cómo lo habían sabido."},
            {"Cuatro patas en la Luna","La nave bajó junto a un gran hoyo iluminado. Una pata tocó el polvo antes que las otras.","Iris recibió una foto del suelo. Se veían huellas, una casita redonda y dos antenas."}
        }},
        Story{{
            {"Saturno de cerca","Leo recibió un mensaje de su nave. Se acercaba a Saturno, el planeta de los grandes anillos.","La nave vio cómo los anillos dibujaban una sombra fina sobre el planeta."},
            {"Una foto rápida","La nave tenía poco tiempo para tomar una foto de Saturno. Apuntó su cámara y esperó.","En el segundo intento, la foto salió bien. Se veía todo el borde iluminado."},
            {"Una foto en partes","La primera foto llegó en dos partes. Al principio no se sabía cuál iba antes.","Leo miró la hora escrita en cada parte. Así pudo volver a unirlas."},
            {"Detrás del planeta","Del otro lado de Saturno, la nave quedó a oscuras. Leo apagó algunos aparatos.","Con la energía que quedaba, la nave pudo guardar una foto más."},
            {"Una chispa","Al asomarse de nuevo al Sol, la nave brilló como un punto junto al anillo.","Leo comparó la foto con su mapa. La nave seguía en el camino esperado."},
            {"Una nube de hielo","En una imagen nueva se veía una franja brillante junto a una luna de Saturno.","Leo apuntó el lugar. Quería saber si aquella luna lanzaba pedacitos de hielo."},
            {"Guardar dos copias","El próximo cohete llevaría una cámara que podía guardar más fotos.","Leo pidió poner la hora en cada parte. Así podrían juntarlas si volvían a separarse."},
            {"La foto de la mañana","Al amanecer llegó la última foto. Un lado de los anillos era dorado y el otro oscuro.","Leo la envió con la hora. Cerca del borde se veía la sombra de un anillo."}
        }},
        Story{{
            {"El Sol se agita","Amalia vio un destello muy grande en el Sol. Poco después, varias radios dejaron de oírse.","Anotó la hora del destello y la de los cortes. Quería saber si habían ocurrido juntos."},
            {"La antena del techo","Una escuela tenía una antena en el techo. Su cable estaba roto por el calor del Sol.","La maestra puso un cable nuevo. Al anochecer esperaron un mensaje del satélite."},
            {"Un puente que ya no está","El mapa mandaba los mensajes a un puente que ya no existía. Era un mapa viejo.","Amalia pidió una foto nueva. Dibujó la escuela como el nuevo punto de encuentro."},
            {"Una caja en el patio","Un coche pequeño llevó una caja de radio hasta el patio. Su cámara leyó la etiqueta.","La maestra vio que era la caja correcta. Ya podían enviar mensajes."},
            {"Dos escuelas escuchan","Una nave pasó sobre el valle y envió el aviso a dos escuelas.","Las dos maestras leyeron el mensaje y apuntaron la hora en su cuaderno."},
            {"Otro camino para el mensaje","Amalia pidió a otra nave que copiara el aviso. Quería probar si servía de ayuda.","Apagó la primera radio por un minuto. Las escuelas siguieron oyendo el mensaje."},
            {"Un equipo de repuesto","En tierra prepararon un cohete con otra caja de radio. Amalia dejó el mapa corregido al lado.","El próximo aviso podría viajar por dos caminos si uno dejaba de funcionar."},
            {"El Sol se calma","Cuando las radios volvieron, la nave tomó una foto nueva del Sol. La zona brillante se había calmado.","Amalia guardó juntas la foto y la historia de las dos escuelas."}
        }}
    }};
    return stories[static_cast<int>(destination(book))][spread];
}
}
