

/* ͷ�ļ����� */
#include "frw_event_sched.h"

#include "oam_ext_if.h"
#include "frw_main.h"
#include "frw_event_main.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_SCHED_C

/*
 * �� �� ��  : frw_event_sched_deactivate_queue_no_lock
 * ��������  : �ӵ��ȶ���ɾ��һ���¼�����
 * �������  : pst_sched_queue: ���ȶ���ָ��
 *             pst_event_queue: �¼�����ָ��
 */
uint32_t frw_event_sched_deactivate_queue_no_lock(frw_event_sched_queue_stru *pst_sched_queue,
                                                  frw_event_queue_stru *pst_event_queue)
{
    if (pst_event_queue->st_queue.uc_element_cnt != 0) {
        return OAL_SUCC;
    }

    /* ���µ��ȶ����ϵ���Ȩ�ؼ����� */
    pst_sched_queue->ul_total_weight_cnt -= pst_event_queue->uc_weight_cnt;

    /* ���¼����е�Ȩ�ؼ��������� */
    pst_event_queue->uc_weight_cnt = 0;

    /* mayuan debug */
    if (((uintptr_t)&pst_event_queue->st_list) == ((uintptr_t)&pst_sched_queue->st_head)) {
        oam_error_log0(0, OAM_SF_FRW, "frw_event_sched_deactivate_queue, delete header itself.");
        return OAL_FAIL;
    }

    /* ���¼����дӵ���������ɾ�� */
    oal_dlist_delete_entry(&pst_event_queue->st_list);

    /* ���¼�������Ϊ����Ծ״̬ */
    pst_event_queue->en_state = FRW_EVENT_QUEUE_STATE_INACTIVE;

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_sched_activate_queue_no_lock
 * ��������  : ����ȶ�������һ���µ��¼�����
 *             ��װΪ�������ӿڣ����ֽӿڶԳ��ԣ��������ӿڵ���
 * �������  : past_sched_queue: ���ȶ���ָ��
 *             pst_event_queue : �¼�����ָ��
 */
uint32_t frw_event_sched_activate_queue_no_lock(frw_event_sched_queue_stru *pst_sched_queue,
                                                frw_event_queue_stru *pst_event_queue)
{
    /* ����¼������Ѿ��ڿ�ִ�ж�����(���ڼ���״̬)����ֱ�ӷ��سɹ� */
    if (pst_event_queue->en_state == FRW_EVENT_QUEUE_STATE_ACTIVE) {
        return OAL_SUCC;
    }

    /* ��Ϊ�¼����е�Ȩ�ؼ����� */
    pst_event_queue->uc_weight_cnt = pst_event_queue->uc_weight;

    /* ���µ��ȶ����ϵ���Ȩ�ؼ����� */
    pst_sched_queue->ul_total_weight_cnt += pst_event_queue->uc_weight_cnt;

    /* ���¼����м������������ĩβ */
    oal_dlist_add_tail(&pst_event_queue->st_list, &pst_sched_queue->st_head);

    /* ���¼�������Ϊ����״̬ */
    pst_event_queue->en_state = FRW_EVENT_QUEUE_STATE_ACTIVE;

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_sched_pick_next_event_queue_wrr
 * ��������  : �ӵ���������ѡ��һ�����������¼�
 * �������  : pst_sched_queue: ���ȶ���ָ��
 * �������  : pst_sched_queue: ���ȶ���ָ��
 */
void *frw_event_sched_pick_next_event_queue_wrr(frw_event_sched_queue_stru *pst_sched_queue)
{
    oal_dlist_head_stru *pst_list = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    void *p_event = NULL;
    oal_uint flag = 0;
    oal_dlist_head_stru *pst_header = NULL;

    if (oal_unlikely(pst_sched_queue == NULL)) {
        oal_warn_on(1);
        return NULL;
    }

    oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &flag);

    pst_header = &pst_sched_queue->st_head;
    /* ���������������� */
    oal_dlist_search_for_each(pst_list, pst_header)
    {
        pst_event_queue = oal_dlist_get_entry(pst_list, frw_event_queue_stru, st_list);
        /* ����¼����е�vap_stateΪ��ͣ����������������ѡ��һ���¼����� */
        if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
            continue;
        }

        /* ����¼����е�Ȩ�ؼ�����Ϊ0������ѡ��һ���¼����� */
        if (pst_event_queue->uc_weight_cnt == 0) {
            continue;
        }

        /* �����¼�����Ȩ�ؼ����� */
        pst_event_queue->uc_weight_cnt--;

        /* ���µ��ȶ��е���Ȩ�ؼ����� */
        pst_sched_queue->ul_total_weight_cnt--;

        /* ���¼�������ȡ��һ���¼� */
        p_event = frw_event_queue_dequeue(pst_event_queue);

        /* ����¼����б�գ���Ҫ����ӵ��ȶ�����ɾ���������¼�����״̬��Ϊ����Ծ(���ɱ�����) */
        frw_event_sched_deactivate_queue_no_lock(pst_sched_queue, pst_event_queue);

        break;
    }

    /* ������ȶ��е���Ȩ�ؼ�����Ϊ0������Ҫ���õ��ȶ����ϸ����¼����е�Ȩ�ؼ����� */
    if (pst_sched_queue->ul_total_weight_cnt == 0) {
        frw_event_sched_reset_weight(pst_sched_queue);
    }

    oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &flag);
    return p_event;
}

/*
 * �� �� ��  : frw_event_sched_init
 * ��������  : ��������ʼ��
 * �������  : pst_sched_queue: ���ȶ���ָ��
 */
uint32_t frw_event_sched_init(frw_event_sched_queue_stru *pst_sched_queue)
{
    if (oal_unlikely(pst_sched_queue == NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_sched_init:: pointer is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ʼ���� */
    oal_spin_lock_init(&pst_sched_queue->st_lock);

    /* ��ʼ�����ȶ�����Ȩ�ؼ����� */
    pst_sched_queue->ul_total_weight_cnt = 0;

    /* ��ʼ����������ͷ */
    oal_dlist_init_head(&pst_sched_queue->st_head);

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_queue_set
 * ��������  : �����¼����в���
 * �������  : pst_prio_queue: �¼�����ָ��
 *             us_weight     : ����Ȩ��
 *             en_policy     : ���е��Ȳ���
 *             en_state      : �¼�����״̬
 */
OAL_STATIC void frw_event_queue_set(frw_event_queue_stru *pst_event_queue,
                                    uint8_t uc_weight,
                                    uint8_t en_policy,
                                    uint8_t en_state)
{
    pst_event_queue->uc_weight = uc_weight;
    pst_event_queue->uc_weight_cnt = 0;
    pst_event_queue->en_policy = en_policy;
    pst_event_queue->en_state = en_state;
    pst_event_queue->en_vap_state = FRW_VAP_STATE_RESUME;
}

/*
 * �� �� ��  : frw_event_queue_init
 * ��������  : �¼����г�ʼ��
 * �������  : pst_event_queue: �¼�����ָ��
 *             us_weight      : ����Ȩ��
 *             en_policy      : ���е��Ȳ���
 *             en_state       : �¼�����״̬
 *             us_max_events  : ����¼�����
 */
uint32_t frw_event_queue_init(frw_event_queue_stru *pst_event_queue,
                              uint8_t uc_weight,
                              uint8_t en_policy,
                              uint8_t en_state,
                              uint8_t uc_max_events)
{
    uint32_t ul_ret;

    if (oal_unlikely(pst_event_queue == NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_queue_init:: pointer is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ʼ���� */
    oal_spin_lock_init(&pst_event_queue->st_lock);

    ul_ret = oal_queue_init(&pst_event_queue->st_queue, uc_max_events);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_FRW, "{frw_event_queue_init:: OAL_QUEUE_INIT return != OAL_SUCC! %d}", ul_ret);
        frw_event_queue_set(pst_event_queue, 0, FRW_SCHED_POLICY_BUTT, FRW_EVENT_QUEUE_STATE_INACTIVE);

        return ul_ret;
    }

    frw_event_queue_set(pst_event_queue, uc_weight, en_policy, en_state);

    return OAL_SUCC;
}

void frw_event_queue_destroy(frw_event_queue_stru *pst_event_queue)
{
    if (oal_unlikely(pst_event_queue == NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_queue_destroy:: pointer is NULL!}");
        return;
    }

    oal_queue_destroy(&pst_event_queue->st_queue);

    frw_event_queue_set(pst_event_queue, 0, FRW_SCHED_POLICY_BUTT, FRW_EVENT_QUEUE_STATE_INACTIVE);
}

/*
 * �� �� ��  : frw_event_sched_deactivate_queue
 * ��������  : �ӵ��ȶ���ɾ��һ���¼�����
 * �������  : pst_sched_queue: ���ȶ���ָ��
 *             pst_event_queue: �¼�����ָ��
 */
uint32_t frw_event_sched_deactivate_queue(frw_event_sched_queue_stru *pst_sched_queue,
                                          frw_event_queue_stru *pst_event_queue)
{
    oal_uint flag = 0;
    uint32_t ret;

    if (oal_unlikely((pst_sched_queue == NULL) || (pst_event_queue == NULL))) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    /* ���жϣ����� */
    oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &flag);

    ret = frw_event_sched_deactivate_queue_no_lock(pst_sched_queue, pst_event_queue);

    /* ���������ж� */
    oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &flag);

    return ret;
}

/*
 * �� �� ��  : frw_event_sched_activate_queue
 * ��������  : ����ȶ�������һ���µ��¼�����
 *             ��װ���ò�����ʵ�֣����ֽӿڶԳ���
 * �������  : past_sched_queue: ���ȶ���ָ��
 *             pst_event_queue : �¼�����ָ��
 */
uint32_t frw_event_sched_activate_queue(frw_event_sched_queue_stru *pst_sched_queue,
                                        frw_event_queue_stru *pst_event_queue)
{
    oal_uint flag = 0;
    uint32_t ret;

    if (oal_unlikely((pst_sched_queue == NULL) || (pst_event_queue == NULL))) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &flag);

    ret = frw_event_sched_activate_queue_no_lock(pst_sched_queue, pst_event_queue);

    oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &flag);

    return ret;
}

/*
 * �� �� ��  : frw_event_sched_pause_queue
 * ��������  : ��ͣ���е���״̬
 * �������  : past_sched_queue: ���ȶ���ָ��
 *             pst_event_queue : �¼�����ָ��
 */
void frw_event_sched_pause_queue(frw_event_sched_queue_stru *pst_sched_queue,
                                 frw_event_queue_stru *pst_event_queue)
{
    oal_uint flag = 0;

    if (oal_unlikely((pst_sched_queue == NULL) || (pst_event_queue == NULL))) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &flag);

    /* ��VAP��Ӧ�������¼�����vap_state����Ϊ��ͣ */
    pst_event_queue->en_vap_state = FRW_VAP_STATE_PAUSE;

    /* ���µ��ȶ��е���Ȩ�ؼ����� */
    if (pst_event_queue->en_state == FRW_EVENT_QUEUE_STATE_ACTIVE) {
        pst_sched_queue->ul_total_weight_cnt -= pst_event_queue->uc_weight_cnt;
    }

    oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &flag);
}

/*
 * �� �� ��  : frw_event_sched_resume_queue
 * ��������  : �ָ����е���״̬
 * �������  : past_sched_queue: ���ȶ���ָ��
 *             pst_event_queue : �¼�����ָ��
 */
void frw_event_sched_resume_queue(frw_event_sched_queue_stru *pst_sched_queue,
                                  frw_event_queue_stru *pst_event_queue)
{
    oal_uint flag = 0;

    if (oal_unlikely((pst_sched_queue == NULL) || (pst_event_queue == NULL))) {
        oal_warn_on(1);
        return;
    }

    oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &flag);

    /* ��VAP��Ӧ�������¼�����vap_state����Ϊ�ָ� */
    pst_event_queue->en_vap_state = FRW_VAP_STATE_RESUME;

    /* ���µ��ȶ��е���Ȩ�ؼ����� */
    if (pst_event_queue->en_state == FRW_EVENT_QUEUE_STATE_ACTIVE) {
        pst_sched_queue->ul_total_weight_cnt += pst_event_queue->uc_weight_cnt;
    }

    oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &flag);
}