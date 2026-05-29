# 2025-2026-4GPa-EviaM-RodriguezO

# Projet Capteur
Dans le cadre du UF « Du capteur au banc de test », ce travail doit concevoir et mettre en œuvre un système de mesure de déformation par un capteur en graphite, lowtech, et comparer son débrouillement à un autre capteur commercial.

## Objectifs du projet
- Concevoir et réaliser un shield Arduino intégrant un capteur de contrainte en graphite de conception simple afin d'en déterminer les caractéristiques.
- Concevoir un circuit électronique analogique capable de transmettre des données via un microcontrôleur à une application Android, ainsi que la mise en place d'un banc d'essai.
- Élaboration de la datasheet du capteur.

## Contexte
Nous avons fabriqué un capteur de déformation en exploitant la conductivité d'une couche de graphite appliquée sur une feuille de papier, créant ainsi un capteur grâce aux propriétés granulaires acquises qui dépendent également de la composition du crayon.

Lorsque le capteur est comprimé, la résistance diminue en raison de la proximité des molécules de graphite, ce qui augmente la conductivité. Lorsqu'il est étiré, c'est l'inverse qui se produit, ce qui augmente la résistance.

C'est sur la base de ce comportement que l'on cherche à déduire l'angle d'inclinaison afin de connaître et de rendre compte des caractéristiques du capteur.

Ce projet est basé du travail trouvé dans: Lin, CW, Zhao, Z., Kim, J. et al. (2014); Nature, "Pencil Drawn Strain Gauges and Chemiresistors on Paper".

## Livrables
### LTSpice
### KiCad
Pour créer la carte qui nous permettrait de disposer de notre circuit, il a fallu réaliser la conception à l'aide du logiciel KiCad, en tenant compte de tous les composants et en utilisant les connexions convenues en cours.

<img width="1067" height="723" alt="image" src="https://github.com/user-attachments/assets/047386df-b6f0-40a4-bb80-ef5c6cd5c5a4" />
<p align="center">
Schéma du circuit sur KiCad.
</p>

Après avoir déterminé les connexions du circuit, nous avons pu passer à la conception du circuit imprimé, qui devait tenir compte de certains aspects tels que l'optimisation de l'espace, la largeur minimale des pistes tant pour la transmission des données que pour l'alimentation, la résolution possible lors de l'impression, ainsi que la disposition des composants afin d'éviter d'avoir à souder trop de ponts.

<img width="776" height="587" alt="image" src="https://github.com/user-attachments/assets/4a89d056-d9c2-4984-8b99-233851680ac5" />
<p align="center">
Dessin du circuit imprimé sur KiCad.
</p>


Voir la documentation dans le dossier **Fichiers Kicad**

### Shield
### Code Arduino
### Logiciel pour portables
Grâce à la plateforme MIT App Inventor, nous avons conçu une application dont la fonction principale était de pouvoir recevoir et représenter graphiquement les données captées par un ou les deux capteurs simultanément, ainsi que de pouvoir envoyer des données pour commander à la fois le servomoteur et le potentiomètre numérique ; toutefois, l'envoi de données n'a pas eu lieu.
<p align="center">
<img width="233" height="356" alt="image" src="https://github.com/user-attachments/assets/0311451c-5eb8-4153-8b4c-7ee4ba206b4d" /> <img width="233" height="356" alt="image" src="https://github.com/user-attachments/assets/94a4a5a3-bd85-4a07-bee5-77647120f4fd" />
</p>
<p align="center">
Application crée, face-avant au démarrage / avec les fonctions.
</p>

Le code des blocs derrière l'interface est visible sur les images suivantes.

<img width="1308" height="655" alt="image" src="https://github.com/user-attachments/assets/e1ab4e29-2118-45b2-89d1-57f97e194c88" />
<img width="1278" height="377" alt="image" src="https://github.com/user-attachments/assets/920bb55d-6b9c-4a3f-b7e2-3a09c574f540" />
<p align="center">
Bloques de code.
</p>


Voir la documentation dans le dossier **Application Mobile**.
### Datasheet
## Conclusion 
Ce projet nous a permis de voir concrètement comment exploiter un phénomène physique pour mesurer un changement au sein d'un système ; en recourant à des moyens simples et en tirant parti de propriétés microscopiques, nous avons pu concevoir un dispositif qui nous a permis de collecter et de traiter des données de manière relativement simple.

Cependant, le projet a également mis en évidence les limites de l'utilisation de ce type de systèmes low-tech, à commencer par la capacité à reproduire les mêmes résultats, car il existe un grand nombre de variables à prendre en compte, telles que l'état du papier, la composition du graphite, la qualité du trait tracé ; toutes ces variables affectent le fonctionnement et l'étalonnage du capteur, et sont difficiles à normaliser manuellement. Et, parallèlement, certaines de ces variables étant très sensibles à l'utilisation, cela finit par nuire à la fiabilité des données recueillies au fil du temps.
