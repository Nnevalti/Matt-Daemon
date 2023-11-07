from threading import Thread
from time import sleep
import sys

class Spinner:
    def __init__(self, type="dots", delay=0.1):
        self.type = type
        self.delay = delay

        self.done = False
        self._thread = Thread(target=self._spin, daemon=True)
        self.steps = {
              "dots": ["⢿", "⣻", "⣽", "⣾", "⣷", "⣯", "⣟", "⡿"],
            "line": ["―", "\\", "|", "/"],
            "bar": ["[    ]", "[=   ]", "[==  ]", "[=== ]", "[ ===]", "[  ==]", "[   =]", "[    ]"],
        }


    def __enter__(self):
        self.start()


    def __exit__(self, exc_type, exc_value, traceback):
        self.stop()


    def _spin(self):
        while True:
            for step in self.steps[self.type]:
                print(f"{step} ", end="", flush=True)
                sys.stdout.write("\b" * (len(step) + 1))
                if self.done:
                    return
                sleep(self.delay)


    def start(self):
        self._thread.start()
        return self


    def stop(self):
        self.done = True