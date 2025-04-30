## HARNOUFI Rayan - RESTREPO Juan

# TP FreeRTOS – STM32F746G-DISCO

Ce document contient les réponses aux questions posées lors des premières séances du TP FreeRTOS avec la carte STM32F746G-DISCO.

---

## Partie 1 : Premiers pas avec STM32 et HAL

---

### 1. Où se situe le fichier main.c ?

Le fichier `main.c` se trouve dans le répertoire suivant :  
`Core/Src/main.c`

---

### 2. À quoi servent les commentaires indiquant BEGIN et END ?

Les commentaires `/* USER CODE BEGIN */` et `/* USER CODE END */` servent à délimiter les zones où l'utilisateur peut ajouter son propre code. Lors de la régénération du code avec STM32CubeMX, seul le code placé entre ces balises sera conservé.

---

### 3. Quels sont les paramètres à passer à HAL_Delay et HAL_GPIO_TogglePin ?

- `HAL_Delay()` prend en paramètre un entier représentant le nombre de millisecondes à attendre.  
- `HAL_GPIO_TogglePin()` prend en paramètre deux éléments : le port GPIO (par exemple `GPIOI`) et la broche (par exemple `GPIO_PIN_1`).

---

### 4. Dans quel fichier les ports d’entrée/sorties sont-ils définis ?

Les ports d’entrée/sorties sont définis dans les fichiers générés automatiquement, notamment `gpio.h` et `gpio.c`, ainsi que dans le fichier `stm32f7xx_hal_gpio.h`.

---

### 5. Écrivez un programme simple permettant de faire clignoter la LED. Elle se situe sur la broche PI1.

```c
/* USER CODE BEGIN 2 */
while (1)
{
  HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // Inverse l'état de la LED
  HAL_Delay(500); // Attend 500 millisecondes
}
/* USER CODE END 2 */
```

---

### 6. Quelle est la fonction main ? Où s’arrête-t-elle ?

La fonction `main()` est le point d'entrée du programme. Elle initialise le matériel (HAL, périphériques, GPIOs, etc.), crée les tâches FreeRTOS si nécessaire, puis appelle `osKernelStart()` pour démarrer l’ordonnanceur. Le code ne continue plus après cet appel sauf si une erreur se produit, car FreeRTOS prend alors le contrôle du système.
