/*
 * 8 bites p�rhuzamos LCD GPIO driver STM32F7-re
 * 5 vez�rl�l�b (CS, RS, WR, RD, RST) + 8 adatl�b + h�tt�rvil�git�s vez�rl�s
 */

//=============================================================================
/* Lcd vez�rl� l�bak hozz�rendel�se (A..M, 0..15) */
#define LCD_CS            F, 3
#define LCD_RS            C, 3
#define LCD_WR            C, 0
#define LCD_RD            A, 3
#define LCD_RST           F, 5

/* Lcd adat l�bak hozz�rendel�se (A..M, 0..15) */
#define LCD_D0            F, 12
#define LCD_D1            D, 15
#define LCD_D2            F, 15
#define LCD_D3            E, 13
#define LCD_D4            F, 14
#define LCD_D5            E, 11
#define LCD_D6            E, 9
#define LCD_D7            F, 13

// Lcd vez�rl� l�bak hozz�rendel�se

// Lcd adat l�bak hozz�rendel�se

/* H�tt�rvil�git�s vez�rl�s
   - BL: A..M, 0..15 (ha nem haszn�ljuk, akkor rendelj�k hozz� az X, 0 �rt�ket)
   - BL_ON: 0 vagy 1, a bekapcsolt �llapothoz tartoz� logikai szint */
#define LCD_BL            X, 0
#define LCD_BLON          0

/* nsec nagys�grend� v�rakoz�s az LCD ir�si �s az olvas�si impulzusn�l
   - kezd� �rt�knek �rdemes 10, 20-bol elindulni, azt�n lehet cs�kkenteni a sebess�g n�vel�se �rdek�ben
     (az �rt�kek f�ggnek a processzor orajel�t�l �s az LCD kijelz� sebess�g�t�l is)
*/
#define LCD_WRITE_DELAY  10
#define LCD_READ_DELAY   20

/*=============================================================================
I/O csoport optimaliz�ci�, hogy ne bitenk�nt t�rt�njenek a GPIO m�veletek:
Megj: ha az adat l�bakat egy porton bel�l emelked� sorrendben defini�ljuk
      automatikusan optimaliz�lva fognak t�rt�nni a GPIO m�veletek akkor is, ha
      itt nem defini�ljuk az optimaliz�lt m�k�d�shez sz�ks�ges elj�r�sokat
A lenti p�lda a k�vetkez� l�bakhoz optimaliz�l:
      LCD_D0<-D14, LCD_D1<-D15, LCD_D2<-D0, LCD_D3<-D1
      LCD_D4<-E7,  LCD_D5<-E8,  LCD_D6<-E9, LCD_D7<-E10 */
#if 0
// 8 adatl�b kimenetre �ll�t�sa (adatir�ny: STM32 -> LCD)
#define LCD_DIRWRITE { \
GPIOD->MODER = (GPIOD->MODER & ~((3 << 2 * 14) | (3 << 2 * 15) | (3 << 2 * 0) | (3 << 2 * 1)))  | \
                                ((1 << 2 * 14) | (1 << 2 * 15) | (1 << 2 * 0) | (1 << 2 * 1));    \
GPIOE->MODER = (GPIOE->MODER & ~((3 << 2 * 7)  | (3 << 2 * 8)  | (3 << 2 * 9) | (3 << 2 * 10))) | \
                                ((1 << 2 * 7)  | (1 << 2 * 8)  | (1 << 2 * 9) | (1 << 2 * 10));   }
// 8 adatl�b bemenetre �ll�t�sa (adatir�ny: STM32 <- LCD)
#define LCD_DIRREAD { \
GPIOD->MODER = (GPIOD->MODER & ~((3 << 2 * 14) | (3 << 2 * 15) | (3 << 2 * 0) | (3 << 2 * 1)))  | \
                                ((0 << 2 * 14) | (0 << 2 * 15) | (0 << 2 * 0) | (0 << 2 * 1));    \
GPIOE->MODER = (GPIOE->MODER & ~((3 << 2 * 7)  | (3 << 2 * 8)  | (3 << 2 * 9) | (3 << 2 * 10))) | \
                                ((0 << 2 * 7)  | (0 << 2 * 8)  | (0 << 2 * 9) | (0 << 2 * 10));   }

// 8 adatl�b �r�sa, STM32 -> LCD (a kiirand� adat a makro dt param�ter�ben van)
#define LCD_WRITE(dt) { \
GPIOD->ODR = (GPIOD->ODR & ~((1 << 14) | (1 << 15) | (1 << 0) | (1 << 1))) |        \
                            (((dt & 0b00000011) << 14) | ((dt & 0b00001100) >> 2)); \
GPIOE->ODR = (GPIOE->ODR & ~((1 << 7) | (1 << 8) | (1 << 9) | (1 << 10))) |         \
                            ((dt & 0b11110000) << (7 - 4));                         }

// 8 adatl�b olvas�sa, STM32 <- LCD (az olvasott adat dt param�terben megadott v�ltozoba ker�l)
#define LCD_READ(dt) { \
dt = ((GPIOD->IDR & 0b1100000000000000) >> (14 - 0)) | ((GPIOD->IDR & 0b0000000000000011) << (2 - 0)) | \
     ((GPIOE->IDR & 0b0000011110000000) >> (7 - 4)); }
#endif