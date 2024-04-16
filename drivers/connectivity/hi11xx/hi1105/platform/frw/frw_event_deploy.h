

#ifndef __FRW_EVENT_DEPLOY_H__
#define __FRW_EVENT_DEPLOY_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "frw_event_main.h"
#include "frw_ipc_msgqueue.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_DEPLOY_H

/* ö�ٶ��� */
typedef enum {
    FRW_IPC_CORE_ID_MASTER = 0,
    FRW_IPC_CORE_ID_SLAVE = 1,

    FRW_IPC_CORE_NUM_BUTT
} frw_ipc_core_id_enum;

typedef enum {
    FRW_IPC_CORE_TYPE_MASTER = 0,
    FRW_IPC_CORE_TYPE_SLAVE = 1,

    FRW_IPC_CORE_TYPE_BUTT
} frw_ipc_core_type_enum;

/* ״̬���壬�ж������Ŀ��ܸ��ģ������Ҫԭ�ӱ��� */
typedef enum {
    FRW_IPC_CORE_STATE_INIT = 0,
    FRW_IPC_CORE_STATE_CONNECTED = 1,
    FRW_IPC_CORE_STATE_RESET = 2,
    FRW_IPC_CORE_STATE_EXIT = 3,

    FRW_IPC_CORE_STATE_BUTT
} frw_ipc_core_state_enum;

typedef enum {
    FRW_IPC_MSG_TYPE_EVENT = 0,             /* �ⲿ�¼� */
    FRW_IPC_MSG_TYPE_CONNECT_REQUEST = 1,   /* IPC�������� */
    FRW_IPC_MSG_TYPE_CONNECT_RESPONSE = 2,  /* IPC������Ӧ */
    FRW_IPC_MSG_TYPE_CONFIG_REQUEST = 3,    /* IPC�������� */
    FRW_IPC_MSG_TYPE_CONFIG_RESPONSE = 4,   /* IPC������Ӧ */
    FRW_IPC_MSG_TYPE_ERROR_NOTICE = 5,      /* IPC�쳣֪ͨ */
    FRW_IPC_MSG_TYPE_RESET_REQUEST = 6,     /* IPC��λ���� */
    FRW_IPC_MSG_TYPE_RESET_RESPONSE = 7,    /* IPC��λ��Ӧ */
    FRW_IPC_MSG_TYPE_OPEN_OAM = 8,          /* IPC��ά�� */
    FRW_IPC_MSG_TYPE_CLOSE_OAM = 9,         /* IPC�ر�ά�� */
    FRW_IPC_MSG_TYPE_EXIT_REQUEST = 10,     /* IPCж������ */
    FRW_IPC_MSG_TYPE_EXIT_RESPONSE = 11,    /* IPCж����Ӧ */
    FRW_IPC_MSG_TYPE_TX_INT_ENABLE = 12,    /* IPC TX�жϿ��� */
    FRW_IPC_MSG_TYPE_TX_INT_DISENABLE = 13, /* IPC TX�жϹر� */

    FRW_IPC_MSG_TYPE_BUTT
} frw_ipc_msg_type_enum;

/* ������к����� */
#define FRW_IPC_MAX_SEQ_NUMBER 0xFFFF

/* ��ȡ��Ϣ���� */
#define frw_ipc_get_msg_queue(_queue, _type) ((_queue) = ((_type) == FRW_IPC_CORE_TYPE_MASTER) ? \
    &g_queue_master_to_slave : &g_queue_slave_to_master)

/* IPC�ڲ���Ϣ�ṹ�� */
typedef struct {
    frw_ipc_msg_header_stru st_ipc_hdr;
    uint8_t auc_resv[2]; /* 2:Ϊ��4�ֽڶ��� */
    uint16_t ul_length; /* ���ݳ��� */
    uint8_t auc_data[4]; /* 4:�ɱ䳤������������ʼλ�� */
} frw_ipc_inter_msg_stru;

/* STRUCT ���� */
typedef struct {
    uint8_t en_cpuid;           /* ����cpuid */
    uint8_t en_target_cpuid;    /* Ŀ���ں�cpuid */
    uint8_t en_states;       /* �ڵ㵱ǰ״̬: ��ʼ��, ����, ��λ, �رյ�״̬ */
    uint8_t en_tx_int_ctl;      /* TX�жϿ��� */
    uint8_t en_cpu_type;      /* ��ǰCPU���ͣ������˻�Ӻ� */
    uint8_t uc_resv[3];                          /* 3:Ϊ��4�ֽڶ��� */
    uint16_t us_seq_num_rx_expect;               /* ������Ϣ�����к�,��ʼֵΪ0,�����һ���������յ����к� */
    uint16_t us_seq_num_tx_expect;               /* ������Ϣ�����к�,��ʼֵΪ0,�����һ���������͵����к� */
    frw_ipc_msg_callback_stru st_ipc_msg_callback; /* �ص��������� */
    oal_irq_dev_stru st_irq_dev;                   /* �жϽṹ�� */
} frw_ipc_node_stru;

/*
 * �� �� ��  : frw_ipc_get_core_type
 * ��������  : ��ȡCPU����
 * �� �� ֵ  : CPU����: master or slave
 */
OAL_STATIC OAL_INLINE frw_ipc_core_type_enum frw_ipc_get_core_type(void)
{
    if (oal_get_core_id() == FRW_IPC_CORE_ID_MASTER) {
        return FRW_IPC_CORE_TYPE_MASTER;
    }

    return FRW_IPC_CORE_TYPE_SLAVE;
}

/*
 * �� �� ��  : frw_ipc_get_header
 * ��������  : ��ȡIPCͷ
 * �������  : pst_ipc_mem_msg �� �¼��ڴ���ַ
 */
OAL_STATIC OAL_INLINE uint8_t *frw_ipc_get_header(frw_ipc_msg_mem_stru *pst_ipc_mem_msg)
{
    pst_ipc_mem_msg->puc_data -= FRW_IPC_MSG_HEADER_LENGTH;
    return pst_ipc_mem_msg->puc_data;
}

uint32_t frw_event_deploy_init(void);
uint32_t frw_event_deploy_exit(void);

#endif /* end of frw_event_deploy.h */