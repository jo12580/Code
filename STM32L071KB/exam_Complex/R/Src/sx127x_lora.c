/*******************************************************************************
*                                  sx127x Driver
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2025 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief  sx127x Driver
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-22  sup, first implementation.
 * \endinternal
 */


#include "sx127x_lora.h"


//#define XTAL_26MHZ
#define XTAL_32MHZ

#define LOW_DATARATE_FORCE      0             /**< \brief ǿ�ƿ����������Ż� */

#ifdef XTAL_32MHZ
#define FREQ_STEP               61.03515625   /**< \brief �ֱ���*/
#endif
#ifdef XTAL_26MHZ
#define FREQ_STEP               49.59106445   /**< \brief �ֱ���*/
#endif

/**
 * \brief ��Ƶ��Ƶ�ٽ�
 */
#define __LORA_RF_MID_BAND_THRESH               525000000

/**
 * ���� RSSI ��Ҫ�õĳ��� (RSSIƫ��ֵ)
 */
#define __LORA_RSSI_OFFSET_LF                   -164
#define __LORA_RSSI_OFFSET_HF                   -157


#define SM_STATE_SET(p_this, state)   ((p_this)->run_state.sm_state = state)
#define SM_STATE_GET(p_this)          ((p_this)->run_state.sm_state)

#define CAD_STATE_SET(p_this, state)  ((p_this)->run_state.cad_state |= state)
#define CAD_STATE_GET(p_this)         ((p_this)->run_state.cad_state)
#define CAD_STATE_CLR(p_this)         ((p_this)->run_state.cad_state = 0)

static int __sx127x_lora_set_op_mode(radio_sx127x_lora_dev_t *p_this, uint8_t opMode);
static int __sx127x_lora_set_rf_frequency (radio_sx127x_lora_dev_t *p_this, uint32_t freq);
static int __sx127x_lora_set_payload_length (radio_sx127x_lora_dev_t *p_this, uint8_t value);
static void __sx127x_lora_set_symb_timeout (radio_sx127x_lora_dev_t *p_this, uint16_t value);
static void __sx127x_lora_set_low_datarate_optimize (radio_sx127x_lora_dev_t *p_this, uint8_t enable);
static void __sx127x_tx_invert_iq_set (radio_sx127x_lora_dev_t *p_this, uint8_t value);
static void __sx127x_rx_invert_iq_set (radio_sx127x_lora_dev_t *p_this, uint8_t value);
static void __sx127x_bw_sensitivity_optimization (radio_sx127x_lora_dev_t *p_this, uint8_t bw);





static void __delay_ms (radio_sx127x_lora_dev_t  *p_this, uint32_t ms)
{
    p_this->p_delay->pfn_delay_ms(ms);
}

static void __delay_us (radio_sx127x_lora_dev_t  *p_this, uint32_t us)
{
    p_this->p_delay->pfn_delay_us(us);
}

static void __send_byte (radio_sx127x_lora_dev_t *p_this, uint8_t byte)
{
    p_this->p_spi->pfn_spi_write_byte(byte);
}

static uint8_t __recv_byte (radio_sx127x_lora_dev_t *p_this)
{
    return p_this->p_spi->pfn_spi_read_byte();
}

static void __sx127x_write_buffer (radio_sx127x_lora_dev_t *p_this,
                                   uint8_t             addr,
                                   uint8_t            *buf,
                                   uint8_t             size)
{
    uint8_t i;

    p_this->p_gpio->pfn_sel_pin_set(0);

    __send_byte(p_this, addr | 0x80);
    for (i = 0; i < size; i++)
    {
        __send_byte(p_this, buf[i]);
    }

    p_this->p_gpio->pfn_sel_pin_set(1);
}

static void __sx127x_read_buffer (radio_sx127x_lora_dev_t *p_this,
                                  uint8_t             addr,
                                  uint8_t            *buf,
                                  uint8_t             size)
{
    uint8_t i;

    p_this->p_gpio->pfn_sel_pin_set(0);

    __send_byte(p_this, addr & 0x7F);

    for (i = 0; i < size; i++ )
    {
        buf[i] = __recv_byte(p_this);
    }

    p_this->p_gpio->pfn_sel_pin_set(1);
}

/*
 * \brief ��FIFO�ж�ȡ����
 */
static void __sx127x_read_fifo (radio_sx127x_lora_dev_t *p_this, uint8_t *buffer, uint8_t size)
{
    __sx127x_read_buffer(p_this, 0, buffer, size);
}

/*
 * \brief ��FIFOд������
 */
static void __sx127x_write_fifo (radio_sx127x_lora_dev_t *p_this, uint8_t *buffer, uint8_t size)
{
    __sx127x_write_buffer(p_this, 0, buffer, size);
}

/*
 * \brief ��Ĵ���д������
 */
static void __sx127x_write_reg (radio_sx127x_lora_dev_t *p_this,
                                uint8_t                  addr,
                                uint8_t                  data)
{
    __sx127x_write_buffer(p_this, addr, &data, 1);
}

/*
 * \brief �ӼĴ�����ȡ����
 */
static uint8_t __sx127x_read_reg (radio_sx127x_lora_dev_t *p_this,
                                  uint8_t                  addr)
{
    uint8_t reg;

    __sx127x_read_buffer(p_this, addr, &reg, 1);
    return reg;
}

uint16_t crc16_ccitt(uint8_t *p_buf, uint32_t len)
{
    uint16_t ccitt16 = 0x1021;
    uint16_t crc     = 0;
    int i, j;

    for (i = 0; i < len; i++)
    {
        crc ^= (p_buf[i] << 8);
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
            {
                crc <<= 1;
                crc ^= ccitt16;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

/*
 * \brief ��λsx127x
 */
int __sx127x_reset (radio_sx127x_lora_dev_t *p_this)
{
    if( p_this->p_gpio->pfn_reset_pin_set == NULL )
    {
        return -1;
    }

    p_this->p_gpio->pfn_reset_pin_set(0);
    __delay_ms(p_this, 2);
    p_this->p_gpio->pfn_reset_pin_set(1);
    __delay_ms(p_this, 10);

    return 0;
}




static int __sx127x_lora_rx_state_enter (radio_sx127x_lora_dev_t *p_this)
{
    uint8_t reg;
    uint8_t sf;
    uint8_t bw;
    //    uint8_t fifo_addr_reg;
    uint8_t hop_channel;

    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);

    __sx127x_bw_sensitivity_optimization(p_this, p_this->p_devinfo->p_lora_param->bandwidth);

    __sx127x_rx_invert_iq_set(p_this, p_this->p_devinfo->p_lora_param->rx_invert_iq);

    /* �ŵ������� */
    if (p_this->p_devinfo->p_lora_param->bandwidth < 9)
    {
        reg = __sx127x_read_reg(p_this, REG_LR_DETECTOPTIMIZE);
        __sx127x_write_reg(p_this, REG_LR_DETECTOPTIMIZE, reg & 0x7F);
        __sx127x_write_reg(p_this, REG_LR_TEST30, 0x00);
        switch (p_this->p_devinfo->p_lora_param->bandwidth)
        {

        case 0: // 7.8 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x48);
            __sx127x_lora_set_rf_frequency(p_this, 7.81e3);
            break;

        case 1: // 10.4 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x44);
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency + 7.81e3);
            break;

        case 2: // 15.6 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x44);
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency + 15.62e3);
            break;

        case 3: // 20.8 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x44);
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency + 20.83e3);
            break;

        case 4: // 31.2 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x44);
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency + 31.25e3);
            break;

        case 5: // 41.4 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x44);
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency + 41.67e3);
            break;

        case 6: // 62.5 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x40);
            break;

        case 7: // 125 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x40);
            break;

        case 8: // 250 kHz
            __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x40);
            break;
        }
    }
    else
    {
        // __sx127x_write_reg(p_this, REG_LR_TEST2F, 0x3C);
        reg = __sx127x_read_reg(p_this, REG_LR_DETECTOPTIMIZE);
        __sx127x_write_reg(p_this, REG_LR_DETECTOPTIMIZE, reg | 0x80);
    }

#if (LOW_DATARATE_FORCE == 0)
    /* ���õ������Ż� */
    sf = p_this->p_devinfo->p_lora_param->sf;
    bw = p_this->p_devinfo->p_lora_param->bandwidth;
    if (((bw == 6) && ((sf == 10) || (sf == 11) || (sf == 12))) ||
            ((bw == 7) && ((sf == 11) || (sf == 12))) ||
            ((bw == 8) && (sf == 12)) ||
            (p_this->p_devinfo->p_lora_param->freq_hop_on))
    {
        __sx127x_lora_set_low_datarate_optimize(p_this, 1);
    }
    else
    {
        __sx127x_lora_set_low_datarate_optimize(p_this, 0);
    }
#endif

    /* ������Ƶ  */
    if (p_this->p_devinfo->p_lora_param->freq_hop_on == 1)
    {
        __sx127x_write_reg(p_this,
                           REG_LR_PLLHOP,
                           (__sx127x_read_reg(p_this, REG_LR_PLLHOP) &
                            RFLR_PLLHOP_FASTHOP_MASK) |
                           RFLR_PLLHOP_FASTHOP_ON);
        __sx127x_write_reg(p_this,
                           REG_LR_HOPPERIOD,
                           p_this->p_devinfo->p_lora_param->hop_period);
        hop_channel    = __sx127x_read_reg(p_this, REG_LR_HOPCHANNEL) & RFLR_HOPCHANNEL_CHANNEL_MASK;
        __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->p_hop_tab[hop_channel %
                                       p_this->p_devinfo->p_lora_param->hop_tab_len]);
    }

    if (p_this->p_devinfo->p_lora_param->freq_hop_on == 1)
    {
        /* �����ж�����  */
        __sx127x_write_reg(p_this,
                           REG_LR_IRQFLAGSMASK,
                           //RFLR_IRQFLAGS_RXTIMEOUT |
                           //RFLR_IRQFLAGS_RXDONE |
                           //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                           RFLR_IRQFLAGS_VALIDHEADER |
                           RFLR_IRQFLAGS_TXDONE |
                           RFLR_IRQFLAGS_CADDONE |
                           //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                           RFLR_IRQFLAGS_CADDETECTED);

        // DIO0=RxDone, DIO2=FhssChangeChannel
        __sx127x_write_reg(p_this,
                           REG_LR_DIOMAPPING1,
                           (__sx127x_read_reg(p_this, REG_LR_DIOMAPPING1) &
                            RFLR_DIOMAPPING1_DIO0_MASK &
                            RFLR_DIOMAPPING1_DIO2_MASK) |
                           RFLR_DIOMAPPING1_DIO0_00 |
                           RFLR_DIOMAPPING1_DIO2_00);
    }
    else
    {
        /* �����ж�����  */
        __sx127x_write_reg(p_this,
                           REG_LR_IRQFLAGSMASK,
                           //RFLR_IRQFLAGS_RXTIMEOUT |
                           //RFLR_IRQFLAGS_RXDONE |
                           //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                           //RFLR_IRQFLAGS_VALIDHEADER |
                           RFLR_IRQFLAGS_TXDONE |
                           RFLR_IRQFLAGS_CADDONE |
                           RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                           RFLR_IRQFLAGS_CADDETECTED);

        // DIO0=RxDone,DIO3=ValidHeader
        __sx127x_write_reg(p_this,
                           REG_LR_DIOMAPPING1,
                           (__sx127x_read_reg(p_this, REG_LR_DIOMAPPING1) &
                            RFLR_DIOMAPPING1_DIO0_MASK &
                            RFLR_DIOMAPPING1_DIO1_MASK &
                            RFLR_DIOMAPPING1_DIO3_MASK) |
                           RFLR_DIOMAPPING1_DIO0_00 |
                           RFLR_DIOMAPPING1_DIO1_00 |
                           RFLR_DIOMAPPING1_DIO3_01);
    }

    __sx127x_write_reg(p_this,
                       REG_LR_DIOMAPPING2,
                       (__sx127x_read_reg(p_this, REG_LR_DIOMAPPING2) &
                        RFLR_DIOMAPPING2_DIO4_MASK) |
                       RFLR_DIOMAPPING2_DIO4_10);

    /* ��������Payload */
    if (p_this->p_devinfo->p_lora_param->header_mode == 1)
    {
        if (p_this->p_devinfo->p_lora_param->crc_software == 1)
        {
            __sx127x_lora_set_payload_length(p_this, p_this->p_devinfo->p_lora_param->payload_len + 2);
        }
        else
        {
            __sx127x_lora_set_payload_length(p_this, p_this->p_devinfo->p_lora_param->payload_len);
        }
    }

    __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, 0xFF);

    SM_STATE_SET(p_this, RFLR_STATE_RX_RUNNING);

    if (p_this->p_devinfo->p_lora_param->rx_mode == SINGLE_RECV_MODE)     /* ���ν���ģʽ */
    {
        __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_RECEIVER_SINGLE);
    }
    else                                                                 /* ��������ģʽ */
    {
        __sx127x_write_reg(p_this, REG_LR_FIFORXBASEADDR, 0);
        __sx127x_write_reg(p_this, REG_LR_FIFOADDRPTR, 0);   /* SPI����FIFO�ĵ�ַ */
        __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_RECEIVER);
    }

    return RADIO_RET_OK;
}



static int __sx127x_lora_set_op_mode(radio_sx127x_lora_dev_t *p_this, uint8_t mode)
{
    static uint8_t mode_prev = RFLR_OPMODE_STANDBY;
    //    static bool    ant_switch_tx_on_prev = true;
    //    bool           ant_switch_tx_on = false;
    uint8_t        mode_reg;

    mode_reg   = __sx127x_read_reg(p_this, REG_LR_OPMODE);
    mode_prev  = mode_reg & ~RFLR_OPMODE_MASK;

    if (mode != mode_prev)
    {

        mode_reg = (mode_reg & RFLR_OPMODE_MASK) | mode;

        __sx127x_write_reg(p_this, REG_LR_OPMODE, mode_reg);
    }

    return RADIO_RET_OK;
}



static void __sx127x_set_lora_on (radio_sx127x_lora_dev_t *p_this, bool enable)
{
    uint8_t mode_reg;

    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_SLEEP);

    mode_reg = __sx127x_read_reg(p_this, REG_LR_OPMODE);
    mode_reg = (mode_reg & RFLR_OPMODE_LONGRANGEMODE_MASK) | RFLR_OPMODE_LONGRANGEMODE_ON;
    __sx127x_write_reg(p_this, REG_LR_OPMODE, mode_reg);
}



//static uint8_t __sx127x_lora_version_get (radio_sx127x_lora_dev_t *p_this)
//{
//    return  __sx127x_read_reg(p_this, REG_LR_VERSION);
//}

/*
 * \brief Ƶ������
 */
static int __sx127x_lora_set_rf_frequency (radio_sx127x_lora_dev_t *p_this, uint32_t freq)
{
    uint8_t freq_reg[3];

    p_this->p_devinfo->p_lora_param->frequency = freq;

    freq = (uint32_t)((double)freq / (double)FREQ_STEP);
    freq_reg[0] = (uint8_t)((freq >> 16) & 0xFF);
    freq_reg[1] = (uint8_t)((freq >> 8) & 0xFF);
    freq_reg[2] = (uint8_t)(freq & 0xFF);
    __sx127x_write_buffer(p_this, REG_LR_FRFMSB, freq_reg, 3);

    return RADIO_RET_OK;
}

static void __sx127x_detect_optimize_set (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    uint8_t test_reserved_reg;

    test_reserved_reg = (__sx127x_read_reg(p_this, 0x31) & 0xF8) | value;
    __sx127x_write_reg(p_this, 0x31, test_reserved_reg);
}


static void __sx127x_detect_threshold_set (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    __sx127x_write_reg(p_this, 0x37, value);
}


/*
 * \brief ������Ƶ����
 */
static void __sx127x_lora_set_spreading_factor (radio_sx127x_lora_dev_t *p_this, uint8_t factor)
{
    uint8_t modem_config2_reg;

    if (factor > 12)
    {
        factor = 12;
    }
    else if (factor < 6)
    {
        factor = 6;
    }

    if (factor == 6)
    {
        __sx127x_detect_optimize_set(p_this, 5);
        __sx127x_detect_threshold_set(p_this, 0x0C);
    }
    else
    {
        __sx127x_detect_optimize_set(p_this, 3);
        __sx127x_detect_threshold_set(p_this, 0x0A);
    }

    modem_config2_reg = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG2);
    modem_config2_reg = (modem_config2_reg & RFLR_MODEMCONFIG2_SF_MASK) | (factor << 4);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG2, modem_config2_reg);
    p_this->p_devinfo->p_lora_param->sf = factor;
}


static void __sx127x_sync_word_set(radio_sx127x_lora_dev_t *p_this, uint8_t syncword)
{
    __sx127x_write_reg(p_this, REG_LR_SYNCWORD, syncword);
}


static void __sx127x_lora_set_error_coding (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    uint8_t modem_config1_reg;

    modem_config1_reg = __sx127x_read_reg (p_this, REG_LR_MODEMCONFIG1);
    modem_config1_reg = (modem_config1_reg & RFLR_MODEMCONFIG1_CODINGRATE_MASK) | (value << 1);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG1, modem_config1_reg);
    p_this->p_devinfo->p_lora_param->coderate = value;
}



static void __sx127x_lora_set_packet_crc_on (radio_sx127x_lora_dev_t *p_this, uint8_t enable)
{
    uint8_t modem_config2_reg;

    modem_config2_reg = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG2);
    modem_config2_reg = (modem_config2_reg & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK) | (enable << 2);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG2, modem_config2_reg);
    p_this->p_devinfo->p_lora_param->crc_on = enable;
}



static void __sx127x_lora_set_signal_bandwidth (radio_sx127x_lora_dev_t *p_this, uint8_t bw)
{
    uint8_t modem_config1_reg;

    modem_config1_reg = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG1);
    modem_config1_reg = (modem_config1_reg & RFLR_MODEMCONFIG1_BW_MASK) | (bw << 4);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG1, modem_config1_reg);
    p_this->p_devinfo->p_lora_param->bandwidth = bw;
}



static void __sx127x_lora_set_implicit_header_on (radio_sx127x_lora_dev_t *p_this, uint8_t enable)
{
    uint8_t modem_config1_reg;

    modem_config1_reg = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG1);
    modem_config1_reg = (modem_config1_reg & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK) | (enable);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG1, modem_config1_reg);
    p_this->p_devinfo->p_lora_param->header_mode = enable;
}



static void __sx127x_lora_set_symb_timeout (radio_sx127x_lora_dev_t *p_this, uint16_t value)
{
    uint8_t modem_config2_reg;
    uint8_t symb_timeout_lsb_reg;

    __sx127x_read_buffer(p_this, REG_LR_MODEMCONFIG2, &modem_config2_reg, 2);

    modem_config2_reg    = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG2);
    symb_timeout_lsb_reg = __sx127x_read_reg(p_this, REG_LR_SYMBTIMEOUTLSB);

    modem_config2_reg = (modem_config2_reg & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK) | ((value >> 8) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK);
    symb_timeout_lsb_reg = value & 0xFF;

    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG2, modem_config2_reg);
    __sx127x_write_reg(p_this, REG_LR_SYMBTIMEOUTLSB, symb_timeout_lsb_reg);
}



static int __sx127x_lora_set_payload_length (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    __sx127x_write_reg(p_this, REG_LR_PAYLOADLENGTH, value);
    //p_this->p_devinfo->p_lora_param->payload_len = value;

    return RADIO_RET_OK;
}



static void __sx127x_lora_set_low_datarate_optimize (radio_sx127x_lora_dev_t *p_this, uint8_t enable)
{
    uint8_t modem_config3_reg;

    modem_config3_reg = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG3);
    modem_config3_reg = (modem_config3_reg & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK) | (enable << 3);
    __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG3, modem_config3_reg);
}



static void __sx127x_lora_set_pa_output (radio_sx127x_lora_dev_t *p_this, uint8_t outputPin)
{
    uint8_t pa_config_reg;

    pa_config_reg = __sx127x_read_reg(p_this, REG_LR_PACONFIG);
    pa_config_reg = (pa_config_reg & RFLR_PACONFIG_PASELECT_MASK) | outputPin;
    __sx127x_write_reg(p_this, REG_LR_PACONFIG, pa_config_reg);
}



static void __sx127x_lora_set_pa_20dbm (radio_sx127x_lora_dev_t *p_this, bool enale )
{
    uint8_t pa_dac_reg;
    uint8_t pa_config_reg;

    pa_dac_reg    = __sx127x_read_reg(p_this, REG_LR_PADAC);
    pa_config_reg = __sx127x_read_reg(p_this, REG_LR_PACONFIG);

    if (enale == true)
    {
        pa_dac_reg = 0x87;
    }
    else
    {
        pa_dac_reg = 0x84;
    }
    if (pa_config_reg)
    {
        ;
    }

    __sx127x_write_reg(p_this, REG_LR_PADAC, pa_dac_reg);  /*����PA_BOOST����*/
}


/*
 * \brief ��������
 */
static uint8_t __sx127x_lora_set_rf_power (radio_sx127x_lora_dev_t *p_this, int8_t power)
{
    uint8_t pa_dac_reg;
    uint8_t pa_config_reg;

    if (power > 17)
    {
        __sx127x_lora_set_pa_20dbm(p_this, true);
    }
    else
    {
        __sx127x_lora_set_pa_20dbm(p_this, false);
    }

    pa_config_reg = __sx127x_read_reg(p_this, REG_LR_PACONFIG);
    pa_dac_reg    = __sx127x_read_reg(p_this, REG_LR_PADAC);

    if ((pa_config_reg & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST)
    {

        /* PA_BOOST ����  ������� */
        if ((pa_dac_reg & 0x87) == 0x87)
        {
            if (power < 5)
            {
                power = 5;
            }
            if (power > 20)
            {
                power = 20;
            }
            pa_config_reg = (pa_config_reg & RFLR_PACONFIG_MAX_POWER_MASK) | 0x70;
            pa_config_reg = (pa_config_reg & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power - 5) & 0x0F);
        }
        else
        {
            if (power < 2)
            {
                power = 2;
            }

            if (power > 17)
            {
                power = 17;
            }
            pa_config_reg = (pa_config_reg & RFLR_PACONFIG_MAX_POWER_MASK) | 0x70;
            pa_config_reg = (pa_config_reg & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power - 2) & 0x0F);
        }
    }
    else
    {
        /* RFO ����  ������� */
        if (power < -1)
        {
            power = -1;
        }

        if (power > 14)
        {
            power = 14;
        }
        pa_config_reg = (pa_config_reg & RFLR_PACONFIG_MAX_POWER_MASK) | 0x70;
        pa_config_reg = (pa_config_reg & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power + 1) & 0x0F);
    }

    __sx127x_write_reg(p_this, REG_LR_PACONFIG, pa_config_reg);
    p_this->p_devinfo->p_lora_param->power = power;

    return power;
}

/*
 * \brief ���ù���
 */
static int __sx127x_power_set_adapter (radio_sx127x_lora_dev_t *p_this, uint8_t level, float *p_power)
{
    uint8_t power;
    float   power_tab[] = {2, 4, 8, 10, 14, 16, 18, 20};

    if( level > (sizeof(power_tab) / sizeof(power_tab[0])))
    {
        return RADIO_RET_CFG_OUT_OF_RANGE;
    }

    power = __sx127x_lora_set_rf_power(p_this, power_tab[level]);
    if (p_power != NULL)
    {
        *p_power = (float)power;
    }

    return RADIO_RET_OK;
}



static int __sx127x_lora_set_preamble_length (radio_sx127x_lora_dev_t *p_this, uint16_t value)
{
    uint8_t preamble[2];

    __sx127x_read_buffer(p_this, REG_LR_PREAMBLEMSB, preamble, 2);

    preamble[0] = (value >> 8) & 0x00FF;
    preamble[1] = value & 0xFF;
    __sx127x_write_buffer(p_this, REG_LR_PREAMBLEMSB, preamble, 2);

    p_this->p_devinfo->p_lora_param->preamble_len = value;

    return RADIO_RET_OK;
}



static int __sx127x_lora_get_preamble_length (radio_sx127x_lora_dev_t *p_this, uint16_t *p_len)
{
    uint8_t preamble[2];

    __sx127x_read_buffer(p_this, REG_LR_PREAMBLEMSB, preamble, 2);
    *p_len = ((preamble[0] & 0x00FF) << 8) | preamble[1];

    return RADIO_RET_OK;
}



static void __sx127x_lora_init (radio_sx127x_lora_dev_t *p_this)
{

    SM_STATE_SET(p_this, RFLR_STATE_IDLE);

    __sx127x_write_reg(p_this, REG_LR_LNA, RFLR_LNA_GAIN_G1);

    // set the RF settings
    __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->frequency);
    __sx127x_lora_set_spreading_factor(p_this, p_this->p_devinfo->p_lora_param->sf);
    __sx127x_lora_set_error_coding(p_this, p_this->p_devinfo->p_lora_param->coderate);
    __sx127x_lora_set_packet_crc_on(p_this, p_this->p_devinfo->p_lora_param->crc_on);
    __sx127x_lora_set_signal_bandwidth(p_this, p_this->p_devinfo->p_lora_param->bandwidth);

    __sx127x_lora_set_implicit_header_on(p_this, p_this->p_devinfo->p_lora_param->header_mode);
    __sx127x_lora_set_symb_timeout(p_this, 0x3FF);
    __sx127x_lora_set_payload_length(p_this, p_this->p_devinfo->p_lora_param->payload_len);
    __sx127x_lora_set_preamble_length(p_this, p_this->p_devinfo->p_lora_param->preamble_len); /* TODO:test cad rx timout */
#if (LOW_DATARATE_FORCE == 1)
    __sx127x_lora_set_low_datarate_optimize(p_this, 1);
#endif
    __sx127x_write_reg(p_this, REG_LR_OCP, 0x3f);    /* Ĭ�ϲ���PA���ڵ���Ϊ100mA,�����20dBmʱ��Ҫ120mA,���Ա�������0x0b����*/
    __sx127x_lora_set_pa_output(p_this, RFLR_PACONFIG_PASELECT_PABOOST);
    __sx127x_lora_set_rf_power(p_this, p_this->p_devinfo->p_lora_param->power);

    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);

}



static int __sx127x_mode_set (radio_sx127x_lora_dev_t *p_this, uint8_t mode)
{
    int ret = RADIO_RET_OK;
    uint8_t temp;

    switch (mode)
    {
    case SLEEP_MODE:
        ret = __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_SLEEP);
        SM_STATE_SET(p_this, RFLR_STATE_IDLE);
        break;

    case STANDBY_MODE:
        ret = __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);
        SM_STATE_SET(p_this, RFLR_STATE_IDLE);
        break;

    case RX_MODE:
        ret = __sx127x_lora_rx_state_enter(p_this);
        SM_STATE_SET(p_this, RFLR_STATE_RX_RUNNING);
        break;

    case CW_MODE:
        temp = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG2);
        __sx127x_write_reg(p_this, REG_LR_MODEMCONFIG2, temp | (1 << 3));
        ret = __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_TRANSMITTER);
        SM_STATE_SET(p_this, RFLR_STATE_TX_RUNNING);
        break;

    default:
        break;
    }

    return ret;
}



static int __sx127x_state_get (radio_sx127x_lora_dev_t *p_this, radio_state_t *p_state)
{
    switch (SM_STATE_GET(p_this))
    {
    case RFLR_STATE_TX_RUNNING:
    case RFLR_STATE_TX_INIT:
        *p_state = TX_RUNING_ST;
        break;

    case RFLR_STATE_TX_DONE:
    case RFLR_STATE_TX_TIMEOUT:
        *p_state = IDLE_ST;
        break;


    case RFLR_STATE_RX_DONE:
    case RFLR_STATE_RX_TIMEOUT:
        if (p_this->p_devinfo->p_lora_param->rx_mode == CONTINUOUS_RECV_MODE)
        {
            *p_state = RX_RUNING_ST;
        }
        else
        {
            *p_state = IDLE_ST;
        }
        break;

    case RFLR_STATE_RX_INIT:
    case RFLR_STATE_RX_RUNNING:
        *p_state = RX_RUNING_ST;
        break;

    case RFLR_STATE_CAD_RUNNING:
    case RFLR_STATE_CAD_INIT:
        *p_state = CAD_ST;
        break;

    case RFLR_STATE_IDLE:
        *p_state = IDLE_ST;
        break;

    default:
        *p_state = IDLE_ST;
        break;
    }

    return RADIO_RET_OK;
}



static uint32_t __sx127x_time_on_air_get (radio_sx127x_lora_dev_t *p_this,
        uint8_t                  pkt_len)
{
    uint32_t len, bw, sf, cr, preamble_len;
    uint64_t ts, tpreamble, tpayload, tmp;    /* unit: 1e-6 */
    uint32_t air_time = 0;
    bool     low_rate_opt;

    len          = pkt_len;
    sf           = p_this->p_devinfo->p_lora_param->sf;
    cr           = p_this->p_devinfo->p_lora_param->coderate;
    preamble_len = p_this->p_devinfo->p_lora_param->preamble_len;
    low_rate_opt = __sx127x_read_reg(p_this, REG_LR_MODEMCONFIG3) & (1 << 3) ? true : false;

    // REMARK: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
    switch (p_this->p_devinfo->p_lora_param->bandwidth)
    {

    case 6:
        bw = 62.5 * 1000;
        break;

    case 7: // 125 kHz
        bw = 125 * 1000;
        break;

    case 8: // 250 kHz
        bw = 250 * 1000;
        break;

    case 9: // 500 kHz
        bw = 500 * 1000;
        break;

    default:
        return 0;
    }

    // Symbol rate : time for one symbol (secs)
    ts = ((uint64_t)(1 << sf)) * (uint64_t)1000000 / bw;

    // time of preamble
    tpreamble = (preamble_len * (uint64_t)1000000 + (uint64_t)(4.25 * 1000000)) * ts / (uint64_t)1000000;

    // Symbol length of payload and time
    tmp = (8 * len - 4 * sf + 28 +
           16 * (p_this->p_devinfo->p_lora_param->crc_on ? 1 : 0) -
           (p_this->p_devinfo->p_lora_param->header_mode ? 20 : 0)) * (uint64_t)(1000000)
          / (4 * (sf - ((low_rate_opt > 0) ? 2 : 0)));

    /* ceil */
    if (tmp > 0)
    {
        if (tmp % (uint64_t)1000000 < 10000)
        {
            tmp = ((tmp / (uint64_t)1000000) * (cr + 4)) * (uint64_t)1000000;
        }
        else
        {
            tmp = ((1 + (tmp / (uint64_t)1000000)) * (cr + 4)) * (uint64_t)1000000;
        }
    }
    else
    {
        tmp = 0;
    }

    tpayload = (8 * (uint64_t)1000000 + tmp) * ts / (uint64_t)1000000;

    air_time = 1 + (tpreamble + tpayload) / (uint64_t)1000;

    // Time on air, ms
    return air_time;
}


static void __sx127x_tx_invert_iq_set (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    if (value == 1)
    {
        __sx127x_write_reg(p_this,
                           REG_LR_INVERTIQ,
                           ((__sx127x_read_reg(p_this, REG_LR_INVERTIQ) &
                             RFLR_INVERTIQ_TX_MASK &
                             RFLR_INVERTIQ_RX_MASK) |
                            RFLR_INVERTIQ_RX_OFF |
                            RFLR_INVERTIQ_TX_ON));
        __sx127x_write_reg(p_this, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON);
    }
    else
    {
        __sx127x_write_reg(p_this,
                           REG_LR_INVERTIQ,
                           ((__sx127x_read_reg(p_this, REG_LR_INVERTIQ) &
                             RFLR_INVERTIQ_TX_MASK &
                             RFLR_INVERTIQ_RX_MASK) |
                            RFLR_INVERTIQ_RX_OFF |
                            RFLR_INVERTIQ_TX_OFF));
        __sx127x_write_reg(p_this, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF);
    }

    p_this->p_devinfo->p_lora_param->tx_invert_iq = value;
}



static void __sx127x_rx_invert_iq_set (radio_sx127x_lora_dev_t *p_this, uint8_t value)
{
    if (value == 1)
    {
        __sx127x_write_reg(p_this,
                           REG_LR_INVERTIQ,
                           ((__sx127x_read_reg(p_this, REG_LR_INVERTIQ) &
                             RFLR_INVERTIQ_TX_MASK &
                             RFLR_INVERTIQ_RX_MASK) |
                            RFLR_INVERTIQ_RX_ON |
                            RFLR_INVERTIQ_TX_OFF));
        __sx127x_write_reg(p_this, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON);
    }
    else
    {
        __sx127x_write_reg(p_this,
                           REG_LR_INVERTIQ,
                           ((__sx127x_read_reg(p_this, REG_LR_INVERTIQ) &
                             RFLR_INVERTIQ_TX_MASK &
                             RFLR_INVERTIQ_RX_MASK) |
                            RFLR_INVERTIQ_RX_OFF |
                            RFLR_INVERTIQ_TX_OFF));
        __sx127x_write_reg(p_this, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF);
    }

    p_this->p_devinfo->p_lora_param->rx_invert_iq = value;
}


static void __sx127x_bw_sensitivity_optimization (radio_sx127x_lora_dev_t *p_this, uint8_t bw)
{
    if ((bw == 9) &&
            (p_this->p_devinfo->p_lora_param->frequency > __LORA_RF_MID_BAND_THRESH))
    {
        // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
        __sx127x_write_reg(p_this, REG_LR_TEST36, 0x02);
        __sx127x_write_reg(p_this, REG_LR_TEST3A, 0x64);
    }
    else if (bw == 9)
    {
        // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
        __sx127x_write_reg(p_this, REG_LR_TEST36, 0x02);
        __sx127x_write_reg(p_this, REG_LR_TEST3A, 0x7F);
    }
    else
    {
        // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
        __sx127x_write_reg(p_this, REG_LR_TEST36, 0x03);
    }
}


static int __sx127x_lora_tx_data (radio_sx127x_lora_dev_t *p_this, uint8_t *pbuf, uint16_t size)
{
    uint8_t   irq_flags_mask_reg;
    uint8_t   hop_period_reg;
    uint8_t   hop_channel_reg;
    uint8_t   pkt_len;
    uint8_t   sf;
    uint8_t   bw;
    uint16_t  crc;
    uint32_t  tx_timeout;
    uint32_t  tx_time_cnt = 0;

    if ((NULL == pbuf))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (p_this->p_devinfo->p_lora_param->crc_software == 1)
    {
        if (size > 253)
        {
            return RADIO_RET_CFG_OUT_OF_RANGE;
        }
    }
    else
    {
        if (size > 255)
        {
            return RADIO_RET_CFG_OUT_OF_RANGE;
        }
    }

    if (p_this->p_devinfo->p_lora_param->crc_software == 1)
    {
        crc = crc16_ccitt(pbuf, size);
        pbuf[size++] = (crc & 0xFF00) >> 8;
        pbuf[size++] = crc & 0xFF;
    }

    pkt_len = size;
    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);
    __sx127x_tx_invert_iq_set(p_this, p_this->p_devinfo->p_lora_param->tx_invert_iq);
    if (p_this->p_devinfo->p_lora_param->freq_hop_on == 1)
    {
        irq_flags_mask_reg = RFLR_IRQFLAGS_RXTIMEOUT |
                             RFLR_IRQFLAGS_RXDONE |
                             RFLR_IRQFLAGS_PAYLOADCRCERROR |
                             RFLR_IRQFLAGS_VALIDHEADER |
                             //RFLR_IRQFLAGS_TXDONE |
                             RFLR_IRQFLAGS_CADDONE |
                             //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                             RFLR_IRQFLAGS_CADDETECTED;

        hop_period_reg = p_this->p_devinfo->p_lora_param->hop_period;

        hop_channel_reg = __sx127x_read_reg(p_this, REG_LR_HOPCHANNEL) & RFLR_HOPCHANNEL_CHANNEL_MASK;
        __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->p_hop_tab[hop_channel_reg %
                                       p_this->p_devinfo->p_lora_param->hop_tab_len]);
    }
    else
    {
        irq_flags_mask_reg = RFLR_IRQFLAGS_RXTIMEOUT |
                             RFLR_IRQFLAGS_RXDONE |
                             RFLR_IRQFLAGS_PAYLOADCRCERROR |
                             RFLR_IRQFLAGS_VALIDHEADER |
                             //RFLR_IRQFLAGS_TXDONE |                /*  ֻ�򿪷����������ж�      */
                             RFLR_IRQFLAGS_CADDONE |
                             RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                             RFLR_IRQFLAGS_CADDETECTED;
        hop_period_reg = 0;
    }
    __sx127x_write_reg(p_this, REG_LR_HOPPERIOD, hop_period_reg);
    __sx127x_write_reg(p_this, REG_LR_IRQFLAGSMASK, irq_flags_mask_reg);

#if (LOW_DATARATE_FORCE == 0)
    /* ���õ������Ż� */
    sf = p_this->p_devinfo->p_lora_param->sf;
    bw = p_this->p_devinfo->p_lora_param->bandwidth;
    if (((bw == 6) && ((sf == 10) || (sf == 11) || (sf == 12))) ||
            ((bw == 7) && ((sf == 11) || (sf == 12))) ||
            ((bw == 8) && (sf == 12)) ||
            (p_this->p_devinfo->p_lora_param->freq_hop_on))
    {
        __sx127x_lora_set_low_datarate_optimize(p_this, 1);
    }
    else
    {
        __sx127x_lora_set_low_datarate_optimize(p_this, 0);
    }
#endif

    // FIFO operations can not take place in Sleep mode
    if ((__sx127x_read_reg(p_this, REG_LR_OPMODE) &
            ~RFLR_OPMODE_MASK) == RFLR_OPMODE_SLEEP)
    {
        __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);
        __delay_us(p_this, 100);
    }

    __sx127x_write_reg(p_this,  REG_LR_PAYLOADLENGTH, pkt_len); /* ��implicitģʽ(��ʽ��ͷ),����д��FIFO����,0x80*/

    __sx127x_write_reg(p_this, REG_LR_FIFOTXBASEADDR, 0x00);    /* д�뷢�͵��׵�ַ */

    __sx127x_write_reg(p_this, REG_LR_FIFOADDRPTR, 0);          /* д��FIFO�ķ��ʵ�ַ */

    __sx127x_write_fifo(p_this, pbuf, pkt_len);                 /* д������Ҫ���͵����� */
    /* DIO0:TxDone   DIO1:Fhss */
    __sx127x_write_reg(p_this,
                       REG_LR_DIOMAPPING1,
                       RFLR_DIOMAPPING1_DIO0_01 |
                       RFLR_DIOMAPPING1_DIO1_11 |
                       RFLR_DIOMAPPING1_DIO2_01 |
                       RFLR_DIOMAPPING1_DIO3_11);

    __sx127x_write_reg(p_this,
                       REG_LR_DIOMAPPING2,
                       RFLR_DIOMAPPING2_DIO4_00 |
                       RFLR_DIOMAPPING2_DIO5_11);

    __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, 0xFF);

    /* ��ȡ����ʱ�� */
    tx_timeout = __sx127x_time_on_air_get(p_this, pkt_len);

    SM_STATE_SET(p_this, RFLR_STATE_TX_RUNNING);
    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_TRANSMITTER);                /* ���뷢��״̬ */

    while ((__sx127x_read_reg(p_this, REG_LR_IRQFLAGS) & RFLR_IRQFLAGS_TXDONE) == 0)
    {
        __delay_ms(p_this, 1); /* ��ʱ��ֹƵ������ģ�� */
        if (p_this->p_devinfo->p_lora_param->tx_timeout)
        {
            tx_time_cnt++;
            if (tx_time_cnt > (tx_timeout + 10))
            {
                if (p_this->p_devinfo->p_events->pfn_tx_timeout_cb != NULL)
                {
                    p_this->p_devinfo->p_events->pfn_tx_timeout_cb();
                }
                __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);
                return RADIO_RET_TX_TIMEOUT;
            }
        }
    }

    /* ִ�з�����ɻص� */
    if (p_this->p_devinfo->p_events->pfn_tx_done_cb != NULL)
    {
        p_this->p_devinfo->p_events->pfn_tx_done_cb();
    }

    __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);         /* �����������ж� */
    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);

    return RADIO_RET_OK;
}


/*
 * \brief ��ȡ�����
 */
static int8_t __get_lora_pkt_snr (radio_sx127x_lora_dev_t *p_this)
{
    int8_t  pkt_snr;
    uint8_t snr_reg_val;

    /* ȡ���һ�����ݵ������ */
    snr_reg_val = __sx127x_read_reg(p_this, REG_LR_PKTSNRVALUE);     /* ����� */
    if (snr_reg_val & 0x80)                                          /* �����С��0����������� */
    {
        pkt_snr = ((~snr_reg_val + 1) & 0xFF) >> 2;                  /* Invert and divide by 4 */
        pkt_snr = -pkt_snr;
    }
    else                                                             /* ����ȴ���0����������� */
    {
        pkt_snr = (snr_reg_val & 0xFF) >> 2;                         /* Divide by 4 */
    }

    return pkt_snr;
}

/*
 * \brief ��ȡ�ź�ǿ��
 */
static int16_t __get_lora_pkt_rssi (radio_sx127x_lora_dev_t *p_this)
{
    uint8_t rssi_reg;
    int16_t rssi_val;

    rssi_reg = __sx127x_read_reg(p_this, REG_LR_PKTRSSIVALUE);
    if (p_this->pkt_info.snr < 0)
    {
        if (p_this->p_devinfo->p_lora_param->frequency > __LORA_RF_MID_BAND_THRESH)
        {
            rssi_val = __LORA_RSSI_OFFSET_HF + rssi_reg + (rssi_reg >> 4) + p_this->pkt_info.snr;
        }
        else
        {
            rssi_val = __LORA_RSSI_OFFSET_LF + rssi_reg + (rssi_reg >> 4) + p_this->pkt_info.snr;
        }
    }
    else
    {
        if (p_this->p_devinfo->p_lora_param->frequency > __LORA_RF_MID_BAND_THRESH)
        {
            rssi_val = __LORA_RSSI_OFFSET_HF + rssi_reg + (rssi_reg >> 4);
        }
        else
        {
            rssi_val = __LORA_RSSI_OFFSET_LF + rssi_reg + (rssi_reg >> 4);
        }
    }

    return rssi_val;
}



static int __cad_detect_state_get (radio_sx127x_lora_dev_t *p_this)
{
    uint8_t reg_val;


    /* ����Ƿ���cad_detect�ж� */
    reg_val = __sx127x_read_reg(p_this, REG_LR_IRQFLAGS);
    if (reg_val & RFLR_IRQFLAGS_CADDETECTED)
    {
        /* ���detect�ж� */
        __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDETECTED);

        return LORA_RET_CODE_OK;
    }
    else
    {
        return LORA_RET_CODE_NO_CAD_DETECT_IRQ;
    }
}



static int __sx127x_lora_read_cur_rssi (radio_sx127x_lora_dev_t *p_this, int16_t *p_rssi)
{
    uint8_t rssi_reg;

    /* �˴�ֻ�ܲ������뵽���߽ӿڴ��Ĺ��ʣ������źź�����������ź�ǿ�Ȳ�һ����
     * ��Ϊ���ź�ǿ�ȿ�ͨ��SNR����ԭ����֮�µ��ź�ǿ��
     */
    rssi_reg = __sx127x_read_reg(p_this, REG_LR_RSSIVALUE);
    if (p_this->p_devinfo->p_lora_param->frequency > __LORA_RF_MID_BAND_THRESH)
    {
        *p_rssi = __LORA_RSSI_OFFSET_HF + rssi_reg;
    }
    else
    {
        *p_rssi = __LORA_RSSI_OFFSET_LF + rssi_reg;
    }

    return RADIO_RET_OK;
}



static int __sx127x_lora_rx_data_read (radio_sx127x_lora_dev_t *p_this, uint8_t *p_buf, uint8_t *p_size)
{
    uint8_t irq_flags_reg    = 0;
    uint8_t fifo_rx_cur_addr = 0;
    //    uint8_t fifo_addr_reg    = 0;
    uint16_t crc;

    if (p_this->p_devinfo->p_lora_param->crc_on)
    {
        if ((__sx127x_read_reg(p_this, REG_LR_HOPCHANNEL) & RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON) != RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON)
        {
            *p_size = 0;
            return RADIO_RET_RX_CRC_ERR;
        }
    }

    irq_flags_reg = __sx127x_read_reg(p_this, REG_LR_IRQFLAGS);
    if ((irq_flags_reg & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK) == RFLR_IRQFLAGS_PAYLOADCRCERROR)    /* CRC���� */
    {
        __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR);

        if (p_this->p_devinfo->p_events != NULL)
        {
            if (p_this->p_devinfo->p_events->pfn_rx_error_cb != NULL)
            {
                p_this->p_devinfo->p_events->pfn_rx_error_cb();
            }
        }

        /* �������CRCУ�鲻�� */
        if (p_this->p_devinfo->p_lora_param->rx_mode == CONTINUOUS_RECV_MODE)
        {
            SM_STATE_SET(p_this, RFLR_STATE_RX_RUNNING);
        }
        else
        {
            SM_STATE_SET(p_this, RFLR_STATE_IDLE);
        }

        *p_size = 0;
        return RADIO_RET_RX_CRC_ERR;
    }

    p_this->pkt_info.snr  = __get_lora_pkt_snr(p_this);             /* ��ȡ���ݰ������   */
    p_this->pkt_info.rssi = __get_lora_pkt_rssi(p_this);            /* ��ȡ���ݰ��ź�ǿ�� */

    /* ȡ���� */
    fifo_rx_cur_addr = __sx127x_read_reg(p_this, REG_LR_FIFORXCURRENTADDR);  /* ��ȡ������һ��������ָ�� */
    if (p_this->p_devinfo->p_lora_param->header_mode == IMPLICIT_HEADER)
    {
        __sx127x_write_reg(p_this, REG_LR_FIFOADDRPTR, fifo_rx_cur_addr);    /* д��FIFO�ķ��ʵ�ַ */
        /* ��ȡ���� */
        if (p_this->p_devinfo->p_lora_param->crc_software == 1)
        {
            __sx127x_read_fifo(p_this, p_buf, p_this->p_devinfo->p_lora_param->payload_len + 2);
            *p_size = p_this->p_devinfo->p_lora_param->payload_len + 2;
        }
        else
        {
            __sx127x_read_fifo(p_this, p_buf, p_this->p_devinfo->p_lora_param->payload_len);
            *p_size = p_this->p_devinfo->p_lora_param->payload_len;
        }
    }
    else
    {
        *p_size = __sx127x_read_reg(p_this, REG_LR_NBRXBYTES);  /* ��ȡ������ */
        __sx127x_write_reg(p_this,  REG_LR_FIFOADDRPTR, fifo_rx_cur_addr);     /* д��FIFO�ķ��ʵ�ַ */
        __sx127x_read_fifo(p_this, p_buf, *p_size);
    }

    if (p_this->p_devinfo->p_lora_param->crc_software == 1)
    {
        crc = crc16_ccitt(p_buf, *p_size - 2);
        if ((((crc & 0xFF00) >> 8) != p_buf[*p_size - 2]) || ((crc & 0xFF) != p_buf[*p_size - 1]))
        {
            *p_size = 0;
            return RADIO_RET_RX_CRC_ERR;
        }
        *p_size -= 2;
    }

    if (p_this->p_devinfo->p_events != NULL)
    {
        if (p_this->p_devinfo->p_events->pfn_rx_done_cb != NULL)
        {
            p_this->p_devinfo->p_events->pfn_rx_done_cb(p_buf,
                    *p_size,
                    p_this->pkt_info.rssi,
                    p_this->pkt_info.snr);
        }
    }

    if (p_this->p_devinfo->p_lora_param->rx_mode == CONTINUOUS_RECV_MODE)
    {
        SM_STATE_SET(p_this, RFLR_STATE_RX_RUNNING);
    }
    else
    {
        SM_STATE_SET(p_this, RFLR_STATE_IDLE);
    }

    return RADIO_RET_OK;
}



static int __sx127x_cad_start (radio_sx127x_lora_dev_t *p_this, uint8_t *p_state)
{
    uint8_t reg_val;
    uint8_t hop_channel_reg = 0;

    if (p_this->p_devinfo->p_lora_param->freq_hop_on == 1)
    {
        hop_channel_reg = __sx127x_read_reg(p_this, REG_LR_HOPCHANNEL) & RFLR_HOPCHANNEL_CHANNEL_MASK;
        __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->p_hop_tab[hop_channel_reg %
                                       p_this->p_devinfo->p_lora_param->hop_tab_len]);
        //__sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->p_hop_tab[0]);
    }

    /* ��ʼ��CAD״̬ */
    CAD_STATE_CLR(p_this);

    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);

    __sx127x_write_reg(p_this,
                       REG_LR_IRQFLAGSMASK,
                       RFLR_IRQFLAGS_RXTIMEOUT |
                       RFLR_IRQFLAGS_RXDONE |
                       RFLR_IRQFLAGS_PAYLOADCRCERROR |
                       RFLR_IRQFLAGS_VALIDHEADER |
                       RFLR_IRQFLAGS_TXDONE |
                       //RFLR_IRQFLAGS_CADDONE |
                       RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL //|
                       /* RFLR_IRQFLAGS_CADDETECTED*/ );

    __sx127x_write_reg(p_this,
                       REG_LR_DIOMAPPING1,
                       RFLR_DIOMAPPING1_DIO0_01 |  /* TX Done */
                       RFLR_DIOMAPPING1_DIO1_00 |  /* RxTimeout */
                       RFLR_DIOMAPPING1_DIO2_00 |  /* FhssChangeChannel */
                       RFLR_DIOMAPPING1_DIO3_00);  /* CadDone */

    __sx127x_write_reg(p_this,
                       REG_LR_DIOMAPPING2,
                       RFLR_DIOMAPPING2_DIO4_01 |  /* Pll Lock */
                       RFLR_DIOMAPPING2_DIO5_00);  /* ModeReady */

    SM_STATE_SET(p_this, RFLR_STATE_CAD_RUNNING);
    CAD_STATE_SET(p_this, CAD_STATE_START);

    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_CAD);

    while(1)
    {
        /* �ж��Ƿ���CAD�ж� */
        reg_val = __sx127x_read_reg(p_this, REG_LR_IRQFLAGS);
        if (reg_val & RFLR_IRQFLAGS_CADDONE)
        {

            /* ���done�ж� */
            __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDONE);

            /* ����CAD DONE ��־ */
            CAD_STATE_SET(p_this, CAD_STATE_DONE);

            /* cad done �ص� */
            if (p_this->p_devinfo->p_events->pfn_cad_done_cb != NULL)
            {
                p_this->p_devinfo->p_events->pfn_cad_done_cb(true);
            }

            /* �ж��Ƿ��⵽ǰ���� */
            if (__cad_detect_state_get(p_this) == LORA_RET_CODE_OK)
            {
                *p_state = SX127X_CAD_DETECT;  /* ��⵽ǰ���� */
                CAD_STATE_SET(p_this, CAD_STATE_DETECT);
            }
            else
            {
                *p_state = SX127X_CAD_NONE;
                CAD_STATE_SET(p_this, CAD_STATE_FINISH);
            }
            break;
        }
    }

    /* CAD ������ */
    __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_STANDBY);
    SM_STATE_SET(p_this, RFLR_STATE_IDLE);

    return RADIO_RET_OK;
}




static int __sx127x_ioctl (radio_sx127x_lora_dev_t *p_this, int cmd, uint8_t *p_arg)
{
    int     ret = RADIO_RET_OK;

    switch (cmd)
    {
    case SX127X_CAD_START:
        ret = __sx127x_cad_start(p_this, p_arg);
        break;

    case SX127X_HOP_SET:
        p_this->p_devinfo->p_lora_param->freq_hop_on = *p_arg ? 1 : 0;
        break;

    case SX127X_BW_SET:
        __sx127x_lora_set_signal_bandwidth(p_this, *p_arg);
        break;

    case SX127X_SF_SET:
        __sx127x_lora_set_spreading_factor(p_this, *p_arg);
        break;

    case SX127X_CR_SET:
        __sx127x_lora_set_error_coding(p_this, *p_arg);
        break;

    case SX127X_CRC_SET:
        __sx127x_lora_set_packet_crc_on(p_this, *p_arg);
        break;

    case SX127X_RX_MODE_SET:
        p_this->p_devinfo->p_lora_param->rx_mode = *p_arg;
        break;

    case SX127X_SYNC_WORD_SET:
        __sx127x_sync_word_set(p_this, *p_arg);
        break;

    default:
        break;
    }

    return ret;
}

void __sx127x_lora_dio0_irq_func (radio_sx127x_lora_dev_t *p_this)
{
    //    uint8_t  reg_val;

    switch (SM_STATE_GET(p_this))
    {
    /* CAD */
    case RFLR_STATE_CAD_RUNNING:
    {


        break;
    }
    case RFLR_STATE_RX_RUNNING:
    {
        /*
         * ��������
         * ���յ�����֮�󽫴���dio0�жϣ����ڲ�֪���û��ǲ�����ѯ���ջ����жϽ��ܣ���������ﲻ�ؽ������ݡ�
         * 1.���û������жϽ���, �����ж��ٵ��� __sx127x_pkt_buf_read ������
         * 2.���û�������ѯ���գ�������ѯ���� __sx127x_pkt_buf_read������
         */
    }


    default:
        break;
    }
}



void __sx127x_lora_dio1_irq_func (radio_sx127x_lora_dev_t *p_this)
{
    switch (SM_STATE_GET(p_this))
    {
    /* ���ճ�ʱ */
    case RFLR_STATE_RX_RUNNING:
        // Clear Irq
        __sx127x_write_reg(p_this,
                           REG_LR_IRQFLAGS,
                           RFLR_IRQFLAGS_RXTIMEOUT);

        if (p_this->p_devinfo->p_lora_param->rx_mode == CONTINUOUS_RECV_MODE)
        {
            SM_STATE_SET(p_this, RFLR_STATE_RX_RUNNING);
        }
        else
        {
            __sx127x_lora_set_op_mode(p_this, RFLR_OPMODE_SLEEP);
            SM_STATE_SET(p_this, RFLR_STATE_IDLE);
        }

        if ((p_this->p_devinfo->p_events != NULL) &&
                (p_this->p_devinfo->p_events->pfn_rx_timeout_cb != NULL))
        {
            p_this->p_devinfo->p_events->pfn_rx_timeout_cb();
        }
        break;

    default:
        break;
    }
}



void __sx127x_lora_dio2_irq_func (radio_sx127x_lora_dev_t *p_this)
{
    uint8_t hop_channel_reg;

    switch (SM_STATE_GET(p_this))
    {
    /* ��Ƶ */
    case RFLR_STATE_RX_RUNNING:
    case RFLR_STATE_TX_RUNNING:
        if (p_this->p_devinfo->p_lora_param->freq_hop_on == 1)
        {
            hop_channel_reg = __sx127x_read_reg(p_this, REG_LR_HOPCHANNEL) & RFLR_HOPCHANNEL_CHANNEL_MASK;
            __sx127x_lora_set_rf_frequency(p_this, p_this->p_devinfo->p_lora_param->p_hop_tab[hop_channel_reg %
                                           p_this->p_devinfo->p_lora_param->hop_tab_len]);

            if (p_this->p_devinfo->p_events != NULL)
            {
                if (p_this->p_devinfo->p_events->pfn_fhss_change_channel_cb != NULL)
                {
                    p_this->p_devinfo->p_events->pfn_fhss_change_channel_cb(hop_channel_reg & RFLR_HOPCHANNEL_CHANNEL_MASK);
                }
            }
        }

        // Clear Irq
        __sx127x_write_reg(p_this, REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL);
        break;

    default:
        break;
    }
}



static int __sx127x_pkt_buf_read (radio_sx127x_lora_dev_t *p_this, uint8_t *p_buf, uint16_t *p_size)
{
    int     ret;
    uint8_t size = 0;
    uint8_t flag;

    if ((p_buf == NULL) || (p_size == NULL))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    *p_size = 0;
    flag = __sx127x_read_reg(p_this, REG_LR_IRQFLAGS);
    if( flag & RFLR_IRQFLAGS_RXDONE)   /* ���ܵ����� */
    {
        __sx127x_write_reg(p_this,  REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE);          /* ִ��д��������������жϱ�־ */
        ret = __sx127x_lora_rx_data_read(p_this, p_buf, &size); /* ��ȡ���� */
        *p_size = size;
        return ret;
    }
    else if (flag & RFLR_IRQFLAGS_RXTIMEOUT)
    {
        SM_STATE_SET(p_this, RFLR_STATE_IDLE);
        return RADIO_RET_RX_TIMEOUT;
    }
    else
    {
        return RADIO_RET_NOT_RECV_PKT;
    }
}



static int spi_check (radio_sx127x_lora_dev_t *p_this)
{
    uint8_t reg = 0x35;

    __sx127x_write_reg(p_this, REG_LR_FRFMID, reg);
    if (reg != __sx127x_read_reg(p_this, REG_LR_FRFMID))
    {
        return -1;
    }

    return 0;
}



static int __sx127x_sync_id_set (radio_sx127x_lora_dev_t *p_this, uint8_t *p_id, uint8_t len)
{
    return RADIO_RET_MODULE_NOT_SUPPORT;
}



static int __sx127x_sync_id_get (radio_sx127x_lora_dev_t *p_this, uint8_t *p_id, uint8_t *p_len)
{
    return RADIO_RET_MODULE_NOT_SUPPORT;
}





static const struct radio_drv_funcs __g_sx127x_drv_funcs[] =
{
    __sx127x_reset,
    __sx127x_sync_id_set,
    __sx127x_sync_id_get,
    __sx127x_lora_set_preamble_length,
    __sx127x_lora_get_preamble_length,
    __sx127x_power_set_adapter,
    __sx127x_lora_set_rf_frequency,
    __sx127x_mode_set,
    __sx127x_state_get,                    /* TODO:need test */
    __sx127x_lora_read_cur_rssi,
    __sx127x_lora_tx_data,
    __sx127x_pkt_buf_read,
    __sx127x_ioctl,
    __sx127x_lora_dio0_irq_func,
    __sx127x_lora_dio1_irq_func,
    __sx127x_lora_dio2_irq_func,
    NULL,
    NULL,
};



static void __rx_chain_calibration (radio_sx127x_lora_dev_t *p_this)
{
#define AM_LORA_FREQ_STEP           ((int64_t)61035156250)
#define AM_SX127X_FSK_REG_IMAGECAL                          0x3B
#define AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_MASK             0xBF
#define AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_START            0x40
#define AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_RUNNING          0x20

    uint8_t  reg_pa_config_init_val;
    uint32_t initial_freq;
    uint8_t  freq_reg[3];
    uint32_t freq;

    // Save context
    reg_pa_config_init_val = __sx127x_read_reg(p_this, REG_LR_PACONFIG);

    initial_freq = (((uint32_t)__sx127x_read_reg(p_this, REG_LR_FRFMSB) << 16) |
                    ((uint32_t)__sx127x_read_reg(p_this, REG_LR_FRFMID) << 8)  |
                    ((uint32_t)__sx127x_read_reg(p_this, REG_LR_FRFLSB))) * (double)FREQ_STEP;

    // Cut the PA just in case, RFO output, power = -1 dBm
    __sx127x_write_reg(p_this, REG_LR_PACONFIG, 0x00);

    // Launch Rx chain calibration for LF band
    __sx127x_write_reg(p_this,
                       AM_SX127X_FSK_REG_IMAGECAL,
                       (__sx127x_read_reg(p_this, AM_SX127X_FSK_REG_IMAGECAL) &
                        AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_MASK) |
                       AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_START);

    while((__sx127x_read_reg(p_this, AM_SX127X_FSK_REG_IMAGECAL) &
            AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_RUNNING) ==
            AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_RUNNING)
    {
    }

    // Sets a Frequency in HF band
    freq = (uint32_t)((double)868000000 / (double)FREQ_STEP);
    freq_reg[0] = (uint8_t)((freq >> 16) & 0xFF);
    freq_reg[1] = (uint8_t)((freq >> 8) & 0xFF);
    freq_reg[2] = (uint8_t)(freq & 0xFF);
    __sx127x_write_buffer(p_this, REG_LR_FRFMSB, freq_reg, 3);

    // Launch Rx chain calibration for HF band
    __sx127x_write_reg(p_this,
                       AM_SX127X_FSK_REG_IMAGECAL,
                       (__sx127x_read_reg(p_this, AM_SX127X_FSK_REG_IMAGECAL) &
                        AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_MASK) |
                       AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_START);

    while((__sx127x_read_reg(p_this, AM_SX127X_FSK_REG_IMAGECAL) &
            AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_RUNNING) ==
            AM_SX127X_FSK_RF_IMAGECAL_IMAGECAL_RUNNING)
    {
    }

    // Restore context
    __sx127x_write_reg(p_this, REG_LR_PACONFIG, reg_pa_config_init_val);
    freq = (uint32_t)((double)initial_freq / (double)FREQ_STEP);
    freq_reg[0] = (uint8_t)((freq >> 16) & 0xFF);
    freq_reg[1] = (uint8_t)((freq >> 8) & 0xFF);
    freq_reg[2] = (uint8_t)(freq & 0xFF);
    __sx127x_write_buffer(p_this, REG_LR_FRFMSB, freq_reg, 3);
}



radio_handle_t radio_sx127x_lora_init (radio_sx127x_lora_dev_t      *p_sx127x,
                                       sx127x_lora_spi_funcs_t      *p_spi,
                                       sx127x_lora_gpio_funcs_t     *p_gpio,
                                       sx127x_lora_delay_funcs_t    *p_delay,
                                       radio_sx127x_lora_devinfo_t  *p_devinfo)
{
    if ((p_sx127x == NULL) ||
            (p_spi == NULL)    ||
            (p_devinfo == NULL))
    {
        return NULL;
    }

    p_sx127x->p_spi        = p_spi;
    p_sx127x->p_gpio       = p_gpio;
    p_sx127x->p_delay      = p_delay;
    p_sx127x->p_devinfo    = p_devinfo;
    p_sx127x->isa.p_funcs  = __g_sx127x_drv_funcs;
    p_sx127x->isa.p_drv    = p_sx127x;

    CAD_STATE_CLR(p_sx127x);
    SM_STATE_SET(p_sx127x, RFLR_STATE_IDLE);

    /* ���SPI�Ƿ����� */
    if (spi_check(p_sx127x) != 0)
    {
        return NULL;
    }

    __sx127x_reset(p_sx127x);

    __rx_chain_calibration(p_sx127x);

    __sx127x_lora_set_op_mode(p_sx127x, RFLR_OPMODE_SLEEP);

    __sx127x_set_lora_on(p_sx127x, true);

    __sx127x_lora_init(p_sx127x);

    return &p_sx127x->isa;
}

/*
 * \brief ʵ��һЩ����
 */
static sx127x_lora_spi_funcs_t      __g_spi_dev;
static sx127x_lora_gpio_funcs_t     __g_gpio_dev;
static sx127x_lora_delay_funcs_t    __g_delay;
static radio_sx127x_lora_dev_t      __g_sx127x;
static radio_sx127x_lora_devinfo_t  __g_sx127x_devinfo;


/*
 * ��Ƶ��
 */
static const uint32_t __g_hop_freq_tab[64] =
{
    420000000,
    421000000,
    422000000,
    423000000,
    424000000,
    425000000,
    426000000,
    427000000,
    428000000,
    429000000,
    430000000,
    431000000,
    432000000,
    433000000,
    434000000,
    435000000,
    436000000,
    437000000,
    438000000,
    439000000,
    440000000,
    441000000,
    442000000,
    443000000,
    444000000,
    445000000,
    446000000,
    447000000,
    448000000,
    449000000,
    450000000,
    451000000,
    452000000,
    453000000,
    454000000,
    455000000,
    456000000,
    457000000,
    458000000,
    459000000,
    460000000,
    461000000,
    462000000,
    463000000,
    464000000,
    465000000,
    466000000,
    467000000,
    468000000,
    469000000,
    470000000,
    471000000,
    472000000,
    473000000,
    474000000,
    475000000,
    476000000,
    477000000,
    478000000,
    479000000,
    480000000,
    481000000,
    482000000,
    483000000
};




/*
 * �¼��ص�����
 */
static void __cad_done_call_func (uint8_t ch_active)
{
    //    printf("cad done!\r\n");
}



static void __rx_timeout_call_func (void)
{
    //    printf("rx timeout!\r\n");
}



static void __rx_crc_err_call_func (void)
{
    //    printf("rx crc err!\r\n");
}



static void __fhss_change_channel_call_func (uint8_t current_channel)
{
    //printf("fssh cur channel:%d\r\n", current_channel);
}

static void __rx_done_call_func (uint8_t *p_payload, uint16_t size, int16_t rssi, int8_t snr)
{
    //printf("pkt rssi:%d   snr:%d \r\n", rssi, snr);
}

/*
 * \brief �¼��ص�
 */
static sx127x_lora_events_t  __g_sx127x_events =
{
    NULL,
    NULL,
    __rx_done_call_func,
    __rx_timeout_call_func,
    __rx_crc_err_call_func,
    __fhss_change_channel_call_func,
    __cad_done_call_func,
};



/**
  * @brief Lora����
  * @param None
  * @retval None
  */
static sx127x_lora_param_set_t __g_lora_param =
{
    433000000,       /* Ƶ�� ע�⣺demo����������radio_freq_set�ӿ��޸�Ϊ����Ƶ�� */
    20,              /* ���书�� */
    8,               /* ����     [6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz,  9: 500 kHz] */
    7,               /* ��Ƶ���� [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips] */
    1,               /* ������   [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
    1,               /* Ӳ��CRC����  [0: �ر�, 1: ����] */
    0,               /* ��ͷģʽ [0: ��ʽ��ͷģʽ, 1: ��ʽ��ͷģʽ] ע��SFΪ6ʱֻ��ʹ����ʽ��ͷ */
    0,               /* ����ģʽ [0: ��������, 1�����ν���] */
    0,               /* ��Ƶ     [0: �ر�, 1: ����] */
    50,              /* ��Ƶ���ڣ�symbols�� */
    100,             /* ���ճ�ʱ(symbols)[���ڵ��ν���ģʽ] */
    0,               /* ���ͳ�ʱʹ��[ע�������ڲ��Զ����㳬ʱʱ�䣬Ҫ����ʱ������ʱ��ȷ] */
    10,              /* ���س��� */
    16,              /* ǰ���볤�� */
    0,               /* ����IQ��ת [0: ����, 1: ��ת] */
    0,               /* ����IQ��ת [0: ����, 1: ��ת] */
    1,               /* ���CRC����[0: �ر�, 1: ����] */
    __g_hop_freq_tab,/* ��Ƶ�� */
    64               /* ��Ƶ���� */
};

/**
  * @brief Lora��ʼ������
  * @param uint8_t (*spi_read_byte_fun) 		(void)		  SPI�����ֽں���
					 void  	(*spi_write_byte_fun)  		(uint8_t)   SPIд�ֽں���
					 void 		(*reset_pin_set_fun) 	 	(uint8_t )	��λ�������ú���
					 void 		(*sel_pin_set_fun) 		 	(uint8_t)		ѡ���������ú���
					 void 		(*delay_ms_fun) 				(uint32_t)	��ʱ���뺯��
					 void 		(*delay_us_fun) 				(uint32_t)  ��ʱ΢�뺯��
  * @retval ��λ�Ƿ�ɹ�
						@arg NULL    ��ʼ��ʧ��
						@arg ����ֵ  ��ʼ���ɹ�
  */
radio_handle_t radio_zm4xx_inst_init (uint8_t (*spi_read_byte_fun) 		(void)		,
                                      void  	(*spi_write_byte_fun)  	(uint8_t)	,
                                      void 		(*reset_pin_set_fun) 	 	(uint8_t ),
                                      void 		(*sel_pin_set_fun) 		 	(uint8_t)	,
                                      void 		(*delay_ms_fun) 				(uint32_t),
                                      void 		(*delay_us_fun) 				(uint32_t))
{
    /* SPI ��д�������� */
    __g_spi_dev.pfn_spi_read_byte  = spi_read_byte_fun;
    __g_spi_dev.pfn_spi_write_byte = spi_write_byte_fun;

    /* GPIO ������������ */
    __g_gpio_dev.pfn_reset_pin_set = reset_pin_set_fun;
    __g_gpio_dev.pfn_sel_pin_set   = sel_pin_set_fun;

    /* ��ʱ�������� */
    __g_delay.pfn_delay_ms = delay_ms_fun;
    __g_delay.pfn_delay_us = delay_us_fun;

    /* �����豸��Ϣ */
    __g_sx127x_devinfo.p_events     = &__g_sx127x_events;
    __g_sx127x_devinfo.p_lora_param = &__g_lora_param;

    return radio_sx127x_lora_init(&__g_sx127x,
                                  &__g_spi_dev,
                                  &__g_gpio_dev,
                                  &__g_delay,
                                  &__g_sx127x_devinfo);
}

/**
  * @brief Lora��λ����
  * @param handle radio���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_reset (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_reset(handle->p_drv);
}


/**
  * @brief Loraͬ��������
  * @param handle radio���
					 p_id   ID���ݵ�ָ��
					 len    ID�ĳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_CFG_OUT_OF_RANGE :ID���ȳ�����Χ
		@retval RADIO_RET_MODULE_NOT_SUPPORT:ģ�鲻֧�ָù���
  */
int radio_sync_id_set (radio_handle_t handle, uint8_t *p_id, uint8_t len)
{
    if (handle == NULL || p_id == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_id_set(handle->p_drv, p_id, len);
}


/**
  * @brief Loraͬ���ֻ�ȡ
  * @param handle radio���
					 p_id   ID���ݵ�ָ��
					 len    ID�ĳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_MODULE_NOT_SUPPORT:ģ�鲻֧�ָù���
  */
int radio_sync_id_get (radio_handle_t handle, uint8_t *p_id, uint8_t *p_len)
{
    if (handle == NULL || p_id == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_id_get(handle->p_drv, p_id, p_len);
}


/**
  * @brief ��������ģ��ǰ���볤��
  * @param handle radio���
					 size    ID�ĳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_CFG_OUT_OF_RANGE :ID���ȳ�����Χ
  */
int radio_preamble_length_set (radio_handle_t handle, uint16_t size)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_preamble_length_set(handle->p_drv, size);
}


/**
  * @brief ��������ģ��ǰ���볤��
  * @param handle radio���
					 p_len  ǰ���볤��
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_preamble_length_get (radio_handle_t handle, uint16_t *p_len)
{
    return handle->p_funcs->pfn_radio_preamble_length_get(handle->p_drv, p_len);
}


/**
  * @brief ��������ģ��ǰ���볤��
  * @param handle radio���
					 p_len  ǰ���볤��
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_pa_set (radio_handle_t handle, uint8_t level, float *p_power)
{
    if ( handle == NULL )
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_pa_set(handle->p_drv, level, p_power);
}


/**
  * @brief ��������ģ��Ƶ��
  * @param handle radio���
					 freq   Ƶ��
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_CFG_OUT_OF_RANGE :Ƶ�ʳ�����Χ
		@retval RADIO_RET_MODULE_NOT_SUPPORT:ģ�鲻֧�ָ�Ƶ��
  */
int radio_freq_set (radio_handle_t handle, uint32_t freq)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_freq_set(handle->p_drv, freq);
}


/**
  * @brief ��ȡ����ģ�鵱ǰRSSIֵ
  * @param handle radio���
					 p_rssi RSSI
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_rssi_read (radio_handle_t handle, int16_t *p_rssi)
{
    if ((handle == NULL) || (p_rssi == NULL))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_rssi_read(handle->p_drv, p_rssi);
}


/**
  * @brief ��������ģ��ģʽ
  * @param handle radio���
					 mode   ����ģʽ
					@arg SLEEP_MODE 	����ģʽ
					@arg STANDBY_MODE ����ģʽ
					@arg RX_MODE 			����ģʽ
					@arg CW_MODE 			�ز�ģʽ
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_mode_set (radio_handle_t handle, uint8_t mode)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_mode_set(handle->p_drv, mode);
}


/**
  * @brief ��������ģ��ģʽ
  * @param handle radio���
					 p_state   ����ģʽ
					@arg IDLE_ST 	����ģʽ
					@arg SLEEP_ST ����ģʽ
					@arg RX_RUNING_ST 			����ģʽ
					@arg TX_RUNING_ST 			����ģʽ
					@arg CAD_ST 			�ز�ģʽ
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
  */
int radio_state_get (radio_handle_t handle, radio_state_t *p_state)
{
    if ((handle == NULL) || (p_state == NULL))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_state_get(handle->p_drv, p_state);
}


/**
  * @brief ����ģ�鷢�ͺ���
  * @param handle ���
					 buf    �������ݻ�����
					 size   ���ݳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_CFG_OUT_OF_RANGE :�����ȳ�����Χ
  */
int radio_buf_send (radio_handle_t handle, uint8_t *buf, uint16_t size)
{
    if ((handle == NULL) || (buf == NULL))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_buf_send(handle->p_drv, buf, size);
}

/**
  * @brief ����ģ����պ���
  * @param handle ���
					 buf    �������ݻ�����
					 size   ���ݳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_CFG_OUT_OF_RANGE :�����ȳ�����Χ
  */
int radio_buf_recv (radio_handle_t handle, uint8_t *buf, uint16_t *p_size)
{
    if ((handle == NULL) || (buf == NULL) || (p_size == NULL))
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_buf_recv(handle->p_drv, buf, p_size);
}

/**
  * @brief ����ģ����ƺ���
  * @param handle ���
					 cmd    ��������
					 p_arg  �������Ӧ�Ĳ���
		@retval RADIO_RET_OK    :��λ�ɹ�
		@retval RADIO_RET_PARA_INVALID:������Ч
		@retval RADIO_RET_ERR		������ִ�д���
		@retval RADIO_RET_MODULE_NOT_SUPPORT:ģ�鲻֧�ָ�����
  */
int radio_ioctl (radio_handle_t handle, int cmd, uint8_t *p_arg)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    return handle->p_funcs->pfn_radio_ioctl(handle->p_drv, cmd, p_arg);
}



/**
 * \brief ����ģ��DIO0�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio0_irq_func (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (handle->p_funcs->pfn_radio_dio0_irq_func != NULL)
    {
        handle->p_funcs->pfn_radio_dio0_irq_func(handle->p_drv);
        return RADIO_RET_OK;
    }
    else
    {
        return RADIO_RET_MODULE_NOT_SUPPORT;
    }
}



/**
 * \brief ����ģ��DIO1�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio1_irq_func (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (handle->p_funcs->pfn_radio_dio1_irq_func != NULL)
    {
        handle->p_funcs->pfn_radio_dio1_irq_func(handle->p_drv);
        return RADIO_RET_OK;
    }
    else
    {
        return RADIO_RET_MODULE_NOT_SUPPORT;
    }
}



/**
 * \brief ����ģ��DIO2�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio2_irq_func (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (handle->p_funcs->pfn_radio_dio2_irq_func != NULL)
    {
        handle->p_funcs->pfn_radio_dio2_irq_func(handle->p_drv);
        return RADIO_RET_OK;
    }
    else
    {
        return RADIO_RET_MODULE_NOT_SUPPORT;
    }
}



/**
 * \brief ����ģ��DIO3�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio3_irq_func (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (handle->p_funcs->pfn_radio_dio3_irq_func != NULL)
    {
        handle->p_funcs->pfn_radio_dio3_irq_func(handle->p_drv);
        return RADIO_RET_OK;
    }
    else
    {
        return RADIO_RET_MODULE_NOT_SUPPORT;
    }
}


/**
 * \brief ����ģ��DIO5�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio5_irq_func (radio_handle_t handle)
{
    if (handle == NULL)
    {
        return RADIO_RET_PARAM_INVALID;
    }

    if (handle->p_funcs->pfn_radio_dio5_irq_func != NULL)
    {
        handle->p_funcs->pfn_radio_dio5_irq_func(handle->p_drv);
        return RADIO_RET_OK;
    }
    else
    {
        return RADIO_RET_MODULE_NOT_SUPPORT;
    }
}
