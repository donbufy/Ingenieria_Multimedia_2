# Coloca el código de tu juego en este archivo.

# Declara los personajes usados en el juego como en el ejemplo:

define javier = Character("Jav")

define amiguito = Character("[player_name]")

image bg estacion = "Estacion.jpg"

image Pinguinoo = "Pinguino.png"

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

    scene bg estacion



    # Muestra un personaje: Se usa un marcador de posición. Es posible
    # reemplazarlo añadiendo un archivo llamado "eileen happy.png" al directorio
    # 'images'.

    



    

    # Presenta las líneas del diálogo.

    show Pinguino

    javier "¿Has escuchado eso?"

    hide Pinguino
    "..."
    show Pinguino

    amiguito "Me estoy encontrando un poco mal"

    hide Pinguino

    show Pinguino2
    

    amiguito "AaaAAaaaAaAaa!"

    hide Pinguino2

    show Pinguino

    javier "Siempre supe que naciste para algo grande"

    hide Pinguino

    show Pinguino3

    amiguito "Inclínate hacia tu emperador, necio pingüino"

    # Finaliza el juego:

    return
