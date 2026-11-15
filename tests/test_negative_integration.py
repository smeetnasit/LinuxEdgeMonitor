import os
import subprocess
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parent.parent
INTEGRATION_TEST = PROJECT_ROOT / "tests/test_integration.py"

MOCK_AGENTS = [
    {
        "name": "Unexpected agent crash",
        "agent": PROJECT_ROOT / "tests/mock_crash_agent.py",
        "expected_messages": [
            "Agent exited before the test timeout."
        ],
        "timeout": 10,
    },
    {
        "name": "Invalid JSON output",
        "agent": PROJECT_ROOT / "tests/mock_invalid_json_agent.py",
        "expected_messages": [
            "Invalid JSON on line 1"
        ],
        "timeout": 20,
    },
    {
        "name": "Agent ignores SIGINT",
        "agent": PROJECT_ROOT / "tests/mock_ignore_sigint_agent.py",
        "expected_messages": [
            "Agent did not stop within 5 seconds.",
            "Agent was forcefully terminated with SIGKILL."
        ],
        "timeout": 25,
    },
]


def run_negative_test(test_case):
    print(f"\n[TEST] {test_case['name']}...")

    env = os.environ.copy()
    env["LINUX_EDGE_AGENT"] = str(test_case["agent"])

    command = [
        sys.executable,
        str(INTEGRATION_TEST)
    ]

    try:
        process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            env=env,
            timeout=test_case["timeout"]
        )

    except subprocess.TimeoutExpired:
        print("[FAIL] Negative test exceeded its timeout.")
        return 1

    output = process.stdout + process.stderr

    messages_found = all(
        message in output
        for message in test_case["expected_messages"]
    )

    if process.returncode != 0 and messages_found:
        print(f"[PASS] {test_case['name']} detected correctly.")
        return 0

    print(f"[FAIL] {test_case['name']} was not detected correctly.")
    print("Return code:", process.returncode)
    print("Output:")
    print(output)

    return 1


def main():
    failures = 0

    for test_case in MOCK_AGENTS:
        failures += run_negative_test(test_case)

    total = len(MOCK_AGENTS)

    print("\n========== NEGATIVE TEST SUMMARY ==========")
    print(f"Total tests: {total}")
    print(f"Passed: {total - failures}")
    print(f"Failed: {failures}")

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
