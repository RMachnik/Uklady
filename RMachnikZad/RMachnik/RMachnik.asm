/*
 * RMachnik.asm
 *
 *  Created: 11/11/2013 11:25:03 AM
 */ 

 
 //napisac macro sleep parametr liczba sekund
 //przyjmuje ze 16000 cycle counter to 1 sekunda
.MACRO	ONESEC
	LDI R21,0x10 // R21 = 6 poniewaz SECLOOP musi wykonac sie 16 razy
	.DEF SIXTEEN=R21
	LDI R20,0x01 // 1
	.DEF ONE=R20  // ONE=1 wartosc do inkrementacji
	LDI R16,0x00
	.DEF COUNTER=R16 //counter dla SECLOOP
	LDI R18,0xC8 //200
	LDI R17,0x00 //0
	.DEF MAX_COUNTER=R18 //MAX_COUNTER=200 //MAX COUNTER dla petli SECLOOP
	.DEF SEC_COUNTER=R17 //licznik dla duzej petli MYLOOP
	MYLOOP://1000*16 = 16000 = 1 sek
		SECLOOP:  //jedno wykonanie to ~1000  
			INC COUNTER   //+1
			CPSE COUNTER,MAX_COUNTER //+1				
			JMP SECLOOP
		LDI COUNTER,0	//zerujemy licznik dla SECLOOP
		INC SEC_COUNTER //inkrementacja licznika MYLOOP
		CPSE SEC_COUNTER,SIXTEEN 	//wykonuje sie 16 razy					
		JMP MYLOOP					
.ENDM

.MACRO MYSLEEP  // tutaj jest makro ktore liczy sobie sekundy podane jako parametr i wywoluje makro ONESEC ktore wykonuje sie jedna sekunde
	LDI R24,0x00 
	LDI R25,@0 
	MLOOP: //petla ktora wywoluje sie tyle razy ile sekund musimy czekac
		ONESEC 1 //wywolanie makra ktore czeka zawsze jedna sekunde
		INC R24 //inkrementacj licznika petli
		CPSE R25,R24 //sprawdzenie warunku petli
	JMP MLOOP
.ENDM



LDI R16,LOW(RAMEND) //inicjalizacja stosu
OUT SPL,R16
LDI R16, HIGH(RAMEND)
OUT SPH,R16

NAPIS: .DB "Hello world0"
LDI ZL,LOW(NAPIS*2)
LDI ZH,LOW(NAPIS*2)
LPM R1, Z

LOOP:
	
	MYSLEEP 2 // - i na liczniku powinnismy miec ~32000
	LDI R26,48
	TOSTACK: //wrzucanie slowa na stos
		LDD R11, Z+2 //przesuwanie wskaznika Z
		PUSH R11 //wrzucanie na stos
		CPSE R11,R26 // porownywanie w czytaniu az natrafimy na koniec napisy tj 0
	JMP TOSTACK 
	FROMSTACK: //zrzucanie slowa ze stosu
		POP R11 // odczytywanie elementu z wierzcholka stosu
		STD Z+2, R11 // zapisywanie do rejestru
		CPSE R11,R26	//porownanie z warunkiem koncowym
	JMP FROMSTACK
		
			
JMP LOOP