
import json
import signal
import subprocess
import time
from pathlib import Path

from jsonschema import Draft202012Validator, FormatChecker


PROJECT_ROOT = Path(__file__).resolve().parent.parent
AGENT = PROJECT_ROOT / "agent/bin/linux_edge_agent"
SCHEMA_FILE = PROJECT_ROOT / "docs/metrics.schema.json"

MINIMUM_SAMPLES = 2
TEST_DURATION_SECONDS = 12


def load_validator():
    with SCHEMA_FILE.open("r") as file:
        schema = json.load(file)

    Draft202012Validator.check_schema(schema)

    return Draft202012Validator(
        schema,
        format_checker=FormatChecker()
    )


def main():
    if not AGENT.exists():
        print(f"[FAIL] Agent executable not found: {AGENT}")
        return 1

    validator = load_validator()

    print("[INFO] Starting Linux Edge Monitor...")

    process = subprocess.Popen(
        [str(AGENT)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    try:
        stdout, stderr = process.communicate(
            timeout=TEST_DURATION_SECONDS
        )

        print("[FAIL] Agent exited before the test timeout.")
        print(stderr)
        return 1

    except subprocess.TimeoutExpired:
        print("[INFO] Test duration reached.")
        print("[INFO] Stopping agent with SIGINT...")

        process.send_signal(signal.SIGINT)

        try:
            stdout, stderr = process.communicate(timeout=5)

        except subprocess.TimeoutExpired:
            print("[FAIL] Agent did not stop within 5 seconds.")

            process.kill()
            stdout, stderr = process.communicate()

            print(stderr)
            return 1

    if process.returncode != 0:
        print(f"[FAIL] Agent exited with code {process.returncode}")
        print(stderr)
        return 1

    print("[PASS] Agent stopped successfully")

    records = []

    for line_number, line in enumerate(stdout.splitlines(), start=1):
        if not line.strip():
            continue

        try:
            record = json.loads(line)
        except json.JSONDecodeError as error:
            print(f"[FAIL] Invalid JSON on line {line_number}: {error}")
            return 1

        errors = list(validator.iter_errors(record))

        if errors:
            print(f"[FAIL] Schema validation failed on line {line_number}")

            for error in errors:
                print(f"  {error.message}")

            return 1

        records.append(record)

        print(f"[PASS] JSON record {len(records)} validated")

    if len(records) < MINIMUM_SAMPLES:
        print(
            f"[FAIL] Expected at least {MINIMUM_SAMPLES} records, "
            f"received {len(records)}"
        )
        return 1

    print("\n========== INTEGRATION TEST SUMMARY ==========")
    print(f"Records generated: {len(records)}")
    print("Schema validation: PASSED")
    print("Agent shutdown: PASSED")
    print("Overall result: PASSED")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
