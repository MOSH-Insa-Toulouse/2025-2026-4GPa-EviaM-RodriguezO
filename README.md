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
Schéma du circuit sur KiCad
</p>

Après avoir déterminé les connexions du circuit, nous avons pu passer à la conception du circuit imprimé, qui devait tenir compte de certains aspects tels que l'optimisation de l'espace, la largeur minimale des pistes tant pour la transmission des données que pour l'alimentation, la résolution possible lors de l'impression, ainsi que la disposition des composants afin d'éviter d'avoir à souder trop de ponts.

<img width="776" height="587" alt="image" src="https://github.com/user-attachments/assets/4a89d056-d9c2-4984-8b99-233851680ac5" />
<p align="center">
Dessin du circuit imprimé sur KiCad
</p>


Voir la documentation dans le dossier **Fichiers Kicad**

### Shield
### Code Arduino
### Logiciel pour portables
### Datasheet
## Conclusion et Bilan Critique
