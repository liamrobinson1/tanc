.PHONY: clean test all

all: clean
	source bin/activate && pip install -e .

clean:
	rm -rf build
	rm -rf *.so

test:
	export PYTHONPATH="./:${PYTHONPATH}" && pytest tests/*.py

start-runner:
	actions-runner/run.sh