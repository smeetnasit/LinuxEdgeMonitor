import json
from jsonschema import Draft202012Validator, FormatChecker

# Load the JSON Schema
with open("docs/metrics.schema.json", "r") as file:
    schema = json.load(file)

validator = Draft202012Validator(
    schema,
    format_checker=FormatChecker()
)

# Read agent output
with open("metrics_only.json", "r") as file:
    decoder = json.JSONDecoder()
    content = file.read()

index = 0
sample_count = 0
valid_count = 0

while index < len(content):

    # Skip whitespace
    while index < len(content) and content[index].isspace():
        index += 1

    if index >= len(content):
        break

    data, index = decoder.raw_decode(content, index)
    sample_count += 1

    # Validate the sample
    errors = list(validator.iter_errors(data))

    if errors:
        print(f"Sample {sample_count}: INVALID")

        for error in errors:
            print(f"  Error: {error.message}")
    else:
        print(f"Sample {sample_count}: VALID")
        valid_count += 1

print("\nValidation Summary")
print("------------------")
print(f"Total samples: {sample_count}")
print(f"Valid samples: {valid_count}")
print(f"Invalid samples: {sample_count - valid_count}")

import sys

if sample_count == 0 or valid_count != sample_count:
    sys.exit(1)

sys.exit(0)
