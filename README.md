# Demonstrate trace overhead for specific process uprobe versus systemwide

Show that tracing a function has an overhead.

```
$ ./uprobetrace prog function <optional_pid>
```

If optional_pid is not specified, we assume tracing is needed system-wide.

Example

```
$ time getpid 10000000 # 10 million getpid() calls
10000000 pid from getpid() (3749695) matches pid from syscall (3749695)

real	0m0.946s
user	0m0.341s
sys	0m0.602s

$ ./uprobetrace /usr/lib64/libc.so.6 getpid & # trace getpid()s systemwide

$ time ./getpid 10000000
10000000 pid from getpid() (3749720) matches pid from syscall (3749720)

real	0m11.077s
user	0m2.324s
sys	0m8.740s

$ fg
<Ctrl^C>
```

Note the 10x overhead in the latter case; it goes away if we trace a
specific process (make sure above uprobetrace was killed)

```
$ ./uprobetrace /usr/lib64/libc.so.6 getpid 318445 &

$ time ./getpid 10000000
10000000 pid from getpid() (3754441) matches pid from syscall (3754441)

real	0m0.961s
user	0m0.334s
sys	0m0.627s
```

So we see that overheads apply to systemwide tracing or specific process
traced, but are significant when they apply.





