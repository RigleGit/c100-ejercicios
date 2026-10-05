"""El reporte debe transmitir el código de salida de la batería."""
import pathlib
import shutil
import subprocess
import tempfile
import unittest
TOOLS = pathlib.Path(__file__).resolve().parents[1]

class ReportTests(unittest.TestCase):
    def test_failed_report_returns_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "tools").mkdir()
            for name in ("docker_test_runner.sh", "test_everything_fresh.sh", "quick_test.sh", "clean_all.sh"):
                shutil.copy2(TOOLS / name, root / "tools" / name)
            exercise = root / "01-fixture/001-fixture"
            exercise.mkdir(parents=True)
            (exercise / "main.c").write_text("int main(void){return 1;}")
            (exercise / "CMakeLists.txt").write_text(
                "cmake_minimum_required(VERSION 3.16)\nproject(fixture C)\n"
                "enable_testing()\nadd_executable(test_fail main.c)\n"
                "add_test(NAME failure COMMAND test_fail)\n")
            result = subprocess.run(["bash", str(root / "tools/docker_test_runner.sh"), "report"],
                                    cwd=root, capture_output=True, timeout=20)
            self.assertIn(b"FAILED", result.stdout)
            self.assertNotEqual(result.returncode, 0, result.stdout)

if __name__ == "__main__":
    unittest.main()
