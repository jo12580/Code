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
 * \note: ������ʹ�÷�����
 *        1������radio_sx127x_lora_init��ʼ���豸����
 *        2������radio_buf_recv�����������ݣ� ����radio_buf_send������������
 *        3������Ҫʹ��CAD����Ƶ���ܣ�����Ҫ����һ�²��裺
 *           ��1����DIO0��DIO1��DIO2 ���ó��������жϴ���
 *           ��2����DIO0�ж��е��� radio_dio0_irq_func()����
 *           ��3����DIO1�ж��е��� radio_dio1_irq_func()����
 *           ��4����DIO2�ж��е��� radio_dio2_irq_func()����
 *          ˵���� ��������ֻ������radio_dio0_irq_func()��radio_dio1_irq_func()��radio_dio2_irq_func()������׼�жϺ���
 *                 ������׼�жϺ�������ԡ�
 *                 ���û���Ҫ�жϽ������ݣ� ����DIO0�������ж��е��� radio_dio0_irq_func()������
 * \internal
 * \par Modification history
 * - 1.00 17-11-22  sup, first implementation.
 * \endinternal
 */

#ifndef __sx127x_H__
#define __sx127x_H__


#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "sx127x_regs_lora.h"


/*
 * \name grp_lora_cad_state  CAD ״̬
 */
#define CAD_STATE_START         0x01
#define CAD_STATE_FINISH        0x02
#define CAD_STATE_DONE          0x04
#define CAD_STATE_DETECT        0x08

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


/**
 * \brief ������Ƶ�豸
 */
typedef struct radio_dev
{
    const struct radio_drv_funcs *p_funcs;   /* �豸����������  */
    void                         *p_drv;
} radio_dev_t;


typedef radio_dev_t *radio_handle_t;
/**
 * \name grp_lora_state
 */
typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
    RFLR_STATE_RX_TIMEOUT,
    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    RFLR_STATE_CAD_INIT,
    RFLR_STATE_CAD_RUNNING,
} sx127x_lora_sm_t;



/**
 * \brief LoRa��������
 */
typedef struct sx127x_lora_param_set
{
    uint32_t frequency;              /* Ƶ�� */
    int8_t   power;                  /* ���书�� */
    uint8_t  bandwidth;              /* ���� [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                              5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz,  9: 500 kHz, other: Reserved] */
    uint8_t  sf;                     /* ��Ƶ���� [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips] */
    uint8_t  coderate;               /* ������ [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
    uint8_t  crc_on;                 /* CRC���� [0: �ر�, 1: ����] */
    uint8_t  header_mode;            /* ��ͷģʽ [0: ��ʽ��ͷģʽ, 1: ��ʽ��ͷģʽ] ע��SFΪ6ʱֻ��ʹ����ʽ��ͷ */
    uint8_t  rx_mode;                /* ����ģʽ [0: Continuous, 1 Single] */
    uint8_t  freq_hop_on;            /* ��Ƶ [0: �ر�, 1: ����] */
    uint8_t  hop_period;             /* ��Ƶ���ڣ�symbols������Ƶ�����ò�����Ч */
    uint32_t rx_pkt_timeout;         /* ���ճ�ʱ(symbols)[���ڵ��ν���ģʽ] */
    uint8_t  tx_timeout;             /* ���ͳ�ʱʹ��[ע�������ڲ��Զ����㳬ʱʱ�䣬Ҫ����ʱ������ʱ��ȷ] */
    uint8_t  payload_len;            /* ���س��� */
    uint16_t preamble_len;           /* ǰ���볤�� */
    uint8_t  rx_invert_iq;           /* ����IQ��ת [0: ����, 1: ��ת] */
    uint8_t  tx_invert_iq;           /* ����IQ��ת [0: ����, 1: ��ת] */
    uint8_t  crc_software;           /* ���CRC����[0: �ر�, 1: ����] */
    const uint32_t *p_hop_tab;       /* ��Ƶ�� */
    uint32_t  hop_tab_len;           /* ��Ƶ���� */
} sx127x_lora_param_set_t;


/*
 * \brief  �¼��ص�������
 */
typedef struct sx127x_lora_events
{

    /**
     * \brief ������ɻص�����ԭ��
     */
    void (*pfn_tx_done_cb)(void);

    /**
     * \brief ���䳬ʱ�ص�����ԭ��
     */
    void (*pfn_tx_timeout_cb)(void);

    /**
     * \brief ������ɻص�����ԭ��
     *
     * \param[in] p_payload : ָ����ջ�����
     * \param[in] size      : ���յ������ݴ�С
     * \param[in] rssi      : ���ո�֡ʱ�� RSSI ֵ [dBm]
     * \param[in] snr       : Ӳ��������ԭʼ�����
     *                         FSK : N/A (set to 0)
     *                         LoRa: SNR value in dB
     */
    void (*pfn_rx_done_cb)(uint8_t *p_payload, uint16_t size, int16_t rssi, int8_t snr);

    /**
     * \brief ���ճ�ʱ�ص�����ԭ��
     */
    void (*pfn_rx_timeout_cb)(void);

    /**
     * \brief ���մ���ص�����ԭ��
     */
    void (*pfn_rx_error_cb)(void);

    /**
     * \brief FHSS��Ƶ�ص�����ԭ��
     *
     * \param[in] current_channel : ��ǰ�ŵ��ı��
     */
    void (*pfn_fhss_change_channel_cb)(uint8_t current_channel);

    /**
     * \brief �ŵ���Ծ�����ɻص�����ԭ��
     *
     * \param[in] channel_activity_detected : �Ƿ��Ծ
     */
    void (*pfn_cad_done_cb)(uint8_t channel_activity_detected);

} sx127x_lora_events_t;


typedef struct radio_sx127x_lora_devinfo
{

    /** \brief �¼��ص����� ��,���û�����Ҫ�����ΪNULL */
    sx127x_lora_events_t    *p_events;

    /** \brief ���ò��� */
    sx127x_lora_param_set_t *p_lora_param;
} radio_sx127x_lora_devinfo_t;

/*
 * \brief SPI����������
 *
 * \note: �ú������������Ƭѡ����
 */
typedef struct sx127x_lora_spi_funcs
{
    /** \brief SPI дһ���ֽں���(�����ṩ) */
    void    (*pfn_spi_write_byte) (uint8_t byte);

    /** \brief SPI ��һ���ֽں��� (�����ṩ) */
    uint8_t (*pfn_spi_read_byte)  (void);
} sx127x_lora_spi_funcs_t;

/*
 * \brief GPIO ����������
 */
typedef struct sx127x_lora_gpio_funcs
{

    /** \brief ��λ�������� , val=0 ��ʾ����Ϊ�͵�ƽ  val =1 ��ʾ����Ϊ�ߵ�ƽ(���ṩʱ����Ϊ NULL) */
    void    (*pfn_reset_pin_set) (uint8_t val);

    /** \brief Ƭѡ��������, val=0 ��ʾ����Ϊ�͵�ƽ  val =1 ��ʾ����Ϊ�ߵ�ƽ (�����ṩ)*/
    void    (*pfn_sel_pin_set)   (uint8_t val);

} sx127x_lora_gpio_funcs_t;

/*
 * \brief ��ʱ������
 */
typedef struct sx127x_lora_delay_funcs
{
    /** \brief ms ��ʱ */
    void (*pfn_delay_ms) (uint32_t ms);

    /** \brief us ��ʱ */
    void (*pfn_delay_us) (uint32_t us);
} sx127x_lora_delay_funcs_t;

/*
 * \brief ����״̬
 *  cad_stateֵ�ο���grp_lora_cad_state
 *  sm_state ֵ�ο���grp_lora_state
 */
typedef struct sx127x_setting
{
    uint8_t          cad_state;
    sx127x_lora_sm_t sm_state;
} sx127x_run_state_t;

/*
 * \brief packet ����Ϣ�ṹ��
 */
typedef struct sx127x_pkt_state
{
    uint8_t    crc_pass;
    uint8_t    pkt_rx_done;
    uint8_t    pkt_tx_done;
    int8_t  snr;
    int16_t rssi;
} sx127x_pkt_info_t;



/*
 * \brief sx127x �豸�ṹ��
 */
typedef struct radio_sx127x_lora_dev
{
    radio_dev_t                 isa;
    sx127x_lora_spi_funcs_t    *p_spi;
    sx127x_lora_gpio_funcs_t   *p_gpio;
    sx127x_lora_delay_funcs_t  *p_delay;
    sx127x_run_state_t          run_state;
    sx127x_pkt_info_t           pkt_info;
    radio_sx127x_lora_devinfo_t *p_devinfo;
} radio_sx127x_lora_dev_t;


/**
 * \brief ģ�鵱ǰ����״̬
 */
typedef enum
{
    IDLE_ST = 0,
    SLEEP_ST,
    RX_RUNING_ST,
    TX_RUNING_ST,
    CAD_ST
} radio_state_t;


/**
 * \brief ģ����������ָ��
 */
struct radio_drv_funcs
{
    int           (*pfn_radio_reset)               (radio_sx127x_lora_dev_t *p_drv);
    int           (*pfn_radio_id_set)              (radio_sx127x_lora_dev_t *p_drv, uint8_t *p_id, uint8_t id_len);
    int           (*pfn_radio_id_get)              (radio_sx127x_lora_dev_t *p_drv, uint8_t *p_id, uint8_t *id_len);
    int           (*pfn_radio_preamble_length_set) (radio_sx127x_lora_dev_t *p_drv, uint16_t size);
    int           (*pfn_radio_preamble_length_get) (radio_sx127x_lora_dev_t *p_drv, uint16_t *p_len);
    int           (*pfn_radio_pa_set)              (radio_sx127x_lora_dev_t *p_drv, uint8_t level, float *p_power);
    int           (*pfn_radio_freq_set)            (radio_sx127x_lora_dev_t *p_drv, uint32_t freq);
    int           (*pfn_radio_mode_set)            (radio_sx127x_lora_dev_t *p_drv, uint8_t mode);
    int           (*pfn_radio_state_get)           (radio_sx127x_lora_dev_t *p_drv, radio_state_t *p_state);
    int           (*pfn_radio_rssi_read)           (radio_sx127x_lora_dev_t *p_drv, int16_t *p_rssi);
    int           (*pfn_radio_buf_send)            (radio_sx127x_lora_dev_t *p_drv, uint8_t *buf, uint16_t size);
    int           (*pfn_radio_buf_recv)            (radio_sx127x_lora_dev_t *p_drv, uint8_t *buf, uint16_t *p_size);
    int           (*pfn_radio_ioctl)               (radio_sx127x_lora_dev_t *p_drv, int cmd, uint8_t *p_arg);
    void          (*pfn_radio_dio0_irq_func)       (radio_sx127x_lora_dev_t *p_drv);
    void          (*pfn_radio_dio1_irq_func)       (radio_sx127x_lora_dev_t *p_drv);
    void          (*pfn_radio_dio2_irq_func)       (radio_sx127x_lora_dev_t *p_drv);
    void          (*pfn_radio_dio3_irq_func)       (radio_sx127x_lora_dev_t *p_drv);
    void          (*pfn_radio_dio5_irq_func)       (radio_sx127x_lora_dev_t *p_drv);
};

/*
 * \brief ��������ֵ
 */
enum
{
    LORA_RET_CODE_OK = 0,
    LORA_RET_CODE_ERR,
    LORA_RET_CODE_NO_CAD_DONE_IRQ,
    LORA_RET_CODE_NO_CAD_DETECT_IRQ,
    LORA_RET_CAD_RUNNING,
    LORA_RET_RECV_CRC_ERR
};





/*
 * \name  grp_lora_recv_mode
 */
#define CONTINUOUS_RECV_MODE      0
#define SINGLE_RECV_MODE          1


/*
 * \name  grp_lora_packet_mode
 */
#define EXPLICIT_HEADER         0
#define IMPLICIT_HEADER         1








/**
 * \brief ����ģ�鹤��ģʽ
 */
#define SLEEP_MODE                       0x00 /* ����ģʽ */
#define STANDBY_MODE                     0x01 /* ����ģʽ */
#define RX_MODE                          0x02 /* ����ģʽ */
#define CW_MODE                          0x03 /* �ز�ģʽ */



/**
 * \brief ���ú�������ֵ
 */
#define RADIO_RET_OK                      0
#define RADIO_RET_ERR                    -1
#define RADIO_RET_CMD_NOT_SUPPORT        -2
#define RADIO_RET_PARAM_INVALID          -3
#define RADIO_RET_CFG_OUT_OF_RANGE       -4
#define RADIO_RET_FREQ_NOT_SUPPORT       -5
#define RADIO_RET_MODULE_NOT_SUPPORT     -6
#define RADIO_RET_NOT_RECV_PKT           -7
#define RADIO_RET_NOT_IN_RX_MODE         -8
#define RADIO_RET_TX_TIMEOUT             -9
#define RADIO_RET_RX_TIMEOUT             -10
#define RADIO_RET_RX_CRC_ERR             -11

/**
 * \brief ����ģ��������
 *  ���� int radio_ioctl (radio_handle_t handle, int cmd, void *p_arg)������cmd ����
 */

#define SX127X_CAD_START      0x01    /**< \brief ����CADģʽ���� */
#define SX127X_HOP_SET        0x02    /**< \brief ����/�ر���Ƶ   */
#define SX127X_BW_SET         0x03    /**< \brief ���ô���        */
#define SX127X_SF_SET         0x04    /**< \brief ������Ƶ����    */
#define SX127X_CR_SET         0x05    /**< \brief ���ñ�����      */
#define SX127X_CRC_SET        0x06
#define SX127X_RX_MODE_SET    0x07
#define SX127X_SYNC_WORD_SET  0x08


/*
 * \brief  ����CADģʽ�����Ӧ�Ĳ���: CAD��ⷵ�ص�״̬
 *
 * \note: ��ʱ�򣬷��Ͷ�δ�������ݣ�оƬ���ܻ����⵽ǰ���룬
 * \par ��ʾ��
 * \code
 * uint8_t status;
 * radio_ioctl(handle,SX127X_CAD_START,&status);  //ֱ��CAD�������˳�
 * if( status ==  SX127X_CAD_DETECT) {
 *    radio_mode_set(handle, RX_MODE);  //����ģʽ
 * }
 *
 */
#define SX127X_CAD_NONE       0x00    /**< \brief δ��⵽ǰ����  */
#define SX127X_CAD_DETECT     0x01    /**< \brief ��⵽ǰ���� */



radio_handle_t radio_zm4xx_inst_init (uint8_t (*spi_read_byte_fun)(void),   void  (*spi_write_byte_fun) (uint8_t), void (*reset_pin_set_fun) (uint8_t ),
                                      void   (*sel_pin_set_fun) (uint8_t), void (*delay_ms_fun) (uint32_t),       void (*delay_us_fun) (uint32_t));



/**
 * \brief ����ģ�鸴λ
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK            : ��λ�ɹ�
 * \retval  RADIO_RET_PARAM_INVALID : ������Ч
 */
int radio_reset (radio_handle_t handle);



/**
 * \brief ��������ģ��ͬ��ID
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  p_id     : ID���ݵ�ָ��
 * \param[in]  id_len   : ID����
 *
 * \retval  RADIO_RET_OK                 : ���óɹ�
 * \retval  RADIO_RET_PARAM_INVALID      : ������Ч
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE   : ID���ȳ�����Χ
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ģ�鲻֧�ָù���
 */
int radio_sync_id_set (radio_handle_t handle, uint8_t *p_id, uint8_t id_len);



/**
 * \brief ��ȡ����ģ��ͬ��ID
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  p_id     : ID���ݵ�ָ��
 * \param[in]  p_len    : ID����
 *
 * \retval  RADIO_RET_OK                 : ��ȡ�ɹ�
 * \retval  RADIO_RET_PARAM_INVALID      : ������Ч
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ģ�鲻֧�ָù���
 */
int radio_sync_id_get (radio_handle_t handle, uint8_t *p_id, uint8_t *p_len);



/**
 * \brief ��������ģ��ǰ���볤��
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  size     : ID���ݵ�ָ��
 *
 * \retval  RADIO_RET_OK               : ���óɹ�
 * \retval  RADIO_RET_PARAM_INVALID    : ������Ч
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : ǰ���볤�ȳ�����Χ
 */
int radio_preamble_length_set (radio_handle_t handle, uint16_t size);



/**
 * \brief ��ȡ����ģ��ǰ���볤��
 *
 * \param[in]  handle   : ����ģ��������
 * \param[out] p_len    : ǰ���볤��
 *
 * \retval  RADIO_RET_OK            : ��ȡ�ɹ�
 * \retval  RADIO_RET_PARAM_INVALID : ������Ч
 */
int radio_preamble_length_get (radio_handle_t handle, uint16_t *p_len);



/**
 * \brief ��������ģ�鷢�书�ʵ�λ��0~7����
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  level    : ���ʵ�λ
 * \param[out] p_power  : ����ʵ�ʵ����ù���
 *
 * \retval  RADIO_RET_OK            : ���óɹ�
 * \retval  RADIO_RET_ERR           : ����ʧ��
 * \retval  RADIO_RET_PARAM_INVALID : ������Ч
 */
int radio_pa_set (radio_handle_t handle, uint8_t level, float *p_power);



/**
 * \brief ��������ģ��Ƶ��
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  freq     : Ƶ��
 *
 * \retval  RADIO_RET_OK               : ���óɹ�
 * \retval  RADIO_RET_PARAM_INVALID    : ������Ч
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : Ƶ�ʳ�����Χ
 * \retval  RADIO_RET_FREQ_NOT_SUPPORT : ��֧�ָ�Ƶ��
 */
int radio_freq_set (radio_handle_t handle, uint32_t freq);



/**
 * \brief ������ģ�鵱ǰRSSIֵ
 *
 * \param[in]  handle : ����ģ��������
 * \param[out] p_rssi : RSSIֵ
 *
 * \retval RADIO_RET_OK            : RSSIֵ��ȡ�ɹ�
 * \retval RADIO_RET_PARAM_INVALID : ������Ч
 */
int radio_rssi_read (radio_handle_t handle, int16_t *p_rssi);



/**
 * \brief ��������ģ��ģʽ
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  mode     : ģʽ
 *
 * \retval  RADIO_RET_OK                 : ���óɹ�
 * \retval  RADIO_RET_PARAM_INVALID      : ������Ч
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�鲻֧�ָ�ģʽ
 */
int radio_mode_set (radio_handle_t handle, uint8_t mode);



/**
 * \brief ��ȡ����ģ�鵱ǰ״̬
 *
 * \param[in]  handle  : ����ģ��������
 * \param[out] p_state : ģ��״̬
 *
 * \retval  RADIO_RET_OK                 : ��ȡ�ɹ�
 * \retval  RADIO_RET_PARAM_INVALID      : ������Ч
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�鲻֧�ָ�ģʽ
 */
int radio_state_get (radio_handle_t handle, radio_state_t *p_state);



/**
 * \brief ����ģ�鷢������
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  p_buf    : �������ݻ�����
 * \param[in]  size     : ���ݳ���
 *
 * \retval  RADIO_RET_OK               : ���ͳɹ�
 * \retval  RADIO_RET_PARAM_INVALID    : ������Ч
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : �����ȳ�����Χ
 */
int radio_buf_send (radio_handle_t handle, uint8_t *p_buf, uint16_t size);



/**
 * \brief ����ģ���������
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  p_buf    : �������ݻ�����
 * \param[in]  size     : ���յ����ݵĳ���
 *
 * \retval  RADIO_RET_OK            : ���ճɹ�
 * \retval  RADIO_RET_PARAM_INVALID : ������Ч
 * \retval  RADIO_RET_NOT_RECV_PKT  : û�н��յ����ݰ�
 */
int radio_buf_recv (radio_handle_t handle, uint8_t *p_buf, uint16_t *p_size);



/**
 * \brief ����ģ����ƺ���
 *
 * \param[in]  handle   : ����ģ��������
 * \param[in]  cmd      : ��������
 * \param[in]  p_arg    : �������Ӧ�Ĳ���
 *
 * \retval  RADIO_RET_OK              : ����ִ�гɹ�
 * \retval  RADIO_RET_ERR             : ����ִ��ʧ��
 * \retval  RADIO_RET_PARAM_INVALID   : ������Ч
 * \retval  RADIO_RET_CMD_NOT_SUPPORT : ��֧�ָ�����
 */
int radio_ioctl (radio_handle_t handle, int cmd, uint8_t *p_arg);



/**
 * \brief ����ģ��DIO0�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio0_irq_func (radio_handle_t handle);



/**
 * \brief ����ģ��DIO1�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio1_irq_func (radio_handle_t handle);



/**
 * \brief ����ģ��DIO2�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio2_irq_func (radio_handle_t handle);



/**
 * \brief ����ģ��DIO3�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio3_irq_func (radio_handle_t handle);



/**
 * \brief ����ģ��DIO5�����жϷ�����
 *
 * \param[in]  handle   : ����ģ��������
 *
 * \retval  RADIO_RET_OK                 : �жϷ�����ִ�����
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : ��ģ�����жϷ�����
 */
int radio_dio5_irq_func (radio_handle_t handle);


#endif

/* end of file */
