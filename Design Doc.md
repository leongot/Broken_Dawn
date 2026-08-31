Progetto Programmazione Avanzata(BrokenDawn)
================================

# Project setup
* [Co-op Action Roguelike Unreal Engine Sample Game](https://github.com/tomlooman/ActionRoguelike)
* [UE Tips & Best Practices](https://flassari.notion.site/UE-Tips-Best-Practices-3ff4c3297b414a66886c969ff741c5ba)
* [Unreal Engine 5 C++ Complete Guide](https://tomlooman.com/unreal-engine-cpp-guide/)
* [Unreal C++ speedrun ](https://landelare.github.io/2023/01/07/cpp-speedrun.html)
* [Code a First-Person Adventure Game](https://dev.epicgames.com/documentation/unreal-engine/code-a-firstperson-adventure-game-in-unreal-engine)
* [Design a Puzzle Adventure](https://dev.epicgames.com/documentation/unreal-engine/design-a-puzzle-adventure-game-in-unreal-engine)

# Design
## Planning
### Elevator pitch
Combattimento in prima persona contro orde di nemici.  
### Tools
* UE5 version 5.7.4
### Scope
* 2 livelli di grandezza media (2017 x 2017 UE unit)
### Pillars
* Combattimento
* Esplorazione
* Orde di nemici
### Experience goals
Il giocatore si deve sentire costantemente sotto assalto
### Mechanics
* Il giocatore puo' prendere dei props e utilizzarli contro i nemici
* Combattimento corpo a corpo 
### Asset list

### Pacing
#### Map 1
* Villagio medievale rurale
* Introduzione del combattimento / nemici
#### Map 2
* Castello


Stile cod zombies
	- (possibile evoluzione) il giocatore puo' selezionare tra diverse classi 
Timer da 30 minuti con spawn di boss alla fine 
	- il gicatore di deve pontenziare il piu' possbile all'interno del timer
	- se il boss viene sconfitto, viene sbloccato in nuovo livello

## Genre
* FPS/Hack-and-Slash/Survival
## Ambientazione
* Light-fantasy
* Squadra di persone (possibile multiplayer) per ripulire dei luoghi (zone di un continente) da infetti (malattia)
	* 4 classi distinte che i giocatori possono selezionare

## UX
* Visualizzazione del mondo di gioco per la selezione dei livelli

## Level Design
[The Level Design Book](https://book.leveldesignbook.com/)
## Player Movement
[PBCharacterMovement](https://github.com/ProjectBorealis/PBCharacterMovement)
[Recreating Quake / GoldSrc Movement in Godot 4.0](https://aneacsu.com/blog/2023-04-09-quake-movement-godot)
[Bunnyhopping from the Programmer's Perspective](https://adrianb.io/2015/02/14/bunnyhop.html)
## Game References
[Warhammer: Vermintide II](https://www.mobygames.com/game/104949/warhammer-vermintide-ii/) --> Ambientazione
[For Honor](https://www.mobygames.com/game/83670/for-honor/) --> UX
Voin --> Stile grafico
Warframe --> Design nemici
[Resident Evil 4: Ultimate HD Edition](https://www.mobygames.com/game/65323/resident-evil-4-ultimate-hd-edition/) -> Ambientazione / Stile grafico / Design Generale
[Left 4 Dead](https://www.mobygames.com/game/38055/left-4-dead/) --> Stile grafico / Design Generale
[Half-Life 2](https://www.mobygames.com/game/15564/half-life-2/) --> Stile grafico / Design Generale

# Resources
## World Bulding
[Beginner’s Guide: Create a Photorealistic Environment in Unreal Engine 5](https://www.youtube.com/watch?v=BaW1Cdy6tE4)
[UE5: Step-by-Step Landscape Material](https://www.youtube.com/playlist?list=PLhlDHJpnPTG-WUD_dqu0elvgbbhxklWIu)
[Ben Cloward](https://www.youtube.com/@BenCloward/videos)
## Height maps for 3D Terrain
[Generating Height Maps with GIMP and splitting over multiple regions. For OpenSim/SL](https://www.youtube.com/watch?v=d4kNYh5faPM) 
[Making 3-D World Heightmaps from Scratch! - Blender, GIMP, 3D Modeling, Terrain, and followup ideas](https://www.youtube.com/watch?v=YoF-GehVwKM)
[Create Your Own Fantasy Maps Using GIMP!](https://www.youtube.com/watch?v=xSMwa3yva2c)
[How to Create Fantasy Maps with Free Tools (GIMP, Krita)](https://www.youtube.com/watch?v=faDvbIF1nuI)
## Photos References / Textures
### Internals
* [brown wooden wheel on brown wooden table](https://unsplash.com/photos/brown-wooden-wheel-on-brown-wooden-table-fsOKJe2eA0k)
### Courtyards
* [The courtyards of the old town.](https://unsplash.com/photos/gray-concrete-tunnel-during-daytime-OXXH-MmizAM)
### Brick buildings
* [brown brick building under cloudy sky](https://unsplash.com/photos/brown-brick-building-under-cloudy-sky-roqJ4FFEq2s)
### Castle/buildings on a rocky hill*
* [a castle sitting on top of a rocky hill](https://unsplash.com/photos/a-castle-sitting-on-top-of-a-rocky-hill-8uBR5vftKfk)
* [brown concrete building on top of mountain](\https://unsplash.com/photos/brown-concrete-building-on-top-of-mountain-me_dOLK5nw8)
* [A castle on top of a hill surrounded by trees](https://unsplash.com/photos/a-castle-on-top-of-a-hill-surrounded-by-trees-3MLWzDP_Svs)
* [A stone castle stands amidst winter's landscape.](https://unsplash.com/photos/a-stone-castle-stands-amidst-winters-landscape-7WQYA_ybC6c)
### Stone tower
	- https://unsplash.com/photos/a-stone-tower-sitting-on-top-of-a-lush-green-hillside-WIFCDJzVzAk
### Fields
	- https://unsplash.com/photos/green-grass-field-W48YTVaXfFM
### Cliffsides
	- https://unsplash.com/photos/cliffside-view-over-a-dark-misty-body-of-water-U4-1ACivv9o
### Roads
	- https://unsplash.com/photos/a-dirt-road-in-the-middle-of-a-forest-on-a-foggy-day-cXMuTnf6syI
## Modelling
[modeling a castle in blender 2.8](https://www.youtube.com/playlist?list=PLYKnBP50d35JpbA3hwqenWBdE399KYKMy)
[Unreal Engine 5 Environment - Blender - Part 1: Asset Modeling](https://www.youtube.com/watch?v=8CPMd09sCgE)
[Let's model a column in Blender - a step-by-step tutorial on non destructive workflows (pt.1)](https://www.youtube.com/watch?v=68oRQvV6GhY)
[PS1 Style Graphics in Blender / Low Poly Assets Beginner Tutorial - Part 1](https://www.youtube.com/watch?v=5ycmDpYen-4)
## 3D models
Daniel Andersson's Medieval Themed Models - https://opengameart.org/content/daniel-anderssons-medieval-themed-models
## Materials
[How to make next-level procedural materials in Blender](https://www.youtube.com/watch?v=K8LVFNVLlYc)
[Substance Designer - Mountain Path Tutorial Part 1: Creating Rocks](https://www.youtube.com/watch?v=_6leC7PcCc0)
[Forest Ground - Substance Designer Material Breakdown](https://www.youtube.com/watch?v=A4HMw46rEHc)
*Forest Path* 
	- https://www.fab.com/listings/6614ed94-d322-48eb-aa7c-658c2d7db47e
## Textures
[Creating Lowpoly PS1 Pixel Art Textures from Images! (GIMP Tutorial)](https://www.youtube.com/watch?v=MchEScmatRE)
## Collections
[Collection: Pine Forest](https://polyhaven.com/collections/pine_forest)
## Lighting
[Unreal Engine Lighting Optimization Guide | Bake Like a Pro](https://www.youtube.com/watch?v=T3wFfxn6m2E) 
[Intro to Physically Based Lighting and Cinematics - Lighting for Videogames](https://www.youtube.com/watch?v=vbMvkEITTmo)
[Lighting Interiors in UE4](https://www.youtube.com/watch?v=ZWADlXyfYCk)
## Artbooks
[Dragon's Dogma Official Design Works](https://archive.org/details/dragons-dogma-official-design-works/mode/2up)
[Dark Souls Design Works](https://archive.org/details/artbook-Dark_Souls_Design_Works/mode/2up)
[Dark souls 3 Design works](https://archive.org/details/artbook-Dark_souls_3_Design_works/mode/2up)
[Witcher 2 Artbook](https://archive.org/details/artbook-Witcher_2_Artbook/mode/2up)
[The Witcher 3 Wild Hunt Artbook](https://archive.org/details/the-witcher-3-wild-hunt-artbook/mode/2up)

# TODO