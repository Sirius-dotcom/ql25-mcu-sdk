/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_ADP_IF_H
#define QL25_ADP_IF_H

int ql25_send_msg_to_adp(utb_ql25_ctrl_t *ql25_ctrl, const uint8_t* data, int len);
utb_thread_handle_t *ql25_if_init_thread(utb_ql25_ctrl_t *ql25_ctrl);
void ql25_if_deinit_thread(utb_ql25_ctrl_t *ql25_ctrl);

#endif //QL25_ADP_IF_H
