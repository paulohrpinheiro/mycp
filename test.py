#!/usr/bin/env python3

import time
import subprocess
import statistics


def clear_buffer():
    with open('/proc/sys/vm/drop_caches', 'w') as proc:
        proc.write('3')
        proc.flush()


def run_command(command, parameters):
    args = list(['strace', '-c', command])
    args.extend(parameters)
    result = subprocess.run(
        args,
        stderr=subprocess.PIPE,
        universal_newlines=True,
    )

    return result.stderr


def bench(method):
    clear_buffer()

    begin = time.time()
    result = run_command(
        './mycp',
        [method, '/tmp/testfile.bin', '/tmp/testfile-copied.bin']
    )
    finish = time.time()

    eleapsed = finish - begin
    _, secs, calls, _, _ = result.split('\n')[-2].split()

    return float(secs.replace(',', '.')), int(calls), float(eleapsed)


def mean(data):
    ordered = sorted(data)
    return statistics.mean(ordered[5:-5])


for method in ('naive', 'mmap', 'sendfile'):
    result = [bench(method) for x in range(20)]
    calls = result[0][1]
    secs = mean([x[0] for x in result])
    eleapsed = mean([x[2] for x in result])
    print(method, secs, eleapsed, calls)
