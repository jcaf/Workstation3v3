/*
Reglas:
-primero se lee el teclado. el indice empiezo con ellos
*/
#ifndef iKB_H_
#define iKB_H_
    //#include "types.h"
    
    //#define iKPAD
    #define iKEY
    
    #define KB_NUM_KEYS 6  //Total Number of keys

    #define KB_PERIODIC_ACCESS 20//msE-3
    #define KB_KEY_SCAN_COUNT_DEBOUNCE 1

    //scan hardware
    #define KB_KEY_PINLEVEL_PRESSED 0
    #define KB_KEY_PINLEVEL_RELEASED 1
    //
    #define _FIRE_AT_TIME_THRESHOLD_ (1000.0/KB_PERIODIC_ACCESS)//in ms
    #define _FIRE_AT_TIME_THRESHOLD2_ (2000.0/KB_PERIODIC_ACCESS)//in ms
    #define KB_BEFORE_THR	0
    #define KB_AFTER_THR	1
    //////////////////////////////////////////////////////////////////////////////////////

    #ifdef iKPAD
        /* physical pinout KEYPAD4x4
        PORTWxKBCOL_1 PORTWxKBCOL_2 PORTWxKBCOL_3 PORTWxKBCOL_4
        PORTWxKBFIL_1 	1  2  3  4
        PORTWxKBFIL_2 	5  6  7  8
        PORTWxKBFIL_3 	9  10 11 12
        PORTWxKBFIL_4 	13 14 15 16
         */
        #define KeyPad4x4_readkey_setupTime() _delay_us(2)

        #define PORTWxKBFIL_0 		PORTD
        #define PORTRxKBFIL_0 		PIND
        #define CONFIGIOxKBFIL_0 	DDRD
        #define PINxKBFIL_0 		2

        #define PORTWxKBFIL_1 		PORTD
        #define PORTRxKBFIL_1 		PIND
        #define CONFIGIOxKBFIL_1 	DDRD
        #define PINxKBFIL_1 		3

        #define PORTWxKBFIL_2 		PORTD
        #define PORTRxKBFIL_2 		PIND
        #define CONFIGIOxKBFIL_2 	DDRD
        #define PINxKBFIL_2 		4

        #define PORTWxKBFIL_3 		PORTD
        #define PORTRxKBFIL_3 		PIND
        #define CONFIGIOxKBFIL_3 	DDRD
        #define PINxKBFIL_3 		5

        #define PORTWxKBCOL_0 		PORTD
        #define PORTRxKBCOL_0 		PIND
        #define CONFIGIOxKBCOL_0 	DDRD
        #define PINxKBCOL_0 		6

        #define PORTWxKBCOL_1 		PORTD
        #define PORTRxKBCOL_1 		PIND
        #define CONFIGIOxKBCOL_1 	DDRD
        #define PINxKBCOL_1		    7

        #define PORTWxKBCOL_2 		PORTB
        #define PORTRxKBCOL_2 		PINB
        #define CONFIGIOxKBCOL_2 	DDRB
        #define PINxKBCOL_2 		0

        #define PORTWxKBCOL_3 		PORTB
        #define PORTRxKBCOL_3 		PINB
        #define CONFIGIOxKBCOL_3 	DDRB
        #define PINxKBCOL_3 		1

        //////////////////////////////////////
        #define KB_LYOUT_KEY_1      0
        #define KB_LYOUT_KEY_2      1
        #define KB_LYOUT_KEY_3      2
        #define KB_LYOUT_KEY_UP     3

        #define KB_LYOUT_KEY_4      4
        #define KB_LYOUT_KEY_5      5
        #define KB_LYOUT_KEY_6      6
        #define KB_LYOUT_KEY_DOWN   7

        #define KB_LYOUT_KEY_7      8
        #define KB_LYOUT_KEY_8      9
        #define KB_LYOUT_KEY_9      10
        #define KB_LYOUT_KEY_2ND    11

        #define KB_LYOUT_KEY_CLEAR  12
        #define KB_LYOUT_KEY_0      13
        #define KB_LYOUT_KEY_MENU   14
        #define KB_LYOUT_KEY_ENTER  15
    #endif // iKB

    #ifdef iKEY
        #define PORTWxKB_KEY0 		PORTB
        #define PORTRxKB_KEY0 		PINB
        #define CONFIGIOxKB_KEY0 	DDRB
        #define PINxKB_KEY0 		0

		#define PORTWxKB_KEY1 		PORTD
        #define PORTRxKB_KEY1 		PIND
        #define CONFIGIOxKB_KEY1 	DDRD
        #define PINxKB_KEY1 		7

		#define PORTWxKB_KEY2 		PORTD
        #define PORTRxKB_KEY2 		PIND
        #define CONFIGIOxKB_KEY2 	DDRD
        #define PINxKB_KEY2 		6

		#define PORTWxKB_KEY3 		PORTD
        #define PORTRxKB_KEY3 		PIND
        #define CONFIGIOxKB_KEY3 	DDRD
        #define PINxKB_KEY3 		5

		#define PORTWxKB_KEY4 		PORTB
        #define PORTRxKB_KEY4 		PINB
        #define CONFIGIOxKB_KEY4 	DDRB
        #define PINxKB_KEY4 		7

		#define PORTWxKB_KEY5 		PORTB
        #define PORTRxKB_KEY5 		PINB
        #define CONFIGIOxKB_KEY5 	DDRB
        #define PINxKB_KEY5 		6

		//
		//ikey layout
		#define KB_LYOUT_KEY_X3 0
		#define KB_LYOUT_KEY_A 	1
		#define KB_LYOUT_KEY_B 	2
		#define KB_LYOUT_KEY_C 	3
		#define KB_LYOUT_KEY_P1 4
		#define KB_LYOUT_KEY_P2 5
		//

    #endif // iKEY

    void ikb_init(void);
    void ikb_job(void);
    void ikb_flush(void);
    //
    uint8_t ikb_get_AtTimeExpired_BeforeOrAfter(uint8_t k);
    //
    uint8_t ikb_key_is_ready2read(uint8_t k);
    void ikb_set_ready2read(uint8_t k);
    void ikb_key_was_read(uint8_t k);
    void ikb_execfunct(uint8_t k);

    //void ikb_processKeyRead_D(void);
    //void kb_change_keyDo(const PTRFX_retVOID const *keyDo);
    //void kb_change_keyDo(PTRFX_retVOID *keyDo);
    //void kb_change_keyDo_pgm(PTRFX_retVOID const *  keyDo);
    //////////////////////////////////////////////////////////////////////////////////////
    struct _key_prop
    {
        union _key_prop_uFlag
        {
            struct _key_prop_uFlag_f
            {
                unsigned onKeyPressed:1;//when pressed
                unsigned reptt:1;
                unsigned onKeyReleased:1;//when released
                unsigned whilePressing:1;//new 2019
                unsigned atTimeExpired:1;
                unsigned atTimeExpired2:1;//new mode 2017
                unsigned __a:2;
            }f;
            
            uint8_t packed;
        }uFlag;
        
        struct _repttTh
        {
            uint16_t breakTime;     //break this time to enter to repetition
            uint16_t period;        //each time access to repp after the "breakTime"
        }repttTh;

        uint8_t  numGroup;
    };
    void ikb_setKeyProp(uint8_t i, struct _key_prop prop);
    extern const struct _key_prop propEmpty;
#endif
