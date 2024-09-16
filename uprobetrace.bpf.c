// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2022, Oracle and/or its affiliates.

#include "vmlinux.h"

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

char _license[] SEC("license") = "GPL";

SEC("uprobe")
int BPF_PROG(uprobetrace)
{
	return 0;
}
