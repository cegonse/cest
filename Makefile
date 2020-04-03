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

backend-build:
	docker build -t cest_backend:latest .

backend-start:
	docker run -d -p 3322:3322 --env-file .env cest_backend:latest

backend-stop:
	docker kill $(docker ps -q --filter ancestor=cest_backend)
