all:
	@rm -f log.xml
	@rm -f test_summary.jsonl
	@./framework/build.py

%:
	@rm -f log.xml
	@rm -f test_summary.jsonl
	@./framework/build.py $@

clean:
	@rm -rf build/*
	@rm -f log.xml
	@rm -f test_summary.jsonl

python_tests:
	@PYTHONPATH=framework/ pytest framework/spec -vv

backend:
	@PYTHONPATH=framework/ python3 docs/backend/backend.py 2> backend.log
