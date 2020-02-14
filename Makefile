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
