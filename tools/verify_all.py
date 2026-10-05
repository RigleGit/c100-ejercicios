"""Compila y ejecuta CTest por ejercicio, guardando resultados y logs."""
import json
import os
import pathlib
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor

ROOT = pathlib.Path(os.environ.get("C100_SOURCE", str(pathlib.Path(__file__).resolve().parents[1]))).resolve()
OUTPUT = pathlib.Path(os.environ.get("C100_OUTPUT", str(ROOT / ".verification"))).resolve()
MODE = os.environ.get("C100_MODE", "Debug")
FOLDERS = sorted(ROOT.glob("[0-9][0-9]-*/[0-9][0-9][0-9]-*"))

def verify(folder):
    build = OUTPUT / folder.name
    build.mkdir(parents=True, exist_ok=True)
    commands = [
        ("configure", ["cmake", "-S", str(folder), "-B", str(build),
                       "-DCMAKE_BUILD_TYPE=" + MODE]),
        ("build", ["cmake", "--build", str(build), "-j2"]),
        ("test", ["ctest", "--test-dir", str(build), "--output-on-failure", "--timeout", "15"]),
    ]
    result = {"exercise": str(folder.relative_to(ROOT)), "number": int(folder.name[:3]), "tests": 0}
    for stage, command in commands:
        try:
            process = subprocess.run(command, capture_output=True, text=True, timeout=75)
            output = process.stdout + process.stderr
            code = process.returncode
        except subprocess.TimeoutExpired as error:
            output, code = str(error), 124
        (build / (stage + ".log")).write_text(output)
        result[stage] = code
        if stage == "test":
            listing = subprocess.run(["ctest", "--test-dir", str(build), "-N"],
                                     capture_output=True, text=True, timeout=15)
            match = re.search(r"Total Tests:\s*(\d+)", listing.stdout)
            result["tests"] = int(match[1]) if match else 0
            if not result["tests"]:
                result["test"] = 1
        if code or (stage == "test" and not result["tests"]):
            result["diagnostic"] = output[-5000:]
            break
    return result

def main():
    if MODE not in ("Debug", "Release"):
        raise SystemExit("C100_MODE debe ser Debug o Release")
    numbers = sorted(int(folder.name[:3]) for folder in FOLDERS)
    if numbers != list(range(1, 101)):
        raise SystemExit("Se requieren exactamente los ejercicios 001–100, sin duplicados")
    OUTPUT.mkdir(parents=True, exist_ok=True)
    with ThreadPoolExecutor(max_workers=5) as pool:
        results = list(pool.map(verify, FOLDERS))
    (OUTPUT / "results.json").write_text(json.dumps(results, ensure_ascii=False, indent=2) + "\n")
    failures = [result for result in results if result.get("test") != 0]
    print(json.dumps({"exercises": len(results), "passed": len(results) - len(failures),
                      "failed": len(failures), "tests": sum(result["tests"] for result in results)}))
    for result in failures:
        print(result["exercise"], result.get("diagnostic"))
    return int(bool(failures))

if __name__ == "__main__":
    raise SystemExit(main())
