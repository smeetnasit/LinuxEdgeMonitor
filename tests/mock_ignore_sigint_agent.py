import signal
import time
import sys

signal.signal(signal.SIGINT, signal.SIG_IGN)

print("[MOCK] Agent started and ignoring SIGINT.", file=sys.stderr)

while True:
    print('{"mock": true}', flush=True)
    time.sleep(1)
