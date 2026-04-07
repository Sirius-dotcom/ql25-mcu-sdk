/**
 * @file utb_ipc_msg.h
 * @brief IPC 消息契约：命令 ID + 所有 payload 结构定义
 *
 * 命令 ID 命名空间：
 *   0x01xx : 控制命令（ctrl_req / ctrl_rsp）
 *   0x02xx : 配置命令（ctrl_req / ctrl_rsp）
 *   0x00xx : 事件（event_ring，已在 utb_ipc.h 定义 UTB_IPC_EVT_xxx）
 *
 * 所有 payload 结构不含消息头，由 ring_push(hdr, payload, len) 分别传入。
 * 接收方根据 hdr.msg_type + hdr.cmd_id 决定 payload 类型。
 */

#ifndef UTB_IPC_MSG_H
#define UTB_IPC_MSG_H

#include "utb_ipc.h"

/* ========================================================================
 * 控制命令 ID（ctrl_req/ctrl_rsp，cmd_id 字段）
 * ======================================================================== */

#define UTB_CMD_NOP             0x0100  /* 空操作（连通性测试） */
#define UTB_CMD_HEARTBEAT       0x0101  /* 心跳探测 */
#define UTB_CMD_RESET_DP        0x0102  /* 复位数据面 */
#define UTB_CMD_GET_STATUS      0x0103  /* 查询数据面状态 */
#define UTB_CMD_GET_VERSION     0x0104  /* 查询固件版本 */

/* 配置命令 ID */
#define UTB_CMD_SET_CONFIG      0x0200  /* 下发配置 */
#define UTB_CMD_GET_CONFIG      0x0201  /* 读取配置 */
#define UTB_CMD_DEL_CONFIG      0x0202  /* 删除配置 */

/* ========================================================================
 * 1. 心跳 payload（双向，CMD_HEARTBEAT）
 * ======================================================================== */

typedef struct {
    uint32_t sender_tick;       /* 发送方当前 tick */
    uint32_t uptime_sec;        /* 发送方运行时间（秒） */
} utb_ipc_heartbeat_payload_t;  /* 8B */

/* ========================================================================
 * 2. 状态查询响应 payload（从核→主核，CMD_GET_STATUS 的 rsp）
 * ======================================================================== */

/* 数据面运行状态 */
#define UTB_DP_STATE_INIT       0
#define UTB_DP_STATE_READY      1
#define UTB_DP_STATE_RUNNING    2
#define UTB_DP_STATE_ERROR      3
#define UTB_DP_STATE_STOPPED    4

typedef struct {
    uint8_t  dp_state;          /* UTB_DP_STATE_xxx */
    uint8_t  link_state;        /* 0=down, 1=up */
    uint16_t _pad;
    uint32_t uptime_sec;
    uint32_t rx_packets;
    uint32_t tx_packets;
    uint32_t rx_bytes;
    uint32_t tx_bytes;
    uint32_t rx_errors;
    uint32_t tx_errors;
    uint32_t rx_drops;
    uint32_t tx_drops;
} utb_ipc_status_payload_t;     /* 40B */

/* ========================================================================
 * 3. 版本查询响应 payload（从核→主核，CMD_GET_VERSION 的 rsp）
 * ======================================================================== */

typedef struct {
    uint8_t  major;
    uint8_t  minor;
    uint16_t patch;
    uint32_t build_id;          /* 编译标识（git hash 低 32 位） */
    char     build_date[16];    /* "YYYY-MM-DD" + '\0' + padding */
} utb_ipc_version_payload_t;    /* 24B */

/* ========================================================================
 * 4. 配置操作 payload（主核→从核，CMD_SET/GET/DEL_CONFIG）
 *
 * 请求：cfg_id + action + data（SET 时有效）
 * 响应：cfg_id + data（GET 时有效）
 * ======================================================================== */

#define UTB_CFG_ACTION_SET      0
#define UTB_CFG_ACTION_GET      1
#define UTB_CFG_ACTION_DEL      2

typedef struct {
    uint16_t cfg_id;            /* 配置项 ID */
    uint8_t  action;            /* UTB_CFG_ACTION_xxx */
    uint8_t  _pad;
    uint16_t data_len;          /* 配置数据长度 */
    uint16_t _pad2;
    uint8_t  data[88];          /* 最大 88B 配置数据（88+8=96 < 104 payload max）*/
} utb_ipc_config_payload_t;     /* 96B */

/* ========================================================================
 * 5. 告警事件 payload（event_ring，EVT_ALARM）
 * ======================================================================== */

#define UTB_ALARM_SEV_INFO      0
#define UTB_ALARM_SEV_MINOR     1
#define UTB_ALARM_SEV_MAJOR     2
#define UTB_ALARM_SEV_CRITICAL  3

typedef struct {
    uint16_t alarm_id;          /* 告警码 */
    uint8_t  severity;          /* UTB_ALARM_SEV_xxx */
    uint8_t  module_id;         /* 产生告警的模块 */
    uint32_t info0;             /* 告警附加信息 */
    uint32_t info1;
    uint32_t timestamp_ms;      /* 告警发生时间 */
} utb_ipc_alarm_payload_t;      /* 16B */

/* ========================================================================
 * 6. 故障事件 payload（event_ring，EVT_FAULT）
 * ======================================================================== */

typedef struct {
    uint16_t fault_id;          /* 故障码 */
    uint8_t  fatal;             /* 非零 = 不可恢复 */
    uint8_t  module_id;
    uint32_t fault_addr;        /* 故障地址（如有） */
    uint32_t info0;
    uint32_t info1;
    uint32_t timestamp_ms;
} utb_ipc_fault_payload_t;      /* 20B */

/* ========================================================================
 * 7. 链路状态事件 payload（event_ring，EVT_LINK_UP / EVT_LINK_DOWN）
 * ======================================================================== */

typedef struct {
    uint8_t  port_id;           /* 端口号 */
    uint8_t  link_state;        /* 0=down, 1=up */
    uint16_t speed_mbps;        /* 链路速率（Mbps） */
    uint32_t timestamp_ms;
} utb_ipc_link_payload_t;       /* 8B */

/* ========================================================================
 * 编译期断言：所有 payload 不超过最大限制
 * ======================================================================== */

typedef char _assert_heartbeat_fit
    [(sizeof(utb_ipc_heartbeat_payload_t) <= UTB_IPC_CTRL_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_status_fit
    [(sizeof(utb_ipc_status_payload_t) <= UTB_IPC_CTRL_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_version_fit
    [(sizeof(utb_ipc_version_payload_t) <= UTB_IPC_CTRL_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_config_fit
    [(sizeof(utb_ipc_config_payload_t) <= UTB_IPC_CTRL_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_alarm_fit
    [(sizeof(utb_ipc_alarm_payload_t) <= UTB_IPC_EVENT_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_fault_fit
    [(sizeof(utb_ipc_fault_payload_t) <= UTB_IPC_EVENT_PAYLOAD_MAX) ? 1 : -1];
typedef char _assert_link_fit
    [(sizeof(utb_ipc_link_payload_t) <= UTB_IPC_EVENT_PAYLOAD_MAX) ? 1 : -1];

#endif /* UTB_IPC_MSG_H */
