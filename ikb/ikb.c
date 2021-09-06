

#include "../system.h"
#include "../main.h"
#include "../types.h"
#include "ikb.h"

struct _key
{
    PTRFX_retUINT8_T keyRead;
    PTRFX_retVOID keyDo;
    uint8_t sm0;        
    uint8_t counter0;
    uint8_t sm1;
    uint16_t counter1;

    struct _key_prop prop;

    struct _key_statusFlag
    {
        unsigned state:1;
        unsigned readyToRead:1;                 
        unsigned atTimeExpired_beforeOrAfter:1;
        unsigned ownerOfGroup:1;
        unsigned inProcessing:1;
        unsigned __a:3;
    }statusFlag;
};

volatile static struct _key key[KB_NUM_KEYS];
const struct _key_prop propEmpty = {0};

void ikey_scan(void);
void ikey_parsing(void);
int8_t ikey_is_all_hw_released(void);
void ikey_clear_all_flag(void);
int8_t ikey_is_all_process_finished(void);
    
#ifdef iKPAD

    static inline uint8_t keyRead_pin_key0(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY0, PINxKBCOL_KEY0);
    }
    static inline uint8_t keyRead_pin_key1(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY1, PINxKBCOL_KEY1);
    }
    static inline uint8_t keyRead_pin_key2(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY2, PINxKBCOL_KEY2);
    }
    static inline uint8_t keyRead_pin_key3(void)
    {
        PinTo0(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY3, PINxKBCOL_KEY3);
    }
    //
    static inline uint8_t keyRead_pin_key4(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY4, PINxKBCOL_KEY4);
    }
    static inline uint8_t keyRead_pin_key6(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY5, PINxKBCOL_KEY5);
    }
    static inline uint8_t keyRead_pin_key7(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY6, PINxKBCOL_KEY6);
    }
    static inline uint8_t keyRead_pin_key8(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo0(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY7, PINxKBCOL_KEY7);
    }
    //
    static inline uint8_t keyRead_pin_key9(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY8, PINxKBCOL_KEY8);
    }
    static inline uint8_t keyRead_pin_key10(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY9, PINxKBCOL_KEY9);
    }
    static inline uint8_t keyRead_pin_key11(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY10, PINxKBCOL_KEY10);
    }
    static inline uint8_t keyRead_pin_key12(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo0(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo1(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY11, PINxKBCOL_KEY11);
    }
    static inline uint8_t keyRead_pin_key13(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY12, PINxKBCOL_KEY12);
    }
    static inline uint8_t keyRead_pin_key14(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY13, PINxKBCOL_KEY13);
    }
    static inline uint8_t keyRead_pin_key15(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY14, PINxKBCOL_KEY14);
    }
    static inline uint8_t keyRead_pin_key16(void)
    {
        PinTo1(PORTWxKBFIL_0, PINxKBFIL_0);
        PinTo1(PORTWxKBFIL_1, PINxKBFIL_1);
        PinTo1(PORTWxKBFIL_2, PINxKBFIL_2);
        PinTo0(PORTWxKBFIL_3, PINxKBFIL_3);
        KeyPad4x4_readkey_setupTime();
        return ReadPin(PORTRxKBCOL_KEY15, PINxKBCOL_KEY15);
    }
#endif // iKB

#ifdef iKEY
    static inline uint8_t keyRead_pin_key0(void)
    {
        return ReadPin(PORTRxKB_KEY0, PINxKB_KEY0);
    }
    static inline uint8_t keyRead_pin_key1(void)
    {
        return ReadPin(PORTRxKB_KEY1, PINxKB_KEY1);
    }
    static inline uint8_t keyRead_pin_key2(void)
    {
        return ReadPin(PORTRxKB_KEY2, PINxKB_KEY2);
    }
    static inline uint8_t keyRead_pin_key3(void)
    {
        return ReadPin(PORTRxKB_KEY3, PINxKB_KEY3);
    }
    static inline uint8_t keyRead_pin_key4(void)
    {
        return ReadPin(PORTRxKB_KEY4, PINxKB_KEY4);
    }
    static inline uint8_t keyRead_pin_key5(void)
	{
		return ReadPin(PORTRxKB_KEY5, PINxKB_KEY5);
	}
#endif // iKEY

void ikb_setKeyProp(uint8_t i, struct _key_prop prop)
{
    key[i].prop = prop;
}
void ikb_init(void)
{
    uint8_t i;
    #ifdef iKPAD
        ConfigOutputPin(CONFIGIOxKBFIL_0, PINxKBFIL_0);
        ConfigOutputPin(CONFIGIOxKBFIL_1, PINxKBFIL_1);
        ConfigOutputPin(CONFIGIOxKBFIL_2, PINxKBFIL_2);
        ConfigOutputPin(CONFIGIOxKBFIL_3, PINxKBFIL_3);

        ConfigInputPin(CONFIGIOxKBCOL_0, PINxKBCOL_0);
        ConfigInputPin(CONFIGIOxKBCOL_1, PINxKBCOL_1);
        ConfigInputPin(CONFIGIOxKBCOL_2, PINxKBCOL_2);
        ConfigInputPin(CONFIGIOxKBCOL_3, PINxKBCOL_3);

        key[0].keyRead = keyRead_pin_key0;
        key[1].keyRead = keyRead_pin_key1;
        key[2].keyRead = keyRead_pin_key2;
        key[3].keyRead = keyRead_pin_key3;
        key[4].keyRead = keyRead_pin_key4;
        key[5].keyRead = keyRead_pin_key6;
        key[6].keyRead = keyRead_pin_key7;
        key[7].keyRead = keyRead_pin_key8;
        key[8].keyRead = keyRead_pin_key9;
        key[9].keyRead = keyRead_pin_key10;
        key[10].keyRead = keyRead_pin_key11;
        key[11].keyRead = keyRead_pin_key12;
        key[12].keyRead = keyRead_pin_key13;
        key[13].keyRead = keyRead_pin_key14;
        key[14].keyRead = keyRead_pin_key15;
        key[15].keyRead = keyRead_pin_key16;


    #endif // iKPAD
    #ifdef iKEY
        ConfigInputPin(CONFIGIOxKB_KEY0, PINxKB_KEY0);
        ConfigInputPin(CONFIGIOxKB_KEY1, PINxKB_KEY1);
        ConfigInputPin(CONFIGIOxKB_KEY2, PINxKB_KEY2);
        ConfigInputPin(CONFIGIOxKB_KEY3, PINxKB_KEY3);
        ConfigInputPin(CONFIGIOxKB_KEY4, PINxKB_KEY4);
        ConfigInputPin(CONFIGIOxKB_KEY5, PINxKB_KEY5);
        key[0].keyRead = keyRead_pin_key0;
        key[1].keyRead = keyRead_pin_key1;
        key[2].keyRead = keyRead_pin_key2;
        key[3].keyRead = keyRead_pin_key3;
        key[4].keyRead = keyRead_pin_key4;
        key[5].keyRead = keyRead_pin_key5;
    #endif // iKEY

    /*by default init with onKeyPressed = 1*/
    struct _key_prop prop = {0};
    prop = propEmpty;
    prop.uFlag.f.onKeyPressed = 1;
    for (i=0; i< KB_NUM_KEYS; i++)
        {ikb_setKeyProp(i, prop);}
}

void ikb_job(void)
{
    ikey_scan();
    ikey_parsing();
}
void ikb_flush(void)
{
    ikey_clear_all_flag();
}
uint8_t ikb_get_AtTimeExpired_BeforeOrAfter(uint8_t k)
{
    return key[k].statusFlag.atTimeExpired_beforeOrAfter;
}
uint8_t ikb_key_is_ready2read(uint8_t k)
{
    return key[k].statusFlag.readyToRead;
}
void ikb_set_ready2read(uint8_t k)
{
    key[k].statusFlag.readyToRead = 1;
}
void ikb_key_was_read(uint8_t k)
{
    key[k].statusFlag.readyToRead = 0;
}
void ikb_execfunct(uint8_t k)
{
    key[k].keyDo();
}

/************* ikey ****************/
//set by software
#define KB_KEY_STATE_PRESSED 1
#define KB_KEY_STATE_RELEASED 0

#define IKEY_SCAN_SETUP_TIMEuS 5//us

void ikey_scan(void)
{
    int8_t k = KB_NUM_KEYS-1;
    do
    {
        __delay_us(IKEY_SCAN_SETUP_TIMEuS);
        
        if (key[k].sm0 == 0)
        {
            if (key[k].keyRead() == KB_KEY_PINLEVEL_PRESSED)
            {
                key[k].sm0++;
            }
        }
        else if (key[k].sm0 == 1)
        {
            if (++key[k].counter0 == KB_KEY_SCAN_COUNT_DEBOUNCE)
            {
                key[k].counter0 = 0x00;

                if (key[k].keyRead() != KB_KEY_PINLEVEL_PRESSED)
                {
                    key[k].sm0--;
                }
                else
                {
                    key[k].statusFlag.state = KB_KEY_STATE_PRESSED;
                    key[k].sm0++;
                }
            }
        }
        else if (key[k].sm0 == 2)
        {
            if (key[k].keyRead() == KB_KEY_PINLEVEL_RELEASED)
                key[k].sm0++;
        }
        else if (key[k].sm0 == 3)
        {
            if (++key[k].counter0 == KB_KEY_SCAN_COUNT_DEBOUNCE)
            {
                key[k].counter0 = 0x00;

                if (key[k].keyRead() != KB_KEY_PINLEVEL_RELEASED)
                {
                    key[k].sm0--;
                }
                else
                {
                    key[k].statusFlag.state = KB_KEY_STATE_RELEASED;
                    key[k].sm0 = 0x00;
                }
            }
        }
    
    }while (--k >= 0);
}

///////////////////////////////////////////////////////////////////////////
#define NO_GROUP_X 0
//#define AGRUPED 1
#define GROUP_LOCKED 1
#define GROUP_UNLOCKED	0

uint8_t group_x[KB_NUM_KEYS];

static uint8_t ikey_is_accessible(uint8_t k)
{
    if (key[k].prop.numGroup > NO_GROUP_X)	//esta agrupado ?
    {
        //aqui fue puesto al presionar la tecla
        if (group_x[key[k].prop.numGroup] == GROUP_LOCKED)
        {
            //a todos sus companeros se les prohibe el ingreso, solo quien fue el primero en ganar el acceso al grupo entra
            if ( !key[k].statusFlag.ownerOfGroup)	//Si no es quien gano la propiedad...
            {
                return 0;
            }
        }
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////
#define KBAPP_READY2READ	1
#define KBAPP_ALREADYREAD	0

//Stages definition, incremental and contiguous order
enum _STAGE_
{
    _STAGE_START_PARSING_=0,
    _STAGE_ISKEY_RELEASED_,
    _STAGE_ISKEY_RELEASED2_,
    _STAGE_ISKEY_READFROMAPP_,
    _STAGE_FIREATIMEEXPIRED_,
    _STAGE_FIREATIMEEXPIRED_2_,
    _STAGE_ONKEYRELEASED_,
    _STAGE_WHILEPRESSING_,
    //Reptt, _1, _2 deben ser contiguos!
    _STAGE_REPTT_,
    _STAGE_REPTT_1_,
    _STAGE_REPTT_2_,
    //
    _STAGE_END_PARSING_,
};

void ikey_parsing(void)
{
    uint8_t k;

    for (k=0; k < KB_NUM_KEYS; k++)
    {
        if ( key[k].sm1 == _STAGE_START_PARSING_ )
        {
            if ( ikey_is_accessible(k) )
            {
                if (key[k].statusFlag.state == KB_KEY_STATE_PRESSED)
                {
                    //........................................................
                    if (key[k].prop.uFlag.f.onKeyPressed)
                    {
                        key[k].statusFlag.readyToRead = KBAPP_READY2READ;	//Populate Key to app.

                        if (key[k].prop.uFlag.f.reptt)
                        {
                            key[k].sm1 = _STAGE_REPTT_;
                        }
                        else
                        {
                            key[k].sm1 = _STAGE_ISKEY_RELEASED_;
                        }
                    }
                    else if ( key[k].prop.uFlag.f.onKeyReleased)
                    {
                        key[k].sm1 = _STAGE_ONKEYRELEASED_;
                    }
                    else if ( key[k].prop.uFlag.f.whilePressing)
                    {
                        key[k].sm1 = _STAGE_WHILEPRESSING_;
                    }
                    else if (key[k].prop.uFlag.f.atTimeExpired)
                    {
                        key[k].sm1 = _STAGE_FIREATIMEEXPIRED_;
                    }
                    else if (key[k].prop.uFlag.f.atTimeExpired2)
                    {
                        key[k].sm1 = _STAGE_FIREATIMEEXPIRED_2_;
                    }

                    //Add++
                    key[k].statusFlag.inProcessing = 1;
                    //++

                    //........................................................

                    if (key[k].prop.numGroup > NO_GROUP_X)
                    {
                        group_x[ key[k].prop.numGroup ] = GROUP_LOCKED;
                        key[k].statusFlag.ownerOfGroup = 1;
                    }
                    //........................................................
                }
            }
        }

        //_________________________________________________________________________________________
        //comprueba entre las pasadas de repeticion/no repeticion si se ha soltado la tecla
        if ( (key[k].sm1 >= _STAGE_REPTT_ ) && (key[k].sm1 <= _STAGE_REPTT_2_))
        {
            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED)			//ya solto?
            {
                key[k].counter1 = 0;				//stop timmer
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;
            }
        }
        //.............................................................................
        if (key[k].sm1 == _STAGE_REPTT_ )
        {
            if ( ++key[k].counter1 >= key[k].prop.repttTh.breakTime)
            {
                key[k].counter1 =0;
                key[k].sm1 = _STAGE_REPTT_1_;
            }
        }
        if (key[k].sm1 == _STAGE_REPTT_1_)	//ping-pong between 1<->2
        {
            if (key[k].statusFlag.readyToRead == KBAPP_ALREADYREAD)			//ya leyo?
            {
                key[k].statusFlag.readyToRead = KBAPP_READY2READ;				//Repopulate again
                key[k].sm1 = _STAGE_REPTT_2_;
            }
        }
        if (key[k].sm1 == _STAGE_REPTT_2_)	//ping-pong 2<->1
        {
            if ( ++key[k].counter1 >= key[k].prop.repttTh.period)
            {
                key[k].counter1 = 0;
                key[k].sm1 = _STAGE_REPTT_1_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ONKEYRELEASED_ )
        {
            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED)	//ya solto?
            {
                key[k].statusFlag.readyToRead = KBAPP_READY2READ;							//Populate Key to app.
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;		//esperar x leer
            }
        }
        if (key[k].sm1 == _STAGE_WHILEPRESSING_ )   //added 2019
        {
            key[k].statusFlag.readyToRead = KBAPP_READY2READ;	//Populate Key to app.
            key[k].sm1 = _STAGE_ISKEY_RELEASED2_;
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_FIREATIMEEXPIRED_ )
        {
            key[k].counter1++;

            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED)	//ya solto?
            {
                if (key[k].counter1 <= (_FIRE_AT_TIME_THRESHOLD_) )	//USANDO UN SOLO CONTADOR,
                {
                    key[k].counter1	= 0;							//PUES SE USA EN TIEMPOS DIFERENTES

                    key[k].statusFlag.readyToRead = KBAPP_READY2READ;
                    key[k].statusFlag.atTimeExpired_beforeOrAfter = KB_BEFORE_THR;

                    key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;		//Go directly to wait for read key from high-level application
                }
            }

            if (key[k].counter1 > (_FIRE_AT_TIME_THRESHOLD_) )
            {
                key[k].counter1 = 0x0000;

                key[k].statusFlag.readyToRead = KBAPP_READY2READ;
                key[k].statusFlag.atTimeExpired_beforeOrAfter = KB_AFTER_THR;	//queda senializado

                if (!key[k].prop.uFlag.f.reptt)							//x No Reptt
                {
                    key[k].sm1 = _STAGE_ISKEY_RELEASED_;   //wait for key is release
                }
                else
                {
                    key[k].sm1 = _STAGE_REPTT_2_;
                }
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //NO SE ADMITE REPETICION
        //.........|thr|....................................|thr2|
        //before       | no se reconoce como pulsacion      |aftersi cumplio este tiempo, se reconoce como la segunda pulsacion
        if (key[k].sm1 == _STAGE_FIREATIMEEXPIRED_2_)//nueva opcion 2017
        {
            key[k].counter1++;

            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED) //ya solto?
            {
                if (key[k].counter1 <= (_FIRE_AT_TIME_THRESHOLD_)) //primer threshold
                {
                    key[k].counter1 = 0; //PUES SE USA EN TIEMPOS DIFERENTES

                    key[k].statusFlag.readyToRead = KBAPP_READY2READ;
                    key[k].statusFlag.atTimeExpired_beforeOrAfter = KB_BEFORE_THR;

                    key[k].sm1 = _STAGE_ISKEY_READFROMAPP_; //Go directly to wait for read key from high-level application
                }
            }
            /* PODRIA SER OTRA OPCION EN DONDE SI SE RECONOCE ANTES DEL THRESHOLD2 COMO PULSACION*/

            //#define _FIRE_AT_TIME_THRESHOLD2_ (3000E-3/KB_PERIODIC_ACCESS)
            if (key[k].counter1 >= (_FIRE_AT_TIME_THRESHOLD2_))
            {
                key[k].counter1 = 0x0000;

                key[k].statusFlag.readyToRead = KBAPP_READY2READ;
                key[k].statusFlag.atTimeExpired_beforeOrAfter = KB_AFTER_THR; //queda senializado

                //NO SE ADMITE REPETICION
                key[k].sm1 = _STAGE_ISKEY_RELEASED2_; //wait for key is release
            }
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_RELEASED_)		//MODO->FireOnKeyPressed + no Repetitibilidad 	//MODO->solo esperar que suelte
        {
            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED)						//ya solto?
            {
                key[k].sm1 = _STAGE_ISKEY_READFROMAPP_;   //esperar x leer
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_RELEASED2_)		//added 2019
        {
            if (key[k].statusFlag.state == KB_KEY_STATE_RELEASED)					
            {
                key[k].statusFlag.readyToRead = KBAPP_ALREADYREAD;
                key[k].sm1 = _STAGE_END_PARSING_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_ISKEY_READFROMAPP_)
        {
            if (key[k].statusFlag.readyToRead == KBAPP_ALREADYREAD)	//ya leyo? //added 2019: || (terminate_key) la app. mata el proceso...
            {
                key[k].sm1 = _STAGE_END_PARSING_;
            }
        }
        //_________________________________________________________________________________________
        if (key[k].sm1 == _STAGE_END_PARSING_)
        {
            if (key[k].prop.numGroup > NO_GROUP_X)	//forma grupo?
            {
                group_x[key[k].prop.numGroup] = GROUP_UNLOCKED;
                key[k].statusFlag.ownerOfGroup = 0;
            }

            key[k].sm1 = _STAGE_START_PARSING_;

            //Add++
            key[k].statusFlag.inProcessing = 0;
            //++
        }
    }
}
int8_t ikey_is_all_hw_released(void)//hardware release
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        if (key[i].statusFlag.state == KB_KEY_STATE_PRESSED)
        {
            return 0;
        }
    }
    return 1;
}

void ikey_clear_all_flag(void)//clear ReadyToRead
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        //if (key[i].statusFlag.ReadyToRead)
            key[i].statusFlag.readyToRead = 0;
    }
}

int8_t ikey_is_all_process_finished(void)
{
    uint8_t i;
    for (i=0; i<KB_NUM_KEYS; i++)
    {
        if (key[i].statusFlag.inProcessing == 1)
        {
            return 0;
        }
    }
    return 1;
}
