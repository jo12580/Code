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
 * \note: 本驱动使用方法：
 *        1、调用radio_sx127x_lora_init初始化设备驱动
 *        2、调用radio_buf_recv函数接收数据， 调用radio_buf_send函数发送数据
 *        3、若需要使用CAD和跳频功能，则需要操作一下步骤：
 *           （1）将DIO0，DIO1，DIO2 配置成上升沿中断触发
 *           （2）在DIO0中断中调用 radio_dio0_irq_func()函数
 *           （3）在DIO1中断中调用 radio_dio1_irq_func()函数
 *           （4）在DIO2中断中调用 radio_dio2_irq_func()函数
 *          说明： 本驱动中只适配了radio_dio0_irq_func()、radio_dio1_irq_func()和radio_dio2_irq_func()三个标准中断函数
 *                 其他标准中断函数请忽略。
 *                 若用户需要中断接收数据， 可在DIO0上升沿中断中调用 radio_dio0_irq_func()函数。
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
 * \name grp_lora_cad_state  CAD 状态
 */
#define CAD_STATE_START         0x01
#define CAD_STATE_FINISH        0x02
#define CAD_STATE_DONE          0x04
#define CAD_STATE_DETECT        0x08

#define XTAL_32MHZ

#define LOW_DATARATE_FORCE      0             /**< \brief 强制开启低速率优化 */

#ifdef XTAL_32MHZ
#define FREQ_STEP               61.03515625   /**< \brief 分辨率*/
#endif
#ifdef XTAL_26MHZ
#define FREQ_STEP               49.59106445   /**< \brief 分辨率*/
#endif

/**
 * \brief 射频中频临界
 */
#define __LORA_RF_MID_BAND_THRESH               525000000

/**
 * 计算 RSSI 需要用的常量 (RSSI偏移值)
 */
#define __LORA_RSSI_OFFSET_LF                   -164
#define __LORA_RSSI_OFFSET_HF                   -157


#define SM_STATE_SET(p_this, state)   ((p_this)->run_state.sm_state = state)
#define SM_STATE_GET(p_this)          ((p_this)->run_state.sm_state)

#define CAD_STATE_SET(p_this, state)  ((p_this)->run_state.cad_state |= state)
#define CAD_STATE_GET(p_this)         ((p_this)->run_state.cad_state)
#define CAD_STATE_CLR(p_this)         ((p_this)->run_state.cad_state = 0)


/**
 * \brief 无线射频设备
 */
typedef struct radio_dev
{
    const struct radio_drv_funcs *p_funcs;   /* 设备的驱动函数  */
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
 * \brief LoRa参数配置
 */
typedef struct sx127x_lora_param_set
{
    uint32_t frequency;              /* 频率 */
    int8_t   power;                  /* 发射功率 */
    uint8_t  bandwidth;              /* 带宽 [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                              5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz,  9: 500 kHz, other: Reserved] */
    uint8_t  sf;                     /* 扩频因子 [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips] */
    uint8_t  coderate;               /* 编码率 [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
    uint8_t  crc_on;                 /* CRC检验 [0: 关闭, 1: 开启] */
    uint8_t  header_mode;            /* 报头模式 [0: 显式报头模式, 1: 隐式报头模式] 注：SF为6时只能使用隐式报头 */
    uint8_t  rx_mode;                /* 接收模式 [0: Continuous, 1 Single] */
    uint8_t  freq_hop_on;            /* 跳频 [0: 关闭, 1: 开启] */
    uint8_t  hop_period;             /* 跳频周期（symbols），跳频开启该参数有效 */
    uint32_t rx_pkt_timeout;         /* 接收超时(symbols)[用于单次接收模式] */
    uint8_t  tx_timeout;             /* 发送超时使能[注：驱动内部自动计算超时时间，要求延时函数延时精确] */
    uint8_t  payload_len;            /* 负载长度 */
    uint16_t preamble_len;           /* 前导码长度 */
    uint8_t  rx_invert_iq;           /* 接收IQ翻转 [0: 正常, 1: 翻转] */
    uint8_t  tx_invert_iq;           /* 发送IQ翻转 [0: 正常, 1: 翻转] */
    uint8_t  crc_software;           /* 软件CRC检验[0: 关闭, 1: 开启] */
    const uint32_t *p_hop_tab;       /* 跳频表 */
    uint32_t  hop_tab_len;           /* 跳频表长度 */
} sx127x_lora_param_set_t;


/*
 * \brief  事件回调函数集
 */
typedef struct sx127x_lora_events
{

    /**
     * \brief 传输完成回调函数原型
     */
    void (*pfn_tx_done_cb)(void);

    /**
     * \brief 传输超时回调函数原型
     */
    void (*pfn_tx_timeout_cb)(void);

    /**
     * \brief 接收完成回调函数原型
     *
     * \param[in] p_payload : 指向接收缓冲区
     * \param[in] size      : 接收到的数据大小
     * \param[in] rssi      : 接收该帧时的 RSSI 值 [dBm]
     * \param[in] snr       : 硬件给出的原始信噪比
     *                         FSK : N/A (set to 0)
     *                         LoRa: SNR value in dB
     */
    void (*pfn_rx_done_cb)(uint8_t *p_payload, uint16_t size, int16_t rssi, int8_t snr);

    /**
     * \brief 接收超时回调函数原型
     */
    void (*pfn_rx_timeout_cb)(void);

    /**
     * \brief 接收错误回调函数原型
     */
    void (*pfn_rx_error_cb)(void);

    /**
     * \brief FHSS跳频回调函数原型
     *
     * \param[in] current_channel : 当前信道的编号
     */
    void (*pfn_fhss_change_channel_cb)(uint8_t current_channel);

    /**
     * \brief 信道活跃检测完成回调函数原型
     *
     * \param[in] channel_activity_detected : 是否活跃
     */
    void (*pfn_cad_done_cb)(uint8_t channel_activity_detected);

} sx127x_lora_events_t;


typedef struct radio_sx127x_lora_devinfo
{

    /** \brief 事件回调函数 集,若用户不需要则可置为NULL */
    sx127x_lora_events_t    *p_events;

    /** \brief 配置参数 */
    sx127x_lora_param_set_t *p_lora_param;
} radio_sx127x_lora_devinfo_t;

/*
 * \brief SPI操作函数集
 *
 * \note: 该函数内无需控制片选引脚
 */
typedef struct sx127x_lora_spi_funcs
{
    /** \brief SPI 写一个字节函数(必须提供) */
    void    (*pfn_spi_write_byte) (uint8_t byte);

    /** \brief SPI 读一个字节函数 (必须提供) */
    uint8_t (*pfn_spi_read_byte)  (void);
} sx127x_lora_spi_funcs_t;

/*
 * \brief GPIO 操作函数集
 */
typedef struct sx127x_lora_gpio_funcs
{

    /** \brief 复位引脚设置 , val=0 表示设置为低电平  val =1 表示设置为高电平(不提供时设置为 NULL) */
    void    (*pfn_reset_pin_set) (uint8_t val);

    /** \brief 片选引脚设置, val=0 表示设置为低电平  val =1 表示设置为高电平 (必须提供)*/
    void    (*pfn_sel_pin_set)   (uint8_t val);

} sx127x_lora_gpio_funcs_t;

/*
 * \brief 延时函数集
 */
typedef struct sx127x_lora_delay_funcs
{
    /** \brief ms 延时 */
    void (*pfn_delay_ms) (uint32_t ms);

    /** \brief us 延时 */
    void (*pfn_delay_us) (uint32_t us);
} sx127x_lora_delay_funcs_t;

/*
 * \brief 运行状态
 *  cad_state值参考：grp_lora_cad_state
 *  sm_state 值参考：grp_lora_state
 */
typedef struct sx127x_setting
{
    uint8_t          cad_state;
    sx127x_lora_sm_t sm_state;
} sx127x_run_state_t;

/*
 * \brief packet 包信息结构体
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
 * \brief sx127x 设备结构体
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
 * \brief 模块当前运行状态
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
 * \brief 模块驱动函数指针
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
 * \brief 函数返回值
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
 * \brief 无线模块工作模式
 */
#define SLEEP_MODE                       0x00 /* 休眠模式 */
#define STANDBY_MODE                     0x01 /* 待机模式 */
#define RX_MODE                          0x02 /* 接收模式 */
#define CW_MODE                          0x03 /* 载波模式 */



/**
 * \brief 配置函数返回值
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
 * \brief 无线模块控制命令集
 *  用于 int radio_ioctl (radio_handle_t handle, int cmd, void *p_arg)函数的cmd 参数
 */

#define SX127X_CAD_START      0x01    /**< \brief 开启CAD模式命令 */
#define SX127X_HOP_SET        0x02    /**< \brief 开启/关闭跳频   */
#define SX127X_BW_SET         0x03    /**< \brief 设置带宽        */
#define SX127X_SF_SET         0x04    /**< \brief 设置扩频因子    */
#define SX127X_CR_SET         0x05    /**< \brief 设置编码率      */
#define SX127X_CRC_SET        0x06
#define SX127X_RX_MODE_SET    0x07
#define SX127X_SYNC_WORD_SET  0x08


/*
 * \brief  开启CAD模式命令对应的参数: CAD检测返回的状态
 *
 * \note: 有时候，发送端未发送数据，芯片可能会误检测到前导码，
 * \par 简单示例
 * \code
 * uint8_t status;
 * radio_ioctl(handle,SX127X_CAD_START,&status);  //直至CAD结束才退出
 * if( status ==  SX127X_CAD_DETECT) {
 *    radio_mode_set(handle, RX_MODE);  //接收模式
 * }
 *
 */
#define SX127X_CAD_NONE       0x00    /**< \brief 未检测到前导码  */
#define SX127X_CAD_DETECT     0x01    /**< \brief 检测到前导码 */



radio_handle_t radio_zm4xx_inst_init (uint8_t (*spi_read_byte_fun)(void),   void  (*spi_write_byte_fun) (uint8_t), void (*reset_pin_set_fun) (uint8_t ),
                                      void   (*sel_pin_set_fun) (uint8_t), void (*delay_ms_fun) (uint32_t),       void (*delay_us_fun) (uint32_t));



/**
 * \brief 无线模块复位
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK            : 复位成功
 * \retval  RADIO_RET_PARAM_INVALID : 参数无效
 */
int radio_reset (radio_handle_t handle);



/**
 * \brief 设置无线模块同步ID
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  p_id     : ID数据的指针
 * \param[in]  id_len   : ID长度
 *
 * \retval  RADIO_RET_OK                 : 设置成功
 * \retval  RADIO_RET_PARAM_INVALID      : 参数无效
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE   : ID长度超出范围
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 模块不支持该功能
 */
int radio_sync_id_set (radio_handle_t handle, uint8_t *p_id, uint8_t id_len);



/**
 * \brief 获取无线模块同步ID
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  p_id     : ID数据的指针
 * \param[in]  p_len    : ID长度
 *
 * \retval  RADIO_RET_OK                 : 获取成功
 * \retval  RADIO_RET_PARAM_INVALID      : 参数无效
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 模块不支持该功能
 */
int radio_sync_id_get (radio_handle_t handle, uint8_t *p_id, uint8_t *p_len);



/**
 * \brief 设置无线模块前导码长度
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  size     : ID数据的指针
 *
 * \retval  RADIO_RET_OK               : 设置成功
 * \retval  RADIO_RET_PARAM_INVALID    : 参数无效
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : 前导码长度超出范围
 */
int radio_preamble_length_set (radio_handle_t handle, uint16_t size);



/**
 * \brief 获取无线模块前导码长度
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[out] p_len    : 前导码长度
 *
 * \retval  RADIO_RET_OK            : 获取成功
 * \retval  RADIO_RET_PARAM_INVALID : 参数无效
 */
int radio_preamble_length_get (radio_handle_t handle, uint16_t *p_len);



/**
 * \brief 设置无线模块发射功率档位（0~7档）
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  level    : 功率档位
 * \param[out] p_power  : 返回实际的设置功率
 *
 * \retval  RADIO_RET_OK            : 设置成功
 * \retval  RADIO_RET_ERR           : 设置失败
 * \retval  RADIO_RET_PARAM_INVALID : 参数无效
 */
int radio_pa_set (radio_handle_t handle, uint8_t level, float *p_power);



/**
 * \brief 设置无线模块频率
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  freq     : 频率
 *
 * \retval  RADIO_RET_OK               : 设置成功
 * \retval  RADIO_RET_PARAM_INVALID    : 参数无效
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : 频率超出范围
 * \retval  RADIO_RET_FREQ_NOT_SUPPORT : 不支持该频率
 */
int radio_freq_set (radio_handle_t handle, uint32_t freq);



/**
 * \brief 读无线模块当前RSSI值
 *
 * \param[in]  handle : 无线模块操作句柄
 * \param[out] p_rssi : RSSI值
 *
 * \retval RADIO_RET_OK            : RSSI值获取成功
 * \retval RADIO_RET_PARAM_INVALID : 参数无效
 */
int radio_rssi_read (radio_handle_t handle, int16_t *p_rssi);



/**
 * \brief 设置无线模块模式
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  mode     : 模式
 *
 * \retval  RADIO_RET_OK                 : 设置成功
 * \retval  RADIO_RET_PARAM_INVALID      : 参数无效
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块不支持该模式
 */
int radio_mode_set (radio_handle_t handle, uint8_t mode);



/**
 * \brief 获取无线模块当前状态
 *
 * \param[in]  handle  : 无线模块操作句柄
 * \param[out] p_state : 模块状态
 *
 * \retval  RADIO_RET_OK                 : 获取成功
 * \retval  RADIO_RET_PARAM_INVALID      : 参数无效
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块不支持该模式
 */
int radio_state_get (radio_handle_t handle, radio_state_t *p_state);



/**
 * \brief 无线模块发送数据
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  p_buf    : 发送数据缓冲区
 * \param[in]  size     : 数据长度
 *
 * \retval  RADIO_RET_OK               : 发送成功
 * \retval  RADIO_RET_PARAM_INVALID    : 参数无效
 * \retval  RADIO_RET_CFG_OUT_OF_RANGE : 包长度超出范围
 */
int radio_buf_send (radio_handle_t handle, uint8_t *p_buf, uint16_t size);



/**
 * \brief 无线模块接收数据
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  p_buf    : 接收数据缓冲区
 * \param[in]  size     : 接收到数据的长度
 *
 * \retval  RADIO_RET_OK            : 接收成功
 * \retval  RADIO_RET_PARAM_INVALID : 参数无效
 * \retval  RADIO_RET_NOT_RECV_PKT  : 没有接收到数据包
 */
int radio_buf_recv (radio_handle_t handle, uint8_t *p_buf, uint16_t *p_size);



/**
 * \brief 无线模块控制函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 * \param[in]  cmd      : 控制命令
 * \param[in]  p_arg    : 该命令对应的参数
 *
 * \retval  RADIO_RET_OK              : 命令执行成功
 * \retval  RADIO_RET_ERR             : 命令执行失败
 * \retval  RADIO_RET_PARAM_INVALID   : 参数无效
 * \retval  RADIO_RET_CMD_NOT_SUPPORT : 不支持该命令
 */
int radio_ioctl (radio_handle_t handle, int cmd, uint8_t *p_arg);



/**
 * \brief 无线模块DIO0引脚中断服务函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK                 : 中断服务函数执行完成
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块无中断服务函数
 */
int radio_dio0_irq_func (radio_handle_t handle);



/**
 * \brief 无线模块DIO1引脚中断服务函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK                 : 中断服务函数执行完成
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块无中断服务函数
 */
int radio_dio1_irq_func (radio_handle_t handle);



/**
 * \brief 无线模块DIO2引脚中断服务函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK                 : 中断服务函数执行完成
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块无中断服务函数
 */
int radio_dio2_irq_func (radio_handle_t handle);



/**
 * \brief 无线模块DIO3引脚中断服务函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK                 : 中断服务函数执行完成
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块无中断服务函数
 */
int radio_dio3_irq_func (radio_handle_t handle);



/**
 * \brief 无线模块DIO5引脚中断服务函数
 *
 * \param[in]  handle   : 无线模块操作句柄
 *
 * \retval  RADIO_RET_OK                 : 中断服务函数执行完成
 * \retval  RADIO_RET_MODULE_NOT_SUPPORT : 该模块无中断服务函数
 */
int radio_dio5_irq_func (radio_handle_t handle);


#endif

/* end of file */
