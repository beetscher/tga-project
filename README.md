# Kreslení TGA obrázku
 Jednoduchý program v jazyce C pro kreslení TGA obrázků pomocí konzole
 ## Použití
 Program požaduje 3 základní vstupní parametry:
 - Výstupní soubor ```--output <nazev-souboru>```
 - Výšku ```--height <vyska>```
 - Šířku ```--width <sirka>```

Na pořadí vstupních parametrů nezáleží, každý parametr musí být každý alespoň jednou.

#### Příklad použití:
- ```./main --output output.tga --width 200 --height 200```

## Práce s programem
Po spuštění programu budete moct zadávat příkazy dokud se nezadá ```exit```
- ```line zacatecni x,zacatecni y,konecna x,konecna y,r,g,b,a``` - vykreslí do výstupního souboru čáru z pozice (x0,y0) na pozici (x1,y1) barvou (r,g,b,a)
- ```rectangle x,y,vyska,sirka,r,g,b,a``` - vykreslí do výstupního souboru obdelník s rozměry výška * šířka na pozici (x,y) s barvou (r,g,b,a)
- ```circle x,y,p,r,g,b,a``` - vykreslí do výstupního souboru kruh se středem na pozici (x,y) s poloměrem p a barvou (r,g,b,a)
- ```triangle x,y,s,r,g,b,a``` - vykreslí do souboru  rovnostranný trojúhelník se středem na pozici (x, y), s délkou strany s a barvou (r, g, b, a)
- ```rotared-rectangle angle,x,y,width,height,r,g,b,a``` - vykreslí do souboru obdelník se středem na pozici (x,y) se šířkou width a výškou height s barvou (r,g,b,a). Obdelník bude zarotovaný o ```angle``` stupňů
- ```save``` - Uloží současnou verzi obrázku do výstupního souboru zadaného argumentem příkazoví řádky --output.
- ```exit``` - Provede příkaz save a poté ukončí program
