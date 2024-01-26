all: clean
	source bin/activate && python setup.py build_ext --inplace

clean:
	rm -rf build
	rm -rf *.so