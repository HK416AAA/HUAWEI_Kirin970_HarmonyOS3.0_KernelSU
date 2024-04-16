/*
 * om_bigdata.h
 *
 * audio socdsp report current state
 *
 * Copyright (c) 2013-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __OM_BIGDATA_H__
#define __OM_BIGDATA_H__

#include <linux/platform_device.h>

#include "dsp_om.h"

enum socdsp_cpu_load_info_enum {
	SOCDSP_CPU_LOAD_REPORT = 0,
	SOCDSP_CPU_LOAD_VOTE_UP,
	SOCDSP_CPU_LOAD_VOTE_DOWN,
	SOCDSP_CPU_LOAD_LACK_PERFORMANCE,
	SOCDSP_CPU_LOAD_INFO_BUTT
};

enum socdsp_cpu_om_info_enum {
	SOCDSP_CPU_OM_LOAD_INFO = 0,
	SOCDSP_CPU_OM_ALGO_MCPS_INFO,
	SOCDSP_CPU_OM_UPDATE_BUFF_DELAY_INFO,
	SOCDSP_CPU_OM_DUMP_LOG_INFO,
	SOCDSP_CPU_OM_INFO_BUTT
};

enum socdsp_voice_3a_info_enum {
	SOCDSP_3A_INFO_MSG = 0,
	SOCDSP_VOIP_INFO_MSG,
	SOCDSP_VOIP_MICBLK_INFO_MSG,
	SOCDSP_3A_MSG_BUTT
};

enum effect_algo_enum {
	ID_EFFECT_ALGO_START = 0,
	ID_EFFECT_ALGO_FORMATER,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT,
	ID_EFFECT_ALGO_FORTE_VOICE_MICIN,
	ID_EFFECT_ALGO_FORTE_VOICE_SPKOUT_BWE,
	ID_EFFECT_ALGO_FORTE_VOIP_MICIN,
	ID_EFFECT_ALGO_FORTE_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_IN_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_INTERLACE,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_GENERAL,
	ID_EFFECT_ALGO_OUT_CONVERT_I2S_HI363X,
	ID_EFFECT_ALGO_SWAP,
	ID_EFFECT_ALGO_IMEDIA_WNR_MICIN,
	ID_EFFECT_ALGO_IMEDIA_WNR_SPKOUT,
	ID_EFFECT_ALGO_SWS_INTERFACE,
	ID_EFFECT_ALGO_DTS,
	ID_EFFECT_ALGO_DRE,
	ID_EFFECT_ALGO_CHC,
	ID_EFFECT_ALGO_SRC,
	ID_EFFECT_ALGO_TTY,
	ID_EFFECT_ALGO_KARAOKE_RECORD,
	ID_EFFECT_ALGO_KARAOKE_PLAY,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_CS_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_MICIN,
	ID_EFFECT_ALGO_MLIB_VOIP_CALL_SPKOUT,
	ID_EFFECT_ALGO_MLIB_AUDIO_PLAY,
	ID_EFFECT_ALGO_MLIB_AUDIO_RECORD,
	ID_EFFECT_ALGO_MLIB_SIRI_MICIN,
	ID_EFFECT_ALGO_MLIB_SIRI_SPKOUT,
	ID_EFFECT_ALGO_EQ,
	ID_EFFECT_ALGO_MBDRC6402,
	ID_EFFECT_ALGO_IMEDIA_VOIP_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOIP_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_MICIN,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT,
	ID_EFFECT_ALGO_IMEDIA_VOICE_CALL_SPKOUT_BWE,
	ID_EFFECT_ALGO_BUTT
};

enum effect_stream_id {
	AUDIO_STREAM_PCM_OUTPUT = 0,
	AUDIO_STREAM_PLAYER_OUTPUT,
	AUDIO_STREAM_DIRECT_OUTPUT,
	AUDIO_STREAM_LOWLATENCY_OUTPUT,
	AUDIO_STREAM_MMAP_OUTPUT,
	AUDIO_STREAM_MIXER_OUTPUT,
	AUDIO_STREAM_VOICE_OUTPUT,
	AUDIO_STREAM_VOICEPP_OUTPUT,
	AUDIO_STREAM_OUTPUT_CNT,

	AUDIO_STREAM_PCM_INPUT = 0x10,
	AUDIO_STREAM_LOWLATENCY_INPUT,
	AUDIO_STREAM_MMAP_INPUT,
	AUDIO_STREAM_VOICE_INPUT,
	AUDIO_STREAM_VOICEPP_INPUT,
	AUDIO_STREAM_INPUT_CNT
};

int om_bigdata_init(struct platform_device *pdev);
void om_bigdata_deinit(void);
int socdsp_om_get_voice_bsd_param(uintptr_t uaddr);
void socdsp_om_rev_data_handle(enum socdsp_om_work_id work_id, const unsigned char *addr, unsigned int len);

#endif /* __OM_BIGDATA_H__ */

