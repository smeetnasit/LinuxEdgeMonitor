import signal
import sys
import time


running = True


def handle_signal(signum, frame):
    global running
    running = False


signal.signal(signal.SIGINT, handle_signal)

print("[MOCK] Starting invalid JSON agent...", file=sys.stderr)

while running:
    print('{"cpu": 45.5, "memory":', flush=True)
    time.sleep(1)

print("[MOCK] Shutdown successful.", file=sys.stderr)
