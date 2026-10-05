"""La limpieza debe conservar fuentes, fixtures y copias del usuario."""
import pathlib
import shutil
import subprocess
import tempfile
import unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "clean_all.sh"

class CleanTests(unittest.TestCase):
    def test_apply_removes_build_only(self):
        with tempfile.TemporaryDirectory() as temp:
            root = pathlib.Path(temp)
            (root / "tools").mkdir()
            shutil.copy2(SCRIPT, root / "tools/clean_all.sh")
            exercise = root / "01-basico/001-fixture"
            (exercise / "tests").mkdir(parents=True)
            (exercise / "build").mkdir()
            (exercise / "tests/test_contrato.c").write_text("int main(void){return 0;}")
            (exercise / "datos.bak").write_text("contactos del usuario")
            (exercise / "test_datos.txt").write_text("fixture")
            (exercise / "build/programa").write_text("generado")
            command = ["bash", str(root / "tools/clean_all.sh")]
            self.assertEqual(subprocess.run(command, cwd=temp, capture_output=True).returncode, 0)
            self.assertTrue((exercise / "build").exists())
            result = subprocess.run(command + ["--apply"], cwd=temp, capture_output=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertFalse((exercise / "build").exists())
            self.assertTrue((exercise / "tests/test_contrato.c").exists())
            self.assertEqual((exercise / "datos.bak").read_text(), "contactos del usuario")
            self.assertEqual((exercise / "test_datos.txt").read_text(), "fixture")

if __name__ == "__main__":
    unittest.main()
