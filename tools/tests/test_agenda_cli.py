"""Persistencia y salida de la agenda desde su interfaz real."""
import pathlib
import subprocess
import tempfile
import unittest


class AgendaCliTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        cls.source = pathlib.Path(__file__).resolve().parents[2] / "15-proyectos-finales/099-agenda-contactos"
        subprocess.run(["cmake", "-S", str(cls.source), "-B", cls.build.name], check=True,
                       stdout=subprocess.DEVNULL)
        subprocess.run(["cmake", "--build", cls.build.name], check=True,
                       stdout=subprocess.DEVNULL)
        cls.program = pathlib.Path(cls.build.name) / "agenda_contactos"

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def run_agenda(self, directory, text):
        return subprocess.run([str(self.program)], cwd=directory, input=text,
                              capture_output=True, text=True, timeout=2)

    def test_eof_terminates(self):
        with tempfile.TemporaryDirectory() as d:
            self.assertEqual(self.run_agenda(d, "").returncode, 0)

    def test_eof_during_contact_cancels_creation(self):
        with tempfile.TemporaryDirectory() as d:
            self.assertEqual(self.run_agenda(d, "1\nAna\n123456789\n").returncode, 0)
            self.assertEqual((pathlib.Path(d)/"data/agenda.csv").read_text().count("\n"), 1)

    def test_invalid_file_is_preserved(self):
        with tempfile.TemporaryDirectory() as d:
            path = pathlib.Path(d) / "data/agenda.csv"
            path.parent.mkdir()
            path.write_text("archivo inválido\n")
            self.assertNotEqual(self.run_agenda(d, "").returncode, 0)
            self.assertEqual(path.read_text(), "archivo inválido\n")


if __name__ == "__main__":
    unittest.main()
