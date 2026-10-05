#!/bin/bash
set -eu
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_DIR"
./tools/quick_test.sh clean
./tools/quick_test.sh
