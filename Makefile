.PHONY: clean test all install docs stubs

install:
	source bin/activate && pip install -e .

clean:
	rm -rf build
	rm -rf *.so

test:
	pytest tests/*.py

sphinx:
	cd docs && sphinx-apidoc -o ./source ../src -f && make html

start-runner:
	actions-runner/run.sh

stubs:
	pybind11-stubgen sidereal --numpy-array-remove-parameters --ignore-all-errors --output-dir src

all: clean install sphinx stubs