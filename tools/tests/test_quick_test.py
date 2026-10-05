"""Contratos del lanzador, con proyectos y pruebas CMake reales."""
import pathlib
import shutil
import subprocess
import tempfile
import unittest

RUNNER = pathlib.Path(__file__).resolve().parents[1] / "quick_test.sh"


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = pathlib.Path(self.temp.name)
        (self.root / "tools").mkdir()
        shutil.copy2(RUNNER, self.root / "tools/quick_test.sh")

    def exercise(self, number, tests):
        folder = self.root / "01-fixture" / f"{number:03}-fixture"
        folder.mkdir(parents=True)
        (folder / "main.c").write_text("int main(void) { return 0; }\n")
        cmake = ["cmake_minimum_required(VERSION 3.16)", "project(fixture C)",
                 "add_executable(program main.c)", "enable_testing()"]
        for name, body in tests:
            (folder / f"{name}.c").write_text(body)
            cmake.extend([f"add_executable({name} {name}.c)",
                          f"add_test(NAME {name} COMMAND {name})"])
        (folder / "CMakeLists.txt").write_text("\n".join(cmake) + "\n")

    def run_runner(self, *args):
        return subprocess.run(["bash", str(self.root / "tools/quick_test.sh"), *args],
                              cwd=self.root, capture_output=True, text=True,
                              timeout=20)

    def test_success(self):
        self.exercise(1, [("test_ok", "int main(void) { return 0; }")])
        self.assertEqual(self.run_runner("1", "1").returncode, 0)

    def test_failure_is_propagated(self):
        self.exercise(1, [("test_fail", "int main(void) { return 1; }")])
        self.assertNotEqual(self.run_runner("1", "1").returncode, 0)

    def test_all_registered_tests_run(self):
        self.exercise(1, [("test_a", "int main(void) { return 0; }"),
                          ("test_b", "int main(void) { return 1; }")])
        self.assertNotEqual(self.run_runner("1", "1").returncode, 0)

    def test_missing_tests_are_failure(self):
        self.exercise(1, [])
        self.assertNotEqual(self.run_runner("1", "1").returncode, 0)

    def test_nonexistent_selection_is_failure(self):
        self.assertNotEqual(self.run_runner("1", "99").returncode, 0)

    def test_timeout_is_failure(self):
        self.exercise(1, [("test_hang", "int main(void) { for (;;) {} }")])
        import os
        env = dict(os.environ, C100_TEST_TIMEOUT="1")
        result = subprocess.run(["bash", str(self.root / "tools/quick_test.sh"), "1", "1"],
                                cwd=self.root, env=env, capture_output=True, text=True, timeout=20)
        self.assertNotEqual(result.returncode, 0)


if __name__ == "__main__":
    unittest.main()
