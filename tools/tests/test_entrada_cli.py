"""EOF y demostración de truncamiento desde la interfaz del ejercicio 094."""
import pathlib
import subprocess
import tempfile
import unittest

class EntradaCliTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        source = pathlib.Path(__file__).resolve().parents[2] / "14-seguridad-buenas-practicas/094-entrada-segura-v2"
        subprocess.run(["cmake", "-S", str(source), "-B", cls.build.name,
                        "-DCMAKE_C_FLAGS=-fsanitize=address,undefined -fno-sanitize-recover=all"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run(["cmake", "--build", cls.build.name], check=True, stdout=subprocess.DEVNULL)
        cls.program = pathlib.Path(cls.build.name) / "entrada_segura"

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def invoke(self, text):
        return subprocess.run([str(self.program)], input=text, capture_output=True,
                              text=True, timeout=3)

    def test_eof_terminates(self):
        self.assertEqual(self.invoke("").returncode, 0)

    def test_invalid_menu_recovers(self):
        self.assertEqual(self.invoke("abc\n9999999999999999999999999\n0\n").returncode, 0)

    def test_long_demonstration_is_bounded(self):
        result = self.invoke("1\n" + "x" * 200 + "\n\n0\n")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("trunc", result.stdout.lower())

if __name__ == "__main__":
    unittest.main()
