#ifndef offsets_h
#define offsets_h

#import <stdint.h>

// offsets from the main kernel 0xfeedfacf
extern uint64_t allproc_offset;
extern uint64_t kernproc_offset;

// offsets in struct proc
extern uint64_t struct_proc_p_pid_offset;
extern uint64_t struct_proc_task_offset;
extern uint64_t struct_proc_p_uthlist_offset;
extern uint64_t struct_proc_p_ucred_offset;
extern uint64_t struct_proc_p_comm_offset;

// offsets in struct kauth_cred
extern uint64_t struct_kauth_cred_cr_ref_offset;

// offsets in struct uthread
extern uint64_t struct_uthread_uu_ucred_offset;
extern uint64_t struct_uthread_uu_list_offset;

// offsets in struct task
extern uint64_t struct_task_ref_count_offset;
extern uint64_t struct_task_itk_space_offset;

// offsets in struct ipc_space
extern uint64_t struct_ipc_space_is_table_offset;

// offsets in struct ipc_port
extern uint64_t struct_ipc_port_ip_kobject_offset;

void init_offsets();
extern uint64_t rootvnode_offset;

#endif
