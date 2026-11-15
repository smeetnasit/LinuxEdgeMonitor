
import json
import sys

from jsonschema import Draft202012Validator, FormatChecker


SCHEMA_FILE = "docs/metrics.schema.json"
METRICS_FILE = sys.argv[1] if len(sys.argv) > 1 else "metrics_only.json"


def load_schema():
    """Load and verify the JSON Schema."""

    try:
        with open(SCHEMA_FILE, "r") as file:
            schema = json.load(file)

        Draft202012Validator.check_schema(schema)

        return schema

    except (OSError, json.JSONDecodeError, Exception) as error:
        print(f"Schema error: {error}")
        sys.exit(1)


def validate_metrics():
    schema = load_schema()

    validator = Draft202012Validator(
        schema,
        format_checker=FormatChecker()
    )

    sample_count = 0
    valid_count = 0
    invalid_count = 0

    try:
        with open(METRICS_FILE, "r") as file:

            for line_number, line in enumerate(file, start=1):

                if not line.strip():
                    continue

                sample_count += 1

                try:
                    data = json.loads(line)

                except json.JSONDecodeError as error:
                    invalid_count += 1
                    print(
                        f"Line {line_number}: INVALID JSON - "
                        f"{error.msg}"
                    )
                    continue

                errors = sorted(
                    validator.iter_errors(data),
                    key=lambda error: list(error.absolute_path)
                )

                if errors:
                    invalid_count += 1
                    print(f"Line {line_number}: INVALID")

                    for error in errors:
                        field = ".".join(
                            str(part)
                            for part in error.absolute_path
                        )

                        print(
                            f"  Field: {field or 'root'}"
                        )
                        print(
                            f"  Error: {error.message}"
                        )

                else:
                    valid_count += 1
                    print(f"Line {line_number}: VALID")

    except OSError as error:
        print(f"File error: {error}")
        return 1

    print("\nValidation Summary")
    print("------------------")
    print(f"Total samples: {sample_count}")
    print(f"Valid samples: {valid_count}")
    print(f"Invalid samples: {invalid_count}")

    if sample_count == 0 or invalid_count > 0:
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(validate_metrics())
