# Coloca el código de tu juego en este archivo.

# Declara los personajes usados en el juego como en el ejemplo:

define javier = Character("Javi", color="#0080FF")

define amiguito = Character("[player_name]", color="#FFD700")

image bg antartida = "Antartida2.jpg"

image bg estacion = "Estacion2.jpg"

image Pinguinoo = "Pinguino2.png"

image Pinguino = "Pinguino.png"

image Pinguino2 = "emperador_Final.png"

image Pinguino3 = "Pinguino_EmperadorFinal.png"

# El juego comienza aquí.

label start:

    $ player_name = renpy.input("¿Cómo se llama usted?")
    $ player_name = player_name.strip()

    if player_name == "":

        $ player_name = "Anónimo Anónimez "

    # Muestra una imagen de fondo: Aquí se usa un marcador de posición por
    # defecto. Es posible añadir un archivo en el directorio 'images' con el
    # nombre "bg room.png" or "bg room.jpg" para que se muestre aquí.

    scene bg antartida

    play music "ambiente.mp3" 

    "Narrador" "En un lugar de la helada, de cuyo nombre no quiero acordarme ..."

    scene bg estacion



    # Muestra un personaje: Se usa un marcador de posición. Es posible
    # reemplazarlo añadiendo un archivo llamado "eileen happy.png" al directorio
    # 'images'.

    



    

    # Presenta las líneas del diálogo.

    show Pinguino

    amiguito "Qué suerte tengo en la vida de tenerte Javi."

    hide Pinguino
    show Pinguinoo

    javier "La verdad que yo también, no me imagino una vida sin tu amistad."

    hide Pinguinoo

    show Pinguino

    amiguito "Pues deja de imaginar, porque no nos vamos a separar nunca."

    hide Pinguino

    show Pinguinoo


    javier "¿Has escuchado eso?"

    hide Pinguinoo
    "..."
    show Pinguino

    amiguito "Me estoy encontrando un poco mal."

    hide Pinguino

    show Pinguino2
    

    amiguito "AaaAAaaaAaAaaAAAAaaAaaaaaAA!"

    hide Pinguino2

    show Pinguinoo

    javier "Siempre supe que naciste para algo grande amigo mío."

    hide Pinguinoo

    show Pinguino3

    amiguito "Inclínate hacia tu emperador, necio pingüino común. No soy tu amigo, soy el EMPERADOR VITALICIO DE LA NUEVA ANTÁRTIDA. "

    # Finaliza el juego:
    scene bg antartida

    "Narrador" "¿Te ha gustado mi mini-novela?"

    menu:

        "Narrador" "¿Te ha gustado mi mini-novela?"

        "Sí":

            jump gustado

        "No":

            jump no_gustado

    label gustado:

        "Narrador" "Me alegro mucho."


        jump final
        
    label no_gustado:

        "Narrador" "Pues sufrirás la ira de [player_name]."
        jump final

    label final:

        "Narrador" "Muchas gracias por ver."


    return
