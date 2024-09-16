// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2022, Oracle and/or its affiliates.
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include <bpf/bpf.h>

#include "uprobetrace.skel.h"

struct uprobetrace_bpf *skel;

void cleanup(int sig)
{
	if (sig == SIGUSR1)
		return;
	uprobetrace_bpf__destroy(skel);
	exit(1);
}

int main(int argc, char *argv[])
{
	DECLARE_LIBBPF_OPTS(bpf_uprobe_opts, uprobe_opts);
	struct uprobetrace_bpf *skel;
	const char *prog, *name;
	int pid = -1;


	if (argc < 3) {
		fprintf(stderr, " usage: %s binary name [pid]\n", argv[0]);
		return 1;
	}
	prog = argv[1];
	name = argv[2];
	if (argc > 3)
		pid = atoi(argv[2]);

	skel = uprobetrace_bpf__open_and_load();
	if (!skel) {
		fprintf(stderr, "skeleton failed\n");
		cleanup(1);
		return 1;
	}
	uprobe_opts.func_name = name;
	uprobe_opts.retprobe = false;
	skel->links.uprobetrace = bpf_program__attach_uprobe_opts(skel->progs.uprobetrace,
							      pid,
							      prog, 
							      0, &uprobe_opts);
	if (!skel->links.uprobetrace) {
		fprintf(stderr, "cannot attach to '%s' in %s\n", name, prog);
		return 1;
	}
	signal(SIGINT, cleanup);
        signal(SIGTERM, cleanup);
	signal(SIGUSR1, cleanup);

	while (true) {};

	return 0;
}
