all:
	@./framework/build.py

clean:
	@rm -rf build/*
	@rm log.xml
	@rm test_summary.jsonl