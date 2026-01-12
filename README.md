# Restaurant Simulation en C++ avec Multithreading

## Description

Ce projet est une simulation d'un restaurant en C++ utilisant le **parallélisme**. Chaque acteur du restaurant (clients, serveurs, cuisiniers, chef) est représenté par un **thread** indépendant. Le but est de reproduire le fonctionnement d'un restaurant de manière concurrente et sûre pour la console.  

Le projet inclut également :  
- Un **système de log thread-safe** pour éviter que plusieurs threads écrivent en même temps dans la console.  
- Un **système de couleurs** pour faciliter le debug et identifier facilement les différents types d'acteurs.  
- La possibilité d'ajouter autant d'acteurs que nécessaire, tout en conservant un fonctionnement correct du restaurant.

---

## Fonctionnement

Le fonctionnement du restaurant est le suivant :  

1. **Client** :  
   - Passe une commande aléatoire parmi les plats disponibles (actuellement 4, mais extensible).  
   - Chaque plat est composé de 3 ingrédients choisis parmi 10 disponibles.  

2. **Serveur** :  
   - Prend les commandes des clients.  
   - Transmet la commande en cuisine.  
   - Récupère les plats une fois préparés et les sert aux clients correspondants.  

3. **Cuisinier** :  
   - Prépare les ingrédients de chaque plat.  

4. **Chef** :  
   - Assemble les ingrédients pour finaliser le plat.  

Toutes ces actions se déroulent en **parallèle**, grâce à l'utilisation de threads, ce qui permet une simulation réaliste du restaurant.

---

## Structure du projet

- `Customer` : représente un client (thread).  
- `Waiter` : représente un serveur (thread).  
- `Cooker` : représente un cuisinier (thread).  
- `Chief` : représente le chef (thread).  
- `Logger` : thread dédié au logging thread-safe.  
- `Colors` : utilitaire pour colorier le texte dans la console.

---

## Exemple de configuration

Dans l'état actuel du projet :  
- 3 clients  
- 1 serveur  
- 2 cuisiniers  
- 1 chef  

Mais vous pouvez ajuster le nombre de chaque acteur selon vos besoins.

---

## Compilation et exécution

### Prérequis

- **C++20** ou supérieur  
- Compilateur compatible avec les threads (`g++`, `clang++`, ou MSVC)



