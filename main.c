/* ATmega328P .. OK!
 *
 * Comandos para sincronizar localmente desde el respositorio
 * git stash (1 vez)
 * git pull origin master
 *
 *
 * main.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jcaf

ATmega328P Clock interno @8Mhz, sin divisor, BOD = 2.7V
avrdude -c usbasp -B5 -p m328P -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xfd:m

Consistenciado:
--------------
APAGAR OUT1, SOLO EN MENU 1 PRENDER
APAGAR OUT2, SOLO EN MENU 2 PRENDER
PRIMER TOQUE EN X3, HABILITA A,B,C, SEGUNDO TOQUE EN X3, DESH.A,B,C
MENU1-> OUT1
MENU2-> OUT2
X3 - > TERMINA TODA LA SECUENCIA Y DEJA OUT3,OUT4 A CERO
X3 - > BUCLE OK
X4 -> ACTIVA OUT1
X5 -> ACTIVA OUT2

 */

#include "system.h"
#include "types.h"
#include "main.h"

#include "ikb/ikb.h"


volatile struct _isr_flag
{
	unsigned f1ms :1;
	unsigned __a :7;
} isr_flag;

struct _main_flag
{
	unsigned f1ms :1;
	unsigned X1onoff:1;
	unsigned keysEnable:1;
	unsigned __a:5;

} main_flag;

struct _job
{
	int8_t sm0;//x jobs
	int8_t key_sm0;//x keys
	uint16_t counter;
	int8_t mode;

	struct _job_f
	{
		unsigned enable:1;
		unsigned job:1;
		unsigned lock:1;
		unsigned __a:5;
	}f;
};
//
#define BUZZERMODE_TACTSW 0
#define BUZZERMODE_X3_SEQUENCER 1

//static
const struct _job job_reset;
struct _job keyX3, keyA,keyB,keyC,keyP1,keyP2;
struct _job job_buzzer;


//---------------------------------------------------------------------------------------------
//-------- Definicion del usuario de tiempos de manera general, excepto la secuencia para P1
#define KEY_TIMEPRESSING 	20//ms Tiempo de pulsacion
#define RELAY_TIMESWITCHING 40//ms Tiempo de conmutacion de Relays


//buzzer delays
#define BUZZER_TACTSW_KTIME 50	//50ms
#define BUZZER_X3_KTIME 500		//500ms x Secuencia de P1


#define X3_DELAY_SECONDS_FROM_P1_TO_P2 8//Seg


void outputs_clear(void)
{
	PinTo0(PORTWxOUT_Z, PINxOUT_Z);
	PinTo0(PORTWxOUT_Y, PINxOUT_Y);
	PinTo0(PORTWxOUT_X, PINxOUT_X);

	PinTo0(PORTWxOUT_4, PINxOUT_4);
	PinTo0(PORTWxOUT_3, PINxOUT_3);
	PinTo0(PORTWxOUT_2, PINxOUT_2);
	PinTo0(PORTWxOUT_1, PINxOUT_1);

	PinTo0(PORTWxOUT_R, PINxOUT_R);
	PinTo0(PORTWxOUT_S, PINxOUT_S);
	PinTo0(PORTWxOUT_T, PINxOUT_T);
}
/*
 * Tiempos en milisegundos
 */
#define P1_T1 20
#define P1_T2 80
#define P1_T3 6000
#define P1_T4 6020
#define P1_T5 6120

//#define P1JOB_TOTALTIME (P1_T1 + P1_T2 + P1_T3 + P1_T4 + P1_T5)
#define P1JOB_TOTALTIME (P1_T5)

int8_t keyP1_job(void)//secuencia
{
	if (keyP1.sm0 == 0)
	{
		PinTo0(PORTWxOUT_R, PINxOUT_R);
		PinTo1(PORTWxOUT_S, PINxOUT_S);
		PinTo0(PORTWxOUT_T, PINxOUT_T);
		//
		keyP1.counter = 0x0000;
		keyP1.sm0++;
	}
	else if (keyP1.sm0 == 1)
	{
		if (main_flag.f1ms)
		{
			if (++keyP1.counter >= P1_T1)//20)
			{
				PinTo0(PORTWxOUT_S, PINxOUT_S);
				keyP1.sm0++;
			}
		}
	}
	else if (keyP1.sm0 == 2)
	{
		if (main_flag.f1ms)
		{
			if (++keyP1.counter >= P1_T2)//80)
			{
				PinTo1(PORTWxOUT_R, PINxOUT_R);
				keyP1.sm0++;
			}
		}
	}
	else if (keyP1.sm0 == 3)
	{
		if (main_flag.f1ms)
		{
			if (++keyP1.counter >= P1_T3)//6000)
			{
				PinTo1(PORTWxOUT_T, PINxOUT_T);
				keyP1.sm0++;
			}
		}
	}
	else if (keyP1.sm0 == 4)
	{
		if (main_flag.f1ms)
		{
			if (++keyP1.counter >= P1_T4)//6020)
			{
				PinTo0(PORTWxOUT_T, PINxOUT_T);
				keyP1.sm0++;
			}
		}
	}
	else if (keyP1.sm0 == 5)
	{
		if (main_flag.f1ms)
		{
			if (++keyP1.counter >= P1_T5)//6120)
			{
				PinTo0(PORTWxOUT_R, PINxOUT_R);
				//
				keyP1.counter = 0x0000;
				keyP1.sm0 = 0x00;

				keyP1.f.job = 0;

				return 1;
			}
		}
	}
	return 0;
}

int8_t keyP2_job(void)
{
	if (keyP2.sm0 == 0)
	{
		PinTo1(PORTWxOUT_S, PINxOUT_S);
		//
		keyP2.counter = 0x0000;
		keyP2.sm0++;
	}
	else if (keyP2.sm0 == 1)
	{
		if (main_flag.f1ms)
		{
			if (++keyP2.counter >= KEY_TIMEPRESSING)	//20)//20ms
			{
				PinTo0(PORTWxOUT_S, PINxOUT_S);
				//
				keyP2.counter = 0x0000;
				keyP2.sm0 = 0x00;

				keyP2.f.job = 0;

				return 1;
			}
		}
	}
	return 0;
}



int main(void)
{
	int8_t kb_counter=0;

	//Active pull-up
	PinTo1(PORTWxKB_KEY0, PINxKB_KEY0);
	PinTo1(PORTWxKB_KEY1, PINxKB_KEY1);
	PinTo1(PORTWxKB_KEY2, PINxKB_KEY2);
	PinTo1(PORTWxKB_KEY3, PINxKB_KEY3);
	PinTo1(PORTWxKB_KEY4, PINxKB_KEY4);
	PinTo1(PORTWxKB_KEY5, PINxKB_KEY5);
	__delay_ms(1);
	ikb_init();

	//
	PinTo0(PORTWxBUZZER, PINxBUZZER);
	ConfigOutputPin(CONFIGIOxBUZZER, PINxBUZZER);
	//
	PinTo0(PORTWxLED1, PINxLED1);
	ConfigOutputPin(CONFIGIOxLED1, PINxLED1);
	//
	PinTo0(PORTWxLED2, PINxLED2);
	ConfigOutputPin(CONFIGIOxLED2, PINxLED2);

	//Outputs
	outputs_clear();

	ConfigOutputPin(CONFIGIOxOUT_Z, PINxOUT_Z);
	ConfigOutputPin(CONFIGIOxOUT_Y, PINxOUT_Y);
	ConfigOutputPin(CONFIGIOxOUT_X, PINxOUT_X);

	ConfigOutputPin(CONFIGIOxOUT_4, PINxOUT_4);
	ConfigOutputPin(CONFIGIOxOUT_3, PINxOUT_3);
	ConfigOutputPin(CONFIGIOxOUT_2, PINxOUT_2);
	ConfigOutputPin(CONFIGIOxOUT_1, PINxOUT_1);

	ConfigOutputPin(CONFIGIOxOUT_R, PINxOUT_R);
	ConfigOutputPin(CONFIGIOxOUT_S, PINxOUT_S);
	ConfigOutputPin(CONFIGIOxOUT_T, PINxOUT_T);


	//Config to 1ms
	TCNT0 = 0x00;
	TCCR0A = (1 << WGM01);
	TCCR0B =  (0 << CS02) | (1 << CS01) | (1 << CS00); //CTC, PRES=64
	OCR0A = CTC_SET_OCR_BYTIME(1e-3, 64);//1ms Exacto @PRES=64
	//
	TIMSK0 |= (1 << OCIE0A);
	sei();
	while (1);
	//

	main_flag.X1onoff = 1;
	main_flag.keysEnable = 1;


	keyA.f.enable = keyB.f.enable = keyC.f.enable = keyX3.f.enable = keyP1.f.enable = keyP2.f.enable = 1;

	//Inicia con secuencia en B
	PinTo0(PORTWxOUT_Z, PINxOUT_Z);
	__delay_ms(RELAY_TIMESWITCHING);
	PinTo1(PORTWxOUT_Y, PINxOUT_Y);

	keyP2.f.lock = 1;

	while (1)
	{
		if (isr_flag.f1ms)
		{
			isr_flag.f1ms = 0;
			main_flag.f1ms = 1;
		}
		//----------------------
		if (main_flag.f1ms)
		{
			if (++kb_counter >= 20)//20ms acceso al keyboard
			{
				kb_counter = 0x00;

				ikb_job();

				if ( (main_flag.X1onoff == 1) && (main_flag.keysEnable) )
				{
					if (ikb_key_is_ready2read(KB_LYOUT_KEY_A))
					{
						if (keyA.f.enable)
						{
							keyA.f.job = 1;

							job_buzzer.mode = BUZZERMODE_TACTSW;
							job_buzzer.f.job = 1;
						}
					}
					if (ikb_key_is_ready2read(KB_LYOUT_KEY_B))
					{
						if (keyB.f.enable)
						{
							if (keyB.f.lock == 0)
							{
								keyB.f.lock = 1;//B locked
								keyC.f.lock = 0;//C unlock
								//
								keyB.f.job = 1;
								//
								job_buzzer.mode = BUZZERMODE_TACTSW;
								job_buzzer.f.job = 1;
							}
						}
					}
					if (ikb_key_is_ready2read(KB_LYOUT_KEY_C))
					{
						if (keyC.f.enable)
						{
							if (keyC.f.lock == 0)
							{
								keyC.f.lock = 1;//B locked
								keyB.f.lock = 0;//C unlock
								//
								keyC.f.job = 1;

								//
								job_buzzer.mode = BUZZERMODE_TACTSW;
								job_buzzer.f.job = 1;
							}
						}
					}

					if (ikb_key_is_ready2read(KB_LYOUT_KEY_X3))
					{
						if ( (!keyX3.f.job) && (!keyP1.f.job) )
						{
							if (keyX3.f.enable)
							{
								keyP1.f.enable = 0;
								keyP2.f.enable = 0;
								keyA.f.enable = keyB.f.enable = keyC.f.enable = 0;//Disable A,B,C

								keyX3.f.job = 1;
								keyX3.sm0 = 0;
								//
								job_buzzer.mode = BUZZERMODE_X3_SEQUENCER;
								job_buzzer.sm0 = 0;
								job_buzzer.f.job = 1;
								//

							}
						}
					}

					if (ikb_key_is_ready2read(KB_LYOUT_KEY_P1))
					{
						if (keyP1.f.enable)
						{
							if (!keyP1.f.lock)
							{
								keyP1.f.lock = 1;
								keyP2.f.lock = 1;
								keyA.f.enable = keyB.f.enable = keyC.f.enable = 0;//Disable A,B,C
								keyX3.f.enable = 0;
								//
								keyP1.f.job = 1;

								//
								PinTo1(PORTWxLED2, PINxLED2);//add

								job_buzzer.mode = BUZZERMODE_X3_SEQUENCER;
								job_buzzer.sm0 = 0;
								job_buzzer.f.job = 1;
								//
							}
						}

					}
					//---------------------------
					if (ikb_key_is_ready2read(KB_LYOUT_KEY_P2))
					{
						if (keyP2.f.enable)
						{
							if (!keyP2.f.lock)
							{
								keyP1.f.lock = 1;
								keyP2.f.lock = 1;
								//
								keyA.f.enable = keyB.f.enable = keyC.f.enable = 0;//Disable A,B,C
								keyX3.f.enable = 0;
								//
								keyP2.f.job = 1;
								//
								job_buzzer.mode = BUZZERMODE_TACTSW;
								job_buzzer.f.job = 1;
							}

						}

					}
				}
			}
		}
		//

		if (main_flag.X1onoff == 1)
		{

			//Activa OUT X 1 pulso = 20ms
			if (keyA.f.job)
			{
				if (keyA.sm0 == 0)
				{
					PinTo1(PORTWxOUT_X, PINxOUT_X);
					keyA.counter = 0;
					keyA.sm0++;
				}
				else if (keyA.sm0 == 1)
				{
					if (main_flag.f1ms)
					{
						if (++keyA.counter >= KEY_TIMEPRESSING) //20)//20ms
						{
							PinTo0(PORTWxOUT_X, PINxOUT_X);
							keyA.counter = 0;
							keyA.sm0 = 0;
							keyA.f.job = 0;
						}
					}
				}
			}
			//Y=on after 40ms, Z=off
			if (keyB.f.job)
			{
				if (keyB.sm0 == 0)
				{
					PinTo0(PORTWxOUT_Z, PINxOUT_Z);
					keyB.counter = 0;
					keyB.sm0++;
				}
				else if (keyB.sm0 == 1)
				{
					if (main_flag.f1ms)
					{
						if (++keyB.counter >= RELAY_TIMESWITCHING) //40)//40ms
						{
							PinTo1(PORTWxOUT_Y, PINxOUT_Y);
							keyB.counter = 0;
							keyB.sm0 = 0;
							keyB.f.job = 0;
						}
					}
				}
			}
			//
			//Z=on after +40ms, Y=off
			if (keyC.f.job)
			{
				if (keyC.sm0 == 0)
				{
					PinTo0(PORTWxOUT_Y, PINxOUT_Y);
					keyC.counter = 0;
					keyC.sm0++;
				}
				else if (keyC.sm0 == 1)
				{
					if (main_flag.f1ms)
					{
						if (++keyC.counter >= RELAY_TIMESWITCHING)	//40)//40ms
						{
							PinTo1(PORTWxOUT_Z, PINxOUT_Z);
							keyC.counter = 0;
							keyC.sm0 = 0;
							keyC.f.job = 0;
						}
					}
				}
			}
			//
			if (keyX3.f.job)
			{
				if (keyX3.sm0 == 0)
				{
					PinTo1(PORTWxOUT_4, PINxOUT_4);

					//
					keyX3.counter = 0x00;
					keyX3.sm0++;
				}
				else if (keyX3.sm0 == 1)
				{
					if (main_flag.f1ms)
					{
						if (++keyX3.counter >= RELAY_TIMESWITCHING)//	40)//40ms
						{
							keyX3.counter = 0x00;
							PinTo1(PORTWxOUT_3, PINxOUT_3);
							//
							keyX3.sm0++;
						}
					}
				}
				else if (keyX3.sm0 == 2)
				{
					//EJEC #2
					if (keyP1_job())
					{
						keyX3.sm0++;
					}
				}
				else if (keyX3.sm0 == 3)
				{
					if (main_flag.f1ms)
					{
						//delay 8s + EJEC #3
						if (++keyX3.counter >= 1000*X3_DELAY_SECONDS_FROM_P1_TO_P2)
						{
							keyX3.counter = 0x0000;
							keyX3.sm0++;
						}
					}
				}
				else if (keyX3.sm0 == 4)
				{
					if (keyP2_job())
					{
						keyX3.f.job = 0;	//finish sequence

						keyX3.sm0 = 0;
						//
						PinTo0(PORTWxOUT_3, PINxOUT_3);
						PinTo0(PORTWxOUT_4, PINxOUT_4);
						//
						PinTo0(PORTWxBUZZER, PINxBUZZER);
						PinTo0(PORTWxLED1, PINxLED1);//add
						job_buzzer = job_reset;

						//+ADD
						keyA.f.enable = keyB.f.enable = keyC.f.enable = 1;//enable A,B,C
						keyP1.f.enable = keyP2.f.enable = 1;
					}
				}
			}

			/*P1 P2 Secuenciadores
			 *
			 *Una vez que sus jobs=1, terminan por si solos,
			 *aun asi cambiando de tecla
			 */
			/***************************
			 * SERIA BUENO SEPARAR KEYP1 DE OTRO SECUENCERxP1 Y SECUENCERxP2
			 **************************/
			if (keyP1.f.job)
			{
				if (keyP1_job())
				{
					keyP1.f.job = 0;
					//
					//keyP1.f.lock = 0;//P1 queda bloqueada y P2 lo desbloquea y viceversa
					keyP2.f.lock = 0;

					PinTo0(PORTWxBUZZER, PINxBUZZER);

					PinTo0(PORTWxLED2, PINxLED2);//add

					job_buzzer = job_reset;
					//

					//keyA.f.enable = keyB.f.enable = keyC.f.enable = 1;//Enable A,B,C
					//keyX3.f.enable = 1;
				}
			}
			if (keyP2.f.job)
			{
				if (keyP2_job())
				{
					keyP2.f.job = 0;
					//
					keyP1.f.lock = 0;//unlock P1
					//keyP2.f.lock = 0;
					//
					keyA.f.enable = keyB.f.enable = keyC.f.enable = 1;//Enable A,B,C
					keyX3.f.enable = 1;
				}
			}


			//Buzzer
			if (job_buzzer.f.job)
			{
				if (job_buzzer.mode == BUZZERMODE_TACTSW )
				{
					if (job_buzzer.sm0 == 0)
					{

						PinTo1(PORTWxBUZZER, PINxBUZZER);
						job_buzzer.counter = 0;
						job_buzzer.sm0++;
					}
					else if (job_buzzer.sm0 == 1)
					{
						if (main_flag.f1ms)
						{
							if (++job_buzzer.counter >= BUZZER_TACTSW_KTIME)
							{

								PinTo0(PORTWxBUZZER, PINxBUZZER);


								job_buzzer.counter = 0;
								job_buzzer.sm0 = 0x0;
								job_buzzer.f.job = 0;
							}
						}
					}
				}
				else if (job_buzzer.mode == BUZZERMODE_X3_SEQUENCER)
				{
					if (job_buzzer.sm0 == 0)
					{
						//add
						if (keyX3.f.job)
						{PinTo1(PORTWxLED1, PINxLED1);}
						//
						PinTo1(PORTWxBUZZER, PINxBUZZER);
						job_buzzer.counter = 0;
						job_buzzer.sm0++;
					}
					else if (job_buzzer.sm0 == 1)
					{
						if (main_flag.f1ms)
						{
							if (++job_buzzer.counter >= BUZZER_X3_KTIME )
							{
								//add
								if (keyX3.f.job)
								{PinToggle(PORTWxLED1, PINxLED1);}
								//
								PinToggle(PORTWxBUZZER, PINxBUZZER);
								job_buzzer.counter = 0;
								//buzzer.sm0 = 0x0;
								//buzzer.f.job = 0;
							}
						}
					}
				}
			}


		}//if onoff

		//
		main_flag.f1ms = 0;
		ikb_flush();

	}//end while

	return 0;
}

ISR(TIMER0_COMPA_vect)
{
	isr_flag.f1ms = 1;
}

